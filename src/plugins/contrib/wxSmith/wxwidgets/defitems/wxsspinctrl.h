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
* $Id: wxsspinctrl.h 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsspinctrl.h $
*/

#ifndef WXSSPINCTRL_H
#define WXSSPINCTRL_H

#include "../wxswidget.h"

/** \brief Class for wxsSpinCtrl widget */
class wxsSpinCtrl: public wxsWidget
{
    public:

        wxsSpinCtrl(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long _Flags);
        virtual void OnEnumWidgetProperties(long _Flags);

        wxString Value;
        long Min;
        long Max;

};

#endif
