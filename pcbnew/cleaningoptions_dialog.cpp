/////////////////////////////////////////////////////////////////////////////
// Name:        cleaningoptions_dialog.cpp
// Purpose:     
// Author:      jean-pierre Charras
// Modified by: 
// Created:     25/05/2007 14:24:54
// RCS-ID:      
// Copyright:   GNU License
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 25/05/2007 14:24:54

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "cleaningoptions_dialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "cleaningoptions_dialog.h"

////@begin XPM images
////@end XPM images


/*!
 * WinEDA_CleaningOptionsFrame type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WinEDA_CleaningOptionsFrame, wxDialog )


/*!
 * WinEDA_CleaningOptionsFrame event table definition
 */

BEGIN_EVENT_TABLE( WinEDA_CleaningOptionsFrame, wxDialog )

////@begin WinEDA_CleaningOptionsFrame event table entries
    EVT_CLOSE( WinEDA_CleaningOptionsFrame::OnCloseWindow )

    EVT_BUTTON( ID_BUTTON_EXECUTE, WinEDA_CleaningOptionsFrame::OnButtonExecuteClick )

////@end WinEDA_CleaningOptionsFrame event table entries

END_EVENT_TABLE()


/*!
 * WinEDA_CleaningOptionsFrame constructors
 */

WinEDA_CleaningOptionsFrame::WinEDA_CleaningOptionsFrame()
{
    Init();
}

WinEDA_CleaningOptionsFrame::WinEDA_CleaningOptionsFrame( WinEDA_PcbFrame* parent, wxDC * DC, wxWindowID id,
	const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
	m_Parent = parent;
	m_DC = DC;
    Create(parent, id, caption, pos, size, style);
}


/*!
 * WinEDA_CleaningOptionsFrame creator
 */

bool WinEDA_CleaningOptionsFrame::Create( wxWindow * parent, wxWindowID id,
	const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WinEDA_CleaningOptionsFrame creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end WinEDA_CleaningOptionsFrame creation
    return true;
}


/*!
 * WinEDA_CleaningOptionsFrame destructor
 */

WinEDA_CleaningOptionsFrame::~WinEDA_CleaningOptionsFrame()
{
////@begin WinEDA_CleaningOptionsFrame destruction
////@end WinEDA_CleaningOptionsFrame destruction
}


/*!
 * Member initialisation
 */

void WinEDA_CleaningOptionsFrame::Init()
{
	m_Parent = NULL;
////@begin WinEDA_CleaningOptionsFrame member initialisation
    m_CleanViasOpt = NULL;
    m_MergetSegmOpt = NULL;
    m_DeleteunconnectedOpt = NULL;
    m_ConnectToPadsOpt = NULL;
////@end WinEDA_CleaningOptionsFrame member initialisation
}


/*!
 * Control creation for WinEDA_CleaningOptionsFrame
 */

void WinEDA_CleaningOptionsFrame::CreateControls()
{    
////@begin WinEDA_CleaningOptionsFrame content construction
    // Generated by DialogBlocks, 21/06/2007 19:58:26 (unregistered)

    WinEDA_CleaningOptionsFrame* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Static"));
    wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_CleanViasOpt = new wxCheckBox( itemDialog1, ID_CHECKBOX_CLEAN_VIAS, _("Delete redundant vias"), wxDefaultPosition, wxDefaultSize, 0 );
    m_CleanViasOpt->SetValue(false);
    itemStaticBoxSizer4->Add(m_CleanViasOpt, 0, wxALIGN_LEFT|wxALL, 5);

    m_MergetSegmOpt = new wxCheckBox( itemDialog1, ID_CHECKBOX_MERGE_SEGMENTS, _("Merge segments"), wxDefaultPosition, wxDefaultSize, 0 );
    m_MergetSegmOpt->SetValue(false);
    itemStaticBoxSizer4->Add(m_MergetSegmOpt, 0, wxALIGN_LEFT|wxALL, 5);

    m_DeleteunconnectedOpt = new wxCheckBox( itemDialog1, ID_CHECKBOX1, _("Delete unconnected tracks"), wxDefaultPosition, wxDefaultSize, 0 );
    m_DeleteunconnectedOpt->SetValue(false);
    itemStaticBoxSizer4->Add(m_DeleteunconnectedOpt, 0, wxALIGN_LEFT|wxALL, 5);

    m_ConnectToPadsOpt = new wxCheckBox( itemDialog1, ID_CHECKBOX, _("Connect to Pads"), wxDefaultPosition, wxDefaultSize, 0 );
    m_ConnectToPadsOpt->SetToolTip( _("Extend dangling tracks which partially cover a pad or via, all the way to pad or via center") );
    m_ConnectToPadsOpt->SetValue(false);
    itemStaticBoxSizer4->Add(m_ConnectToPadsOpt, 0, wxALIGN_LEFT|wxALL, 5);

    itemStaticBoxSizer4->Add(5, 5, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton11 = new wxButton( itemDialog1, ID_BUTTON_EXECUTE, _("Clean pcb"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton11->SetDefault();
    itemBoxSizer10->Add(itemButton11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    // Set validators
    m_CleanViasOpt->SetValidator( wxGenericValidator(& s_CleanVias) );
    m_MergetSegmOpt->SetValidator( wxGenericValidator(& s_MergeSegments) );
    m_DeleteunconnectedOpt->SetValidator( wxGenericValidator(& s_DeleteUnconnectedSegm) );
////@end WinEDA_CleaningOptionsFrame content construction
}


/*!
 * Should we show tooltips?
 */

bool WinEDA_CleaningOptionsFrame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WinEDA_CleaningOptionsFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WinEDA_CleaningOptionsFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WinEDA_CleaningOptionsFrame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WinEDA_CleaningOptionsFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WinEDA_CleaningOptionsFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WinEDA_CleaningOptionsFrame icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_EXECUTE
 */

void WinEDA_CleaningOptionsFrame::OnButtonExecuteClick( wxCommandEvent& event )
{
    s_CleanVias = m_CleanViasOpt->GetValue();
    s_MergeSegments = m_MergetSegmOpt->GetValue();
    s_DeleteUnconnectedSegm = m_DeleteunconnectedOpt->GetValue();
	s_ConnectToPads = m_ConnectToPadsOpt->GetValue();
	
	Clean_Pcb_Items(m_Parent, m_DC );
	
	Close(TRUE);
}










/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_WIN_EDA_CLEANINGOPTIONSFRAME
 */

void WinEDA_CleaningOptionsFrame::OnCloseWindow( wxCloseEvent& event )
{
    s_CleanVias = m_CleanViasOpt->GetValue();
    s_MergeSegments = m_MergetSegmOpt->GetValue();
    s_DeleteUnconnectedSegm = m_DeleteunconnectedOpt->GetValue();
	s_ConnectToPads = m_ConnectToPadsOpt->GetValue();
	
	event.Skip();
}

