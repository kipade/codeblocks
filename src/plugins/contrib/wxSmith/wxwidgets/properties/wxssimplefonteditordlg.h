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
* $Id: wxssimplefonteditordlg.h 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/properties/wxssimplefonteditordlg.h $
*/

#ifndef WXSSIMPLEFONTEDITORDLG_H
#define WXSSIMPLEFONTEDITORDLG_H

#include "wxsfontproperty.h"

//(*Headers(wxsSimpleFontEditorDlg)
#include "scrollingdialog.h"
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)


class wxsSimpleFontEditorDlg: public wxScrollingDialog
{
    public:

        wxsSimpleFontEditorDlg(wxWindow* parent,wxsFontData& Data,wxWindowID id = -1);
        virtual ~wxsSimpleFontEditorDlg();

        //(*Identifiers(wxsSimpleFontEditorDlg)
        static const wxWindowID ID_STATICTEXT1;
        static const wxWindowID ID_STATICLINE2;
        static const wxWindowID ID_BUTTON1;
        static const wxWindowID ID_BUTTON3;
        static const wxWindowID ID_STATICLINE1;
        static const wxWindowID ID_BUTTON2;
        static const wxWindowID ID_TEXTCTRL1;
        static const wxWindowID ID_STATICTEXT2;
        static const wxWindowID ID_STATICTEXT3;
        //*)

    private:

        //(*Handlers(wxsSimpleFontEditorDlg)
        void OnOK(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnButton3Click(wxCommandEvent& event);
        //*)

        //(*Declarations(wxsSimpleFontEditorDlg)
        wxBoxSizer* BoxSizer2;
        wxButton* Button1;
        wxButton* Button2;
        wxButton* Button3;
        wxFlexGridSizer* FlexGridSizer2;
        wxStaticBoxSizer* StaticBoxSizer1;
        wxStaticLine* StaticLine1;
        wxStaticLine* StaticLine2;
        wxStaticText* FontDescription;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxTextCtrl* TestArea;
        //*)

        wxsFontData& m_Data;
        wxsFontData  m_WorkingCopy;

        void UpdateFontDescription();

        DECLARE_EVENT_TABLE()
};

#endif
