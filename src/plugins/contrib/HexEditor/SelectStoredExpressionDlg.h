/*
* This file is part of HexEditor plugin for Code::Blocks Studio
* Copyright (C) 2008-2009 Bartlomiej Swiecki
*
* HexEditor plugin is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* HexEditor pluging is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with HexEditor. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 13570 $
* $Id: SelectStoredExpressionDlg.h 13570 2024-09-14 05:03:57Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/HexEditor/SelectStoredExpressionDlg.h $
*/

#ifndef SELECTSTOREDEXPRESSIONDLG_H
#define SELECTSTOREDEXPRESSIONDLG_H

#include <map>

//(*Headers(SelectStoredExpressionDlg)
#include "scrollingdialog.h"
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)


class SelectStoredExpressionDlg: public wxScrollingDialog
{
	public:

		SelectStoredExpressionDlg( wxWindow* parent, const wxString& startingExpresion = wxEmptyString );
		virtual ~SelectStoredExpressionDlg();

		wxString GetExpression() { return m_Expression; }

	private:

        wxString m_Expression;

		//(*Declarations(SelectStoredExpressionDlg)
		wxButton* Button1;
		wxButton* Button2;
		wxButton* Button3;
		wxButton* Button4;
		wxListBox* m_Expressions;
		wxStaticText* StaticText1;
		wxTextCtrl* m_Filter;
		wxTimer Timer1;
		//*)

		//(*Identifiers(SelectStoredExpressionDlg)
		static const wxWindowID ID_LISTBOX1;
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_TEXTCTRL1;
		static const wxWindowID ID_BUTTON1;
		static const wxWindowID ID_BUTTON2;
		static const wxWindowID ID_BUTTON3;
		static const wxWindowID ID_BUTTON4;
		static const wxWindowID ID_TIMER1;
		//*)

		//(*Handlers(SelectStoredExpressionDlg)
		void OnOkClick(wxCommandEvent& event);
		void OnCancelClick(wxCommandEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void Onm_ExpressionsSelect(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		void OnButton3Click(wxCommandEvent& event);
		void Onm_FilterText(wxCommandEvent& event);
		void Onm_FilterTextEnter(wxCommandEvent& event);
		void OnTimer1Trigger(wxTimerEvent& event);
		void OnButton4Click(wxCommandEvent& event);
		void Onm_ExpressionsDClick(wxCommandEvent& event);
		//*)

		void BuildContent(wxWindow* parent);
		void ReadExpressions();
		void StoreExpressions();
		void RecreateExpressionsList( const wxString& selectionHint = wxEmptyString );

        typedef std::map< wxString, wxString > CacheT;
        CacheT m_Cache;
        bool m_CacheChanged;

        /** \brief Helper class to identify items on expression list */
        class ListData: public wxClientData
        {
            public:

                /** \brief Ctor */
                inline ListData( const CacheT::iterator& i ): m_Iterator( i )
                {}

                /** \brief Fetch iterator */
                inline const CacheT::iterator& GetIterator()
                {
                    return m_Iterator;
                }

                /** \brief Fetch key */
                inline const wxString& GetKey()
                {
                    return m_Iterator->first;
                }

                /** \brief Fetch value */
                inline const wxString& GetValue()
                {
                    return m_Iterator->second;
                }

            private:

                CacheT::iterator m_Iterator;        ///< \brief Item's iterator
        };

        ListData* GetSelection();

        void AddingExpression( const wxString& defaultName, const wxString& defaultValue );

        void FilterUpdated();

        void StoreExpressionsQuery();

        static wxString GetListName( const wxString& name, const wxString& expr );


		DECLARE_EVENT_TABLE()
};

#endif
