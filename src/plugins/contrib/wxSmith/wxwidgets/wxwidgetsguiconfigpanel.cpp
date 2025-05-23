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
* $Id: wxwidgetsguiconfigpanel.cpp 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxwidgetsguiconfigpanel.cpp $
*/

#include "wxwidgetsguiconfigpanel.h"
#include "../wxsproject.h"

#include <wx/tokenzr.h>
#include <wx/filedlg.h>

//(*InternalHeaders(wxWidgetsGUIConfigPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <prep.h>

//(*IdInit(wxWidgetsGUIConfigPanel)
const wxWindowID wxWidgetsGUIConfigPanel::ID_TEXTCTRL1 = wxNewId();
const wxWindowID wxWidgetsGUIConfigPanel::ID_BUTTON1 = wxNewId();
const wxWindowID wxWidgetsGUIConfigPanel::ID_BUTTON2 = wxNewId();
const wxWindowID wxWidgetsGUIConfigPanel::ID_COMBOBOX1 = wxNewId();
const wxWindowID wxWidgetsGUIConfigPanel::ID_CHECKBOX2 = wxNewId();
const wxWindowID wxWidgetsGUIConfigPanel::ID_CHECKBOX1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxWidgetsGUIConfigPanel,cbConfigurationPanel)
    //(*EventTable(wxWidgetsGUIConfigPanel)
    //*)
END_EVENT_TABLE()

wxWidgetsGUIConfigPanel::wxWidgetsGUIConfigPanel(wxWindow* parent,wxWidgetsGUI* GUI): m_GUI(GUI)
{
    static const wxWindowID id = -1;
    //(*Initialize(wxWidgetsGUIConfigPanel)
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer3;
    wxBoxSizer* BoxSizer4;
    wxButton* Button1;
    wxButton* Button2;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxStaticBoxSizer* StaticBoxSizer2;
    wxStaticBoxSizer* StaticBoxSizer3;
    wxStaticText* StaticText1;
    wxStaticText* StaticText2;

    Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Automatically loaded resources:"));
    StaticText1 = new wxStaticText(this, wxID_ANY, _("Each line will be passed as file name to\nwxXmlResource::Get()->Load."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("wxID_ANY"));
    StaticBoxSizer1->Add(StaticText1, 0, wxEXPAND, 5);
    AutoLoad = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticBoxSizer1->Add(AutoLoad, 1, wxTOP|wxEXPAND, 5);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    Button1 = new wxButton(this, ID_BUTTON1, _("Add file"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer3->Add(Button1, 0, wxEXPAND, 5);
    BoxSizer3->Add(-1,-1,1, wxALIGN_CENTER_VERTICAL, 5);
    Button2 = new wxButton(this, ID_BUTTON2, _("Clear all"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer3->Add(Button2, 0, wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(BoxSizer3, 0, wxTOP|wxEXPAND, 5);
    BoxSizer2->Add(StaticBoxSizer1, 1, wxEXPAND, 5);
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Main resource"));
    MainRes = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY, wxDefaultValidator, _T("ID_COMBOBOX1"));
    StaticBoxSizer2->Add(MainRes, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    StaticText2 = new wxStaticText(this, wxID_ANY, _("Window selected here will be created and\nshown while initializing application.\nYou should use one of frames here.\nUsing dialog as main resource is also\nacceptable but may lead to problems\nwith terminating application\nafter closing main dialog."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("wxID_ANY"));
    StaticBoxSizer2->Add(StaticText2, 0, wxTOP|wxALIGN_CENTER_HORIZONTAL, 5);
    BoxSizer4->Add(StaticBoxSizer2, 1, wxLEFT|wxEXPAND, 5);
    StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, this, _("XRC system initialization"));
    InitAll = new wxCheckBox(this, ID_CHECKBOX2, _("Call wxXmlResource::InitAllHandlers()"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    InitAll->SetValue(false);
    StaticBoxSizer3->Add(InitAll, 0, wxEXPAND, 5);
    InitAllNecessary = new wxCheckBox(this, ID_CHECKBOX1, _("Only when necessary"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    InitAllNecessary->SetValue(false);
    InitAllNecessary->Disable();
    StaticBoxSizer3->Add(InitAllNecessary, 0, wxTOP|wxEXPAND, 5);
    BoxSizer4->Add(StaticBoxSizer3, 0, wxTOP|wxLEFT|wxEXPAND, 5);
    BoxSizer2->Add(BoxSizer4, 0, wxEXPAND, 5);
    BoxSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxWidgetsGUIConfigPanel::OnButton1Click));
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(wxWidgetsGUIConfigPanel::OnButton2Click));
    Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(wxWidgetsGUIConfigPanel::OnInitAllChange));
    //*)

    for ( size_t i=0; i<m_GUI->m_LoadedResources.Count(); ++i )
    {
        AutoLoad->AppendText(m_GUI->m_LoadedResources[i]);
        AutoLoad->AppendText(_T("\n"));
    }

    MainRes->Append(_("-- None --"));
    wxArrayString Resources;
    m_GUI->EnumerateMainResources(Resources);
    MainRes->Append(Resources);
    if ( m_GUI->m_MainResource.empty() )
    {
        MainRes->SetValue(_("-- None --"));
    }
    else
    {
        MainRes->SetValue(m_GUI->m_MainResource);
    }

    InitAll->SetValue(m_GUI->m_CallInitAll);
    InitAllNecessary->Enable(m_GUI->m_CallInitAll);
    InitAllNecessary->SetValue(m_GUI->m_CallInitAllNecessary);
}

wxWidgetsGUIConfigPanel::~wxWidgetsGUIConfigPanel()
{
    //(*Destroy(wxWidgetsGUIConfigPanel)
    //*)
}


void wxWidgetsGUIConfigPanel::OnInitAllChange(cb_unused wxCommandEvent& event)
{
    InitAllNecessary->Enable(InitAll->GetValue());
}

void wxWidgetsGUIConfigPanel::OnButton1Click(cb_unused wxCommandEvent& event)
{
    wxString FileName = ::wxFileSelector(
        _("Select resource file"),
        _T(""),_T(""),_T(""),
        _("XRC files (*.xrc)|*.xrc|"
          "Zipped files (*.zip)|*.zip|"
          "All files (*.*)|*.*"),
        wxFD_OPEN | wxFD_FILE_MUST_EXIST | compatibility::wxHideReadonly);

    if ( !FileName.empty() )
    {
        wxFileName FN(FileName);
        FN.MakeRelativeTo(m_GUI->GetProjectPath());
        wxString Ext = FN.GetExt();
        FileName = FN.GetFullPath();
        if ( Ext == _T("zip") || Ext == _T("ZIP") )
        {
            FileName.Append(_T("#zip:*.xrc"));
        }
        wxString CurrentContent = AutoLoad->GetValue();
        if ( CurrentContent.Length() && ( CurrentContent[CurrentContent.Length()-1] != _T('\n') ) )
        {
            AutoLoad->AppendText(_T("\n"));
        }
        AutoLoad->AppendText(FileName);
        AutoLoad->AppendText(_T("\n"));
    }
}

void wxWidgetsGUIConfigPanel::OnButton2Click(cb_unused wxCommandEvent& event)
{
    AutoLoad->Clear();
}

wxString wxWidgetsGUIConfigPanel::GetTitle() const
{
    return _("wxSmith");
}

wxString wxWidgetsGUIConfigPanel::GetBitmapBaseName() const
{
    return wxEmptyString;
}

void wxWidgetsGUIConfigPanel::OnApply()
{
    wxStringTokenizer Tokens(AutoLoad->GetValue(),_T("\n"));
    m_GUI->m_LoadedResources.Clear();
    while ( Tokens.HasMoreTokens() )
    {
        m_GUI->m_LoadedResources.Add(Tokens.GetNextToken());
    }
    m_GUI->m_MainResource = MainRes->GetValue();
    m_GUI->m_CallInitAll = InitAll->GetValue();
    m_GUI->m_CallInitAllNecessary = m_GUI->m_CallInitAll && InitAllNecessary->GetValue();
    m_GUI->NotifyChange();
    m_GUI->OnRebuildApplicationCode();
}

void wxWidgetsGUIConfigPanel::OnCancel()
{
}
