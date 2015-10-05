/*
** RLReplayManager
**
** Copyright (C) 2015 Tobias Taschner <github@tc84.de>
**
** Licensed under GPL v3 or later
*/
#ifndef __ManagerFrame__
#define __ManagerFrame__

#include "RLRMFrames.h"
#include "Replay.h"

#include <wx/fswatcher.h>

class ManagerFrame : public BaseManagerFrame
{
public:
	ManagerFrame(wxWindow* parent);
protected:
	void OnQuitClicked(wxCommandEvent& event);
	void OnExportClicked(wxCommandEvent& event);
	void OnAboutClicked(wxCommandEvent& event);
	void OnReplaySelectionChanged(wxDataViewEvent& event);

	void OnFileSystemChange(wxFileSystemWatcherEvent& event);

private:
	wxVector<Replay::Ptr> m_replays;
	wxFileSystemWatcher m_fsWatcher;

	friend class ReplayDataModel;
};

#endif // __ManagerFrame__
