/*
** RLReplayManager
**
** Copyright (C) 2015 Tobias Taschner <github@tc84.de>
**
** Licensed under GPL v3 or later
*/

#include <wx/wx.h>
#include <wx/intl.h>

#include "ManagerFrame.h"

class ReplayManagerApp : public wxApp
{
public:
	virtual bool OnInit()
	{
		wxImage::AddHandler(new wxPNGHandler());
		wxImage::AddHandler(new wxJPEGHandler());
		wxImage::AddHandler(new wxGIFHandler());

		m_locale.Init();

		SetAppName("RLReplayManager");
		SetAppDisplayName("RLReplayManager");

		ManagerFrame* frame = new ManagerFrame(NULL);
		frame->Show();

		return true;
	}

private:
	wxLocale m_locale;
};

wxIMPLEMENT_APP(ReplayManagerApp);
