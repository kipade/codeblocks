/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 13564 $
 * $Id: projectpathpanel.cpp 13564 2024-09-14 04:44:50Z mortenmacfly $
 * $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/scriptedwizard/projectpathpanel.cpp $
 */

#include <sdk.h>
#ifndef CB_PRECOMP
    #include <wx/filename.h>
    //(*InternalHeadersPCH(ProjectPathPanel)
    #include <wx/intl.h>
    #include <wx/string.h>
    //*)
#endif // CB_PRECOMP
#include <filefilters.h>

#include "projectpathpanel.h"


//(*IdInit(ProjectPathPanel)
const wxWindowID ProjectPathPanel::ID_STATICTEXT1 = wxNewId();
const wxWindowID ProjectPathPanel::ID_STATICTEXT4 = wxNewId();
const wxWindowID ProjectPathPanel::ID_TEXTCTRL3 = wxNewId();
const wxWindowID ProjectPathPanel::ID_STATICTEXT2 = wxNewId();
const wxWindowID ProjectPathPanel::ID_TEXTCTRL1 = wxNewId();
const wxWindowID ProjectPathPanel::ID_BUTTON1 = wxNewId();
const wxWindowID ProjectPathPanel::ID_STATICTEXT3 = wxNewId();
const wxWindowID ProjectPathPanel::ID_TEXTCTRL2 = wxNewId();
const wxWindowID ProjectPathPanel::ID_STATICTEXT5 = wxNewId();
const wxWindowID ProjectPathPanel::ID_TEXTCTRL4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ProjectPathPanel,wxPanel)
    //(*EventTable(ProjectPathPanel)
    //*)
END_EVENT_TABLE()

ProjectPathPanel::ProjectPathPanel(wxWindow* parent,wxWindowID id) :
    m_LockUpdates(false)
{
    //(*Initialize(ProjectPathPanel)
    wxStaticText* StaticText1;
    wxStaticText* StaticText2;
    wxStaticText* StaticText3;
    wxStaticText* StaticText4;
    wxStaticText* StaticText5;

    Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Please select the folder where you want the new project\nto be created as well as its title."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BoxSizer1->Add(StaticText1, 0, wxALL|wxEXPAND, 8);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Project title:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    BoxSizer1->Add(StaticText4, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 8);
    txtPrjTitle = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    BoxSizer1->Add(txtPrjTitle, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 8);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Folder to create project in:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer1->Add(StaticText2, 0, wxLEFT|wxRIGHT|wxEXPAND, 8);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    txtPrjPath = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    BoxSizer2->Add(txtPrjPath, 1, wxALIGN_CENTER_VERTICAL, 5);
    btnPrjPathBrowse = new wxButton(this, ID_BUTTON1, _T("..."), wxDefaultPosition, wxSize(22,22), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(btnPrjPathBrowse, 0, wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 8);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Project filename:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    BoxSizer1->Add(StaticText3, 0, wxLEFT|wxRIGHT|wxEXPAND, 8);
    txtPrjName = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    BoxSizer1->Add(txtPrjName, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 8);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Resulting filename:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    BoxSizer1->Add(StaticText5, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT, 8);
    txtFinalDir = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    BoxSizer1->Add(txtFinalDir, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 8);
    SetSizer(BoxSizer1);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_TEXTCTRL3,wxEVT_COMMAND_TEXT_UPDATED,wxCommandEventHandler(ProjectPathPanel::OntxtPrjTitleText));
    Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,wxCommandEventHandler(ProjectPathPanel::OnFullPathChanged));
    Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,wxCommandEventHandler(ProjectPathPanel::OnFullPathChanged));
    Connect(ID_TEXTCTRL4,wxEVT_COMMAND_TEXT_UPDATED,wxCommandEventHandler(ProjectPathPanel::OntxtFinalDirText));
    //*)
}

ProjectPathPanel::~ProjectPathPanel()
{
}

void ProjectPathPanel::UpdateFromResulting()
{
    if (m_LockUpdates || !txtPrjPath || !txtPrjName || !txtFinalDir || txtFinalDir->GetValue().IsEmpty())
        return; // not ready yet

    m_LockUpdates = true;
    wxFileName fn = txtFinalDir->GetValue();
    txtPrjPath->SetValue(fn.GetPath(wxPATH_GET_VOLUME));
    txtPrjName->SetValue(fn.GetName());
    m_LockUpdates = false;
}

void ProjectPathPanel::Update()
{
    if (m_LockUpdates || !txtPrjPath || !txtPrjName || !txtFinalDir)
        return; // not ready yet

    wxString final = txtPrjPath->GetValue();
    if (!final.IsEmpty())
    {
        wxFileName fname(txtPrjName->GetValue());
        wxFileName prjpath(final, wxEmptyString);
        fname.MakeAbsolute(prjpath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + fname.GetName());
        final = fname.GetFullPath();
    }
    if (final.IsEmpty() || txtPrjName->GetValue().IsEmpty())
        final = _("<invalid path>");

    m_LockUpdates = true;
    txtFinalDir->SetValue(final);
    m_LockUpdates = false;
}

void ProjectPathPanel::OnFullPathChanged(cb_unused wxCommandEvent& event)
{
    Update();
}

void ProjectPathPanel::OntxtFinalDirText(cb_unused wxCommandEvent& event)
{
    if (!txtPrjName->GetValue().StartsWith(_("<invalid path>")))
        UpdateFromResulting();
}

void ProjectPathPanel::OntxtPrjTitleText(cb_unused wxCommandEvent& event)
{
    wxString prjtitle = txtPrjTitle->GetValue();
    // Make a check if the project title has any extension or not
    if (!prjtitle.IsEmpty() &&
        !prjtitle.Right(4).IsSameAs(FileFilters::CODEBLOCKS_DOT_EXT))
        prjtitle = prjtitle + FileFilters::CODEBLOCKS_DOT_EXT;
    txtPrjName->SetValue(prjtitle);

// FIXME (Biplab#1#): In Linux, text update event is not thrown
    if (!platform::windows)
        Update();
}
