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
#include <wx/zipstrm.h>
#include <wx/wfstream.h>

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
	m_localPath = path;
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

Replay* ReplayProvider::FindReplay(const wxString& filename) const
{
	for (auto it = replay.begin(); it != replay.end(); ++it)
	{
		if ((*it)->GetFileName().IsSameAs(filename, false))
			return it->get();
	}

	return NULL;
}

void ReplayProvider::OnFileSystemChange(wxFileSystemWatcherEvent& event)
{
	wxLogDebug(event.ToString());
	if (!event.GetPath().GetExt().IsSameAs("replay", false))
		return;

	if (event.GetChangeType() & wxFSW_EVENT_CREATE)
	{
		Replay* existingReplay = FindReplay(event.GetPath().GetFullPath());
		if (existingReplay) // Replay is already in list
			return;

		// Give rocket league some time to write the file
		wxMilliSleep(500);

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

bool ReplayProvider::Import(const wxString& filename, bool move)
{
	wxFileName fn(filename);
	if (fn.GetExt().IsSameAs("zip", false))
	{
		// TODO: handle zip files
		wxFileInputStream fstr(filename);
		wxZipInputStream zip(fstr);
		while (wxZipEntry* zipEntry = zip.GetNextEntry())
		{
			wxFileName entryFN(zipEntry->GetName());
			if (entryFN.GetExt().IsSameAs("replay", false))
			{
				wxFileName targetFN(m_localPath, entryFN.GetFullName());
				wxFileOutputStream targetFile(targetFN.GetFullPath());
				zip.Read(targetFile);
				targetFile.Close();
				targetFN.SetTimes(NULL, &zipEntry->GetDateTime(), &zipEntry->GetDateTime());

				Replay::Ptr ri(new Replay(targetFN.GetFullPath()));
				replay.push_back(ri);
			}
		}
	}
	else if (fn.GetExt().IsSameAs("replay", false))
	{
		wxFileName targetFN(m_localPath, fn.GetFullName());
		bool copyRes = wxCopyFile(filename, targetFN.GetFullPath(), false);
		if (copyRes)
		{
			Replay::Ptr ri(new Replay(targetFN.GetFullPath()));
			replay.push_back(ri);
		}

		return copyRes;
	}

	return false;
}