/*
* This file is part of lib_finder plugin for Code::Blocks Studio
* Copyright (C) 2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*
* $Revision: 13570 $
* $Id: libselectdlg.h 13570 2024-09-14 05:03:57Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/lib_finder/libselectdlg.h $
*/

#ifndef LIBSELECTDLG_H
#define LIBSELECTDLG_H

//(*Headers(LibSelectDlg)
#include "scrollingdialog.h"
class wxBoxSizer;
class wxCheckBox;
class wxCheckListBox;
class wxRadioButton;
class wxStaticBoxSizer;
class wxStaticText;
class wxStdDialogButtonSizer;
//*)

class LibSelectDlg: public wxScrollingDialog
{
	public:

		LibSelectDlg(wxWindow* parent,const wxArrayString& Names,bool addOnly);
		virtual ~LibSelectDlg();

        /** \brief Set list of selected items */
		void SetSelections(const wxArrayInt& Selections);

        /** \brief Get lis of selected items */
		wxArrayInt GetSelections();

		/** \brief Check if we should setup global variables */
		bool GetSetupGlobalVars();

		/** \brief Check if we should not clear previous results */
		bool GetDontClearPrevious();

		/** \brief Check if we should clear previous results only for selected items */
		bool GetClearSelectedPrevious();

		/** \brief Check if we should clear all previous results */
		bool GetClearAllPrevious();

	private:

		//(*Declarations(LibSelectDlg)
		wxCheckBox* m_SetupGlobalVars;
		wxCheckListBox* m_Libraries;
		wxRadioButton* m_ClearAll;
		wxRadioButton* m_ClearSelected;
		wxRadioButton* m_DontClear;
		wxStaticText* StaticText1;
		//*)

		//(*Identifiers(LibSelectDlg)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_CHECKLISTBOX1;
		static const wxWindowID ID_RADIOBUTTON1;
		static const wxWindowID ID_RADIOBUTTON2;
		static const wxWindowID ID_RADIOBUTTON3;
		static const wxWindowID ID_CHECKBOX1;
		//*)

		//(*Handlers(LibSelectDlg)
		void OnOk(wxCommandEvent& event);
		//*)

		bool m_AddOnly;

		DECLARE_EVENT_TABLE()
};

#endif
