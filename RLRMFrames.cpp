///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "RLRMFrames.h"

///////////////////////////////////////////////////////////////////////////

BaseManagerFrame::BaseManagerFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar = new wxMenuBar( 0 );
	file = new wxMenu();
	wxMenuItem* exportItem;
	exportItem = new wxMenuItem( file, ID_EXPORT, wxString( _("&Export...") ) + wxT('\t') + wxT("Ctrl+E"), wxEmptyString, wxITEM_NORMAL );
	file->Append( exportItem );
	exportItem->Enable( false );
	
	file->AppendSeparator();
	
	wxMenuItem* quit;
	quit = new wxMenuItem( file, wxID_EXIT, wxString( wxEmptyString ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( quit );
	
	m_menubar->Append( file, _("&File") ); 
	
	help = new wxMenu();
	wxMenuItem* about;
	about = new wxMenuItem( help, wxID_ABOUT, wxString( wxEmptyString ) , wxEmptyString, wxITEM_NORMAL );
	help->Append( about );
	
	m_menubar->Append( help, _("&Help") ); 
	
	this->SetMenuBar( m_menubar );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE );
	m_splitter->Connect( wxEVT_IDLE, wxIdleEventHandler( BaseManagerFrame::m_splitterOnIdle ), NULL, this );
	
	m_panel1 = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_replayListCtrl = new wxListCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER|wxLC_REPORT|wxNO_BORDER );
	bSizer2->Add( m_replayListCtrl, 1, wxEXPAND, 5 );
	
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_panel2 = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_goalListCtrl = new wxListCtrl( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER|wxLC_REPORT|wxNO_BORDER );
	bSizer3->Add( m_goalListCtrl, 1, wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( bSizer3 );
	m_panel2->Layout();
	bSizer3->Fit( m_panel2 );
	m_splitter->SplitVertically( m_panel1, m_panel2, 0 );
	bSizer1->Add( m_splitter, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( exportItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnExportClicked ) );
	this->Connect( quit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnQuitClicked ) );
	this->Connect( about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnAboutClicked ) );
	m_replayListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( BaseManagerFrame::OnReplaySelected ), NULL, this );
}

BaseManagerFrame::~BaseManagerFrame()
{
	// Disconnect Events
	this->Disconnect( ID_EXPORT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnExportClicked ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnQuitClicked ) );
	this->Disconnect( wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnAboutClicked ) );
	m_replayListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( BaseManagerFrame::OnReplaySelected ), NULL, this );
	
}
