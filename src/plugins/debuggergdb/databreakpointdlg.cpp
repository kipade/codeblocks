/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 13562 $
 * $Id: databreakpointdlg.cpp 13562 2024-09-14 04:43:57Z mortenmacfly $
 * $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/debuggergdb/databreakpointdlg.cpp $
 */

#include "databreakpointdlg.h"
#include "debugger_defs.h"

//(*InternalHeaders(DataBreakpointDlg)
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(DataBreakpointDlg)
const wxWindowID DataBreakpointDlg::ID_CHK_ENABLED = wxNewId();
const wxWindowID DataBreakpointDlg::ID_TXT_DATA_EXPRESION = wxNewId();
const wxWindowID DataBreakpointDlg::ID_RDO_CONDITION = wxNewId();
//*)

BEGIN_EVENT_TABLE(DataBreakpointDlg,wxScrollingDialog)
    //(*EventTable(DataBreakpointDlg)
    //*)
END_EVENT_TABLE()

DataBreakpointDlg::DataBreakpointDlg(wxWindow *parent, const wxString& dataExpression, bool enabled, int selection)
{
    //(*Initialize(DataBreakpointDlg)
    wxBoxSizer* bszMain;

    Create(parent, wxID_ANY, _("Data breakpoint"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    bszMain = new wxBoxSizer(wxVERTICAL);
    m_enabled = new wxCheckBox(this, ID_CHK_ENABLED, _("Enabled"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHK_ENABLED"));
    m_enabled->SetValue(false);
    bszMain->Add(m_enabled, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 8);
    m_dataExpression = new wxTextCtrl(this, ID_TXT_DATA_EXPRESION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_TXT_DATA_EXPRESION"));
    m_dataExpression->SetFocus();
    bszMain->Add(m_dataExpression, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 8);
    wxString __wxRadioBoxChoices_1[3] =
    {
      _("Break on read"),
      _("Break on write"),
      _("Break on read or write")
    };
    m_condition = new wxRadioBox(this, ID_RDO_CONDITION, _("Condition"), wxDefaultPosition, wxDefaultSize, 3, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RDO_CONDITION"));
    bszMain->Add(m_condition, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 8);
    StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
    StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
    StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
    StdDialogButtonSizer1->Realize();
    bszMain->Add(StdDialogButtonSizer1, 0, wxALL|wxEXPAND, 8);
    SetSizer(bszMain);
    bszMain->SetSizeHints(this);
    Center();
    //*)

    m_enabled->SetValue(enabled);
    m_condition->SetSelection(selection);
    m_dataExpression->SetValue(dataExpression);
}

DataBreakpointDlg::~DataBreakpointDlg()
{
    //(*Destroy(DataBreakpointDlg)
    //*)
}

bool DataBreakpointDlg::IsBreakpointEnabled()
{
    return m_enabled->IsChecked();
}

int DataBreakpointDlg::GetSelection()
{
    return m_condition->GetSelection();
}

wxString DataBreakpointDlg::GetDataExpression() const
{
    return CleanStringValue(m_dataExpression->GetValue());
}
