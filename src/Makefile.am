AM_YFLAGS = -d
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c
CLEANFILES = rules_bison.cc rules_flex.cc

libosmgl_LTLIBRARIES = libosmgl.la
libosmgldir=$(includedir)/osmgl
libosmgl_la_SOURCES = osmgl.cc rules_flex.ll rules_bison.yy 
libosmgl_la_CXXFLAGS = -I@top_srcdir@/include ${XML_CFLAGS}
libosmgl_la_LIBS = ${XML_LIBS}
libosmgl_HEADERS = @top_srcdir@/include/osmgl/osmgl.hh
BUILT_SOURCES = rules_bison.h
