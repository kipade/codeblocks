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
* $Id: wxscontainer.cpp 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxscontainer.cpp $
*/

#include "wxscontainer.h"
#include "wxsitemresdata.h"
#include "wxstool.h"
#include "wxscodercontext.h"
#include <logmanager.h>

wxsContainer::wxsContainer(
    wxsItemResData* Data,
    const wxsItemInfo* Info,
    const wxsEventDesc* EventArray,
    const wxsStyleSet* StyleSet,
    long PropertiesFlags):
        wxsParent(Data,Info,PropertiesFlags,EventArray,StyleSet)
{
}

bool wxsContainer::OnCanAddChild(wxsItem* Item,bool ShowMessage)
{
    if ( Item->GetType() == wxsTSpacer )
    {
        if ( ShowMessage )
        {
            wxMessageBox(_("Spacer can be added into sizer only"));
        }
        return false;
    }

    if ( Item->GetType() == wxsTSizer )
    {
        if ( GetChildCount() > 0 )
        {
            if ( GetChild(0)->GetType() == wxsTSizer )
            {
                if ( ShowMessage )
                {
                    wxMessageBox(_("This item can contain only one sizer"));
                }
                return false;
            }
            else
            {
                if ( ShowMessage )
                {
                    wxMessageBox(_("Item can not contain sizer if it has other items inside"));
                }
                return false;
            }
        }
        return true;
    }

    if ( GetChildCount() > 0 )
    {
        if ( GetChild(0)->GetType() == wxsTSizer )
        {
            if ( ShowMessage )
            {
                wxMessageBox(_("Item contains sizer, can not add other items into it."));
            }
            return false;
        }
    }

    return true;
}

void wxsContainer::AddChildrenPreview(wxWindow* This,long _Flags)
{
    for ( int i=0; i<GetChildCount(); i++ )
    {
        wxsItem* Child = GetChild(i);
        wxObject* ChildPreviewAsObject = Child->BuildPreview(This,_Flags);
        if ( Child->GetType() == wxsTSizer )
        {
            wxSizer* ChildPreviewAsSizer = wxDynamicCast(ChildPreviewAsObject,wxSizer);
            if ( ChildPreviewAsSizer )
            {
                This->SetSizer(ChildPreviewAsSizer);
            }
        }
    }

    if ( IsRootItem() )
    {
        // Adding all tools before calling Fit and SetSizeHints()

        wxsItemResData* Data = GetResourceData();
        if ( Data )
        {
            for ( int i=0; i<Data->GetToolsCount(); i++ )
            {
                Data->GetTool(i)->BuildPreview(This,_Flags);
            }
        }

    }

    for ( int i=0; i<GetChildCount(); i++ )
    {
        wxsItem* Child = GetChild(i);
        if ( Child->GetType() == wxsTSizer )
        {
            wxObject* ChildPreviewAsObject = Child->GetLastPreview();
            wxSizer*  ChildPreviewAsSizer  = wxDynamicCast(ChildPreviewAsObject,wxSizer);
            wxWindow* ChildPreviewAsWindow = wxDynamicCast(ChildPreviewAsObject,wxWindow);

            if ( ChildPreviewAsSizer )
            {
                // Child preview was created directly as sizer, we use it to
                // call Fit() and SetSizeHints() directly
                if ( GetBaseProps()->m_Size.IsDefault )
                {
                    ChildPreviewAsSizer->Fit(This);
                }
                ChildPreviewAsSizer->SetSizeHints(This);
            }
            else if ( ChildPreviewAsWindow )
            {
                // Preview of sizer is given actually as some kind of panel which paints
                // some extra data of sizer. So we have to create out own sizer to call
                // Fit and SetSizeHints

                wxSizer* IndirectSizer = new wxBoxSizer(wxHORIZONTAL);
                IndirectSizer->Add(ChildPreviewAsWindow,1,wxEXPAND,0);
                This->SetSizer(IndirectSizer);

                if ( GetBaseProps()->m_Size.IsDefault )
                {
                    IndirectSizer->Fit(This);
                }
                IndirectSizer->SetSizeHints(This);
            }
        }
    }
}

void wxsContainer::AddChildrenCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            wxsCoderContext* Context = GetCoderContext();
            if ( !Context ) return;

            // Update parent in context and clear flRoot flag
            wxString PreviousParent = Context->m_WindowParent;
            Context->m_WindowParent = Codef(Context,_T("%O"));

            for ( int i=0; i<GetChildCount(); i++ )
            {
                wxsItem* Child = GetChild(i);
                Child->BuildCode(Context);
                if ( Child->GetType() == wxsTSizer )
                {
                    // TODO: Is this right place to set-up sizer ?
                    Codef(_T("%ASetSizer(%o);\n"),i);
                }
            }

            if ( IsRootItem() )
            {
                // Adding all tools before calling Fit and SetSizeHints()
                wxsItemResData* Data = GetResourceData();
                if ( Data )
                {
                    for ( int i=0; i<Data->GetToolsCount(); i++ )
                    {
                        Data->GetTool(i)->BuildCode(Context);
                    }
                }

                wxsBaseProperties* Props      = GetBaseProps();
                bool               IsTopLevel = GetPropertiesFlags() & flTopLevel;
                if (Props && Props->m_UseLayout && IsTopLevel)
                {
                    for ( int i=0; i<GetChildCount(); i++ ) // See if item contains a sizer
                    {
                        wxsItem* Child = GetChild(i);
                        if ( Child->GetType() == wxsTSizer )
                        {
                            if ( Props->m_Size.IsDefault )
                            {
                                if ( Props->m_MinSize.IsDefault && Props->m_MaxSize.IsDefault )
                                {
                                    wxString ChildAccessPrefix = Child->GetAccessPrefix(GetLanguage());
                                    Codef(_T("%sSetSizeHints(%O);\n"),ChildAccessPrefix.wx_str());
                                }
                                else
                                {
                                    Codef(_T("Fit();\n"));
                                }
                            }
                            else
                            {
                                Codef(_T("Layout();\n"));
                            }
                            break;
                        }
                    }
                }
            }
            Context->m_WindowParent = PreviousParent;
            return;
        }

        case wxsUnknownLanguage:
        default:
        {
            wxsCodeMarks::Unknown(_T("wxsContainer::AddChildrenCode"),GetLanguage());
        }
    }
}

void wxsContainer::OnEnumItemProperties(long _Flags)
{
    OnEnumContainerProperties(_Flags);
}

void wxsContainer::OnAddItemQPP(wxsAdvQPP* QPP)
{
    OnAddContainerQPP(QPP);
}

