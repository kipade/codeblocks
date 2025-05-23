/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 13547 $
* $Id: wxsfonteditordlg.cpp 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/properties/wxsfonteditordlg.cpp $
*/

#include "wxsfonteditordlg.h"
#include "wxsfontfaceeditordlg.h"
#include <wx/fontmap.h>
#include <wx/fontdlg.h>
#include <wx/settings.h>
#include "globals.h"

#define FT_DFAULT   0x00
#define FT_CUSTOM   0x01
#define FT_SYSTEM   0x02

#define FFAM_DECORATIVE   0x00
#define FFAM_ROMAN        0x01
#define FFAM_SCRIPT       0x02
#define FFAM_SWISS        0x03
#define FFAM_MODERN       0x04
#define FFAM_TELETYPE     0x05

//(*InternalHeaders(wxsFontEditorDlg)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(wxsFontEditorDlg)
const wxWindowID wxsFontEditorDlg::ID_CHOICE1 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_CHECKBOX8 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_STATICTEXT1 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_CHOICE2 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_CHECKBOX2 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_COMBOBOX1 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_CHECKBOX1 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_CHOICE3 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_CHECKBOX7 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_SPINCTRL1 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_CHECKBOX6 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_TEXTCTRL1 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_CHECKBOX3 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_RADIOBUTTON1 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_RADIOBUTTON2 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_RADIOBUTTON3 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_CHECKBOX4 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_RADIOBUTTON4 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_RADIOBUTTON5 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_RADIOBUTTON6 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_CHECKBOX5 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_RADIOBUTTON7 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_RADIOBUTTON8 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_LISTBOX1 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_BUTTON4 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_BUTTON5 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_BUTTON6 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_BUTTON8 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_BUTTON7 = wxNewId();
const wxWindowID wxsFontEditorDlg::ID_TEXTCTRL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxsFontEditorDlg,wxScrollingDialog)
    //(*EventTable(wxsFontEditorDlg)
    //*)
    EVT_BUTTON(wxID_OK,wxsFontEditorDlg::OnButton2Click)
END_EVENT_TABLE()

wxsFontEditorDlg::wxsFontEditorDlg(wxWindow* parent,wxsFontData& _Data,wxWindowID id):
    Data(_Data)
{
    Initialized = false;
    //(*Initialize(wxsFontEditorDlg)
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer6;
    wxStaticBoxSizer* StaticBoxSizer7;
    wxStdDialogButtonSizer* StdDialogButtonSizer1;

    Create(parent, id, _("Advanced font settings"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer5 = new wxBoxSizer(wxVERTICAL);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Font type"));
    FontType = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    FontType->SetSelection( FontType->Append(_("Default font")) );
    FontType->Append(_("Custom font"));
    FontType->Append(_("System-based font"));
    StaticBoxSizer1->Add(FontType, 1, wxEXPAND, 5);
    BoxSizer5->Add(StaticBoxSizer1, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Settings"));
    FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
    FlexGridSizer2->AddGrowableCol(1);
    BaseFontSizer = new wxBoxSizer(wxHORIZONTAL);
    BaseFontUse = new wxCheckBox(this, ID_CHECKBOX8, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX8"));
    BaseFontUse->SetValue(true);
    BaseFontUse->Disable();
    BaseFontSizer->Add(BaseFontUse, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    BaseFontTxt = new wxStaticText(this, ID_STATICTEXT1, _("Base Font:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BaseFontSizer->Add(BaseFontTxt, 1, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(BaseFontSizer, 1, wxEXPAND, 5);
    BaseFontVal = new wxChoice(this, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
    BaseFontVal->SetSelection( BaseFontVal->Append(_("wxSYS_OEM_FIXED_FONT")) );
    BaseFontVal->Append(_("wxSYS_ANSI_FIXED_FONT"));
    BaseFontVal->Append(_("wxSYS_ANSI_VAR_FONT"));
    BaseFontVal->Append(_("wxSYS_SYSTEM_FONT"));
    BaseFontVal->Append(_("wxSYS_DEVICE_DEFAULT_FONT"));
    BaseFontVal->Append(_("wxSYS_DEFAULT_PALETTE"));
    BaseFontVal->Append(_("wxSYS_SYSTEM_FIXED_FONT"));
    BaseFontVal->Append(_("wxSYS_DEFAULT_GUI_FONT"));
    FlexGridSizer2->Add(BaseFontVal, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    FamUse = new wxCheckBox(this, ID_CHECKBOX2, _("Family:"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    FamUse->SetValue(false);
    FlexGridSizer2->Add(FamUse, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
    FamVal = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    FamVal->SetSelection( FamVal->Append(_("Decorative")) );
    FamVal->Append(_("Roman"));
    FamVal->Append(_("Script"));
    FamVal->Append(_("Swiss"));
    FamVal->Append(_("Modern"));
    FamVal->Append(_("Teletype"));
    FlexGridSizer2->Add(FamVal, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    EncodUse = new wxCheckBox(this, ID_CHECKBOX1, _("Font encoding:"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    EncodUse->SetValue(false);
    FlexGridSizer2->Add(EncodUse, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
    EncodVal = new wxChoice(this, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
    FlexGridSizer2->Add(EncodVal, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    SizeUse = new wxCheckBox(this, ID_CHECKBOX7, _("Size:"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
    SizeUse->SetValue(false);
    FlexGridSizer2->Add(SizeUse, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
    SizeVal = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, 0, -1000, 1000, 0, _T("ID_SPINCTRL1"));
    SizeVal->SetValue(_T("0"));
    FlexGridSizer2->Add(SizeVal, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    RelSizeUse = new wxCheckBox(this, ID_CHECKBOX6, _("Relative size:"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
    RelSizeUse->SetValue(false);
    FlexGridSizer2->Add(RelSizeUse, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
    RelSizeVal = new wxTextCtrl(this, ID_TEXTCTRL1, _T("1.0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    FlexGridSizer2->Add(RelSizeVal, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticBoxSizer2->Add(FlexGridSizer2, 0, wxEXPAND, 5);
    BoxSizer5->Add(StaticBoxSizer2, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, this, wxEmptyString);
    StyleUse = new wxCheckBox(this, ID_CHECKBOX3, _("Style"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    StyleUse->SetValue(false);
    StaticBoxSizer3->Add(StyleUse, 0, wxALL|wxEXPAND, 5);
    StyleNorm = new wxRadioButton(this, ID_RADIOBUTTON1, _("Normal"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
    StaticBoxSizer3->Add(StyleNorm, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    StyleItal = new wxRadioButton(this, ID_RADIOBUTTON2, _("Italic"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
    StaticBoxSizer3->Add(StyleItal, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    StyleSlant = new wxRadioButton(this, ID_RADIOBUTTON3, _("Slant"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
    StaticBoxSizer3->Add(StyleSlant, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer2->Add(StaticBoxSizer3, 1, wxRIGHT|wxEXPAND, 5);
    StaticBoxSizer4 = new wxStaticBoxSizer(wxVERTICAL, this, wxEmptyString);
    WeightUse = new wxCheckBox(this, ID_CHECKBOX4, _("Weight"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    WeightUse->SetValue(false);
    StaticBoxSizer4->Add(WeightUse, 0, wxALL|wxEXPAND, 5);
    WeightLight = new wxRadioButton(this, ID_RADIOBUTTON4, _("Light"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
    StaticBoxSizer4->Add(WeightLight, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    WeightNorm = new wxRadioButton(this, ID_RADIOBUTTON5, _("Normal"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON5"));
    StaticBoxSizer4->Add(WeightNorm, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    WeightBold = new wxRadioButton(this, ID_RADIOBUTTON6, _("Bold"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON6"));
    StaticBoxSizer4->Add(WeightBold, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer2->Add(StaticBoxSizer4, 1, wxRIGHT|wxEXPAND, 5);
    StaticBoxSizer5 = new wxStaticBoxSizer(wxVERTICAL, this, wxEmptyString);
    UnderUse = new wxCheckBox(this, ID_CHECKBOX5, _("Underlined"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
    UnderUse->SetValue(false);
    StaticBoxSizer5->Add(UnderUse, 0, wxALL|wxEXPAND, 5);
    UnderYes = new wxRadioButton(this, ID_RADIOBUTTON7, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON7"));
    StaticBoxSizer5->Add(UnderYes, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    UnderNo = new wxRadioButton(this, ID_RADIOBUTTON8, _("No"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON8"));
    StaticBoxSizer5->Add(UnderNo, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer2->Add(StaticBoxSizer5, 1, wxRIGHT|wxEXPAND, 5);
    BoxSizer5->Add(BoxSizer2, 1, wxBOTTOM|wxLEFT|wxEXPAND, 5);
    BoxSizer4->Add(BoxSizer5, 1, wxTOP|wxRIGHT|wxEXPAND, 5);
    BoxSizer6 = new wxBoxSizer(wxVERTICAL);
    StaticBoxSizer6 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Faces"));
    FaceList = new wxListBox(this, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    StaticBoxSizer6->Add(FaceList, 1, wxEXPAND, 5);
    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    FaceAdd = new wxButton(this, ID_BUTTON4, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer3->Add(FaceAdd, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    FaceDel = new wxButton(this, ID_BUTTON5, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    BoxSizer3->Add(FaceDel, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    FaceEdit = new wxButton(this, ID_BUTTON6, _("Edit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
    BoxSizer3->Add(FaceEdit, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    FaceUp = new wxButton(this, ID_BUTTON8, _("Move Up"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
    BoxSizer3->Add(FaceUp, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    FaceDown = new wxButton(this, ID_BUTTON7, _("Move Down"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
    BoxSizer3->Add(FaceDown, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticBoxSizer6->Add(BoxSizer3, 0, wxALIGN_TOP, 0);
    BoxSizer6->Add(StaticBoxSizer6, 1, wxTOP|wxRIGHT|wxEXPAND, 5);
    BoxSizer4->Add(BoxSizer6, 1, wxEXPAND, 5);
    BoxSizer1->Add(BoxSizer4, 1, wxEXPAND, 5);
    StaticBoxSizer7 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Test area"));
    TestArea = new wxTextCtrl(this, ID_TEXTCTRL2, _("This is sample text"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    StaticBoxSizer7->Add(TestArea, 1, wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(StaticBoxSizer7, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
    StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
    StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
    StdDialogButtonSizer1->Realize();
    BoxSizer1->Add(StdDialogButtonSizer1, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->SetSizeHints(this);
    Center();

    Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdateContent));
    Connect(ID_CHECKBOX8,wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnBaseFontUseChange));
    Connect(ID_CHOICE2,wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdateContent));
    Connect(ID_COMBOBOX1,wxEVT_COMMAND_COMBOBOX_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdateContent));
    Connect(ID_CHOICE3,wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_CHECKBOX7,wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdateContent));
    Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,wxSpinEventHandler(wxsFontEditorDlg::OnSizeValChange));
    Connect(ID_CHECKBOX6,wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdateContent));
    Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_CHECKBOX3,wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdateContent));
    Connect(ID_RADIOBUTTON1,wxEVT_COMMAND_RADIOBUTTON_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_RADIOBUTTON2,wxEVT_COMMAND_RADIOBUTTON_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_RADIOBUTTON3,wxEVT_COMMAND_RADIOBUTTON_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_CHECKBOX4,wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdateContent));
    Connect(ID_RADIOBUTTON4,wxEVT_COMMAND_RADIOBUTTON_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_RADIOBUTTON5,wxEVT_COMMAND_RADIOBUTTON_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_RADIOBUTTON6,wxEVT_COMMAND_RADIOBUTTON_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_CHECKBOX5,wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdateContent));
    Connect(ID_RADIOBUTTON7,wxEVT_COMMAND_RADIOBUTTON_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_RADIOBUTTON8,wxEVT_COMMAND_RADIOBUTTON_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdatePreview));
    Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,wxCommandEventHandler(wxsFontEditorDlg::OnUpdateContent));
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnFaceAddClick));
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnFaceDelClick));
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnFaceEditClick));
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnFaceUpClick));
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsFontEditorDlg::OnFaceDownClick));
    //*)
    Initialized = true;

    size_t Count = wxFontMapper::Get()->GetSupportedEncodingsCount();
    for ( size_t i = 0; i<Count; i++ )
    {
        wxFontEncoding Enc = wxFontMapper::Get()->GetEncoding(i);
        if ( Enc == wxFONTENCODING_DEFAULT || Enc == wxFONTENCODING_SYSTEM ) continue;
        wxString Name = wxFontMapper::Get()->GetEncodingName(Enc);
        Encodings.Add(Name);
        EncodVal->Append(wxFontMapper::Get()->GetEncodingDescription(Enc));
    }

    ReadData(Data);
    UpdateContent();
    UpdatePreview();
}

wxsFontEditorDlg::~wxsFontEditorDlg()
{
    //(*Destroy(wxsFontEditorDlg)
    //*)
}

void wxsFontEditorDlg::OnButton1Click(cb_unused wxCommandEvent& event)
{
}

void wxsFontEditorDlg::UpdateContent()
{
    int Type = FontType->GetSelection();

    bool TypeSystem = Type == FT_SYSTEM;
    bool TypeNotDef = Type != FT_DFAULT;
    bool RelSizeEn  = !SizeUse->GetValue();

    BaseFontUse->Show(TypeSystem);
    BaseFontTxt->Show(TypeSystem);
    BaseFontVal->Show(TypeSystem);
    BaseFontVal->Enable(BaseFontUse->GetValue());
    FamUse->Enable(TypeNotDef);
    FamVal->Enable(TypeNotDef && FamUse->GetValue());
    EncodUse->Enable(TypeNotDef);
    EncodVal->Enable(TypeNotDef && EncodUse->GetValue());
    SizeUse->Enable(TypeNotDef);
    SizeVal->Enable(TypeNotDef && SizeUse->GetValue());
    RelSizeUse->Show(TypeSystem);
    RelSizeVal->Show(TypeSystem);
    RelSizeUse->Enable(RelSizeEn);
    RelSizeVal->Enable(RelSizeEn && RelSizeUse->GetValue());
    FaceList->Enable(TypeNotDef);
    FaceAdd->Enable(TypeNotDef);
    FaceDel->Enable(TypeNotDef && FaceList->GetSelection() != wxNOT_FOUND);
    FaceEdit->Enable(TypeNotDef && FaceList->GetSelection() != wxNOT_FOUND);
    FaceUp->Enable(TypeNotDef && FaceList->GetSelection() != wxNOT_FOUND);
    FaceDown->Enable(TypeNotDef && FaceList->GetSelection() != wxNOT_FOUND);
    StyleUse->Enable(TypeNotDef);
    StyleNorm->Enable(TypeNotDef && StyleUse->GetValue());
    StyleItal->Enable(TypeNotDef && StyleUse->GetValue());
    StyleSlant->Enable(TypeNotDef && StyleUse->GetValue());
    WeightUse->Enable(TypeNotDef);
    WeightLight->Enable(TypeNotDef && WeightUse->GetValue());
    WeightNorm->Enable(TypeNotDef && WeightUse->GetValue());
    WeightBold->Enable(TypeNotDef && WeightUse->GetValue());
    UnderUse->Enable(TypeNotDef);
    UnderYes->Enable(TypeNotDef && UnderUse->GetValue());
    UnderNo->Enable(TypeNotDef && UnderUse->GetValue());
    Layout();
}

void wxsFontEditorDlg::UpdatePreview()
{
    wxsFontData TempData;
    StoreData(TempData);
    wxFont Font = TempData.BuildFont();
    if ( FaceList->GetSelection() != wxNOT_FOUND )
    {
        Font.SetFaceName(FaceList->GetStringSelection());
    }
    TestArea->SetFont(Font);
}

void wxsFontEditorDlg::ReadData(wxsFontData& _Data)
{
    if ( _Data.IsDefault )
    {
        FontType->SetSelection(FT_DFAULT);
        return;
    }

    if ( !_Data.SysFont.empty() && _Data.HasSysFont )
    {
        FontType->SetSelection(FT_SYSTEM);
        if ( _Data.HasSysFont )
        {
            BaseFontUse->SetValue(true);
            if ( !BaseFontVal->SetStringSelection(_Data.SysFont) )
            {
                BaseFontVal->SetSelection(0);
            }
        }

        if ( !_Data.HasSize && _Data.HasRelativeSize )
        {
            RelSizeUse->SetValue(true);

            // TODO: Shouldn't we also use non-locale-printf fix ?
            RelSizeVal->SetValue(wxString::Format(_T("%f"),_Data.RelativeSize));
        }
    }
    else
    {
        FontType->SetSelection(FT_CUSTOM);
    }

    if ( _Data.HasFamily )
    {
        FamUse->SetValue(true);
        switch ( _Data.Family )
        {
            case wxFONTFAMILY_DECORATIVE: FamVal->SetSelection(FFAM_DECORATIVE); break;
            case wxFONTFAMILY_ROMAN:      FamVal->SetSelection(FFAM_ROMAN); break;
            case wxFONTFAMILY_SCRIPT:     FamVal->SetSelection(FFAM_SCRIPT); break;
            case wxFONTFAMILY_SWISS:      FamVal->SetSelection(FFAM_SWISS); break;
            case wxFONTFAMILY_MODERN:     FamVal->SetSelection(FFAM_MODERN); break;
            case wxFONTFAMILY_TELETYPE:   FamVal->SetSelection(FFAM_TELETYPE); break;
            case wxFONTFAMILY_DEFAULT: // fall-though
            case wxFONTFAMILY_UNKNOWN: // fall-though
            default:                      FamUse->SetValue(false);
        }
    }

    if ( _Data.HasEncoding )
    {
        int Index = Encodings.Index(_Data.Encoding);
        if ( Index != wxNOT_FOUND )
        {
            EncodUse->SetValue(true);
            EncodVal->SetSelection(Index);
        }
    }

    if ( _Data.HasSize )
    {
        SizeUse->SetValue(true);
        SizeVal->SetValue(_Data.Size);
    }

    FaceList->Append(_Data.Faces);
    FaceList->SetSelection(0);

    if ( _Data.HasStyle )
    {
        StyleUse->SetValue(true);
        StyleNorm->SetValue(_Data.Style == wxFONTSTYLE_NORMAL);
        StyleItal->SetValue(_Data.Style == wxFONTSTYLE_ITALIC);
        StyleSlant->SetValue(_Data.Style == wxFONTSTYLE_SLANT);
    }

    if ( _Data.HasWeight )
    {
        WeightUse->SetValue(true);
        WeightNorm->SetValue(_Data.Weight == wxFONTWEIGHT_NORMAL);
        WeightLight->SetValue(_Data.Weight == wxFONTWEIGHT_LIGHT);
        WeightBold->SetValue(_Data.Weight == wxFONTWEIGHT_BOLD);
    }

    if ( _Data.HasUnderlined )
    {
        UnderUse->SetValue(true);
        UnderYes->SetValue(_Data.Underlined);
        UnderNo->SetValue(!_Data.Underlined);
    }
}

void wxsFontEditorDlg::StoreData(wxsFontData& _Data)
{
    if ( FontType->GetSelection() == FT_DFAULT )
    {
        _Data.IsDefault = true;
        _Data.HasSize = false;
        _Data.HasStyle = false;
        _Data.HasWeight = false;
        _Data.HasUnderlined = false;
        _Data.HasFamily = false;
        _Data.HasEncoding = false;
        _Data.HasSysFont = false;
        _Data.HasRelativeSize = false;
        _Data.Faces.Clear();
        return;
    }

    Data.IsDefault = false;
    if ( FontType->GetSelection() == FT_CUSTOM )
    {
        Data.HasSysFont = false;
        Data.HasRelativeSize = false;
    }
    else
    {
        if ( (Data.HasSysFont = BaseFontUse->GetValue()) )
        {
            Data.SysFont = BaseFontVal->GetStringSelection();
        }

        if ( (Data.HasRelativeSize = ( !SizeUse->GetValue() && RelSizeUse->GetValue() )) )
        {
            wxString val = RelSizeVal->GetValue();
            val.ToDouble(&Data.RelativeSize);
        }
    }

    if ( (Data.HasFamily = FamUse->GetValue()) )
    {
        switch ( FamVal->GetSelection() )
        {
            case FFAM_DECORATIVE: Data.Family = wxFONTFAMILY_DECORATIVE; break;
            case FFAM_ROMAN:      Data.Family = wxFONTFAMILY_ROMAN;      break;
            case FFAM_SCRIPT:     Data.Family = wxFONTFAMILY_SCRIPT;     break;
            case FFAM_SWISS:      Data.Family = wxFONTFAMILY_SWISS;      break;
            case FFAM_MODERN:     Data.Family = wxFONTFAMILY_MODERN;     break;
            case FFAM_TELETYPE:   Data.Family = wxFONTFAMILY_TELETYPE;   break;
            default:              Data.HasFamily = false;
        }
    }

    if ( (Data.HasEncoding = EncodUse->GetValue()) )
    {
        const int Index = EncodVal->GetSelection();
        if ( Index == wxNOT_FOUND )
        {
            Data.HasEncoding = false;
        }
        else
        {
            Data.Encoding = Encodings[Index];
        }
    }

    if ( (Data.HasSize = SizeUse->GetValue()) )
    {
        Data.Size = SizeVal->GetValue();
    }

    if ( (Data.HasStyle = StyleUse->GetValue()) )
    {
        if ( StyleNorm->GetValue() ) Data.Style = wxFONTSTYLE_NORMAL;
        if ( StyleItal->GetValue() ) Data.Style = wxFONTSTYLE_ITALIC;
        if ( StyleSlant->GetValue() ) Data.Style = wxFONTSTYLE_SLANT;
    }

    if ( (Data.HasWeight = WeightUse->GetValue()) )
    {
        if ( WeightLight->GetValue() ) Data.Weight = wxFONTWEIGHT_LIGHT;
        if ( WeightNorm->GetValue() ) Data.Weight = wxFONTWEIGHT_NORMAL;
        if ( WeightBold->GetValue() ) Data.Weight = wxFONTWEIGHT_BOLD;
    }

    if ( (Data.HasUnderlined = UnderUse->GetValue()) )
    {
        if ( UnderYes->GetValue() ) Data.Underlined = true;
        if ( UnderNo->GetValue() ) Data.Underlined = false;
    }

    Data.Faces.Clear();
    size_t Count = FaceList->GetCount();
    for ( size_t i=0; i<Count; i++ )
    {
        Data.Faces.Add(FaceList->GetString(i));
    }
}

void wxsFontEditorDlg::OnUpdateContent(cb_unused wxCommandEvent& event)
{
    UpdateContent();
    UpdatePreview();
}

void wxsFontEditorDlg::OnButton2Click(cb_unused wxCommandEvent& event)
{
    StoreData(Data);
    EndModal(wxID_OK);
}

void wxsFontEditorDlg::OnButton1Click1(cb_unused wxCommandEvent& event)
{
    wxFont Font  = ::wxGetFontFromUser();
    if ( !Font.Ok() ) return;

    FontType->SetSelection(FT_CUSTOM);
    FamUse->SetValue(true);
    switch ( Font.GetFamily() )
    {
        case wxFONTFAMILY_DECORATIVE: FamVal->SetSelection(FFAM_DECORATIVE); break;
        case wxFONTFAMILY_ROMAN:      FamVal->SetSelection(FFAM_ROMAN); break;
        case wxFONTFAMILY_SCRIPT:     FamVal->SetSelection(FFAM_SCRIPT); break;
        case wxFONTFAMILY_SWISS:      FamVal->SetSelection(FFAM_SWISS); break;
        case wxFONTFAMILY_MODERN:     FamVal->SetSelection(FFAM_MODERN); break;
        case wxFONTFAMILY_TELETYPE:   FamVal->SetSelection(FFAM_TELETYPE); break;
        case wxFONTFAMILY_UNKNOWN:
        case wxFONTFAMILY_DEFAULT:
        default:                      FamUse->SetValue(false);
    }

    EncodUse->SetValue(false);
    SizeUse->SetValue(true);
    SizeVal->SetValue(Font.GetPointSize());
    RelSizeUse->SetValue(false);

    FaceList->Clear();
    FaceList->Append(Font.GetFaceName());
    FaceList->SetSelection(0);

    StyleNorm->SetValue(Font.GetStyle()==wxFONTSTYLE_NORMAL);
    StyleItal->SetValue(Font.GetStyle()==wxFONTSTYLE_ITALIC);
    StyleSlant->SetValue(Font.GetStyle()==wxFONTSTYLE_SLANT);
    StyleUse->SetValue(Font.GetStyle()!=wxFONTSTYLE_NORMAL);

    WeightLight->SetValue(Font.GetWeight()==wxFONTWEIGHT_LIGHT);
    WeightNorm->SetValue(Font.GetWeight()==wxFONTWEIGHT_NORMAL);
    WeightBold->SetValue(Font.GetWeight()==wxFONTWEIGHT_BOLD);
    WeightUse->SetValue(Font.GetWeight()!=wxFONTWEIGHT_NORMAL);

    UnderYes->SetValue(Font.GetUnderlined());
    UnderNo->SetValue(!Font.GetUnderlined());
    UnderUse->SetValue(!Font.GetUnderlined());

    UpdateContent();
    UpdatePreview();
}

void wxsFontEditorDlg::OnFaceAddClick(cb_unused wxCommandEvent& event)
{
    wxString Face;
    wxsFontFaceEditorDlg Dlg(this,Face);
    PlaceWindow(&Dlg);
    if ( Dlg.ShowModal() == wxID_OK && !Face.empty() )
    {
        FaceList->Append(Face);
        FaceList->SetSelection(FaceList->GetCount()-1);
        UpdateContent();
        UpdatePreview();
    }
}

void wxsFontEditorDlg::OnFaceDelClick(cb_unused wxCommandEvent& event)
{
    int Index = FaceList->GetSelection();
    if ( Index != wxNOT_FOUND )
    {
        FaceList->Delete(Index);
        if ( Index < (int)FaceList->GetCount() )
        {
            FaceList->SetSelection(Index);
        }
        else
        {
            FaceList->SetSelection(Index-1);
        }
    }
    UpdateContent();
    UpdatePreview();
}

void wxsFontEditorDlg::OnFaceEditClick(cb_unused wxCommandEvent& event)
{
    int Index = FaceList->GetSelection();
    if ( Index != wxNOT_FOUND )
    {
        wxString Face = FaceList->GetString(Index);
        wxsFontFaceEditorDlg Dlg(this,Face);
        PlaceWindow(&Dlg);
        if ( Dlg.ShowModal() == wxID_OK )
        {
            FaceList->SetString(Index,Face);
        }
    }
    UpdatePreview();
}

void wxsFontEditorDlg::OnFaceUpClick(cb_unused wxCommandEvent& event)
{
    int Index = FaceList->GetSelection();
    if ( Index != wxNOT_FOUND && Index > 0 )
    {
        wxString Face = FaceList->GetString(Index);
        FaceList->Delete(Index);
        FaceList->Insert(Face,Index-1);
    }
    UpdatePreview();
}

void wxsFontEditorDlg::OnFaceDownClick(cb_unused wxCommandEvent& event)
{
    int Index = FaceList->GetSelection();
    if ( Index != wxNOT_FOUND && Index < (int)FaceList->GetCount()-1 )
    {
        wxString Face = FaceList->GetString(Index);
        FaceList->Delete(Index);
        FaceList->Insert(Face,Index+1);
    }
    UpdatePreview();
}

void wxsFontEditorDlg::OnUpdatePreview(cb_unused wxCommandEvent& event)
{
    if ( Initialized )
    {
        UpdatePreview();
    }
}

void wxsFontEditorDlg::OnSizeValChange(cb_unused wxSpinEvent& event)
{
    if ( Initialized )
    {
        UpdatePreview();
    }
}

void wxsFontEditorDlg::OnBaseFontUseChange(cb_unused wxCommandEvent& event)
{
    BaseFontUse->SetValue(true);
}
