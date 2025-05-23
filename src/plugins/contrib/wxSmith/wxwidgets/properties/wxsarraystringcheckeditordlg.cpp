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
* $Id: wxsarraystringcheckeditordlg.cpp 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/properties/wxsarraystringcheckeditordlg.cpp $
*/

#include "wxsarraystringcheckeditordlg.h"

//(*InternalHeaders(wxsArrayStringCheckEditorDlg)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(wxsArrayStringCheckEditorDlg)
const wxWindowID wxsArrayStringCheckEditorDlg::ID_TEXTCTRL1 = wxNewId();
const wxWindowID wxsArrayStringCheckEditorDlg::ID_BUTTON1 = wxNewId();
const wxWindowID wxsArrayStringCheckEditorDlg::ID_CHECKLISTBOX1 = wxNewId();
const wxWindowID wxsArrayStringCheckEditorDlg::ID_BUTTON2 = wxNewId();
const wxWindowID wxsArrayStringCheckEditorDlg::ID_BUTTON4 = wxNewId();
const wxWindowID wxsArrayStringCheckEditorDlg::ID_BUTTON3 = wxNewId();
const wxWindowID wxsArrayStringCheckEditorDlg::ID_BUTTON5 = wxNewId();
const wxWindowID wxsArrayStringCheckEditorDlg::ID_BUTTON6 = wxNewId();
const wxWindowID wxsArrayStringCheckEditorDlg::ID_BUTTON7 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxsArrayStringCheckEditorDlg,wxScrollingDialog)
    //(*EventTable(wxsArrayStringCheckEditorDlg)
    //*)
    EVT_LISTBOX(ID_CHECKLISTBOX1,wxsArrayStringCheckEditorDlg::OnStringListToggled)
END_EVENT_TABLE()

wxsArrayStringCheckEditorDlg::wxsArrayStringCheckEditorDlg(wxWindow* parent,wxArrayString& _Strings,wxArrayBool& _Bools,wxWindowID id):
    Strings(_Strings),
    Bools(_Bools)
{
    //(*Initialize(wxsArrayStringCheckEditorDlg)
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer3;
    wxBoxSizer* BoxSizer5;
    wxStaticLine* StaticLine1;
    wxStaticLine* StaticLine2;

    Create(parent, id, _("Choices:"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    EditArea = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    BoxSizer2->Add(EditArea, 1, wxEXPAND, 5);
    Button1 = new wxButton(this, ID_BUTTON1, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(Button1, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxEXPAND, 0);
    StaticLine1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("wxID_ANY"));
    BoxSizer1->Add(StaticLine1, 0, wxTOP|wxEXPAND, 5);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    StringList = new wxCheckListBox(this, ID_CHECKLISTBOX1, wxDefaultPosition, wxSize(180,255), 0, 0, 0, wxDefaultValidator, _T("ID_CHECKLISTBOX1"));
    BoxSizer3->Add(StringList, 1, wxEXPAND, 5);
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    Button2 = new wxButton(this, ID_BUTTON2, _("Update"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer4->Add(Button2, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    Button4 = new wxButton(this, ID_BUTTON4, _("Remove"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer4->Add(Button4, 0, wxTOP|wxALIGN_CENTER_HORIZONTAL, 5);
    Button3 = new wxButton(this, ID_BUTTON3, _("Up"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer4->Add(Button3, 0, wxTOP|wxALIGN_CENTER_HORIZONTAL, 5);
    Button5 = new wxButton(this, ID_BUTTON5, _("Down"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    BoxSizer4->Add(Button5, 0, wxTOP|wxALIGN_CENTER_HORIZONTAL, 5);
    BoxSizer3->Add(BoxSizer4, 0, wxLEFT|wxALIGN_TOP, 5);
    BoxSizer1->Add(BoxSizer3, 1, wxTOP|wxEXPAND, 5);
    StaticLine2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("wxID_ANY"));
    BoxSizer1->Add(StaticLine2, 0, wxTOP|wxEXPAND, 5);
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    Button6 = new wxButton(this, ID_BUTTON6, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
    BoxSizer5->Add(Button6, 1, wxALIGN_CENTER_VERTICAL, 5);
    Button7 = new wxButton(this, ID_BUTTON7, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
    BoxSizer5->Add(Button7, 1, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer5, 0, wxTOP|wxALIGN_RIGHT, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->SetSizeHints(this);
    Center();

    Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_ENTER,wxCommandEventHandler(wxsArrayStringCheckEditorDlg::OnButton1Click));
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsArrayStringCheckEditorDlg::OnButton1Click));
    Connect(ID_CHECKLISTBOX1,wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,wxCommandEventHandler(wxsArrayStringCheckEditorDlg::OnStringListToggled));
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsArrayStringCheckEditorDlg::OnButton2Click));
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsArrayStringCheckEditorDlg::OnButton4Click));
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsArrayStringCheckEditorDlg::OnButton3Click));
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsArrayStringCheckEditorDlg::OnButton5Click));
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsArrayStringCheckEditorDlg::OnButton6Click));
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxsArrayStringCheckEditorDlg::OnButton7Click));
    //*)

    for ( size_t i = 0; i<Strings.Count(); i++ )
    {
        StringList->Append(Strings[i]);
        StringList->Check(i,Bools[i]);
    }
}

wxsArrayStringCheckEditorDlg::~wxsArrayStringCheckEditorDlg()
{
    //(*Destroy(wxsArrayStringCheckEditorDlg)
    //*)
}


void wxsArrayStringCheckEditorDlg::OnButton1Click(cb_unused wxCommandEvent& event)
{
    StringList->Append(EditArea->GetValue());
}

void wxsArrayStringCheckEditorDlg::OnButton2Click(cb_unused wxCommandEvent& event)
{
    int Sel = StringList->GetSelection();
    if ( Sel == wxNOT_FOUND ) return;
    StringList->SetString(Sel,EditArea->GetValue());
}

void wxsArrayStringCheckEditorDlg::OnButton4Click(cb_unused wxCommandEvent& event)
{
    int Sel = StringList->GetSelection();
    if ( Sel == wxNOT_FOUND ) return;
    StringList->Delete(Sel);
}

void wxsArrayStringCheckEditorDlg::OnButton3Click(cb_unused wxCommandEvent& event)
{
    int Sel = StringList->GetSelection();
    if ( Sel == wxNOT_FOUND ) return;
    if ( Sel < 1 ) return;
    bool Checked = StringList->IsChecked(Sel);
    wxString Str = StringList->GetString(Sel);
    StringList->Delete(Sel);
    StringList->Insert(Str,--Sel);
    StringList->Check(Sel,Checked);
    StringList->SetSelection(Sel);
}

void wxsArrayStringCheckEditorDlg::OnButton5Click(cb_unused wxCommandEvent& event)
{
    int Sel = StringList->GetSelection();
    if ( Sel == wxNOT_FOUND ) return;
    if ( Sel >= (int)StringList->GetCount()-1 ) return;
    bool Checked = StringList->IsChecked(Sel);
    wxString Str = StringList->GetString(Sel);
    StringList->Delete(Sel);
    StringList->Insert(Str,++Sel);
    StringList->Check(Sel,Checked);
    StringList->SetSelection(Sel);
}

void wxsArrayStringCheckEditorDlg::OnButton6Click(cb_unused wxCommandEvent& event)
{
    size_t Count = StringList->GetCount();
    Strings.Clear();
    Bools.Clear();
    for ( size_t i=0; i<Count; i++ )
    {
        Strings.Add(StringList->GetString(i));
        Bools.Add(StringList->IsChecked(i));
    }
    EndModal(wxID_OK);
}

void wxsArrayStringCheckEditorDlg::OnButton7Click(cb_unused wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void wxsArrayStringCheckEditorDlg::OnStringListToggled(cb_unused wxCommandEvent& event)
{
    EditArea->SetValue(StringList->GetStringSelection());
}
