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

//
// ManagerFrame
//

ManagerFrame::ManagerFrame( wxWindow* parent ):
	BaseManagerFrame( parent )
{
	SetTitle(wxTheApp->GetAppDisplayName());

	wxPersistentRegisterAndRestore(this);

	m_replayListCtrl->AppendColumn(_("Description"));
	m_replayListCtrl->AppendColumn(_("Team Size"), wxLIST_FORMAT_RIGHT);
	m_replayListCtrl->AppendColumn(_("Length"), wxLIST_FORMAT_RIGHT);
	m_replayListCtrl->AppendColumn(_("Date"));
	m_replayListCtrl->AppendColumn(_("Score"), wxLIST_FORMAT_CENTER);

	m_goalListCtrl->AppendColumn(_("Player"));
	m_goalListCtrl->AppendColumn(_("Score"), wxLIST_FORMAT_CENTER);
	m_goalListCtrl->AppendColumn(_("Team"), wxLIST_FORMAT_RIGHT);

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

		int id = m_replayListCtrl->InsertItem(m_replays.size(), ri->GetDescription(), 0);

		m_replayListCtrl->SetItem(id, 1, wxString::Format("%d", (*ri)["TeamSize"].As<wxUint32>()));
		m_replayListCtrl->SetItem(id, 2, wxString::Format("%d", (*ri)["NumFrames"].As<wxUint32>()));
		if (ri->GetDate().IsValid())
			m_replayListCtrl->SetItem(id, 3, ri->GetDate().Format());
		int team0score = 0;
		int team1score = 0;
		if (ri->find("Team0Score") != ri->end())
			team0score = (*ri)["Team0Score"].As<wxUint32>();
		if (ri->find("Team1Score") != ri->end())
			team1score = (*ri)["Team1Score"].As<wxUint32>();
		m_replayListCtrl->SetItem(id, 4,
			wxString::Format("%d:%d", team0score, team1score));

		m_replays.push_back(ri);
	}
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
	long sel = m_replayListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
		wxLIST_STATE_SELECTED);
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

void ManagerFrame::OnReplaySelected( wxListEvent& event )
{
	m_goalListCtrl->DeleteAllItems();

	if (m_replayListCtrl->GetSelectedItemCount() != 1)
	{
		m_menubar->Enable(ID_EXPORT, false);
		return;
	}

	long sel = m_replayListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
		wxLIST_STATE_SELECTED);
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

		itemId++;
	}
}
