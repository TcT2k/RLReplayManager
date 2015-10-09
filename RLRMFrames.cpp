///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "RLRMFrames.h"

///////////////////////////////////////////////////////////////////////////

BaseManagerFrame::BaseManagerFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar = new wxMenuBar( 0 );
	file = new wxMenu();
	wxMenuItem* newCategory;
	newCategory = new wxMenuItem( file, ID_NEW_CATEGORY, wxString( _("&New Category") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	file->Append( newCategory );
	
	file->AppendSeparator();
	
	wxMenuItem* exportItem;
	exportItem = new wxMenuItem( file, ID_EXPORT, wxString( _("&Export...") ) + wxT('\t') + wxT("Ctrl+E"), wxEmptyString, wxITEM_NORMAL );
	file->Append( exportItem );
	exportItem->Enable( false );
	
	wxMenuItem* upload;
	upload = new wxMenuItem( file, ID_UPLOAD, wxString( _("&Upload...") ) + wxT('\t') + wxT("Ctrl+U"), wxEmptyString, wxITEM_NORMAL );
	file->Append( upload );
	upload->Enable( false );
	
	file->AppendSeparator();
	
	wxMenuItem* autoUpload;
	autoUpload = new wxMenuItem( file, ID_AUTO_UPLOAD, wxString( _("&Auto Upload") ) , wxEmptyString, wxITEM_CHECK );
	file->Append( autoUpload );
	
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
	
	m_treeSplitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE, wxT("treeSplitter") );
	m_treeSplitter->Connect( wxEVT_IDLE, wxIdleEventHandler( BaseManagerFrame::m_treeSplitterOnIdle ), NULL, this );
	m_treeSplitter->SetMinimumPaneSize( 100 );
	
	m_treePanel = new wxPanel( m_treeSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_providerDV = new wxDataViewCtrl( m_treePanel, ID_PROVIDER_DV, wxDefaultPosition, wxDefaultSize, wxDV_NO_HEADER|wxNO_BORDER );
	bSizer4->Add( m_providerDV, 1, wxEXPAND, 5 );
	
	
	m_treePanel->SetSizer( bSizer4 );
	m_treePanel->Layout();
	bSizer4->Fit( m_treePanel );
	m_replayPanel = new wxPanel( m_treeSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter = new wxSplitterWindow( m_replayPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE, wxT("contentSplitter") );
	m_splitter->Connect( wxEVT_IDLE, wxIdleEventHandler( BaseManagerFrame::m_splitterOnIdle ), NULL, this );
	m_splitter->SetMinimumPaneSize( 100 );
	
	m_replayListPanel = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_replayDV = new wxDataViewCtrl( m_replayListPanel, ID_REPLAY_DV, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	bSizer2->Add( m_replayDV, 1, wxEXPAND, 5 );
	
	
	m_replayListPanel->SetSizer( bSizer2 );
	m_replayListPanel->Layout();
	bSizer2->Fit( m_replayListPanel );
	m_goalPanel = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_goalListCtrl = new wxListCtrl( m_goalPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER|wxLC_REPORT|wxNO_BORDER );
	bSizer3->Add( m_goalListCtrl, 1, wxEXPAND, 5 );
	
	
	m_goalPanel->SetSizer( bSizer3 );
	m_goalPanel->Layout();
	bSizer3->Fit( m_goalPanel );
	m_splitter->SplitHorizontally( m_replayListPanel, m_goalPanel, 0 );
	bSizer5->Add( m_splitter, 1, wxEXPAND, 5 );
	
	
	m_replayPanel->SetSizer( bSizer5 );
	m_replayPanel->Layout();
	bSizer5->Fit( m_replayPanel );
	m_treeSplitter->SplitVertically( m_treePanel, m_replayPanel, 0 );
	bSizer1->Add( m_treeSplitter, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_toolBar = this->CreateToolBar( wxTB_HORIZONTAL|wxTB_HORZ_TEXT|wxTB_NODIVIDER, wxID_ANY ); 
	m_newCategoryTool = m_toolBar->AddTool( ID_NEW_CATEGORY, _("New Category"), wxArtProvider::GetBitmap( wxART_NEW_DIR, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolBar->AddSeparator(); 
	
	m_exportTool = m_toolBar->AddTool( ID_EXPORT, _("Export"), wxArtProvider::GetBitmap( wxART_FILE_SAVE_AS, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_uploadTool = m_toolBar->AddTool( ID_UPLOAD, _("Upload"), wxArtProvider::GetBitmap( wxART_NORMAL_FILE, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolBar->Realize(); 
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( BaseManagerFrame::OnFrameClose ) );
	this->Connect( newCategory->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnNewCategoryClicked ) );
	this->Connect( exportItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnExportClicked ) );
	this->Connect( upload->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnUploadClicked ) );
	this->Connect( autoUpload->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnAutoUploadClicked ) );
	this->Connect( quit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnQuitClicked ) );
	this->Connect( about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnAboutClicked ) );
	this->Connect( ID_PROVIDER_DV, wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( BaseManagerFrame::OnProviderSelectionChanged ) );
	m_providerDV->Connect( wxEVT_SIZE, wxSizeEventHandler( BaseManagerFrame::OnProviderSizeChanged ), NULL, this );
	this->Connect( ID_REPLAY_DV, wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( BaseManagerFrame::OnReplaySelectionChanged ) );
	m_statusBar->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( BaseManagerFrame::OnStatusBarDoubleClicked ), NULL, this );
}

BaseManagerFrame::~BaseManagerFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( BaseManagerFrame::OnFrameClose ) );
	this->Disconnect( ID_NEW_CATEGORY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnNewCategoryClicked ) );
	this->Disconnect( ID_EXPORT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnExportClicked ) );
	this->Disconnect( ID_UPLOAD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnUploadClicked ) );
	this->Disconnect( ID_AUTO_UPLOAD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnAutoUploadClicked ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnQuitClicked ) );
	this->Disconnect( wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseManagerFrame::OnAboutClicked ) );
	this->Disconnect( ID_PROVIDER_DV, wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( BaseManagerFrame::OnProviderSelectionChanged ) );
	m_providerDV->Disconnect( wxEVT_SIZE, wxSizeEventHandler( BaseManagerFrame::OnProviderSizeChanged ), NULL, this );
	this->Disconnect( ID_REPLAY_DV, wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( BaseManagerFrame::OnReplaySelectionChanged ) );
	m_statusBar->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( BaseManagerFrame::OnStatusBarDoubleClicked ), NULL, this );
	
}
