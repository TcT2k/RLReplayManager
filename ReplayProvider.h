/*
** RLReplayManager
**
** Copyright (C) 2015 Tobias Taschner <github@tc84.de>
**
** Licensed under GPL v3 or later
*/
#ifndef ReplayProvider_H
#define ReplayProvider_H

#include "Replay.h"

#include <wx/vector.h>
#include <wx/sharedptr.h>
#include <wx/fswatcher.h>

class ReplayProvider
{
public:
	typedef wxSharedPtr<ReplayProvider> Ptr;

	ReplayProvider(ReplayProvider* parent, const wxString& path = wxString(), const wxString& description = wxString());

	wxVector<Replay::Ptr> replay;
	wxVector<ReplayProvider::Ptr> provider;

	const wxString& GetDescription() const
	{
		return m_description;
	}

	ReplayProvider* GetParent() const
	{
		return m_parent;
	}

private:
	ReplayProvider* m_parent;
	wxString m_description;
	wxFileSystemWatcher m_fsWatcher;

	void FindFilesInFolder(const wxString& path);

	void OnFileSystemChange(wxFileSystemWatcherEvent& event);
};


#endif // ReplayProvider_H
