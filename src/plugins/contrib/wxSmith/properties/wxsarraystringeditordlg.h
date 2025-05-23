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
* $Id: wxsarraystringeditordlg.h 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/properties/wxsarraystringeditordlg.h $
*/

#ifndef WXSARRAYSTRINGEDITORDLG_H
#define WXSARRAYSTRINGEDITORDLG_H

//(*Headers(wxsArrayStringEditorDlg)
#include "scrollingdialog.h"
#include <wx/sizer.h>
#include <wx/textctrl.h>
//*)

#include <cbplugin.h>

class PLUGIN_EXPORT wxsArrayStringEditorDlg: public wxScrollingDialog
{
    public:

        wxsArrayStringEditorDlg(wxWindow* parent,wxArrayString& Array,wxWindowID id = -1);
        virtual ~wxsArrayStringEditorDlg();

        //(*Identifiers(wxsArrayStringEditorDlg)
        static const wxWindowID ID_TEXTCTRL1;
        //*)

    protected:

        //(*Handlers(wxsArrayStringEditorDlg)
        void OnOK(wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        //*)

        //(*Declarations(wxsArrayStringEditorDlg)
        wxTextCtrl* Items;
        //*)

    private:

        wxArrayString& Data;

        DECLARE_EVENT_TABLE()
};

#endif
