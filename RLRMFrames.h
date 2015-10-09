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
#include <wx/statusbr.h>
#include <wx/toolbar.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_NEW_CATEGORY 1000
#define ID_EXPORT 1001
#define ID_UPLOAD 1002
#define ID_AUTO_UPLOAD 1003
#define ID_PROVIDER_DV 1004
#define ID_REPLAY_DV 1005

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
		wxSplitterWindow* m_treeSplitter;
		wxPanel* m_treePanel;
		wxDataViewCtrl* m_providerDV;
		wxPanel* m_replayPanel;
		wxSplitterWindow* m_splitter;
		wxPanel* m_replayListPanel;
		wxDataViewCtrl* m_replayDV;
		wxPanel* m_goalPanel;
		wxListCtrl* m_goalListCtrl;
		wxStatusBar* m_statusBar;
		wxToolBar* m_toolBar;
		wxToolBarToolBase* m_newCategoryTool; 
		wxToolBarToolBase* m_exportTool; 
		wxToolBarToolBase* m_uploadTool; 
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFrameClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnNewCategoryClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUploadClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAutoUploadClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuitClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAboutClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnProviderSelectionChanged( wxDataViewEvent& event ) { event.Skip(); }
		virtual void OnProviderSizeChanged( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnReplaySelectionChanged( wxDataViewEvent& event ) { event.Skip(); }
		virtual void OnStatusBarDoubleClicked( wxMouseEvent& event ) { event.Skip(); }
		
	
	public:
		
		BaseManagerFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("MainWindow") );
		
		~BaseManagerFrame();
		
		void m_treeSplitterOnIdle( wxIdleEvent& )
		{
			m_treeSplitter->SetSashPosition( 0 );
			m_treeSplitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( BaseManagerFrame::m_treeSplitterOnIdle ), NULL, this );
		}
		
		void m_splitterOnIdle( wxIdleEvent& )
		{
			m_splitter->SetSashPosition( 0 );
			m_splitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( BaseManagerFrame::m_splitterOnIdle ), NULL, this );
		}
	
};

#endif //__RLRMFRAMES_H__
