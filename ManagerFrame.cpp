/*
** RLReplayManager
**
** Copyright (C) 2015 Tobias Taschner <github@tc84.de>
**
** Licensed under GPL v3 or later
*/

#include "ManagerFrame.h"
#include "VersionInfo.h"

#include <wx/app.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/persist.h>
#include <wx/persist/splitter.h>
#include <wx/persist/toplevel.h>
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/config.h>
#include <wx/textdlg.h>

enum ReplayColumnIndex
{
	RCIDescription,
	RCIArena,
	RCITeamSize,
	RCILength,
	RCIDate,
	RCIScore,

	RCIMax
};

enum ProviderColumnIndex
{
	PCIDescription,

	PCIMax
};

//
// ReplayDataModel
//

class ReplayDataModel : public wxDataViewVirtualListModel
{
public:
	ReplayDataModel(ReplayProvider* provider) :
		wxDataViewVirtualListModel((provider) ? provider->replay.size() : 0),
		m_provider(provider)
	{

	}

	virtual unsigned int GetColumnCount() const
	{
		return RCIMax;
	}

	// return type as reported by wxVariant
	virtual wxString GetColumnType(unsigned int col) const
	{
		return wxString();
	}

	void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const
	{
		Replay::Ptr ri = m_provider->replay[row];

		switch (col)
		{
			case RCIDescription:
				variant = ri->GetDescription();
				break;
			case RCIArena:
				variant = ri->GetMapDisplayName();
				break;
			case RCITeamSize:
				variant = wxString::Format("%d", (*ri)["TeamSize"].As<wxUint32>());
				break;
			case RCILength:
				variant = ri->GetLength().Format("%M:%S");
				break;
			case RCIDate:
				variant = ri->GetDate().Format();
				break;
			case RCIScore:
			{
				int team0score = 0;
				int team1score = 0;
				if (ri->find("Team0Score") != ri->end())
					team0score = (*ri)["Team0Score"].As<wxUint32>();
				if (ri->find("Team1Score") != ri->end())
					team1score = (*ri)["Team1Score"].As<wxUint32>();
				variant = wxString::Format("%d:%d", team0score, team1score);
				break;
			}
		}
	}

	bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col)
	{
		return false;
	}

	ReplayProvider* GetProvider() const
	{
		return m_provider;
	}

private:
	ReplayProvider* m_provider;
};

//
// ProviderDataModel
//

class ProviderDataModel : public wxDataViewModel
{
public:
	ProviderDataModel(ReplayProvider* root):
		m_root(root)
	{

	}

	virtual unsigned int GetColumnCount() const
	{
		return PCIMax;
	}

	virtual wxString GetColumnType(unsigned int col) const
	{
		return "wxDataViewIconText";
	}

	virtual void GetValue(wxVariant &variant, const wxDataViewItem &item, unsigned int col) const
	{
		ReplayProvider* provider = (ReplayProvider*)item.GetID();

		wxDataViewIconText iconText;
		iconText.SetText(provider->GetDescription());
		// TODO: scale icon size
		iconText.SetIcon(wxArtProvider::GetIcon(wxART_FOLDER, wxART_LIST, wxSize(16, 16)));

		variant << iconText;
	}

	virtual bool SetValue(const wxVariant &variant, const wxDataViewItem &item, unsigned int col)
	{
		return false;
	}

	virtual bool IsEnabled(const wxDataViewItem &item, unsigned int col) const
	{
		return true;
	}

	virtual wxDataViewItem GetParent(const wxDataViewItem &item) const
	{
		// the invisible root node has no parent
		if (!item.IsOk())
			return wxDataViewItem(0);

		ReplayProvider* provider = (ReplayProvider*)item.GetID();

		if (provider == m_root)
			return wxDataViewItem(0);

		return wxDataViewItem((void*)provider->GetParent());
	}

	virtual bool IsContainer(const wxDataViewItem &item) const
	{
		// the invisble root node can have children
		if (!item.IsOk())
			return true;

		ReplayProvider* provider = (ReplayProvider*)item.GetID();
		return !provider->provider.empty();
	}

	virtual unsigned int GetChildren(const wxDataViewItem &parent, wxDataViewItemArray &array) const
	{
		const ReplayProvider* provider = (ReplayProvider*)parent.GetID();
		if (!provider)
			provider = m_root;

		for (auto it = provider->provider.begin(); it != provider->provider.end(); ++it)
		{
			ReplayProvider* childProvider = (ReplayProvider*)it->get();
			array.Add(wxDataViewItem((void*)childProvider));
		}

		return provider->provider.size();
	}

private:
	ReplayProvider* m_root;
};

//
// ManagerFrame
//

ManagerFrame::ManagerFrame( wxWindow* parent ):
	BaseManagerFrame(parent),
	m_replayProvider(NULL)
{
	SetTitle(wxTheApp->GetAppDisplayName());

#ifdef __WXMSW__
	SetIcon(wxIcon("APPICON"));
#endif

	// Initalize default size / position
	SetMinClientSize(wxDLG_UNIT(this, wxSize(200, 100)));
	SetClientSize(wxDLG_UNIT(this, wxSize(480, 280)));
	m_treeSplitter->SetSashPosition(wxDLG_UNIT(this, wxSize(120, -1)).GetWidth());
	int sashHeight = GetClientSize().GetHeight() - m_toolBar->GetSize().GetHeight() - wxDLG_UNIT(this, wxSize(-1, 80)).GetHeight();
	m_splitter->SetSashPosition(sashHeight);
	Center();

	// Disconnect wxFormBuilder idle events
	m_treeSplitter->Disconnect(wxEVT_IDLE, wxIdleEventHandler(BaseManagerFrame::m_treeSplitterOnIdle), NULL, this);
	m_splitter->Disconnect(wxEVT_IDLE, wxIdleEventHandler(BaseManagerFrame::m_splitterOnIdle), NULL, this);

	wxPersistentRegisterAndRestore(this);
	wxPersistentRegisterAndRestore(m_treeSplitter);
	wxPersistentRegisterAndRestore(m_splitter);

	m_providerDV->AppendIconTextColumn(_("Category"), PCIDescription, wxDATAVIEW_CELL_INERT, wxDLG_UNIT(this, wxSize(80, -1)).GetWidth());

	wxSizeEvent sizeEvt;
	OnProviderSizeChanged(sizeEvt);

	m_replayDV->AppendTextColumn(_("Arena"), RCIArena, wxDATAVIEW_CELL_INERT, wxDLG_UNIT(this, wxSize(80, -1)).GetWidth());
	m_replayDV->AppendTextColumn(_("Team Size"), RCITeamSize, wxDATAVIEW_CELL_INERT, wxDLG_UNIT(this, wxSize(20, -1)).GetWidth(), wxALIGN_RIGHT);
	m_replayDV->AppendTextColumn(_("Score"), RCIScore, wxDATAVIEW_CELL_INERT, wxDLG_UNIT(this, wxSize(28, -1)).GetWidth(), wxALIGN_CENTER);
	m_replayDV->AppendTextColumn(_("Length"), RCILength, wxDATAVIEW_CELL_INERT, wxDLG_UNIT(this, wxSize(28, -1)).GetWidth());
	m_replayDV->AppendTextColumn(_("Date"), RCIDate, wxDATAVIEW_CELL_INERT, wxDLG_UNIT(this, wxSize(60, -1)).GetWidth());
	m_replayDV->AppendTextColumn(_("Description"), RCIDescription);

	m_goalListCtrl->AppendColumn(_("Player"), wxLIST_FORMAT_CENTER, wxDLG_UNIT(this, wxSize(80, -1)).GetWidth());
	m_goalListCtrl->AppendColumn(_("Score"), wxLIST_FORMAT_CENTER, wxDLG_UNIT(this, wxSize(28, -1)).GetWidth());
	m_goalListCtrl->AppendColumn(_("Team"), wxLIST_FORMAT_RIGHT, wxDLG_UNIT(this, wxSize(20, -1)).GetWidth());
	m_goalListCtrl->AppendColumn(_("Time"), wxLIST_FORMAT_LEFT, wxDLG_UNIT(this, wxSize(28, -1)).GetWidth());

	wxFileName basePath(wxStandardPaths::Get().GetDocumentsDir(), "");
	basePath.AppendDir("My Games");
	basePath.AppendDir("Rocket League");
	basePath.AppendDir("TAGame");
	basePath.AppendDir("Demos");

	// Prepare various UI elements
	m_menubar->Check(ID_AUTO_UPLOAD, wxConfig::Get()->ReadBool("AutoUpload", false));
	m_toolBar->EnableTool(ID_EXPORT, false);
	m_toolBar->EnableTool(ID_UPLOAD, false);

	ReplayProvider::Ptr activeReplays(new ReplayProvider(&m_replayProvider, basePath.GetFullPath(), "Active Game Replays"));
	m_replayProvider.provider.push_back(activeReplays);
	if (activeReplays->replay.empty())
		wxLogError(_("No replays could be found"));

	ReplayProvider::Ptr archiveRoot(new ReplayProvider(&m_replayProvider, "", "Archived Replays"));
	m_replayProvider.provider.push_back(archiveRoot);

	wxObjectDataPtr<ProviderDataModel> provModel(new ProviderDataModel(&m_replayProvider));
	m_providerDV->AssociateModel(provModel.get());
}

void ManagerFrame::AddUpload(Replay::Ptr replay)
{
	wxString apiKey = wxConfig::Get()->Read("UploadKey", "");
	if (apiKey.empty())
	{
		wxTextEntryDialog dlg(this, _("Please enter your rocketleaguereplays.com API key:"), _("Rocket League Replays API Key"));
		dlg.SetTextValidator(wxFILTER_EMPTY);
		if (dlg.ShowModal() == wxID_OK)
		{
			wxConfig::Get()->Write("UploadKey", dlg.GetValue());
			wxConfig::Get()->Flush();
		}
	}

	m_transferManager.Upload(replay);
}

void ManagerFrame::UpdateStatus(const wxString& statusText)
{
	m_statusBar->SetStatusText(statusText);
}

void ManagerFrame::OnFrameClose(wxCloseEvent& event)
{
	if (event.CanVeto() && wxConfig::Get()->ReadBool("AutoUpload", false))
	{
		if (wxMessageBox(_("New replays will not be automatically uploaded if you quit.\n\nAre you sure you want to quit?"), 
			_("Warning"), wxICON_WARNING | wxYES_NO | wxNO_DEFAULT | wxCENTER, this) != wxYES)
		{
			event.Veto();
			return;
		}
	}

	event.Skip();
}

void ManagerFrame::OnQuitClicked( wxCommandEvent& event )
{
	Close();
}

void ManagerFrame::OnAboutClicked(wxCommandEvent& event)
{
	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName(wxTheApp->GetAppDisplayName());
	aboutInfo.SetDescription(_("RL Replay Manager"));
	aboutInfo.SetCopyright("(C) 2015");
	aboutInfo.SetWebSite("https://github.com/TcT2k/RLReplayManager");
	aboutInfo.AddDeveloper("Tobias Taschner");
	aboutInfo.SetVersion(APP_VERSION);
	wxAboutBox(aboutInfo);
}

void ManagerFrame::OnExportClicked(wxCommandEvent& event)
{
	size_t sel = (size_t)m_replayDV->GetSelection().GetID() - 1;
	ReplayProvider* provider = static_cast<ReplayDataModel*>(m_replayDV->GetModel())->GetProvider();
	Replay::Ptr replay = provider->replay[sel];

	wxFileDialog fdlg(
		this, 
		_("Export replay"),
		wxStandardPaths::Get().GetDocumentsDir(),
		replay->GetExportFileName(),
		"*.zip",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
		);

	if (fdlg.ShowModal() == wxID_OK)
	{
		replay->Export(fdlg.GetPath());

		wxMessageDialog msgDlg(this, _("Export has finished"), _("Information"), wxICON_INFORMATION | wxOK | wxCANCEL | wxCENTER);
		msgDlg.SetOKCancelLabels(_("Open Folder"), _("Close"));
		if (msgDlg.ShowModal() == wxID_OK)
		{
			wxFileName fn(fdlg.GetPath());
			wxLaunchDefaultApplication(fn.GetPath());
		}
	}
}

void ManagerFrame::OnUploadClicked(wxCommandEvent& event)
{
	size_t sel = (size_t)m_replayDV->GetSelection().GetID() - 1;
	ReplayProvider* provider = static_cast<ReplayDataModel*>(m_replayDV->GetModel())->GetProvider();
	Replay::Ptr replay = provider->replay[sel];

	AddUpload(replay);
}

void ManagerFrame::OnAutoUploadClicked(wxCommandEvent& event)
{
	wxConfig::Get()->Write("AutoUpload", m_menubar->IsChecked(ID_AUTO_UPLOAD));

	event.Skip();
}

void ManagerFrame::OnProviderSelectionChanged(wxDataViewEvent& event)
{
	ReplayProvider* provider = (ReplayProvider*)event.GetItem().GetID();

	m_goalListCtrl->DeleteAllItems();

	wxObjectDataPtr<ReplayDataModel> model(new ReplayDataModel(provider));
	m_replayDV->UnselectAll();
	m_replayDV->AssociateModel(model.get());
	m_replayDV->Refresh();
}

void ManagerFrame::OnProviderSizeChanged(wxSizeEvent& event)
{
#if defined(wxUSE_GENERICDATAVIEWCTRL)
	// automatically resize our only column to take the entire control width
	if (m_providerDV->GetColumnCount())
	{
		wxSize size = m_providerDV->GetClientSize();
		m_providerDV->GetColumn(0)->SetWidth(size.x);
	}
#endif
	event.Skip();
}

void ManagerFrame::OnReplaySelectionChanged(wxDataViewEvent& event)
{
	m_goalListCtrl->DeleteAllItems();

	m_menubar->Enable(ID_UPLOAD, m_replayDV->GetSelectedItemsCount() > 0);
	m_toolBar->EnableTool(ID_UPLOAD, m_replayDV->GetSelectedItemsCount() > 0);

	if (m_replayDV->GetSelectedItemsCount() != 1)
	{
		m_menubar->Enable(ID_EXPORT, false);
		m_toolBar->EnableTool(ID_EXPORT, false);
		return;
	}

	size_t sel = (size_t)m_replayDV->GetSelection().GetID() - 1;
	m_menubar->Enable(ID_EXPORT, true);
	m_toolBar->EnableTool(ID_EXPORT, true);

	ReplayProvider* provider = static_cast<ReplayDataModel*>(m_replayDV->GetModel())->GetProvider();

	ReplayProperties::List goals = (*provider->replay[sel])["Goals"].As<ReplayProperties::List>();

	int score1 = 0;
	int score2 = 0;

	int itemId = 0;
	for (auto goal = goals.begin(); goal != goals.end(); ++goal)
	{
		int id = m_goalListCtrl->InsertItem(itemId, (*goal)["PlayerName"].As<wxString>());

		wxUint32 playerTeam = (*goal)["PlayerTeam"].As<wxUint32>();
		if (playerTeam == 0)
			score1++;
		else
			score2++;

		m_goalListCtrl->SetItem(id, 1, wxString::Format("%d:%d", score1, score2));
		m_goalListCtrl->SetItem(id, 2, wxString::Format("%d", playerTeam + 1));
		m_goalListCtrl->SetItem(id, 3, (*provider->replay[sel]).ConvertFrames((*goal)["frame"].As<wxUint32>()).Format("%M:%S"));

		itemId++;
	}
}

void ManagerFrame::OnNewCategoryClicked(wxCommandEvent& event)
{

}
