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
	TransferManager();

	void Upload(Replay::Ptr replay);

	virtual void* Entry();

protected:


private:
	std::queue<Replay::Ptr> m_uploadQueue;

	wxString GetAPIURL(const wxString& method);
};

#endif // TransferManager_H
