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
* $Id: wxstoolbaritem.h 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxstoolbaritem.h $
*/

#ifndef WXSTOOLBARITEM_H
#define WXSTOOLBARITEM_H

#include "../wxstool.h"

/** \brief Items inside toolbar (f.ex button) which are not wxContainer) */
class wxsToolBarItem : public wxsTool
{
    public:

        // Order must match that of the m_Type wxChoice in wxsToolBarEditor
        enum ToolType : int32_t
        {
            Normal,
            Radio,
            Check,
            Separator,
            Stretchable,
            Control
        };

        wxsToolBarItem(wxsItemResData* Data, ToolType Tool);

    private:

        virtual void OnBuildCreatingCode();
        virtual void OnEnumToolProperties(long _Flags);
        virtual bool OnIsPointer() { return true; }
        virtual bool OnCanAddToResource(cb_unused wxsItemResData* Data,cb_unused bool ShowMessage) { return false; }
        virtual bool OnXmlWrite(TiXmlElement* Element,bool IsXRC,bool IsExtra);
        virtual bool OnXmlRead(TiXmlElement* Element,bool IsXRC,bool IsExtra);
        virtual bool OnCanAddToParent(wxsParent* Parent,bool ShowMessage);
        virtual void OnBuildDeclarationsCode();
        virtual wxString OnGetTreeLabel(int& Image);

        ToolType      m_Type;
        wxString      m_Variable;
        wxString      m_Label;
        wxsBitmapData m_Bitmap;
        wxsBitmapData m_Bitmap2;
        wxString      m_ToolTip;
        wxString      m_HelpText;

        friend class wxsToolBarEditor;
};

#endif
