# SUBDIRS = resources **Deprecated**

AM_CPPFLAGS = $(WX_CXXFLAGS) \
		$(WX_GTK_CFLAGS) \
		$(WX_X11_CFLAGS) \
		-I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/sdk/wxscintilla/include \
		-I$(top_srcdir)/src/plugins/contrib/codesnippets

pluginlibdir = $(pkglibdir)/plugins

pluginlib_LTLIBRARIES = libcodesnippets.la

libcodesnippets_la_LDFLAGS = @MODULE_SHARED_LDFLAGS@ -version-info 0:1:0 -no-undefined -avoid-version

libcodesnippets_la_LIBADD = ../../../sdk/libcodeblocks.la \
		$(WX_LIBS) \
		$(WX_GTK_LIBS) \
		$(WX_X11_LIBS)

libcodesnippets_la_SOURCES = codesnippets.cpp \
		codesnippetstreectrl.cpp \
		codesnippetswindow.cpp \
		settingsdlg.cpp \
		settingsdlgform.cpp \
		snippetitemdata.cpp \
		snippetproperty.cpp \
		snippetpropertyform.cpp \
		snippetsconfig.cpp \
		snippetsimages.cpp \
		version.cpp


noinst_HEADERS =  	codesnippets.h \
		codesnippetstreectrl.h \
		codesnippetswindow.h \
		menuidentifiers.h \
		settingsdlg.h \
		settingsdlgform.h \
		snippetitemdata.h \
		snippetproperty.h \
		snippetpropertyform.h \
		snippetsconfig.h \
		snippetsimages.h \
		FileImport.h \
		version.h \
		wx_pch.h

man_MANS = codesnippets.1

EXTRA_DIST = 	$(srcdir)/*.cbp \
		$(srcdir)/update* \
		readme.html \
		license.txt \
		manifest.xml \
		$(man_MANS)

pkgdata_DATA = codesnippets.zip
CLEANFILES = $(pkgdata_DATA)

codesnippets.zip: manifest.xml
	PWD=`pwd` cd $(srcdir) && zip $(PWD)/codesnippets.zip manifest.xml
