noinst_PROGRAMS = auto_revision
auto_revision_SOURCES = autorevision.cpp

AM_CPPFLAGS = -I$(top_srcdir)/src/include


auto_revision_LDADD = $(CB_TINYXML_LIBS)

all-local: auto_revision$(EXEEXT)
	./auto_revision +int +wx +t $(top_srcdir) $(top_builddir)/src/include/autorevision.h
