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
#include <wx/persist/toplevel.h>
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>

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

//
// ReplayDataModel
//

class ReplayDataModel : public wxDataViewVirtualListModel
{
public:
	ReplayDataModel(ManagerFrame* frame) :
		wxDataViewVirtualListModel(frame->m_replays.size()),
		m_frame(frame)
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
		Replay::Ptr ri = m_frame->m_replays[row];

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

private:
	ManagerFrame* m_frame;
};


//
// ManagerFrame
//

ManagerFrame::ManagerFrame( wxWindow* parent ):
	BaseManagerFrame( parent )
{
	SetTitle(wxTheApp->GetAppDisplayName());

	wxPersistentRegisterAndRestore(this);

	m_replayDV->AppendTextColumn(_("Description"), RCIDescription);
	m_replayDV->AppendTextColumn(_("Arena"), RCIArena);
	m_replayDV->AppendTextColumn(_("Team Size"), RCITeamSize, wxDATAVIEW_CELL_INERT, wxDLG_UNIT(this, wxSize(20, -1)).GetWidth(), wxALIGN_RIGHT);
	m_replayDV->AppendTextColumn(_("Length"), RCILength, wxDATAVIEW_CELL_INERT, wxDLG_UNIT(this, wxSize(28, -1)).GetWidth());
	m_replayDV->AppendTextColumn(_("Date"), RCIDate, wxDATAVIEW_CELL_INERT, wxDLG_UNIT(this, wxSize(60, -1)).GetWidth());
	m_replayDV->AppendTextColumn(_("Score"), RCIScore, wxDATAVIEW_CELL_INERT, wxDLG_UNIT(this, wxSize(28, -1)).GetWidth(), wxALIGN_CENTER);

	m_goalListCtrl->AppendColumn(_("Player"), wxLIST_FORMAT_CENTER, wxDLG_UNIT(this, wxSize(80, -1)).GetWidth());
	m_goalListCtrl->AppendColumn(_("Score"), wxLIST_FORMAT_CENTER, wxDLG_UNIT(this, wxSize(28, -1)).GetWidth());
	m_goalListCtrl->AppendColumn(_("Team"), wxLIST_FORMAT_RIGHT, wxDLG_UNIT(this, wxSize(20, -1)).GetWidth());
	m_goalListCtrl->AppendColumn(_("Time"), wxLIST_FORMAT_LEFT, wxDLG_UNIT(this, wxSize(28, -1)).GetWidth());

	wxFileName basePath(wxStandardPaths::Get().GetDocumentsDir(), "");
	basePath.AppendDir("My Games");
	basePath.AppendDir("Rocket League");
	basePath.AppendDir("TAGame");
	basePath.AppendDir("Demos");

	wxArrayString replayFiles;
	wxDir::GetAllFiles(basePath.GetFullPath(), &replayFiles, "*.replay", wxDIR_FILES);

	if (replayFiles.empty())
		wxLogError(_("No replays could be found"));

	for (auto filename = replayFiles.begin(); filename != replayFiles.end(); ++filename)
	{
		Replay::Ptr ri(new Replay(*filename));

		m_replays.push_back(ri);
	}

	wxObjectDataPtr<ReplayDataModel> model(new ReplayDataModel(this));
	m_replayDV->UnselectAll();
	m_replayDV->AssociateModel(model.get());
	m_replayDV->Refresh();

	m_fsWatcher.Bind(wxEVT_FSWATCHER, &ManagerFrame::OnFileSystemChange, this);
	m_fsWatcher.Add(basePath.GetFullPath(), wxFSW_EVENT_CREATE | wxFSW_EVENT_DELETE | wxFSW_EVENT_RENAME);
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
	Replay::Ptr replay = m_replays[sel];

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

void ManagerFrame::OnFileSystemChange(wxFileSystemWatcherEvent& event)
{
	wxLogDebug(event.ToString());
	if (!event.GetPath().GetExt().IsSameAs("replay", false))
		return;

	if (event.GetChangeType() & wxFSW_EVENT_CREATE)
	{
		// Add new file
		Replay::Ptr ri(new Replay(event.GetPath().GetFullPath()));
		m_replays.push_back(ri);

		static_cast<ReplayDataModel*>(m_replayDV->GetModel())->RowAppended();
	}
	else if (event.GetChangeType() & wxFSW_EVENT_DELETE)
	{
		// Find replay and remove it
		size_t index = 0;
		wxString changePath = event.GetPath().GetFullPath();

		for (auto replay = m_replays.begin(); replay != m_replays.end(); ++replay, ++index)
		{
			if ((*replay)->GetFileName().IsSameAs(changePath, false))
			{
				m_replays.erase(replay);
				static_cast<ReplayDataModel*>(m_replayDV->GetModel())->RowDeleted(index);
				break;
			}
		}
	}
	else if (event.GetChangeType() & wxFSW_EVENT_RENAME)
	{
		wxString changePath = event.GetPath().GetFullPath();

		// Find replay and update filename
		for (auto replay = m_replays.begin(); replay != m_replays.end(); ++replay)
		{
			if ((*replay)->GetFileName().IsSameAs(changePath, false))
			{
				(*replay)->SetFileName(event.GetNewPath().GetFullPath());
				break;
			}
		}
	}

	event.Skip();
}

void ManagerFrame::OnReplaySelectionChanged(wxDataViewEvent& event)
{
	m_goalListCtrl->DeleteAllItems();

	if (m_replayDV->GetSelectedItemsCount() != 1)
	{
		m_menubar->Enable(ID_EXPORT, false);
		return;
	}

	size_t sel = (size_t)m_replayDV->GetSelection().GetID() - 1;
	m_menubar->Enable(ID_EXPORT, true);

	ReplayProperties::List goals = (*m_replays[sel])["Goals"].As<ReplayProperties::List>();

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
		m_goalListCtrl->SetItem(id, 3, (*m_replays[sel]).ConvertFrames((*goal)["frame"].As<wxUint32>()).Format("%M:%S"));

		itemId++;
	}
}
