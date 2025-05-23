/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 13575 $
 * $Id: configmanager.cpp 13575 2024-09-30 06:50:24Z ollydbg $
 * $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/sdk/configmanager.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "configmanager.h"
    #include "globals.h"
    #include "personalitymanager.h"
    #include "cbexception.h"
    #include "logmanager.h"
    #include <wx/file.h>
    #include <wx/dir.h>
    #include <wx/log.h> // for wxSafeShowMessage()
#endif

#include "crc32.h"

#include <wx/url.h>
#include <wx/stream.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

#ifdef __WXMSW__
#include <shlobj.h>
#endif

#include "annoyingdialog.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <sys/param.h>
#include <mach-o/dyld.h>
#endif

#ifdef __WXMAC__
#include "wx/osx/core/cfstring.h"
#include "wx/intl.h"

#include <CoreFoundation/CFBundle.h>
#include <CoreFoundation/CFURL.h>
#endif

#include "tinywxuni.h"
#include <stdlib.h>

#ifdef __linux__
#include <glib.h>
#endif // __linux__

template<> CfgMgrBldr* DLLIMPORT Mgr<CfgMgrBldr>::instance = nullptr;
template<> bool DLLIMPORT Mgr<CfgMgrBldr>::isShutdown = false;

wxString ConfigManager::alternate_user_data_path;
bool ConfigManager::has_alternate_user_data_path=false;

wxString ConfigManager::config_folder;
wxString ConfigManager::home_folder;
wxString ConfigManager::data_path_user;
wxString ConfigManager::data_path_global;
#ifdef CB_AUTOCONF
wxString ConfigManager::plugin_path_global;
#endif
wxString ConfigManager::app_path;
wxString ConfigManager::temp_folder;

namespace CfgMgrConsts
{
    const wxString app_path(_T("app_path"));
    const wxString data_path(_T("data_path"));
    const wxString dotDot(_T(".."));
    const int version = 1;
}

namespace
{
    wxString DetermineExecutablePath()
    {
        #ifdef __WXMSW__
            wxChar name[MAX_PATH];
            GetModuleFileName(0L, name, MAX_PATH);
            wxFileName fname(name);
            return fname.GetPath(wxPATH_GET_VOLUME);
        #else
        #ifdef __linux__
            char c[PATH_MAX+1];
            char *p = realpath("/proc/self/exe", &c[0]);
            if (p == nullptr)
                return _T(".");
            wxFileName fname(cbC2U(p));
            return fname.GetPath(wxPATH_GET_VOLUME);
        #elif defined(sun) || defined(__sun)
            wxFileName fname(cbC2U(getexecname()));
            return fname.GetPath(wxPATH_GET_VOLUME);
        #elif defined(__APPLE__) && defined(__MACH__)
            char path[MAXPATHLEN+1];
            uint32_t path_len = MAXPATHLEN;
            // SPI first appeared in Mac OS X 10.2
            _NSGetExecutablePath(path, &path_len);
            wxFileName fname(wxString(path, wxConvUTF8));
            return fname.GetPath(wxPATH_GET_VOLUME);
        #else
            return _T(".");
        #endif
        #endif
    }

    wxString DetermineResourcesPath()
    {
        #if defined(__WXMAC__)
            CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
            CFURLRef absoluteURL  = CFURLCopyAbsoluteURL(resourcesURL); // relative -> absolute
            CFRelease(resourcesURL);
            CFStringRef cfStrPath = CFURLCopyFileSystemPath(absoluteURL,kCFURLPOSIXPathStyle);
            CFRelease(absoluteURL);
            wxString str = wxCFStringRef(cfStrPath).AsString(wxLocale::GetSystemEncoding());
            if (!str.Contains(wxString(_T("/Resources"))))
               return ::DetermineExecutablePath() + _T("/.."); // not a bundle, use relative path
            return str;
        #else
            return _T(".");
        #endif
    }
}

inline void ConfigManager::Collapse(wxString& str) const
{
    const wxChar *src = str.c_str();
    wxChar *dst = (wxChar*) src;
    wxChar c;
    size_t len = 0;

    while ((c = *src))
    {
        ++src;

        *dst = c;
        ++dst;
        ++len;

        if (c == _T('/'))
        while (*src == _T('/'))
            ++src;
    }
    str.Truncate(len);
}

ISerializable::ISerializable()
{}

ISerializable::~ISerializable()
{}

/* ------------------------------------------------------------------------------------------------------------------
*  "Builder pattern" class for ConfigManager
*  Do not use this class  -  Manager::Get()->GetConfigManager() is a lot friendlier
*/

CfgMgrBldr::CfgMgrBldr() : doc(nullptr), volatile_doc(nullptr), r(false)
{
    ConfigManager::MigrateFolders();

    TiXmlBase::SetCondenseWhiteSpace(false);
    wxString personality(Manager::Get()->GetPersonalityManager()->GetPersonality());

    if (personality.StartsWith(_T("http://")))
    {
        SwitchToR(personality);
        return;
    }

    cfg = FindConfigFile(personality + _T(".conf"));

    if (cfg.IsEmpty())
    {
        cfg = ConfigManager::GetConfigFolder() + wxFILE_SEP_PATH + personality + _T(".conf");
        doc = new TiXmlDocument();
        doc->InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", "yes"));
        doc->InsertEndChild(TiXmlElement("CodeBlocksConfig"));
        doc->FirstChildElement("CodeBlocksConfig")->SetAttribute("version", CfgMgrConsts::version);
        return;
    }
    SwitchTo(cfg);
}

wxString CfgMgrBldr::FindConfigFile(const wxString& filename)
{

    wxString u(ConfigManager::GetUserDataFolder() + wxFILE_SEP_PATH + filename);
    wxString exePath(::DetermineExecutablePath());
    wxString e(exePath + wxFILE_SEP_PATH + filename);

    if (!ConfigManager::has_alternate_user_data_path && ::wxFileExists(e))
    {
        ConfigManager::SetUserDataFolder(exePath);
        return e;
    }
    if (::wxFileExists(u))
    {
        return u;
    }
    return wxEmptyString;
}

/// Print error message and allow the user to either discard the old config or close the application.
/// Call this function when you've detected an error while reading the config.
static void handleConfigError(TiXmlDocument &doc, const wxString &fileName, const wxString &additionalMessage)
{
    wxString message;
    if (doc.ErrorId())
    {
        message = wxString::Format(_("TinyXML error: %s\nIn file: %s\nAt row %d, column: %d.\n\n"),
                                   cbC2U(doc.ErrorDesc()).c_str(), fileName.wx_str(),
                                   doc.ErrorRow(), doc.ErrorCol());
    }
    message += additionalMessage;

    // Show a message box and ask the user to either abort or discard the old config.
    wxMessageDialog dlg(Manager::Get()->GetAppWindow(),
                        message + _("\n\nDiscard old config file?"), _("Config file read error"),
                        wxSTAY_ON_TOP|wxCENTRE|wxYES|wxNO|wxNO_DEFAULT|wxICON_ERROR);
    dlg.SetYesNoLabels(_("&Discard"), _("&Close"));
    if (dlg.ShowModal() != wxID_YES)
        cbThrow(message);

    doc.ClearError();
    doc.InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", "yes"));
    doc.InsertEndChild(TiXmlElement("CodeBlocksConfig"));
    doc.FirstChildElement("CodeBlocksConfig")->SetAttribute("version", CfgMgrConsts::version);
}

void CfgMgrBldr::SwitchTo(const wxString& fileName)
{
    doc = new TiXmlDocument();

    if (!TinyXML::LoadDocument(fileName, doc))
    {
        const wxString message = wxString::Format(_("Error reading config file: %s"),
                                                  fileName.wx_str());
        handleConfigError(*doc, fileName, message);
    }

    TiXmlElement* docroot = doc->FirstChildElement("CodeBlocksConfig");
    if (!docroot)
    {
        const wxString message = wxString::Format(_("Cannot find docroot in config file '%s'"), fileName);
        handleConfigError(*doc, fileName, message);
        docroot = doc->FirstChildElement("CodeBlocksConfig");

        if (!docroot)
            cbThrow(_("Something really bad happened while reading the config file. Aborting!"));
    }

    const char *vers = docroot->Attribute("version");
    if (!vers || atoi(vers) != 1)
        cbMessageBox(_("ConfigManager encountered an unknown config file version. Continuing happily."), _("Warning"), wxICON_WARNING);

    doc->ClearError();

    wxString info;
    info.Printf(_T(" application info:\n"
                    "\t svn_revision:\t%u\n"
                    "\t build_date:\t%s, %s\n"), ConfigManager::GetRevisionNumber(), wxT(__DATE__), wxT(__TIME__));
#if defined(__clang__)
    info += wxString::Format(wxT("\t compiler_version:\tclang %d.%d.%d\n"), __clang_major__,
                             __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
    info += wxString::Format(wxT("\t compiler_version:\tgcc %d.%d.%d\n"), __GNUC__, __GNUC_MINOR__,
                             __GNUC_PATCHLEVEL__);
#endif

    if (platform::windows)
        info.append(_T("\t Windows "));
    else if (platform::Linux)
        info.append(_T("\t Linux "));
    else if (platform::macosx)
        info.append(_T("\t Mac OS X "));
    else if (platform::Unix)
        info.append(_T("\t Unix "));

    info.append(platform::unicode ? _T("Unicode") : _T("ANSI"));
    info.append(wxT("\n"));
    TiXmlComment c;
    c.SetValue((const char*) info.mb_str());

    TiXmlNode *firstchild = docroot->FirstChild();
    if (firstchild && firstchild->ToComment())
    {
        docroot->RemoveChild(firstchild);
        firstchild = docroot->FirstChild();
    }

    if (firstchild)
        docroot->InsertBeforeChild(firstchild, c);
    else
        docroot->InsertEndChild(c);
}

void CfgMgrBldr::SwitchToR(const wxString& absFileName)
{
    if (doc)
        delete doc;
    doc = new TiXmlDocument();
    doc->ClearError();

    cfg = absFileName;

    wxURL url(absFileName);
    url.SetProxy(ConfigManager::GetProxy());
    if (url.GetError() == wxURL_NOERR)
    {
        wxInputStream *is = url.GetInputStream();
        if (is && is->IsOk())
        {
            size_t size = is->GetSize();
            wxString str;
            wxChar* c = wxStringBuffer(str, size);
            is->Read(c, size);

            doc = new TiXmlDocument();

            if (doc->Parse(cbU2C(str)))
            {
                doc->ClearError();
                delete is;
                return;
            }
            if (Manager::Get()->GetLogManager())
            {
                Manager::Get()->GetLogManager()->DebugLog(_("##### Error loading or parsing remote config file"));
                Manager::Get()->GetLogManager()->DebugLog(cbC2U(doc->ErrorDesc()));
                doc->ClearError();
            }
        }
        delete is;
    }
    cfg.Empty();
    SwitchTo(wxEmptyString); // fall back
}

CfgMgrBldr::~CfgMgrBldr()
{
    NamespaceMap::iterator it;
    for ( it = namespaces.begin(); it != namespaces.end(); ++it )
        delete it->second;

    namespaces.clear();
    Close();
    delete volatile_doc;
}

void CfgMgrBldr::Flush()
{
    if (doc)
    {
        if (!cfg.StartsWith(_T("http://")))
        {
            bool done = false;
            do
            {
                if (TinyXML::SaveDocument(cfg, doc))
                    done = true;
                else
                {
                    AnnoyingDialog dlg(_("Error"),
                                       wxString::Format(_("Could not save config file '%s'!"), cfg),
                                       wxART_ERROR, AnnoyingDialog::TWO_BUTTONS,
                                       AnnoyingDialog::rtTWO, _("&Retry"), _("&Close"));
                    switch (dlg.ShowModal())
                    {
                        case AnnoyingDialog::rtONE:
                            done = false;
                            break;
                        case AnnoyingDialog::rtTWO:
                        default:
                            done = true;
                    }
                }
            } while (!done);
        }
        else
        {
            // implement WebDAV another time
        }
    }
}

void CfgMgrBldr::Close()
{
    Flush();

    if (doc)
        delete doc;

    doc = nullptr;
}

ConfigManager* CfgMgrBldr::GetConfigManager(const wxString& name_space)
{
    return Get()->Build(name_space);
}

ConfigManager* CfgMgrBldr::Build(const wxString& name_space)
{
    if (name_space.IsEmpty())
        cbThrow(_("You attempted to get a ConfigManager instance without providing a namespace."));

    wxCriticalSectionLocker locker(cs);
    NamespaceMap::iterator it = namespaces.find(name_space);
    if (it != namespaces.end())
        return it->second;

    TiXmlElement* docroot;

    if (name_space.StartsWith(_T("volatile:")))
    {
        if (!volatile_doc)
        {
            volatile_doc = new TiXmlDocument();
            volatile_doc->InsertEndChild(TiXmlElement("CodeBlocksConfig"));
            volatile_doc->SetCondenseWhiteSpace(false);
        }
        docroot = volatile_doc->FirstChildElement("CodeBlocksConfig");
    }
    else
    {
        docroot = doc->FirstChildElement("CodeBlocksConfig");
        if (!docroot)
        {
            wxString err(_("Fatal error parsing supplied configuration file.\nParser error message:\n"));
            err << wxString::Format(_("%s\nAt row %d, column %d."), cbC2U(doc->ErrorDesc()), doc->ErrorRow(), doc->ErrorCol());
            cbThrow(err);
        }
    }

    TiXmlElement* root = docroot->FirstChildElement(cbU2C(name_space));

    if (!root) // namespace does not exist
    {
        docroot->InsertEndChild(TiXmlElement(cbU2C(name_space)));
        root = docroot->FirstChildElement(cbU2C(name_space));
    }

    if (!root) // now what!
        cbThrow(_("Unable to create namespace in document tree (actually not possible..?)"));

    ConfigManager *c = new ConfigManager(root);
    namespaces[name_space] = c;

    return c;
}

wxString CfgMgrBldr::GetConfigFile() const
{
    return cfg;
}

/*
*  Hack to enable Turkish language. wxString::Upper will convert lowercase 'i' to \u0130 instead of \u0069 in Turkish locale,
*  which will break the config file when used in a tag
*/
inline void to_upper(wxString& s)
{
    wxStringCharType *p = const_cast<wxStringCharType*>(s.wx_str());
    wxStringCharType q;
    size_t len = s.length()+1;
    for (;--len;++p)
    {
        q = *p;
        if (q >= 'a' && q <= 'z')
            *p = q - 32;
    }
}

inline void to_lower(wxString& s)
{
    wxStringCharType *p = const_cast<wxStringCharType*>(s.wx_str());
    wxStringCharType q;
    size_t len = s.length()+1;
    for (;--len;++p)
    {
        q = *p;
        if (q >= 'A' && q <= 'Z')
            *p = q + 32;
    }
}


/* ------------------------------------------------------------------------------------------------------------------
*  Functions to retrieve system paths and locate data files in a defined, consistent way.
*  Please note that the application determines app_path and data_path at runtime and passes the results
*  to ConfigManager. GetExecutableFolder() and GetDataFolder() are therefore under normal conditions
*  simply more efficient shortcuts for Read("app_path") and Read("data_path").
*/

wxString ConfigManager::GetProxy()
{
    return Manager::Get()->GetConfigManager(_T("app"))->Read(_T("network_proxy"));
}

wxString ConfigManager::GetFolder(SearchDirs dir)
{
    static bool once = 1;

    if (once)
    {
        InitPaths();
        once = false;
    }

    switch (dir)
    {
        case sdHome:
            return ConfigManager::home_folder;

        case sdBase:
            return ConfigManager::app_path;

        case sdTemp:
            return ConfigManager::temp_folder;

        case sdConfig:
            return ConfigManager::config_folder;

        case sdCurrent:
            return ::wxGetCwd();

        case sdPluginsGlobal:
#ifndef CB_AUTOCONF
            return ConfigManager::data_path_global + wxFILE_SEP_PATH + _T("plugins");
#else
            return ConfigManager::plugin_path_global;
#endif

        case sdPluginsUser:
            return ConfigManager::data_path_user   + wxFILE_SEP_PATH + _T("plugins");

        case sdScriptsGlobal:
            return ConfigManager::data_path_global + wxFILE_SEP_PATH + _T("scripts");

        case sdScriptsUser:
            return ConfigManager::data_path_user   + wxFILE_SEP_PATH + _T("scripts");

        case sdDataGlobal:
            return ConfigManager::data_path_global;

        case sdDataUser:
            return ConfigManager::data_path_user;

        case sdPath:
        case sdAllUser:
        case sdAllGlobal:
        case sdAllKnown:
        default:
            return wxEmptyString;
    }
}

inline wxString ConfigManager::GetUserDataFolder()
{
    if (has_alternate_user_data_path)
        return alternate_user_data_path;
#ifdef __WINDOWS__
    TCHAR buffer[MAX_PATH];
    if (!ConfigManager::has_alternate_user_data_path && ::GetEnvironmentVariable(_T("APPDATA"), buffer, MAX_PATH))
        return wxString::Format(_T("%s\\CodeBlocks"), buffer);
    else
        return wxStandardPathsBase::Get().GetUserDataDir();
#else
#ifdef __linux__
    gchar *filename = g_build_filename(g_get_user_config_dir(), "codeblocks", nullptr);
    wxString result=wxString::FromUTF8(filename);
    g_free(filename);
    return result;
#else
    return wxStandardPathsBase::Get().GetUserDataDir();
#endif // __linux__
#endif // __WINDOWS__
}

bool ConfigManager::SetUserDataFolder(const wxString &user_data_path)
{
    wxString udp = wxFileName::DirName(user_data_path).GetFullPath();
    if (!CreateDirRecursively(udp))
    {
        cbMessageBox(wxString::Format(_("The --user-data-dir directory %s does not exist and could not be created. Please check the path and try again"),
                                            user_data_path.c_str()), _("Command Line Error"));
        return false;
    }
    has_alternate_user_data_path = true;
    ConfigManager::alternate_user_data_path = udp;
    return true;
}

wxString ConfigManager::LocateDataFile(const wxString& filename, int search_dirs)
{
    wxPathList searchPaths;

    // user dirs have precedence
    if (search_dirs & sdPluginsUser)
        searchPaths.Add(GetPluginsFolder(false));
    if (search_dirs & sdScriptsUser)
        searchPaths.Add(GetScriptsFolder(false));
    if (search_dirs & sdDataUser)
        searchPaths.Add(GetDataFolder(false));

    // then we have global dirs
    if (search_dirs & sdPluginsGlobal)
        searchPaths.Add(GetPluginsFolder(true));
    if (search_dirs & sdScriptsGlobal)
        searchPaths.Add(GetScriptsFolder(true));
    if (search_dirs & sdDataGlobal)
        searchPaths.Add(GetDataFolder(true));

    // rest of the dirs
    if (search_dirs & sdCurrent)
        searchPaths.Add(::wxGetCwd());
    if (search_dirs & sdConfig)
        searchPaths.Add(GetConfigFolder());
    if (search_dirs & sdHome)
        searchPaths.Add(GetHomeFolder());
    if (search_dirs & sdBase)
        searchPaths.Add(GetExecutableFolder());
    if (search_dirs & sdTemp)
        searchPaths.Add(GetTempFolder());

    // PATH env. var
    if (search_dirs & sdPath)
        searchPaths.AddEnvList(_T("PATH"));

    return searchPaths.FindValidPath(filename);
}

/* ------------------------------------------------------------------------------------------------------------------
*  ConfigManager
*/

ConfigManager::ConfigManager(TiXmlElement* r) : doc(r->GetDocument()), root(r), pathNode(r)
{
}

/* ------------------------------------------------------------------------------------------------------------------
*  Configuration path handling
*  Note that due to namespaces, you do no longer need to care about saving/restoring the current path in the normal case.
*  Mostly, there will be only one module working with one namespace, and every namespace keeps track of its own path.
*  Also, please note that GetPath() is more expensive than it seems (not a mere accessor to a member variable!), while
*  SetPath() not only sets the current path, but also creates the respective nodes in the XML document if these don't exist.
*/

wxString ConfigManager::GetPath() const
{
    TiXmlElement *e = pathNode;
    wxString ret;
    ret.Alloc(64);

    ret = cbC2U(e->Value());
    while ((e = e->Parent()->ToElement()) && e != root)
    {
        ret.Prepend(_T('/'));
        ret.Prepend(cbC2U(e->Value()));
    }
    ret.Prepend(_T('/'));
    return ret;
}

void ConfigManager::SetPath(const wxString& path)
{
    wxString p(path + _T('/'));
    pathNode = AssertPath(p);
}

wxString ConfigManager::InvalidNameMessage(const wxString& what, const wxString& sub, TiXmlElement *localPath) const
{
    wxString s;
    s.Printf(_("The %s %s (child of node \"%s\" in namespace \"%s\") does not meet the standard for path naming (must start with a letter)."),
    what,
    sub,
    cbC2U(localPath->Value()).c_str(),
    cbC2U(root->Value()).c_str());

    return s;
}

TiXmlElement* ConfigManager::AssertPath(wxString& path)
{
    Collapse(path);

    wxString illegal(_T(" -:.\"\'$&()[]<>+#"));
    size_t i = 0;
    while ((i = path.find_first_of(illegal, i)) != wxString::npos)
        path[i] = _T('_');

    TiXmlElement *localPath = pathNode ? pathNode : root;

    if (path.GetChar(0) == '/')  // absolute path
    {
        localPath = root;
        path = path.Mid(1);
    }

    if (path.find(_T('/')) != wxString::npos) // need for path walking
        to_lower(path);

    wxString sub;

    while (path.find(_T('/')) != wxString::npos)
    {
        sub = path.BeforeFirst(_T('/'));
        path = path.AfterFirst(_T('/'));

        if (localPath != root && sub.IsSameAs(CfgMgrConsts::dotDot))
            localPath = localPath->Parent()->ToElement();
        else if (sub.GetChar(0) < _T('a') || sub.GetChar(0) > _T('z'))
        {
            cbThrow(InvalidNameMessage(_T("subpath"), sub, localPath));
        }
        else
        {
            TiXmlElement* n = localPath->FirstChildElement(cbU2C(sub));
            if (n)
                localPath = n;
            else
                localPath = (TiXmlElement*) localPath->InsertEndChild(TiXmlElement(cbU2C(sub)));
        }
    }

    to_upper(path);

    if (!path.IsEmpty() && (path.GetChar(0) < _T('A') || path.GetChar(0) > _T('Z')))
        cbThrow(InvalidNameMessage(_T("key"), path, localPath));

    return localPath;
}

/* -----------------------------------------------------------------------------------------------------
*  Clear all nodes from your namespace or delete the namespace alltogether (removing it from the config file).
*  After Delete() returns, the pointer to your instance is invalid.
*/

void ConfigManager::Clear()
{
    root->Clear();
}

void ConfigManager::Delete()
{
    CfgMgrBldr * bld = CfgMgrBldr::Get();
    const wxString ns(cbC2U(root->Value()));

    root->Clear();
    doc->RootElement()->RemoveChild(root);

    wxCriticalSectionLocker(bld->cs);
    NamespaceMap::iterator it = bld->namespaces.find(ns);
    if (it != bld->namespaces.end())
        bld->namespaces.erase(it);

    delete this;
}

void ConfigManager::DeleteAll()
{
    CfgMgrBldr * bld = CfgMgrBldr::Get();
    const wxString ns(cbC2U(root->Value()));

    if (!ns.IsSameAs(_T("app")))
        cbThrow(_("Illegal attempt to invoke DeleteAll()."));

    wxCriticalSectionLocker(bld->cs);
    doc->RootElement()->Clear();
    for (NamespaceMap::iterator it = bld->namespaces.begin(); it != bld->namespaces.end(); ++it)
        delete it->second;

    bld->namespaces.clear();
}

void ConfigManager::Flush()
{
    CfgMgrBldr * bld = CfgMgrBldr::Get();
    wxCriticalSectionLocker(bld->cs);
    bld->Flush();
}

/* ------------------------------------------------------------------------------------------------------------------
*  Utility functions for writing nodes
*/

TiXmlElement* ConfigManager::GetUniqElement(TiXmlElement* p, const wxString& q)
{
    TiXmlElement* r;
    if ((r = p->FirstChildElement(cbU2C(q))))
        return r;

    return (TiXmlElement*)(p->InsertEndChild(TiXmlElement(cbU2C(q))));
}

void ConfigManager::SetNodeText(TiXmlElement* n, const TiXmlText& t)
{
    TiXmlNode *c = n->FirstChild();
    if (c)
        n->ReplaceChild(c, t);
    else
        n->InsertEndChild(t);
}

/* ------------------------------------------------------------------------------------------------------------------
*  Write and read values
*  Regardless of namespaces, the string keys app_path and data_path always refer to the location of the application's executable
*  and the data path, respectively. These values are never saved to the configuration, but kept in static variables.
*  The application makes use of this by "writing" to the configuration file after determining these values at runtime.
*/
void ConfigManager::Write(const wxString& name,  const wxString& value, bool ignoreEmpty)
{
    if (name.IsSameAs(CfgMgrConsts::app_path))
    {
        return;
    }
    else if (name.IsSameAs(CfgMgrConsts::data_path))
    {
        data_path_global = value;
        return;
    }
    if (ignoreEmpty && value.IsEmpty())
    {
        UnSet(name);
        return;
    }

    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *str = GetUniqElement(e, key);

    TiXmlElement *s = GetUniqElement(str, _T("str"));

    TiXmlText t(value.mb_str(wxConvUTF8));
    t.SetCDATA(true);
    SetNodeText(s, t);
}

void ConfigManager::Write(const wxString& key, const char* str)
{
    /* NOTE (mandrav#1#): Do *not* remove 'false' from the call because in ANSI builds,
    it matches this very function and overflows the stack... */
    Write(key, cbC2U(str), false);
}

wxString ConfigManager::Read(const wxString& name, const wxString& defaultVal)
{
    if (name.IsSameAs(CfgMgrConsts::app_path))
        return app_path;
    else if (name.IsSameAs(CfgMgrConsts::data_path))
        return data_path_global;

    wxString ret;

    if (Read(name, &ret))
        return ret;
    else
        return defaultVal;
}

bool ConfigManager::Read(const wxString& name, wxString* str)
{
    if (name.IsSameAs(CfgMgrConsts::app_path))
    {
        str->assign(app_path);
        return true;
    }
    else if (name.IsSameAs(CfgMgrConsts::data_path))
    {
        str->assign(data_path_global);
        return true;
    }

    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlText *t = (TiXmlText *) parentHandle.FirstChild(cbU2C(key)).FirstChild("str").FirstChild().Node();

    if (t)
    {
        str->assign(cbC2U(t->Value()));
        return true;
    }
    return false;
}

void ConfigManager::Write(const wxString& name,  const wxColour& c)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *leaf = GetUniqElement(e, key);

    TiXmlElement *s = GetUniqElement(leaf, _T("colour"));
    if (c == wxNullColour)
    {
        s->SetAttribute("null", "true");
        s->SetAttribute("r", 0);
        s->SetAttribute("g", 0);
        s->SetAttribute("b", 0);
    }
    else
    {
        s->SetAttribute("r", c.Red());
        s->SetAttribute("g", c.Green());
        s->SetAttribute("b", c.Blue());
    }
}

wxColour ConfigManager::ReadColour(const wxString& name, const wxColour& defaultVal)
{
    wxColour ret;

    if (Read(name, &ret))
        return ret;
    else
        return defaultVal;
}

bool ConfigManager::Read(const wxString& name, wxColour* ret)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlElement *c = (TiXmlElement *) parentHandle.FirstChild(cbU2C(key)).FirstChild("colour").Element();

    if (c)
    {
        const char *isNull = c->Attribute("null");
        if (isNull && strcmp(isNull, "true") == 0)
        {
            *ret = wxNullColour;
            return true;
        }
        else
        {
            int r, g, b;
            if (c->QueryIntAttribute("r", &r) == TIXML_SUCCESS
                    && c->QueryIntAttribute("g", &g) == TIXML_SUCCESS
                    && c->QueryIntAttribute("b", &b) == TIXML_SUCCESS)
            {
                ret->Set(r, g, b);
                return true;
            }
        }
    }
    *ret = wxNullColour;
    return false;
}

void ConfigManager::Write(const wxString& name,  int value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);
    TiXmlElement *leaf = GetUniqElement(e, key);

    leaf->SetAttribute("int", value);
}

int  ConfigManager::ReadInt(const wxString& name,  int defaultVal)
{
    int ret;

    if (Read(name, &ret))
        return ret;
    else
        return defaultVal;
}

bool ConfigManager::Read(const wxString& name,  int* value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlElement *leaf = parentHandle.FirstChild(cbU2C(key)).Element();

    if (leaf)
        return leaf->QueryIntAttribute("int", value) == TIXML_SUCCESS;
    return false;
}

void ConfigManager::Write(const wxString& name,  bool value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);
    TiXmlElement *leaf = GetUniqElement(e, key);

    leaf->SetAttribute("bool", value ? "1" : "0");
}

bool  ConfigManager::ReadBool(const wxString& name,  bool defaultVal)
{
    bool ret;

    if (Read(name, &ret))
        return ret;
    else
        return defaultVal;
}

bool ConfigManager::Read(const wxString& name,  bool* value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlElement *leaf = parentHandle.FirstChild(cbU2C(key)).Element();

    if (leaf && leaf->Attribute("bool"))
    {
        *value = leaf->Attribute("bool")[0] == '1';
        return true;
    }
    return false;
}

void ConfigManager::Write(const wxString& name,  double value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);
    TiXmlElement *leaf = GetUniqElement(e, key);

    leaf->SetDoubleAttribute("double", value);
}

double  ConfigManager::ReadDouble(const wxString& name,  double defaultVal)
{
    double ret;

    if (Read(name, &ret))
        return ret;
    else
        return defaultVal;
}

bool ConfigManager::Read(const wxString& name,  double* value)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlElement *leaf = parentHandle.FirstChild(cbU2C(key)).Element();

    if (leaf)
        return leaf->QueryDoubleAttribute("double", value) == TIXML_SUCCESS;
    return false;
}

void ConfigManager::Set(const wxString& name)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);
    GetUniqElement(e, key);
}

void ConfigManager::UnSet(const wxString& name)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlNode *leaf = GetUniqElement(e, key);
    e->RemoveChild(leaf);
}

bool ConfigManager::Exists(const wxString& name)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlElement *leaf = parentHandle.FirstChild(cbU2C(key)).Element();

    return leaf;
}

void ConfigManager::Write(const wxString& name,  const wxArrayString& arrayString)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *leaf = GetUniqElement(e, key);

    TiXmlElement *as;
    as = GetUniqElement(leaf, _T("astr"));
    leaf->RemoveChild(as);
    as = GetUniqElement(leaf, _T("astr"));

    for (unsigned int i = 0; i < arrayString.GetCount(); ++i)
    {
        TiXmlElement s("s");

        TiXmlText t(arrayString[i].mb_str(wxConvUTF8));
        t.SetCDATA(true);

        s.InsertEndChild(t);
        as->InsertEndChild(s);
    }
}

void ConfigManager::Read(const wxString& name, wxArrayString *arrayString)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlNode *asNode = parentHandle.FirstChild(cbU2C(key)).FirstChild("astr").Node();

    TiXmlNode *curr = nullptr;
    if (asNode)
    {
        while ((curr = asNode->IterateChildren("s", curr)))
            arrayString->Add(cbC2U(curr->FirstChild()->ToText()->Value()));
    }
}

wxArrayString ConfigManager::ReadArrayString(const wxString& name)
{
    wxArrayString as;
    Read(name, &as);
    return as;
}

void ConfigManager::WriteBinary(const wxString& name,  const wxString& source)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *str = GetUniqElement(e, key);

    TiXmlElement *s = GetUniqElement(str, _T("bin"));
    s->SetAttribute("crc", wxCrc32::FromString(source));
    SetNodeText(s, TiXmlText(wxBase64::Encode(source).mb_str(wxConvUTF8)));
}

void ConfigManager::WriteBinary(const wxString& name,  void* ptr, size_t len)
{
    wxString s((wxChar*)ptr, len);
    WriteBinary(name,  s);
}

wxString ConfigManager::ReadBinary(const wxString& name)
{
    wxString str;
    wxString key(name);
    TiXmlElement* e = AssertPath(key);
    unsigned int crc = 0;

    TiXmlHandle parentHandle(e);
    TiXmlElement* bin = parentHandle.FirstChild(cbU2C(key)).FirstChild("bin").Element();

    if (!bin)
        return wxEmptyString;

    if (bin->QueryIntAttribute("crc", (int*)&crc) != TIXML_SUCCESS)
        return wxEmptyString;

    if (const TiXmlText* t = bin->FirstChild()->ToText())
    {
        str.assign(cbC2U(t->Value()));
        str = wxBase64::Decode(str);
        if (crc ==  wxCrc32::FromString(str))
            return str;
    }
    return wxEmptyString;
}

wxArrayString ConfigManager::EnumerateSubPaths(const wxString& path)
{
    wxString key(path + _T('/')); // the trailing slash hack is required because AssertPath expects a key name
    TiXmlNode* e = AssertPath(key);
    wxArrayString ret;

    TiXmlElement *curr = nullptr;
    if (e)
    {
        while (e->IterateChildren(curr) && (curr = e->IterateChildren(curr)->ToElement()))
        {
            wxUniChar c = cbC2U(curr->Value())[0];
            if (c < _T('A') || c > _T('Z')) // first char must be a letter, uppercase letters are key names
                ret.Add(cbC2U(curr->Value()));
        }
    }
    return ret;
}

void ConfigManager::DeleteSubPath(const wxString& thePath)
{
    if (doc->ErrorId())
    {
        cbMessageBox(wxString(_T("### TinyXML error:\n")) << cbC2U(doc->ErrorDesc()));
        doc->ClearError();
    }

    wxString path(thePath);
    to_lower(path);

    Collapse(path);

    wxString illegal(_T(" :.,;!\"\'$%&()[]<>{}?*+-|#"));
    size_t i;
    while ((i = path.find_first_of(illegal)) != wxString::npos)
        path[i] = _T('_');

    if (path.Last() == _T('/'))
        path.RemoveLast();

    if (path.IsSameAs(_T("/"))) // this function will refuse to remove root!
        return;

    TiXmlElement* parent = pathNode ? pathNode : root;

    if (path.find(_T('/')) != wxString::npos)
    {
        wxString sub;
        do
        {
            sub = path.BeforeFirst(_T('/'));
            path = path.AfterFirst(_T('/'));

            if (sub.IsEmpty())
                parent = root;
            else if (sub.IsSameAs(_T(".")))
                ;
            else if (parent != root && sub.IsSameAs(_T("..")))
                parent = parent->Parent()->ToElement();
            else
            {
                TiXmlElement* n = parent->FirstChildElement(cbU2C(sub));
                if (n)
                    parent = n;
                else
                    return;
            }
        }
        while (path.find(_T('/')) != wxString::npos);
    }

    if (!path.IsEmpty())
    {
        if (TiXmlNode *toRemove = parent->FirstChild(cbU2C(path)))
        {
            toRemove->Clear();
            parent->RemoveChild(toRemove);
        }
    }
}

wxArrayString ConfigManager::EnumerateKeys(const wxString& path)
{
    wxString key(path + _T('/')); // the trailing slash hack is required because AssertPath expects a key name
    TiXmlNode* e = AssertPath(key);
    wxArrayString ret;

    TiXmlElement *curr = nullptr;
    if (e)
    {
        while (e->IterateChildren(curr) && (curr = e->IterateChildren(curr)->ToElement()))
        {
            wxUniChar c = cbC2U(curr->Value())[0];
            if (c >= _T('A') && c <= _T('Z')) // opposite of the above
                ret.Add(cbC2U(curr->Value()));
        }
    }
    return ret;
}

void ConfigManager::Write(const wxString& name, const ISerializable& object)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *obj = GetUniqElement(e, key);

    TiXmlElement *s = GetUniqElement(obj, _T("obj"));
    SetNodeText(s, TiXmlText(cbU2C(wxBase64::Encode(object.SerializeOut()))));
}

bool ConfigManager::Read(const wxString& name, ISerializable* object)
{
    wxString str;
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlText *t = (TiXmlText *) parentHandle.FirstChild(cbU2C(key)).FirstChild("obj").FirstChild().Node();

    if (t)
    {
        str.assign(cbC2U(t->Value()));
        object->SerializeIn(wxBase64::Decode(str));
    }
    return wxEmptyString;
}

void ConfigManager::Write(const wxString& name, const ConfigManagerContainer::StringToStringMap& map)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *leaf = GetUniqElement(e, key);

    TiXmlElement *mNode;
    mNode = GetUniqElement(leaf, _T("ssmap"));
    leaf->RemoveChild(mNode);
    mNode = GetUniqElement(leaf, _T("ssmap"));

    for (ConfigManagerContainer::StringToStringMap::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        TiXmlElement s(cbU2C(it->first));

        TiXmlText t(cbU2C(it->second));
        t.SetCDATA(true);

        s.InsertEndChild(t);
        mNode->InsertEndChild(s);
    }
}

void ConfigManager::Read(const wxString& name, ConfigManagerContainer::StringToStringMap* map)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlNode *mNode = parentHandle.FirstChild(cbU2C(key)).FirstChild("ssmap").Node();

    TiXmlNode *curr = nullptr;
    if (mNode)
    {
        while ((curr = mNode->IterateChildren(curr)))
            (*map)[cbC2U(curr->Value())] = cbC2U(curr->FirstChild()->ToText()->Value());
    }
}

ConfigManagerContainer::StringToStringMap ConfigManager::ReadSSMap(const wxString& name)
{
    ConfigManagerContainer::StringToStringMap ret;
    Read(name, &ret);
    return ret;
}

void ConfigManager::Write(const wxString& name, const ConfigManagerContainer::IntToStringMap& map)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *leaf = GetUniqElement(e, key);

    TiXmlElement *mNode;
    mNode = GetUniqElement(leaf, _T("ismap"));
    leaf->RemoveChild(mNode);
    mNode = GetUniqElement(leaf, _T("ismap"));

    wxString tmp;
    for (ConfigManagerContainer::IntToStringMap::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        tmp.Printf(_T("x%d"), (int) it->first);
        TiXmlElement s(tmp.mb_str());

        TiXmlText t(cbU2C(it->second));
        t.SetCDATA(true);

        s.InsertEndChild(t);
        mNode->InsertEndChild(s);
    }
}

void ConfigManager::Read(const wxString& name, ConfigManagerContainer::IntToStringMap* map)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlNode *mNode = parentHandle.FirstChild(cbU2C(key)).FirstChild("ismap").Node();

    TiXmlNode *curr = nullptr;
    long tmp;
    if (mNode)
    {
        while ((curr = mNode->IterateChildren(curr)))
        {
            cbC2U(curr->Value()).Mid(1).ToLong(&tmp);
            (*map)[tmp] = cbC2U(curr->FirstChild()->ToText()->Value());
        }
    }
}

ConfigManagerContainer::IntToStringMap ConfigManager::ReadISMap(const wxString& name)
{
    ConfigManagerContainer::IntToStringMap ret;
    Read(name, &ret);
    return ret;
}

void ConfigManager::Write(const wxString& name, const ConfigManagerContainer::StringSet& set)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *leaf = GetUniqElement(e, key);

    TiXmlElement *mNode;
    mNode = GetUniqElement(leaf, _T("sset"));
    leaf->RemoveChild(mNode);
    mNode = GetUniqElement(leaf, _T("sset"));

    for (ConfigManagerContainer::StringSet::const_iterator it = set.begin(); it != set.end(); ++it)
    {
        TiXmlElement s("s");

        TiXmlText t(cbU2C(*it));
        t.SetCDATA(true);

        s.InsertEndChild(t);
        mNode->InsertEndChild(s);
    }
}

void ConfigManager::Read(const wxString& name, ConfigManagerContainer::StringSet* set)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlHandle parentHandle(e);
    TiXmlNode *mNode = parentHandle.FirstChild(cbU2C(key)).FirstChild("sset").Node();

    TiXmlNode *curr = nullptr;
    if (mNode)
    {
        while ((curr = mNode->IterateChildren(curr)))
            set->insert(cbC2U(curr->FirstChild()->ToText()->Value()));
    }
}

ConfigManagerContainer::StringSet ConfigManager::ReadSSet(const wxString& name)
{
    ConfigManagerContainer::StringSet ret;
    Read(name, &ret);
    return ret;
}

void ConfigManager::Write(const wxString& name, const ConfigManagerContainer::SerializableObjectMap* map)
{
    wxString key(name);
    TiXmlElement* e = AssertPath(key);

    TiXmlElement *leaf = GetUniqElement(e, key);

    TiXmlElement *mNode;
    mNode = GetUniqElement(leaf, _T("objmap"));
    leaf->RemoveChild(mNode);
    mNode = GetUniqElement(leaf, _T("objmap"));

    for (ConfigManagerContainer::SerializableObjectMap::const_iterator it = map->begin(); it != map->end(); ++it)
    {
        TiXmlElement s(cbU2C(it->first));
        s.InsertEndChild(TiXmlText(cbU2C(wxBase64::Encode(it->second->SerializeOut()))));
        mNode->InsertEndChild(s);
    }
}

void ConfigManager::InitPaths()
{
    ConfigManager::config_folder = ConfigManager::GetUserDataFolder();
    ConfigManager::home_folder = wxStandardPathsBase::Get().GetUserConfigDir();
    ConfigManager::app_path = ::DetermineExecutablePath();
    wxString res_path = ::DetermineResourcesPath();

    // if non-empty, the app has overriden it (e.g. "--prefix" was passed in the command line)
    if (data_path_global.IsEmpty())
    {
        if (platform::windows)
#ifdef CB_AUTOCONF
            ConfigManager::data_path_global = app_path + _T("/../share/codeblocks");
#else
            ConfigManager::data_path_global = app_path + _T("\\share\\codeblocks");
#endif
        else if (platform::macosx)
            ConfigManager::data_path_global = res_path + _T("/share/codeblocks");
        else
            ConfigManager::data_path_global = wxStandardPathsBase::Get().GetDataDir();
    }
    else
        ConfigManager::data_path_global = UnixFilename(data_path_global);

#ifdef CB_AUTOCONF
    if (plugin_path_global.IsEmpty())
    {
        if (platform::windows)
            ConfigManager::plugin_path_global = app_path + _T("\\..\\lib\\codeblocks\\plugins");
        else if (platform::macosx)
            ConfigManager::plugin_path_global = data_path_global + _T("/plugins");
        else
        {
#ifdef __WXGTK__
            // It seems we can not longer rely on wxStandardPathsBase::Get().GetPluginsDir(),
            // because its behaviour has changed on some systems (at least Fedora 14 64-bit).
            // So we create the pathname manually
            ConfigManager::plugin_path_global = ((const wxStandardPaths&)wxStandardPaths::Get()).GetInstallPrefix() + _T("/lib/codeblocks/plugins");
            // first assume, we use standard-paths
            if (!wxDirExists(ConfigManager::plugin_path_global) && wxIsPlatform64Bit())
            {
                // if standard-path does not exist and we are on 64-bit system, use lib64 instead
                ConfigManager::plugin_path_global = ((const wxStandardPaths&)wxStandardPaths::Get()).GetInstallPrefix() + _T("/lib64/codeblocks/plugins");
            }
#endif // __WXGTK__
        }
    }
#endif

    wxString dataPathUser = ConfigManager::config_folder + wxFILE_SEP_PATH + _T("share");
#ifdef __linux__
    if (!has_alternate_user_data_path)
    {
        gchar *filename = g_build_filename(g_get_user_data_dir(), nullptr);
        dataPathUser = wxString::FromUTF8(filename);
        g_free(filename);
    }
#endif // __linux__

    ConfigManager::data_path_user = dataPathUser + wxFILE_SEP_PATH + _T("codeblocks");

    // if user- and global-datapath are the same (can happen in portable mode) we run in conflicts
    // so we extend the user-datapath with the users name
    if (wxFileName(ConfigManager::data_path_user) == wxFileName(ConfigManager::data_path_global))
        ConfigManager::data_path_user.append("." + wxGetUserId());

    CreateDirRecursively(ConfigManager::config_folder);
    CreateDirRecursively(ConfigManager::data_path_user   + _T("/plugins/"));
    CreateDir(ConfigManager::data_path_user   + _T("/scripts/"));

    ConfigManager::temp_folder = wxStandardPathsBase::Get().GetTempDir();
}

void ConfigManager::MigrateFolders()
{
#ifdef __linux__
    // if the old config-folder (~/.codeblocks) does not exist, we have nothing to do.
    if (!wxDirExists(wxStandardPaths::Get().GetUserDataDir()))
        return;

    // ConfigManager::config_folder might be the portable-path but we want to migrate the standard-conform folder,
    // but only if it not already exists
    wxString newConfigFolder = wxString::FromUTF8(g_build_filename (g_get_user_config_dir(), "codeblocks", nullptr));
    // if the new config folder already exist, we step out immediately
    if (wxDirExists(newConfigFolder))
        return;

    wxString oldConfigFolder = wxStandardPaths::Get().GetUserDataDir();
    wxString oldDataFolder = oldConfigFolder + wxFILE_SEP_PATH + _T("share") + wxFILE_SEP_PATH + _T("codeblocks");
    wxString newDataFolder = wxString::FromUTF8(g_build_filename (g_get_user_data_dir(), nullptr)) + wxFILE_SEP_PATH + _T("codeblocks");
    wxString msg;
    msg = F(_("The places where the configuration files and user-data files are stored\n"
              "have been changed to be more standard-conform.\n"
              "\n"
              "Now moving \"%s\"\n"
              "to \"%s\"\n"
              "and \"%s\"\n"
              "to \"%s\".\n"),
            oldDataFolder.wx_str(),
            newDataFolder.wx_str(),
            oldConfigFolder.wx_str(),
            newConfigFolder.wx_str());
    cbMessageBox(msg, _("Try to migrate config-folder ..."), wxICON_INFORMATION);

    bool success = true;
    if (wxDirExists(oldDataFolder))
    {
        // make sure the target-folder exists
        CreateDirRecursively(newDataFolder);
        success = wxRenameFile(oldDataFolder, newDataFolder);
        wxRmdir(oldConfigFolder + wxFILE_SEP_PATH + _T("share"));
    }
    if (success)
    {
        // make sure the target-folder exists
        CreateDirRecursively(newConfigFolder);
        success = wxRenameFile(oldConfigFolder, newConfigFolder);
    }
    if (!success)
    {
        msg = F(_("Error moving \"%s\"\n"
                  "to \"%s\"\n"
                  "or \"%s\"\n"
                  "to \"%s\".\n\n"
                  "Please check the folders manually (access rights?) !\n"
                  "A new configuration will be created from scratch!"),
                oldDataFolder.wx_str(),
                newDataFolder.wx_str(),
                oldConfigFolder.wx_str(),
                newConfigFolder.wx_str());
        cbMessageBox(msg, _("Error migrating config-folder ..."), wxICON_ERROR);
    }
#endif // __linux__
}

void ConfigManagerWrapper::Write(const wxString& name, const wxString& value, bool ignoreEmpty)
{
    if (m_namespace.empty())
        return;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    c->Write(m_basepath + name, value, ignoreEmpty);
}

wxString ConfigManagerWrapper::Read(const wxString& key, const wxString& defaultVal)
{
    if (m_namespace.empty())
        return defaultVal;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    return c->Read(m_basepath + key, defaultVal);
}

bool ConfigManagerWrapper::Read(const wxString& key, wxString* str)
{
    if (m_namespace.empty())
        return false;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    return c->Read(key, str);
}

void ConfigManagerWrapper::Write(const wxString& key, const char* str)
{
    if (m_namespace.empty())
        return;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    c->Write(key, str);
}

void ConfigManagerWrapper::Write(const wxString& name, int value)
{
    if (m_namespace.empty())
        return;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    c->Write(m_basepath + name, value);
}

bool ConfigManagerWrapper::Read(const wxString& name, int* value)
{
    if (m_namespace.empty())
        return false;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    return c->Read(m_basepath + name, value);
}

int ConfigManagerWrapper::ReadInt(const wxString& name, int defaultVal)
{
    if (m_namespace.empty())
        return defaultVal;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    return c->ReadInt(m_basepath + name, defaultVal);
}

void ConfigManagerWrapper::Write(const wxString& name, bool value)
{
    if (m_namespace.empty())
        return;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    c->Write(m_basepath + name, value);
}

bool ConfigManagerWrapper::Read(const wxString& name, bool* value)
{
    if (m_namespace.empty())
        return false;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    return c->Read(m_basepath + name, value);
}

bool ConfigManagerWrapper::ReadBool(const wxString& name, bool defaultVal)
{
    if (m_namespace.empty())
        return defaultVal;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    return c->ReadBool(m_basepath + name, defaultVal);
}

void ConfigManagerWrapper::Write(const wxString& name, double value)
{
    if (m_namespace.empty())
        return;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    c->Write(m_basepath + name, value);
}

bool ConfigManagerWrapper::Read(const wxString& name, double* value)
{
    if (m_namespace.empty())
        return false;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    return c->Read(m_basepath + name, value);
}

double ConfigManagerWrapper::ReadDouble(const wxString& name, double defaultVal)
{
    if (m_namespace.empty())
        return defaultVal;
    ConfigManager *c = Manager::Get()->GetConfigManager(m_namespace);
    return c->ReadDouble(m_basepath + name, defaultVal);
}

static wxString getCompilerPluginFilename()
{
    if (platform::windows)
#ifdef CB_AUTOCONF
        return wxT("libcompiler.dll");
#else
        return wxT("compiler.dll");
#endif
    else if (platform::darwin || platform::macosx)
        return wxT("libcompiler.dylib");
    else
        return wxT("libcompiler.so");
}

wxArrayString cbReadBatchBuildPlugins()
{
    ConfigManager *bbcfg = Manager::Get()->GetConfigManager(_T("plugins"));
    wxArrayString bbplugins = bbcfg->ReadArrayString(_T("/batch_build_plugins"));

    if (!bbplugins.GetCount())
        bbplugins.Add(getCompilerPluginFilename());

    return bbplugins;
}

void cbWriteBatchBuildPlugins(wxArrayString bbplugins, wxWindow *messageBoxParent)
{
    const wxString &compiler = getCompilerPluginFilename();

    if (bbplugins.Index(compiler) == wxNOT_FOUND)
    {
        bbplugins.Add(compiler);
        cbMessageBox(_("The compiler plugin must always be loaded for batch builds!\n"
                    "Automatically re-enabled."),
                    _("Warning"), wxICON_WARNING, messageBoxParent);
    }
    ConfigManager *bbcfg = Manager::Get()->GetConfigManager(_T("plugins"));
    bbcfg->Write(_T("/batch_build_plugins"), bbplugins);
}
