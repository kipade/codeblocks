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
* $Id: wxsfontfaceeditordlg.h 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/properties/wxsfontfaceeditordlg.h $
*/

#ifndef WXSFONTFACEEDITORDLG_H
#define WXSFONTFACEEDITORDLG_H

#include <wx/wxprec.h>

//(*Headers(wxsFontFaceEditorDlg)
#include "scrollingdialog.h"
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
//*)

class wxsFontFaceEditorDlg: public wxScrollingDialog
{
    public:

        wxsFontFaceEditorDlg(wxWindow* parent,wxString& Face,wxWindowID id = -1);
        virtual ~wxsFontFaceEditorDlg();

        //(*Identifiers(wxsFontFaceEditorDlg)
        static const wxWindowID ID_TEXTCTRL1;
        static const wxWindowID ID_BUTTON1;
        //*)

    protected:

        //(*Handlers(wxsFontFaceEditorDlg)
        void OnButton2Click(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        //*)

        //(*Declarations(wxsFontFaceEditorDlg)
        wxButton* Button1;
        wxButton* Button2;
        wxButton* Button3;
        wxTextCtrl* FaceName;
        //*)

    private:

        wxString& Face;

        DECLARE_EVENT_TABLE()
};

#endif
