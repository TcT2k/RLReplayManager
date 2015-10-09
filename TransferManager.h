/*
** RLReplayManager
**
** Copyright (C) 2015 Tobias Taschner <github@tc84.de>
**
** Licensed under GPL v3 or later
*/
#ifndef TransferManager_H
#define TransferManager_H

#include "Replay.h"

#include <wx/thread.h>
#include <queue>

class TransferManager: public wxThreadHelper
{
public:
	void Upload(Replay::Ptr replay);

	virtual void* Entry();

	static TransferManager& Get();

protected:


private:
	static TransferManager* ms_transferManager;

	std::queue<Replay::Ptr> m_uploadQueue;

	wxString GetAPIURL(const wxString& method);

	TransferManager();
};

#endif // TransferManager_H
