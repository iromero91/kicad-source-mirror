///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 17 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOG_DIMENSION_EDITOR_BASE_H__
#define __DIALOG_DIMENSION_EDITOR_BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class DIALOG_SHIM;
class PCB_LAYER_BOX_SELECTOR;
class TEXT_CTRL_EVAL;

#include "dialog_shim.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/radiobox.h>
#include <wx/bmpcbox.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DIALOG_DIMENSION_EDITOR_BASE
///////////////////////////////////////////////////////////////////////////////
class DIALOG_DIMENSION_EDITOR_BASE : public DIALOG_SHIM
{
	private:
	
	protected:
		wxStaticText* m_staticTextDim;
		wxTextCtrl* m_Name;
		wxStaticText* m_staticTextSizeX;
		TEXT_CTRL_EVAL* m_TxtSizeXCtrl;
		wxStaticText* m_staticTextSizeY;
		TEXT_CTRL_EVAL* m_TxtSizeYCtrl;
		wxStaticText* m_staticTextWidth;
		TEXT_CTRL_EVAL* m_TxtWidthCtrl;
		wxStaticText* m_staticTextPosX;
		TEXT_CTRL_EVAL* m_textCtrlPosX;
		wxStaticText* m_staticTextPosY;
		TEXT_CTRL_EVAL* m_textCtrlPosY;
		wxRadioBox* m_rbMirror;
		wxStaticText* m_staticTextLayer;
		PCB_LAYER_BOX_SELECTOR* m_SelLayerBox;
		wxStaticLine* m_staticline1;
		wxStdDialogButtonSizer* m_sdbSizerBts;
		wxButton* m_sdbSizerBtsOK;
		wxButton* m_sdbSizerBtsCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOKClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DIALOG_DIMENSION_EDITOR_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Dimension Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DIALOG_DIMENSION_EDITOR_BASE();
	
};

#endif //__DIALOG_DIMENSION_EDITOR_BASE_H__
