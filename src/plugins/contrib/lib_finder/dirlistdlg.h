/*
* This file is part of lib_finder plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
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
* $Id: dirlistdlg.h 13570 2024-09-14 05:03:57Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/lib_finder/dirlistdlg.h $
*/

#ifndef DIRLISTDLG_H
#define DIRLISTDLG_H

#include <wx/arrstr.h>

// NOTE : once the new wxSmith is up and running these includes should be
// moved to the cpp, forward declarations is what we need here

//(*Headers(DirListDlg)
#include "scrollingdialog.h"
class wxBoxSizer;
class wxButton;
class wxFlexGridSizer;
class wxStaticBoxSizer;
class wxTextCtrl;
//*)


class wxCommandEvent;

class DirListDlg: public wxScrollingDialog
{
	public:

		DirListDlg(wxWindow* parent,wxWindowID id = -1);
		virtual ~DirListDlg();

		wxArrayString Dirs;

	private:

        //(*Declarations(DirListDlg)
        wxBoxSizer* BoxSizer1;
        wxBoxSizer* BoxSizer2;
        wxButton* Button3;
        wxButton* Button4;
        wxFlexGridSizer* FlexGridSizer1;
        wxTextCtrl* DirList;
        //*)

		//(*Handlers(DirListDlg)
		void OnButton1Click(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		void OnButton3Click(wxCommandEvent& event);
		void OnButton4Click(wxCommandEvent& event);
		//*)

		//(*Identifiers(DirListDlg)
		static const wxWindowID ID_TEXTCTRL1;
		static const wxWindowID ID_BUTTON1;
		static const wxWindowID ID_BUTTON2;
		static const wxWindowID ID_BUTTON3;
		static const wxWindowID ID_BUTTON4;
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
