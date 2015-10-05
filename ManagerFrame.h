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
#include <wx/thread.h>

#include <queue>

class ManagerFrame : public BaseManagerFrame, public wxThreadHelper
{
public:
	ManagerFrame(wxWindow* parent);
protected:
	void OnFrameClose(wxCloseEvent& event);
	void OnQuitClicked(wxCommandEvent& event);
	void OnExportClicked(wxCommandEvent& event);
	void OnUploadClicked(wxCommandEvent& event);
	void OnAutoUploadClicked(wxCommandEvent& event);
	void OnAboutClicked(wxCommandEvent& event);
	void OnReplaySelectionChanged(wxDataViewEvent& event);

	void OnFileSystemChange(wxFileSystemWatcherEvent& event);

	void* Entry();

private:
	wxVector<Replay::Ptr> m_replays;
	wxFileSystemWatcher m_fsWatcher;
	std::queue<Replay::Ptr> m_uploadQueue;

	void AddUpload(Replay::Ptr replay);

	void UpdateStatus(const wxString& statusText);

	friend class ReplayDataModel;
};

#endif // __ManagerFrame__
