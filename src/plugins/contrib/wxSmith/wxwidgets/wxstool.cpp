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
* $Id: wxstool.cpp 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxstool.cpp $
*/

#include "wxstool.h"

#include "wxsitemresdata.h"
#include <logmanager.h>

wxsTool::wxsTool(
    wxsItemResData* Data,
    const wxsItemInfo* Info,
    const wxsEventDesc* EventArray,
    const wxsStyleSet* StyleSet,
    long PropertiesFlags):
        wxsParent(Data,Info,PropertiesFlags,EventArray,StyleSet)
{
}

bool wxsTool::OnCanAddToResource(wxsItemResData* Data,bool ShowMessage)
{
    if ( GetInfo().AllowInXRC )
    {
        // We do not filter if we can use this tool in XRC mode
        return true;
    }

    switch ( Data->GetPropertiesFilter() & (flSource|flMixed|flFile) )
    {
        case flSource:
            return true;

        case flMixed:
        case flFile:
            if ( ShowMessage )
            {
                cbMessageBox(
                    wxString::Format(_("%s is not supported in XRC"),GetInfo().ClassName.c_str()),
                    _("Tool insertion error"));
            }
            return false;

        default:
            break;
    }

    return false;
}

void wxsTool::OnEnumItemProperties(long _Flags)
{
    OnEnumToolProperties(_Flags);
}

void wxsTool::OnAddItemQPP(wxsAdvQPP* QPP)
{
    OnAddToolQPP(QPP);
}
