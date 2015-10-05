///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __RLRMFRAMES_H__
#define __RLRMFRAMES_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_EXPORT 1000

///////////////////////////////////////////////////////////////////////////////
/// Class BaseManagerFrame
///////////////////////////////////////////////////////////////////////////////
class BaseManagerFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar;
		wxMenu* file;
		wxMenu* help;
		wxSplitterWindow* m_splitter;
		wxPanel* m_panel1;
		wxDataViewCtrl* m_replayDV;
		wxPanel* m_panel2;
		wxListCtrl* m_goalListCtrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnExportClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuitClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAboutClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnReplaySelectionChanged( wxDataViewEvent& event ) { event.Skip(); }
		
	
	public:
		
		BaseManagerFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~BaseManagerFrame();
		
		void m_splitterOnIdle( wxIdleEvent& )
		{
			m_splitter->SetSashPosition( 0 );
			m_splitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( BaseManagerFrame::m_splitterOnIdle ), NULL, this );
		}
	
};

#endif //__RLRMFRAMES_H__
