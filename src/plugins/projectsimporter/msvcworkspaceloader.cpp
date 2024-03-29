/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 13270 $
 * $Id: msvcworkspaceloader.cpp 13270 2023-05-10 15:52:02Z wh11204 $
 * $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/projectsimporter/msvcworkspaceloader.cpp $
 */

#include "sdk.h"

#ifndef CB_PRECOMP
    #include <wx/string.h>
    #include <wx/intl.h>
    #include <wx/txtstrm.h>
    #include <wx/dynarray.h>
    #include <wx/filename.h>
    #include <wx/msgdlg.h>
    #include <wx/wfstream.h>

    #include "globals.h"
    #include "manager.h"
    #include "logmanager.h"
    #include "projectmanager.h"
    #include "compilerfactory.h"
    #include "compiler.h"
    #include "cbproject.h"
#endif

#include <wx/stream.h>
#include <wx/progdlg.h>

#include "msvcworkspaceloader.h"
#include "importers_globals.h"
#include "filefilters.h"

MSVCWorkspaceLoader::MSVCWorkspaceLoader()
{
    //ctor
}

MSVCWorkspaceLoader::~MSVCWorkspaceLoader()
{
    //dtor
}

bool MSVCWorkspaceLoader::Open(const wxString& filename, wxString& Title)
{
    bool askForCompiler = false;
    bool askForTargets = false;
    switch (cbMessageBox(_("Do you want the imported projects to use the default compiler?\n"
                           "(If you answer No, you will be asked for each and every project"
                           " which compiler to use...)"), _("Question"), wxICON_QUESTION | wxYES_NO | wxCANCEL))
    {
        case wxID_YES:
            askForCompiler = false;
            break;
        case wxID_NO:
            askForCompiler = true;
            break;
        case wxID_CANCEL: // fall-through
        default:
            return false;
    }
    switch (cbMessageBox(_("Do you want to import all configurations (e.g. Debug/Release) from the "
                           "imported projects?\n"
                           "(If you answer No, you will be asked for each and every project"
                           " which configurations to import...)"), _("Question"), wxICON_QUESTION | wxYES_NO | wxCANCEL))
    {
        case wxID_YES:
            askForTargets = false; break;
        case wxID_NO:
            askForTargets = true; break;
        case wxID_CANCEL: // fall-through
        default:
            return false;
    }

    wxFileInputStream file(filename);
    if (!file.Ok())
        return false; // error opening file???

    wxArrayString comps;
    wxTextInputStream input(file);

    // read "header"
    if (!file.Eof())
    {
        wxString line = input.ReadLine();
        if (line.IsEmpty())
        {
            Manager::Get()->GetLogManager()->DebugLog(_T("Workspace file has unsupported format."));
            return false;
        }
        comps = GetArrayFromString(line, _T(","));
        line = comps[0];
        line.Trim(true);
        line.Trim(false);
        if (line != _T("Microsoft Developer Studio Workspace File"))
        {
            Manager::Get()->GetLogManager()->DebugLog(_T("Workspace file has unsupported format."));
            return false;
        }
        line = comps.GetCount() > 1 ? comps[1] : wxString(wxEmptyString);
        line.Trim(true);
        line.Trim(false);

        if (line != _T("Format Version 6.00"))
            Manager::Get()->GetLogManager()->DebugLog(_T("Workspace format not recognized. Will try to parse though..."));
    }

    ImportersGlobals::UseDefaultCompiler = !askForCompiler;
    ImportersGlobals::ImportAllTargets = !askForTargets;

    wxProgressDialog progress(_("Importing MSVC 6 workspace"),
                              _("Please wait while importing MSVC 6 workspace..."),
                              100, 0, wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT);

    int count = 0;
    cbProject* project = nullptr;
    cbProject* firstproject = nullptr;
    wxFileName wfname = filename;
    wfname.Normalize(wxPATH_NORM_DOTS | wxPATH_NORM_TILDE | wxPATH_NORM_ABSOLUTE | wxPATH_NORM_LONG | wxPATH_NORM_SHORTCUT);
    Manager::Get()->GetLogManager()->DebugLog(_T("Workspace dir: ") + wfname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));

    while (!file.Eof())
    {
        wxString line = input.ReadLine();

        line.Trim(true);
        line.Trim(false);

        // example wanted line:
        //Project: "Demo_BSP"=.\Samples\BSP\scripts\Demo_BSP.dsp - Package Owner=<4>
        if (line.StartsWith(_T("Project:")))
        {
            line.Remove(0, 8); // remove "Project:"
            // now we need to find the equal sign (=) that separates the
            // project title from the filename, and the minus sign (-)
            // that separates the filename from junk info - at least to this importer ;)
            int equal = line.Find(_T('='));
            int minus = line.Find(_T('-'), true); // search from end

            if (equal == -1 || minus == -1)
            {
                Manager::Get()->GetLogManager()->DebugLog(_T("Skipping invalid project (unrecognised format) in workspace file."));
                continue;
            }

            // read project title and trim quotes
            wxString prjTitle = line.Left(equal);
            prjTitle.Trim(true);
            prjTitle.Trim(false);
            if (prjTitle.IsEmpty())
            {
                Manager::Get()->GetLogManager()->DebugLog(_T("Skipping invalid project (empty name) in workspace file."));
                continue;
            }
            if (prjTitle.GetChar(0) == _T('\"'))
            {
                prjTitle.Truncate(prjTitle.Length() - 1);
                prjTitle.Remove(0, 1);
            }

            // read project filename and trim quotes
            ++equal;
            wxString prjFile = line.Mid(equal, minus - equal);
            prjFile.Trim(true);
            prjFile.Trim(false);

            if (prjFile.IsEmpty())
            {
                Manager::Get()->GetLogManager()->DebugLog(_T("Skipping invalid project (empty file) in workspace file."));
                continue;
            }

            if (prjFile.GetChar(0) == _T('\"'))
            {
                prjFile.Truncate(prjFile.Length() - 1);
                prjFile.Remove(0, 1);
            }

            wxFileName fname(UnixFilename(prjFile));
            fname.Normalize(wxPATH_NORM_DOTS | wxPATH_NORM_TILDE | wxPATH_NORM_ABSOLUTE | wxPATH_NORM_LONG | wxPATH_NORM_SHORTCUT,
                            wfname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR), wxPATH_NATIVE);
            if (!fname.FileExists())
            {
                Manager::Get()->GetLogManager()->DebugLog(wxString::Format("Project '%s' from '%s' not found.", prjTitle, fname.GetFullPath()));
                continue;
            }
            Manager::Get()->GetLogManager()->DebugLog(wxString::Format("Found project '%s' in '%s'", prjTitle, fname.GetFullPath()));

            int percentage = ((int)file.TellI())*100 / (int)(file.GetLength());
            // While updating the progrerss dialog check for cancellation - probably interrupt.
            if (!progress.Update(percentage, _("Importing project: ") + prjTitle))
                break;

            ++count;

            // project will always be NULL, because the Project Manager use the MIME plugin method "OpenFile"
            // this method returns only an int.
            project = Manager::Get()->GetProjectManager()->LoadProject(fname.GetFullPath(), false);
            if (!project)
            {
                // try to find the opened project
                wxFileName sCodeBlockProject(fname);
                sCodeBlockProject = fname.GetFullPath();
                sCodeBlockProject.SetExt(FileFilters::CODEBLOCKS_EXT);

                project = Manager::Get()->GetProjectManager()->IsOpen(sCodeBlockProject.GetFullPath());
            }

            if (!firstproject)
                firstproject = project;

            if (project)
                registerProject(project->GetTitle(), project);
        }

        /*
         * example wanted line:
         * Project_Dep_Name VstSDK
         * and add the dependency/link of the VstSDK project to the current project
         * be carefull, the dependent projects could not have already been read, so we have to remember them
         */
        else if (line.StartsWith(_T("Project_Dep_Name")))
        {
            line.Remove(0, 16);
            line.Trim(false);
            if (project) addDependency(project->GetTitle(), line);
        }
    }

    if (firstproject)
        Manager::Get()->GetProjectManager()->SetProject(firstproject);

    updateProjects();
    ImportersGlobals::ResetDefaults();

    Title = wxFileName(filename).GetName() + _(" workspace");
    return count != 0;
}

bool MSVCWorkspaceLoader::Save(cb_unused const wxString& title, cb_unused const wxString& filename)
{
    // no support for saving workspace files (.dsw)
    return false;
}
