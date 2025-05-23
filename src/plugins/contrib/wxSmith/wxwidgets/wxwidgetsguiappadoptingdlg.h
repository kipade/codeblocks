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
* $Id: wxwidgetsguiappadoptingdlg.h 13547 2024-09-14 04:35:04Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxwidgetsguiappadoptingdlg.h $
*/

#ifndef WXWIDGETSGUIAPPADOPTINGDLG_H
#define WXWIDGETSGUIAPPADOPTINGDLG_H

//(*Headers(wxWidgetsGUIAppAdoptingDlg)
#include "scrollingdialog.h"
#include <wx/button.h>
#include <wx/gauge.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
//*)

#include <wx/timer.h>
#include <projectfile.h>

class wxWidgetsGUI;

/** \brief Dialog used when adopting wxApp class
 * \note most of work while scanning sources and
 *       adding wxApp support is done in wxGUI class
 *       actually, this dialog is only to represent
 *       results and get user choice what to do after
 *       scanning
 */
class wxWidgetsGUIAppAdoptingDlg: public wxScrollingDialog
{
    public:

        wxWidgetsGUIAppAdoptingDlg(wxWindow* parent,wxWidgetsGUI* GUI,wxWindowID id = -1);
        virtual ~wxWidgetsGUIAppAdoptingDlg();

        //(*Identifiers(wxWidgetsGUIAppAdoptingDlg)
        static const wxWindowID ID_LISTBOX1;
        static const wxWindowID ID_GAUGE1;
        static const wxWindowID ID_STATICTEXT1;
        static const wxWindowID ID_STATICTEXT2;
        static const wxWindowID ID_BUTTON5;
        static const wxWindowID ID_BUTTON2;
        static const wxWindowID ID_BUTTON3;
        static const wxWindowID ID_STATICLINE2;
        static const wxWindowID ID_BUTTON4;
        static const wxWindowID ID_STATICLINE1;
        static const wxWindowID ID_BUTTON6;
        //*)

        wxString m_RelativeFileName;

    protected:

        //(*Handlers(wxWidgetsGUIAppAdoptingDlg)
        void OnButton6Click(wxCommandEvent& event);
        void OnButton4Click(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnTimer(wxTimerEvent& event);
        void OnUseFileBtnClick(wxCommandEvent& event);
        void OnSelectBtnClick(wxCommandEvent& event);
        void OnCreateBtnClick(wxCommandEvent& event);
        //*)

        //(*Declarations(wxWidgetsGUIAppAdoptingDlg)
        wxBoxSizer* BoxSizer2;
        wxButton* Button4;
        wxButton* Button6;
        wxButton* CreateBtn;
        wxButton* SelectBtn;
        wxButton* UseFileBtn;
        wxGauge* Progress;
        wxListBox* FoundFiles;
        wxStaticLine* StaticLine1;
        wxStaticLine* StaticLine2;
        wxStaticText* ScanningFile;
        wxStaticText* ScanningTxt;
        //*)

    private:

        /** \brief Starting scanning of files
         *  \note This function creates it's own
         *        event-dispatching loop while searching inside files
         */
        void Run();

        /** \brief Scanning one file
         *  \return true when file can be used as wxApp container, false otherwise
         */
        bool ScanFile(ProjectFile* File);

        /** \brief Adding smith bindings for given file */
        void AddSmith(wxString RelativeFileName);

        cbProject* m_Project;       ///< \brief Helper pointer to C::B project
        wxWidgetsGUI* m_GUI;        ///< \brief GUI which has requested the scan
        wxTimer m_Timer;            ///< \brief Timer used to call Run() after dialog is shown
        bool m_Run;                 ///< \brief Flag used to break scanning of files after closing dialog

        DECLARE_EVENT_TABLE()
};

#endif
