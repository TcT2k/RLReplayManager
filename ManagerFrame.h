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
#include "ReplayProvider.h"
#include "TransferManager.h"

class ManagerFrame : public BaseManagerFrame
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
	void OnProviderSelectionChanged(wxDataViewEvent& event);
	void OnProviderSizeChanged(wxSizeEvent& event);
	void OnNewCategoryClicked(wxCommandEvent& event);

	void OnReplayAdded(wxCommandEvent& event);
	void OnReplayRemoved(wxCommandEvent& event);

	void* Entry();

private:
	ReplayProvider m_replayProvider;
	TransferManager m_transferManager;

	void UpdateStatus(const wxString& statusText);

	void AddUpload(Replay::Ptr replay);

	friend class ReplayDataModel;
};

#endif // __ManagerFrame__
