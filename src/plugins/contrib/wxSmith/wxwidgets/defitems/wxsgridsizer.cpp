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
* $Id: wxsgridsizer.cpp 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsgridsizer.cpp $
*/

#include "wxsgridsizer.h"

namespace
{
    wxsRegisterItem<wxsGridSizer> Reg(_T("GridSizer"),wxsTSizer,_T("Layout"),60);
}

wxsGridSizer::wxsGridSizer(wxsItemResData* Data):
    wxsSizer(Data,&Reg.Info),
    Cols(3),
    Rows(0)
{
}

wxSizer* wxsGridSizer::OnBuildSizerPreview(wxWindow* Parent)
{
    return new wxGridSizer(Rows,Cols,
        VGap.GetPixels(Parent),HGap.GetPixels(Parent));
}

void wxsGridSizer::OnBuildSizerCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/sizer.h>"),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%d, %d, %s, %s);\n"),Rows,Cols,
                  VGap.GetPixelsCode(GetCoderContext()).wx_str(),
                  HGap.GetPixelsCode(GetCoderContext()).wx_str());
            return;
        }

        case wxsUnknownLanguage: // fall-through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxsGridSizer::OnBuildSizerCreatingCode"),GetLanguage());
        }
    }
}

void wxsGridSizer::OnEnumSizerProperties(cb_unused long _Flags)
{
    WXS_LONG(wxsGridSizer,Cols,_("Cols"),_T("cols"),0);
    WXS_LONG(wxsGridSizer,Rows,_("Rows"),_T("rows"),0);
    WXS_DIMENSION(wxsGridSizer,VGap,_("V-Gap"),_("V-Gap in dialog units"),_T("vgap"),0,false);
    WXS_DIMENSION(wxsGridSizer,HGap,_("H-Gap"),_("H-Gap in dialog units"),_T("hgap"),0,false);
}
