/*
** RLReplayManager
**
** Copyright (C) 2015 Tobias Taschner <github@tc84.de>
**
** Licensed under GPL v3 or later
*/
#include <wx/wxprec.h>

#include "ReplayProvider.h"
#include "TransferManager.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/config.h>

wxDEFINE_EVENT(wxEVT_REPLAY_REMOVED, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_REPLAY_ADDED, wxCommandEvent);

//
// ReplayProvider
//

ReplayProvider::ReplayProvider(ReplayProvider* parent, const wxString& path, const wxString& description):
	m_parent(parent),
	m_description(description)
{
	if (!path.empty())
		FindFilesInFolder(path);
}

void ReplayProvider::FindFilesInFolder(const wxString& path)
{
	replay.clear();

	wxArrayString replayFiles;
	wxDir::GetAllFiles(path, &replayFiles, "*.replay", wxDIR_FILES);

	for (auto filename = replayFiles.begin(); filename != replayFiles.end(); ++filename)
	{
		Replay::Ptr ri(new Replay(*filename));

		replay.push_back(ri);
	}

	m_fsWatcher.Bind(wxEVT_FSWATCHER, &ReplayProvider::OnFileSystemChange, this);
	m_fsWatcher.Add(path, wxFSW_EVENT_CREATE | wxFSW_EVENT_DELETE | wxFSW_EVENT_RENAME);
}

void ReplayProvider::OnFileSystemChange(wxFileSystemWatcherEvent& event)
{
	wxLogDebug(event.ToString());
	if (!event.GetPath().GetExt().IsSameAs("replay", false))
		return;

	if (event.GetChangeType() & wxFSW_EVENT_CREATE)
	{
		// Add new file
		Replay::Ptr ri(new Replay(event.GetPath().GetFullPath()));
		replay.push_back(ri);

		if (wxConfig::Get()->ReadBool("AutoUpload", false))
		{
			TransferManager::Get().Upload(ri);
		}

		wxCommandEvent evt(wxEVT_REPLAY_ADDED);
		evt.SetInt(replay.size() - 1);
		evt.SetEventObject(this);
		GetRoot()->ProcessEvent(evt);
	}
	else if (event.GetChangeType() & wxFSW_EVENT_DELETE)
	{
		// Find replay and remove it
		size_t index = 0;
		wxString changePath = event.GetPath().GetFullPath();

		for (auto replayIT = replay.begin(); replayIT != replay.end(); ++replayIT, ++index)
		{
			if ((*replayIT)->GetFileName().IsSameAs(changePath, false))
			{
				replay.erase(replayIT);
				break;
			}
		}

		wxCommandEvent evt(wxEVT_REPLAY_REMOVED);
		evt.SetInt(index);
		evt.SetEventObject(this);
		GetRoot()->ProcessEvent(evt);
	}
	else if (event.GetChangeType() & wxFSW_EVENT_RENAME)
	{
		wxString changePath = event.GetPath().GetFullPath();

		// Find replay and update filename
		for (auto replayIT = replay.begin(); replayIT != replay.end(); ++replayIT)
		{
			if ((*replayIT)->GetFileName().IsSameAs(changePath, false))
			{
				(*replayIT)->SetFileName(event.GetNewPath().GetFullPath());
				break;
			}
		}
	}

	event.Skip();
}

ReplayProvider* ReplayProvider::GetRoot() const
{
	ReplayProvider* provider = const_cast<ReplayProvider*>(this);
	while (provider->GetParent() != NULL)
		provider = provider->GetParent();

	return provider;
}
