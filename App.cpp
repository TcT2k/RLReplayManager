/*
** RLReplayManager
**
** Copyright (C) 2015 Tobias Taschner <github@tc84.de>
**
** Licensed under GPL v3 or later
*/

#include <wx/wx.h>
#include <wx/intl.h>
#include <wx/debugrpt.h>
#include <wx/msgdlg.h>
#include <wx/filename.h>

#include "ManagerFrame.h"

class ReplayManagerApp : public wxApp
{
public:
	virtual bool OnInit()
	{
		wxHandleFatalExceptions();

		wxImage::AddHandler(new wxPNGHandler());
		wxImage::AddHandler(new wxJPEGHandler());
		wxImage::AddHandler(new wxGIFHandler());

		m_locale.Init();

		SetAppName("RLReplayManager");
		SetAppDisplayName("RL Replay Manager");

		ManagerFrame* frame = new ManagerFrame(NULL);
		frame->Show();

		return true;
	}

	void ShowCrashReport(wxDebugReport::Context context)
	{
		wxDebugReportCompress report;
		wxDebugReportPreviewStd preview;

		report.AddAll(context);

		if (preview.Show(report))
		{
			if (report.Process())
			{
				wxMessageDialog msgDlg(NULL, wxString::Format(
					_("A debug report has been saved to:\n%s"),
					report.GetCompressedFileName()), 
					_("Information"), wxOK | wxCANCEL | wxICON_INFORMATION |wxCENTER);
				msgDlg.SetOKCancelLabels(_("Open Folder"), wxID_CLOSE);
				if (msgDlg.ShowModal() == wxID_OK)
				{
					wxFileName fn(report.GetCompressedFileName());
					wxLaunchDefaultApplication(fn.GetPath());
				}
			}
		}
	}

	virtual void OnFatalException() override
	{
		ShowCrashReport(wxDebugReport::Context_Exception);
	}

	virtual void OnUnhandledException() override
	{
		ShowCrashReport(wxDebugReport::Context_Current);
	}

private:
	wxLocale m_locale;
};

wxIMPLEMENT_APP(ReplayManagerApp);
