/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2007  Bartlomiej Swiecki
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
* $Id: wxsdeleteitemres.h 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxsdeleteitemres.h $
*/

#ifndef WXSDELETEITEMRES_H
#define WXSDELETEITEMRES_H

//(*Headers(wxsDeleteItemRes)
#include "scrollingdialog.h"
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
//*)

class wxsDeleteItemRes: public wxScrollingDialog
{
    public:

        wxsDeleteItemRes();
        virtual ~wxsDeleteItemRes();

        //(*Identifiers(wxsDeleteItemRes)
        static const wxWindowID ID_CHECKBOX1;
        static const wxWindowID ID_CHECKBOX2;
        static const wxWindowID ID_CHECKBOX3;
        //*)

        //(*Handlers(wxsDeleteItemRes)
        void Onm_DeleteSourcesClick(wxCommandEvent& event);
        //*)

        //(*Declarations(wxsDeleteItemRes)
        wxCheckBox* m_DeleteSources;
        wxCheckBox* m_PhysDeleteSources;
        wxCheckBox* m_PhysDeleteWXS;
        //*)

    private:

        DECLARE_EVENT_TABLE()
};

#endif
