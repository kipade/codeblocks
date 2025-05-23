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
* $Id: ExpressionTester.h 13570 2024-09-14 05:03:57Z mortenmacfly $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/HexEditor/ExpressionTester.h $
*/

#ifndef EXPRESSIONTESTER_H
#define EXPRESSIONTESTER_H

//(*Headers(ExpressionTester)
#include "scrollingdialog.h"
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "FileContentBase.h"

class ExpressionTester: public wxScrollingDialog
{
	public:

		ExpressionTester( wxWindow* parent, FileContentBase* content, FileContentBase::OffsetT current );
		virtual ~ExpressionTester();

	private:

		//(*Declarations(ExpressionTester)
		wxButton* Button1;
		wxButton* Button2;
		wxButton* Button3;
		wxButton* Button4;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* m_Parsing;
		wxStaticText* m_Result;
		wxTextCtrl* m_Dump;
		wxTextCtrl* m_Expr;
		//*)

		//(*Identifiers(ExpressionTester)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_TEXTCTRL1;
		static const wxWindowID ID_BUTTON3;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_STATICTEXT5;
		static const wxWindowID ID_STATICTEXT6;
		static const wxWindowID ID_TEXTCTRL2;
		static const wxWindowID ID_BUTTON4;
		static const wxWindowID ID_BUTTON2;
		static const wxWindowID ID_BUTTON1;
		//*)

		//(*Handlers(ExpressionTester)
		void OnButton1Click(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		void OnButton3Click(wxCommandEvent& event);
		void OnButton4Click(wxCommandEvent& event);
		//*)

		FileContentBase* m_Content;
		FileContentBase::OffsetT m_Current;

		void BuildContent(wxWindow* parent);

		DECLARE_EVENT_TABLE()
};

#endif
