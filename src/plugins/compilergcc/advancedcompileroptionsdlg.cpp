/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 13355 $
 * $Id: advancedcompileroptionsdlg.cpp 13355 2023-09-17 13:52:36Z mortenmacfly $
 * $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/compilergcc/advancedcompileroptionsdlg.cpp $
 */

#include "sdk.h"
#ifndef CB_PRECOMP
    #include <wx/button.h>
    #include <wx/checkbox.h>
    #include <wx/choice.h>
    #include <wx/combobox.h>
    #include <wx/intl.h>
    #include <wx/listbox.h>
    #include <wx/sizer.h>
    #include <wx/spinctrl.h>
    #include <wx/textctrl.h>
    #include <wx/xrc/xmlres.h>
    #include "compilerfactory.h"
    #include "globals.h"
#endif
#include "advancedcompileroptionsdlg.h"

// TODO : implement full IsDirty mechanism, now always forced to true when OK is pressed
// NOTE : when OK pressed settings are already applied to the compiler, although in the compiler settings
//        that the parent dialog that spawned this dialog the user can still click on cancel there,
//        meaning also the changes here should not be applied, but as it is now : too late, applied (TO FIX)


inline wxString ControlCharsToString(const wxString& src)
{
    wxString ret = src;
    ret.Replace("\t", "\\t");
    ret.Replace("\n", "\\n");
    ret.Replace("\r", "\\r");
    ret.Replace("\a", "\\a");
    ret.Replace("\b", "\\b");
    return ret;
}

wxString StringToControlChars(const wxString& src)
{
    wxString ret = src;
    ret.Replace("\\t", "\t");
    ret.Replace("\\n", "\n");
    ret.Replace("\\r", "\r");
    ret.Replace("\\a", "\a");
    ret.Replace("\\b", "\b");
    return ret;
}

BEGIN_EVENT_TABLE(AdvancedCompilerOptionsDlg, wxScrollingDialog)
    EVT_CHOICE(XRCID("lstCommands"),        AdvancedCompilerOptionsDlg::OnCommandsChange)
    EVT_CHOICE(XRCID("lstExt"),             AdvancedCompilerOptionsDlg::OnExtChange)
    EVT_BUTTON(XRCID("btnAddExt"),          AdvancedCompilerOptionsDlg::OnAddExt)
    EVT_BUTTON(XRCID("btnRemoveExt"),       AdvancedCompilerOptionsDlg::OnDelExt)
    EVT_LISTBOX(XRCID("lstRegex"),          AdvancedCompilerOptionsDlg::OnRegexChange)
    EVT_BUTTON(XRCID("btnRegexTest"),       AdvancedCompilerOptionsDlg::OnRegexTest)
    EVT_BUTTON(XRCID("btnRegexAdd"),        AdvancedCompilerOptionsDlg::OnRegexAdd)
    EVT_BUTTON(XRCID("btnRegexDelete"),     AdvancedCompilerOptionsDlg::OnRegexDelete)
    EVT_BUTTON(XRCID("btnRegexDefaults"),   AdvancedCompilerOptionsDlg::OnRegexDefaults)
    EVT_SPIN_UP(XRCID("spnRegexOrder"),     AdvancedCompilerOptionsDlg::OnRegexUp)
    EVT_SPIN_DOWN(XRCID("spnRegexOrder"),   AdvancedCompilerOptionsDlg::OnRegexDown)
END_EVENT_TABLE()

AdvancedCompilerOptionsDlg::AdvancedCompilerOptionsDlg(wxWindow* parent, const wxString& compilerId)
    : m_CompilerId(compilerId),
    m_LastCmdIndex(-1),
    m_LastExtIndex(-1)
{
    //ctor
    wxXmlResource::Get()->LoadObject(this, parent, "dlgAdvancedCompilerOptions", "wxScrollingDialog");
    XRCCTRL(*this, "wxID_OK", wxButton)->SetDefault();
    ReadCompilerOptions();
    m_bDirty = false;

    GetSizer()->SetSizeHints(this);
}

AdvancedCompilerOptionsDlg::~AdvancedCompilerOptionsDlg()
{
    //dtor
}

void AdvancedCompilerOptionsDlg::ReadCompilerOptions()
{
    Compiler* compiler = CompilerFactory::GetCompiler(m_CompilerId);
    if (!compiler)
        return;

    wxChoice* lst = XRCCTRL(*this, "lstCommands", wxChoice);
    lst->Clear();
    for (int i = 0; i < ctCount; ++i)
    {
        m_Commands[i] = compiler->GetCommandToolsVector((CommandType)i);
        lst->Append(Compiler::CommandTypeDescriptions[i]);
    }

    lst->SetSelection(0);
    DisplayCommand(0, 0);
    ReadExtensions(0);

    // switches
    const CompilerSwitches& switches = compiler->GetSwitches();
    XRCCTRL(*this, "txtAddIncludePath",       wxTextCtrl)->SetValue(switches.includeDirs);
    XRCCTRL(*this, "txtAddLibPath",           wxTextCtrl)->SetValue(switches.libDirs);
    XRCCTRL(*this, "txtAddLib",               wxTextCtrl)->SetValue(switches.linkLibs);
    XRCCTRL(*this, "txtLibPrefix",            wxTextCtrl)->SetValue(switches.libPrefix);
    XRCCTRL(*this, "txtLibExt",               wxTextCtrl)->SetValue(switches.libExtension);
    XRCCTRL(*this, "txtDefine",               wxTextCtrl)->SetValue(switches.defines);
    XRCCTRL(*this, "txtGenericSwitch",        wxTextCtrl)->SetValue(switches.genericSwitch);
    XRCCTRL(*this, "txtObjectExt",            wxTextCtrl)->SetValue(switches.objectExtension);
    XRCCTRL(*this, "chkFwdSlashes",           wxCheckBox)->SetValue(switches.forceFwdSlashes);
    XRCCTRL(*this, "chkLinkerNeedsLibPrefix", wxCheckBox)->SetValue(switches.linkerNeedsLibPrefix);
    XRCCTRL(*this, "chkLinkerNeedsLibExt",    wxCheckBox)->SetValue(switches.linkerNeedsLibExtension);
    XRCCTRL(*this, "chkLinkerNeedsPathRes",   wxCheckBox)->SetValue(switches.linkerNeedsPathResolved);
    XRCCTRL(*this, "chkNeedDeps",             wxCheckBox)->SetValue(switches.needDependencies);
    XRCCTRL(*this, "chkForceCompilerQuotes",  wxCheckBox)->SetValue(switches.forceCompilerUseQuotes);
    XRCCTRL(*this, "chkForceLinkerQuotes",    wxCheckBox)->SetValue(switches.forceLinkerUseQuotes);
    XRCCTRL(*this, "chkSupportsPCH",          wxCheckBox)->SetValue(switches.supportsPCH);
    XRCCTRL(*this, "txtPCHExt",               wxTextCtrl)->SetValue(switches.PCHExtension);
    XRCCTRL(*this, "chkUseFlatObjects",       wxCheckBox)->SetValue(switches.UseFlatObjects);
    XRCCTRL(*this, "chkUseFullSourcePaths",   wxCheckBox)->SetValue(switches.UseFullSourcePaths);
    XRCCTRL(*this, "txtIncludeDirSeparator",  wxTextCtrl)->SetValue(switches.includeDirSeparator);
    XRCCTRL(*this, "txtLibDirSeparator",      wxTextCtrl)->SetValue(switches.libDirSeparator);
    XRCCTRL(*this, "txtObjectSeparator",      wxTextCtrl)->SetValue(switches.objectSeparator);
    XRCCTRL(*this, "spnStatusSuccess",        wxSpinCtrl)->SetValue(switches.statusSuccess);
    XRCCTRL(*this, "chkUse83Paths",           wxCheckBox)->SetValue(switches.Use83Paths);

    m_Regexes = compiler->GetRegExArray();
    m_SelectedRegex = (m_Regexes.size() > 0) ? 0 : -1;
    FillRegexes();
}

void AdvancedCompilerOptionsDlg::WriteCompilerOptions()
{
    Compiler* compiler = CompilerFactory::GetCompiler(m_CompilerId);
    if (!compiler)
        return;

    for (int i = 0; i < ctCount; ++i)
        compiler->GetCommandToolsVector((CommandType)i) = m_Commands[i];

    // switches
    CompilerSwitches switches;
    switches.includeDirs             = XRCCTRL(*this, "txtAddIncludePath",       wxTextCtrl)->GetValue();
    switches.libDirs                 = XRCCTRL(*this, "txtAddLibPath",           wxTextCtrl)->GetValue();
    switches.linkLibs                = XRCCTRL(*this, "txtAddLib",               wxTextCtrl)->GetValue();
    switches.libPrefix               = XRCCTRL(*this, "txtLibPrefix",            wxTextCtrl)->GetValue();
    switches.libExtension            = XRCCTRL(*this, "txtLibExt",               wxTextCtrl)->GetValue();
    switches.defines                 = XRCCTRL(*this, "txtDefine",               wxTextCtrl)->GetValue();
    switches.genericSwitch           = XRCCTRL(*this, "txtGenericSwitch",        wxTextCtrl)->GetValue();
    switches.objectExtension         = XRCCTRL(*this, "txtObjectExt",            wxTextCtrl)->GetValue();
    switches.forceFwdSlashes         = XRCCTRL(*this, "chkFwdSlashes",           wxCheckBox)->GetValue();
    switches.linkerNeedsLibPrefix    = XRCCTRL(*this, "chkLinkerNeedsLibPrefix", wxCheckBox)->GetValue();
    switches.linkerNeedsLibExtension = XRCCTRL(*this, "chkLinkerNeedsLibExt",    wxCheckBox)->GetValue();
    switches.linkerNeedsPathResolved = XRCCTRL(*this, "chkLinkerNeedsPathRes",   wxCheckBox)->GetValue();
    switches.needDependencies        = XRCCTRL(*this, "chkNeedDeps",             wxCheckBox)->GetValue();
    switches.forceCompilerUseQuotes  = XRCCTRL(*this, "chkForceCompilerQuotes",  wxCheckBox)->GetValue();
    switches.forceLinkerUseQuotes    = XRCCTRL(*this, "chkForceLinkerQuotes",    wxCheckBox)->GetValue();
    switches.supportsPCH             = XRCCTRL(*this, "chkSupportsPCH",          wxCheckBox)->GetValue();
    switches.PCHExtension            = XRCCTRL(*this, "txtPCHExt",               wxTextCtrl)->GetValue();
    switches.UseFlatObjects          = XRCCTRL(*this, "chkUseFlatObjects",       wxCheckBox)->GetValue();
    switches.UseFullSourcePaths      = XRCCTRL(*this, "chkUseFullSourcePaths",   wxCheckBox)->GetValue();
    {
        wxString value               = XRCCTRL(*this, "txtIncludeDirSeparator",  wxTextCtrl)->GetValue();
        if (!value.empty())
            switches.includeDirSeparator = value[0];
    }
    {
        wxString value               = XRCCTRL(*this, "txtLibDirSeparator",      wxTextCtrl)->GetValue();
        if (!value.empty())
            switches.libDirSeparator = value[0];
    }
    {
        wxString value               = XRCCTRL(*this, "txtObjectSeparator",      wxTextCtrl)->GetValue();
        if (!value.empty())
            switches.objectSeparator = value[0];
    }
    switches.statusSuccess           = XRCCTRL(*this, "spnStatusSuccess",        wxSpinCtrl)->GetValue();
    switches.Use83Paths              = XRCCTRL(*this, "chkUse83Paths",           wxCheckBox)->GetValue();

    compiler->SetSwitches(switches);
}

void AdvancedCompilerOptionsDlg::ReadExtensions(int nr)
{
    wxChoice* cho = XRCCTRL(*this, "lstExt", wxChoice);
    cho->Clear();
    for (size_t i = 0; i < m_Commands[nr].size(); ++i)
        cho->Append(GetStringFromArray(m_Commands[nr][i].extensions, DEFAULT_ARRAY_SEP, false));

    cho->SetSelection(cho->FindString(wxEmptyString));
}

CompilerTool* AdvancedCompilerOptionsDlg::GetCompilerTool(int cmd, int ext)
{
    wxChoice* cho = XRCCTRL(*this, "lstExt", wxChoice);
    for (size_t i = 0; i < m_Commands[cmd].size(); ++i)
    {
        wxString extension;
        if (ext < int(cho->GetCount()))
            extension = cho->GetString(ext);

        if (extension.empty() && m_Commands[cmd][i].extensions.GetCount() == 0)
            return &m_Commands[cmd][i];

        const int selExt = m_Commands[cmd][i].extensions.Index(extension);
        if (selExt != -1)
            return &m_Commands[cmd][i];
    }

    return nullptr;
}

void AdvancedCompilerOptionsDlg::DisplayCommand(int cmd, int ext)
{
    wxTextCtrl* text = XRCCTRL(*this, "txtCommand",   wxTextCtrl);
    wxTextCtrl* gen  = XRCCTRL(*this, "txtGenerated", wxTextCtrl);
    if (CompilerTool* tool = GetCompilerTool(cmd,ext))
    {
        text->SetValue(tool->command);
        gen->SetValue(GetStringFromArray(tool->generatedFiles, '\n', false));
    }
    else
    {
        text->Clear();
        gen->Clear();
    }

    m_LastCmdIndex = cmd;
    m_LastExtIndex = ext;
}

void AdvancedCompilerOptionsDlg::CheckForChanges()
{
    // changes can only happen after constructor : aka no longer (-1, -1)
    if (m_LastCmdIndex != -1 && m_LastExtIndex != -1)
        SaveCommands(m_LastCmdIndex, m_LastExtIndex);
}

void AdvancedCompilerOptionsDlg::SaveCommands(int cmd, int ext)
{
    if (cmd == -1 || ext == -1)
        return;

    if (CompilerTool* tool = GetCompilerTool(cmd, ext))
    {
        wxTextCtrl* text = XRCCTRL(*this, "txtCommand",   wxTextCtrl);
        wxTextCtrl* gen  = XRCCTRL(*this, "txtGenerated", wxTextCtrl);
        if (text->GetValue() != tool->command) // last command was changed; save it
            tool->command = text->GetValue();

        const wxString gens(GetStringFromArray(tool->generatedFiles, '\n', false));
        if (gen->GetValue() != gens) // last genfiles are changed; save it
            tool->generatedFiles = GetArrayFromString(gen->GetValue(), '\n');
    }
}

void AdvancedCompilerOptionsDlg::FillRegexes()
{
    wxListBox* list = XRCCTRL(*this, "lstRegex", wxListBox);
    list->Clear();

    for (size_t i = 0; i < m_Regexes.size(); ++i)
    {
        RegExStruct& rs = m_Regexes[i];
        list->Append(rs.desc);
    }

    list->SetSelection(m_SelectedRegex);
    FillRegexDetails(m_SelectedRegex);
}

void AdvancedCompilerOptionsDlg::FillRegexDetails(int index)
{
    if (index == -1)
    {
        XRCCTRL(*this, "txtRegexDesc",     wxTextCtrl)->SetValue(wxString());
        XRCCTRL(*this, "cmbRegexType",     wxComboBox)->SetSelection(-1);
        XRCCTRL(*this, "txtRegex",         wxTextCtrl)->SetValue(wxString());
        XRCCTRL(*this, "spnRegexMsg1",     wxSpinCtrl)->SetValue(0);
        XRCCTRL(*this, "spnRegexMsg2",     wxSpinCtrl)->SetValue(0);
        XRCCTRL(*this, "spnRegexMsg3",     wxSpinCtrl)->SetValue(0);
        XRCCTRL(*this, "spnRegexFilename", wxSpinCtrl)->SetValue(0);
        XRCCTRL(*this, "spnRegexLine",     wxSpinCtrl)->SetValue(0);
        return;
    }

    RegExStruct& rs = m_Regexes[index];
    XRCCTRL(*this, "txtRegexDesc",     wxTextCtrl)->SetValue(rs.desc);
    XRCCTRL(*this, "cmbRegexType",     wxComboBox)->SetSelection((int)rs.lt);
    XRCCTRL(*this, "txtRegex",         wxTextCtrl)->SetValue(ControlCharsToString(rs.GetRegExString()));
    XRCCTRL(*this, "spnRegexMsg1",     wxSpinCtrl)->SetValue(rs.msg[0]);
    XRCCTRL(*this, "spnRegexMsg2",     wxSpinCtrl)->SetValue(rs.msg[1]);
    XRCCTRL(*this, "spnRegexMsg3",     wxSpinCtrl)->SetValue(rs.msg[2]);
    XRCCTRL(*this, "spnRegexFilename", wxSpinCtrl)->SetValue(rs.filename);
    XRCCTRL(*this, "spnRegexLine",     wxSpinCtrl)->SetValue(rs.line);
}

void AdvancedCompilerOptionsDlg::SaveRegexDetails(int index)
{
    if (index == -1)
        return;

    RegExStruct& rs = m_Regexes[index];
    rs.desc     = XRCCTRL(*this, "txtRegexDesc",     wxTextCtrl)->GetValue();
    rs.lt       = (CompilerLineType)XRCCTRL(*this, "cmbRegexType", wxComboBox)->GetSelection();
    rs.SetRegExString(StringToControlChars(XRCCTRL(*this, "txtRegex",  wxTextCtrl)->GetValue()));
    rs.msg[0]   = XRCCTRL(*this, "spnRegexMsg1",     wxSpinCtrl)->GetValue();
    rs.msg[1]   = XRCCTRL(*this, "spnRegexMsg2",     wxSpinCtrl)->GetValue();
    rs.msg[2]   = XRCCTRL(*this, "spnRegexMsg3",     wxSpinCtrl)->GetValue();
    rs.filename = XRCCTRL(*this, "spnRegexFilename", wxSpinCtrl)->GetValue();
    rs.line     = XRCCTRL(*this, "spnRegexLine",     wxSpinCtrl)->GetValue();
}

void AdvancedCompilerOptionsDlg::OnCommandsChange(wxCommandEvent& WXUNUSED(event))
{
    CheckForChanges();
    const int cmd = XRCCTRL(*this, "lstCommands", wxChoice)->GetSelection();
    ReadExtensions(cmd); // can change the extension cmb list !!!!!!!!!!!!!!!
    const int ext = XRCCTRL(*this, "lstExt",      wxChoice)->GetSelection();
    DisplayCommand(cmd, ext);
}

void AdvancedCompilerOptionsDlg::OnExtChange(wxCommandEvent& WXUNUSED(event))
{
    CheckForChanges();
    const int cmd = XRCCTRL(*this, "lstCommands", wxChoice)->GetSelection();
    const int ext = XRCCTRL(*this, "lstExt",      wxChoice)->GetSelection();
    DisplayCommand(cmd, ext);
}

void AdvancedCompilerOptionsDlg::OnAddExt(wxCommandEvent& WXUNUSED(event))
{
    wxString ext = cbGetTextFromUser(_("Please enter a semi-colon separated list of extensions, without the leading dot:"),
                                     _("New extension"), wxString(), this);
    ext.Trim(false);
    ext.Trim(true);
    if (!ext.empty())
    {
        const int nr = XRCCTRL(*this, "lstCommands", wxChoice)->GetSelection();
        CompilerTool* ptool = GetCompilerTool(nr, 0);
        CompilerTool tool(ptool ? ptool->command : wxString(), ext);
        m_Commands[nr].push_back(tool);
        ReadExtensions(nr);
        wxChoice* cho = XRCCTRL(*this, "lstExt", wxChoice);
        cho->SetStringSelection(ext);
        DisplayCommand(nr, cho->GetSelection());
    }
}

void AdvancedCompilerOptionsDlg::OnDelExt(wxCommandEvent& WXUNUSED(event))
{
    if (cbMessageBox(_("Are you sure you want to remove this extension set from the list?"), _("Confirmation"), wxYES_NO, this) == wxID_YES)
    {
        const int nr = XRCCTRL(*this, "lstCommands", wxChoice)->GetSelection();
        wxChoice* cho = XRCCTRL(*this, "lstExt", wxChoice);
        const wxString extS(cho->GetStringSelection());
        if (!extS.empty())
        {
            const int ext = cho->GetSelection();
            m_Commands[nr].erase(m_Commands[nr].begin() + ext);
            ReadExtensions(nr);
            cho->SetSelection(0);
            m_LastExtIndex = -1;
            DisplayCommand(nr, 0);
        }
        else
            cbMessageBox(_("Can't remove default commands!"), _("Error"), wxICON_ERROR, this);
    }
}

void AdvancedCompilerOptionsDlg::OnRegexChange(wxCommandEvent& WXUNUSED(event))
{
    // If we just have deleted the actual item or cleared the list, we should leave,
    // otherwise m_Regexes will be broken, because the regex details of an invalid
    // item will be saved
    wxListBox* list = XRCCTRL(*this, "lstRegex", wxListBox);
    if (list->GetSelection() == wxNOT_FOUND)
        return;

    SaveRegexDetails(m_SelectedRegex);

    // update regex list, in case desc was changed
    XRCCTRL(*this, "lstRegex", wxListBox)->SetString(m_SelectedRegex, XRCCTRL(*this, "txtRegexDesc", wxTextCtrl)->GetValue());

    m_SelectedRegex = XRCCTRL(*this, "lstRegex", wxListBox)->GetSelection();
    FillRegexDetails(m_SelectedRegex);
}

void AdvancedCompilerOptionsDlg::OnRegexAdd(wxCommandEvent& WXUNUSED(event))
{
    SaveRegexDetails(m_SelectedRegex);
    m_Regexes.push_back(RegExStruct(_("New regular expression"), cltError, wxString(), 0));
    m_SelectedRegex = m_Regexes.size() - 1;
    FillRegexes();
}

void AdvancedCompilerOptionsDlg::OnRegexDelete(wxCommandEvent& WXUNUSED(event))
{
    if (cbMessageBox(_("Are you sure you want to delete this regular expression?"), _("Confirmation"), wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT, this) == wxID_YES)
    {
        m_Regexes.erase((m_Regexes.begin() + m_SelectedRegex));
        if (m_SelectedRegex >= (int)m_Regexes.size())
            --m_SelectedRegex;

        FillRegexes();
    }
}

void AdvancedCompilerOptionsDlg::OnRegexDefaults(wxCommandEvent& WXUNUSED(event))
{
    if (cbMessageBox(_("Are you sure you want to load the default regular expressions "
                    "for this compiler?\n"
                    "ALL regular expressions will be erased and replaced with their default "
                    "counterparts!\n\n"
                    "Are you REALLY sure?"), _("Confirmation"), wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT, this) == wxID_YES)
    {
        Compiler* compiler = CompilerFactory::GetCompiler(m_CompilerId);
        if (!compiler)
            return;

        compiler->LoadDefaultRegExArray(true);
        m_Regexes = compiler->GetRegExArray();
        while (m_SelectedRegex >= (int)m_Regexes.size())
            --m_SelectedRegex;

        FillRegexes();
    }
}

void AdvancedCompilerOptionsDlg::OnRegexUp(wxSpinEvent& WXUNUSED(event))
{
    if (m_SelectedRegex <= 0)
        return;

    SaveRegexDetails(m_SelectedRegex);
    RegExStruct rs = m_Regexes[m_SelectedRegex];
    m_Regexes.erase(m_Regexes.begin() + m_SelectedRegex);
    m_Regexes.insert((m_Regexes.begin() + (m_SelectedRegex - 1)), rs);
    --m_SelectedRegex;
    FillRegexes();
}

void AdvancedCompilerOptionsDlg::OnRegexDown(wxSpinEvent& WXUNUSED(event))
{
    if (m_SelectedRegex >= (int)m_Regexes.size() - 1)
        return;

    SaveRegexDetails(m_SelectedRegex);
    RegExStruct rs = m_Regexes[m_SelectedRegex];
    m_Regexes.erase(m_Regexes.begin() + m_SelectedRegex);
    m_Regexes.insert((m_Regexes.begin() + (m_SelectedRegex + 1)), rs);
    ++m_SelectedRegex;
    FillRegexes();
}

void AdvancedCompilerOptionsDlg::OnRegexTest(wxCommandEvent& WXUNUSED(event))
{
    if (m_SelectedRegex == -1)
        return;

    const wxString text(XRCCTRL(*this, "txtRegexTest", wxTextCtrl)->GetValue());
    if (text.empty())
    {
        cbMessageBox(_("Please enter a compiler line in the \"Compiler output\" text box..."), _("Error"), wxICON_ERROR, this);
        return;
    }

    Compiler* compiler = CompilerFactory::GetCompiler(m_CompilerId);
    if (!compiler)
        return;

    // backup regexes
    RegExArray regex_copy = m_Regexes;
    SaveRegexDetails(m_SelectedRegex);

    // test-run
    compiler->SetRegExArray(m_Regexes);
    long regex_id; wxString regex_desc;
    CompilerLineType clt = compiler->CheckForWarningsAndErrors(text, regex_id, regex_desc);

    // restore regexes
    compiler->SetRegExArray(regex_copy);
    m_Regexes = regex_copy;

    wxString msg;
    msg.Printf(_("Regular expression analyzed as follows:\n\n"
                 "Type: %s message\n"
                 "Filename: %s\n"
                 "Line number: %s\n"
                 "Message: %s\n\n"
                 "Matching regex-id: %ld ('%s')"),
                    clt == cltNormal ? _("Normal")
                 : (clt == cltInfo   ? _("Info")
                 : (clt == cltError  ? _("Error") : _("Warning") ) ),
                compiler->GetLastErrorFilename(),
                compiler->GetLastErrorLine(),
                compiler->GetLastError(),
                regex_id, regex_desc
              );

    cbMessageBox(msg, _("Test results"), wxICON_INFORMATION, this);
}

void AdvancedCompilerOptionsDlg::EndModal(int retCode)
{
    if (retCode == wxID_OK)
    {
        m_bDirty = true;
        Compiler* compiler = CompilerFactory::GetCompiler(m_CompilerId);

        // make sure we update the first command, if it changed
        CheckForChanges();
        // write options
        WriteCompilerOptions();
        // save regexes
        SaveRegexDetails(m_SelectedRegex);
        if (compiler)
            compiler->SetRegExArray(m_Regexes);
    }

    wxScrollingDialog::EndModal(retCode);
}
