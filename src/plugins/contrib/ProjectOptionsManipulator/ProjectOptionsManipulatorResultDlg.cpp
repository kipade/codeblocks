/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 13550 $
 * $Id: ProjectOptionsManipulatorResultDlg.cpp 13550 2024-09-14 04:36:54Z mortenmacfly $
 * $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/ProjectOptionsManipulator/ProjectOptionsManipulatorResultDlg.cpp $
 */

#include "ProjectOptionsManipulatorResultDlg.h"

//(*InternalHeaders(ProjectOptionsManipulatorResultDlg)
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
//*)

//(*IdInit(ProjectOptionsManipulatorResultDlg)
const wxWindowID ProjectOptionsManipulatorResultDlg::ID_TXT_RESULT = wxNewId();
//*)

BEGIN_EVENT_TABLE(ProjectOptionsManipulatorResultDlg,wxDialog)
	//(*EventTable(ProjectOptionsManipulatorResultDlg)
	//*)
END_EVENT_TABLE()

ProjectOptionsManipulatorResultDlg::ProjectOptionsManipulatorResultDlg(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(ProjectOptionsManipulatorResultDlg)
	wxBoxSizer* bszMainH;
	wxBoxSizer* bszMainV;
	wxStaticText* lblResult;
	wxStdDialogButtonSizer* sbzOK;

	Create(parent, id, _("Project Options Plugin - Results"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	bszMainH = new wxBoxSizer(wxHORIZONTAL);
	bszMainV = new wxBoxSizer(wxVERTICAL);
	lblResult = new wxStaticText(this, wxID_ANY, _("Result of search:"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	bszMainV->Add(lblResult, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	m_TxtResult = new wxTextCtrl(this, ID_TXT_RESULT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TXT_RESULT"));
	m_TxtResult->SetMinSize(wxSize(400,200));
	bszMainV->Add(m_TxtResult, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
	sbzOK = new wxStdDialogButtonSizer();
	sbzOK->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	sbzOK->Realize();
	bszMainV->Add(sbzOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
	bszMainH->Add(bszMainV, 1, wxEXPAND, 5);
	SetSizer(bszMainH);
	bszMainH->SetSizeHints(this);
	Center();
	//*)
}

ProjectOptionsManipulatorResultDlg::~ProjectOptionsManipulatorResultDlg()
{
	//(*Destroy(ProjectOptionsManipulatorResultDlg)
	//*)
}

void ProjectOptionsManipulatorResultDlg::ApplyResult(const wxArrayString& result)
{
  m_TxtResult->Clear();
  for (size_t i=0; i<result.Count(); ++i)
    m_TxtResult->AppendText( result[i] + wxT("\n") );
}
