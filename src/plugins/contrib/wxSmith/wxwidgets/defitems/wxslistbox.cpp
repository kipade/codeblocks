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
* $Id: wxslistbox.cpp 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxslistbox.cpp $
*/

#include "wxslistbox.h"

#include <prep.h>
#include <wx/listbox.h>

namespace
{
    wxsRegisterItem<wxsListBox> Reg(_T("ListBox"),wxsTWidget,_T("Standard"),240);

    WXS_ST_BEGIN(wxsListBoxStyles,_T(""))
        WXS_ST_CATEGORY("wxListBox")
        WXS_ST(wxLB_SINGLE)
        WXS_ST(wxLB_MULTIPLE)
        WXS_ST(wxLB_EXTENDED)
        WXS_ST(wxLB_HSCROLL)
        WXS_ST(wxLB_ALWAYS_SB)
        WXS_ST(wxLB_NEEDED_SB)
        WXS_ST(wxLB_SORT)
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxsListBoxEvents)
        WXS_EVI(EVT_LISTBOX,wxEVT_COMMAND_LISTBOX_SELECTED,wxCommandEvent,Select)
        WXS_EVI(EVT_LISTBOX_DCLICK,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,wxCommandEvent,DClick)
    WXS_EV_END()
}

wxsListBox::wxsListBox(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsListBoxEvents,
        wxsListBoxStyles),
    DefaultSelection(-1)
{}

void wxsListBox::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/listbox.h>"),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %P, %S, 0, 0, %T, %V, %N);\n"));
            for ( size_t i = 0; i <  ArrayChoices.GetCount(); ++i )
            {
                if ( DefaultSelection == (int)i )
                {
                    Codef(_T("%ASetSelection( "));
                }
                Codef( _T("%AAppend(%t)"), ArrayChoices[i].wx_str());
                if ( DefaultSelection == (int)i )
                {
                    Codef(_T(" )"));
                }
                Codef(_T(";\n"));
            }

            BuildSetupWindowCode();
            return;
        }

        case wxsUnknownLanguage: // fall-through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxsListBox::OnBuildCreatingCode"),GetLanguage());
        }
    }
}


wxObject* wxsListBox::OnBuildPreview(wxWindow* Parent,long _Flags)
{
    wxListBox* Preview = new wxListBox(Parent,GetId(),Pos(Parent),Size(Parent),0,0, Style());
    for ( size_t i = 0; i <  ArrayChoices.GetCount(); ++i )
    {
        int Val = Preview->Append(ArrayChoices[i]);
        if ( (int)i == DefaultSelection )
        {
            Preview->SetSelection(Val);
        }
    }

    return SetupWindow(Preview,_Flags);
}

void wxsListBox::OnEnumWidgetProperties(cb_unused long _Flags)
{
      WXS_ARRAYSTRING(wxsListBox,ArrayChoices, _("Choices"), "content", "item")
      WXS_LONG(wxsListBox,DefaultSelection, _("Default"), "default", 0)
}
