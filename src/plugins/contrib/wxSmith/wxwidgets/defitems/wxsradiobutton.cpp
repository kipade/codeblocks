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
* $Id: wxsradiobutton.cpp 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsradiobutton.cpp $
*/

#include <wx/radiobut.h>
#include "wxsradiobutton.h"

namespace
{
    wxsRegisterItem<wxsRadioButton> Reg(_T("RadioButton"),wxsTWidget,_T("Standard"),170);

    WXS_ST_BEGIN(wxsRadioButtonStyles,_T(""))
        WXS_ST_CATEGORY("wxRadioButton")
        WXS_ST(wxRB_GROUP)
        WXS_ST(wxRB_SINGLE)
        WXS_ST_DEFAULTS()
    WXS_ST_END()


    WXS_EV_BEGIN(wxsRadioButtonEvents)
        WXS_EVI(EVT_RADIOBUTTON,wxEVT_COMMAND_RADIOBUTTON_SELECTED,wxCommandEvent,Select)
    WXS_EV_END()
}

wxsRadioButton::wxsRadioButton(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsRadioButtonEvents,
        wxsRadioButtonStyles),
    Label(_("Label")),
    IsSelected(false)
{}


void wxsRadioButton::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/radiobut.h>"),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %t, %P, %S, %T, %V, %N);\n"),Label.wx_str());
            if ( IsSelected ) Codef( _T("%ASetValue(%b);\n"), true);
            BuildSetupWindowCode();
            return;
        }

        case wxsUnknownLanguage: // fall-through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxsRadioButton::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxsRadioButton::OnBuildPreview(wxWindow* Parent,long _Flags)
{
    wxRadioButton* Preview = new wxRadioButton(Parent,GetId(),Label,Pos(Parent),Size(Parent),Style());
    Preview->SetValue(IsSelected);
    return SetupWindow(Preview,_Flags);
}

void wxsRadioButton::OnEnumWidgetProperties(cb_unused long _Flags)
{
    WXS_SHORT_STRING(wxsRadioButton,Label,_("Label"),_T("label"),_T(""),true)
    WXS_BOOL(wxsRadioButton,IsSelected,_("Is Selected"),_T("selected"),false)
}
