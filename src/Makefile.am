BUILT_SOURCES = rules_bison.h
AM_YFLAGS = -d -p rules
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).cc
#AM_LFLAGS = -olex.yy.c


libosmgl_LTLIBRARIES = libosmgl.la
libosmgldir=$(includedir)/osmgl
libosmgl_la_SOURCES = osmgl.cc rules_bison.yy rules_flex.ll
libosmgl_la_CPPFLAGS = -I@top_srcdir@/include ${XML_CFLAGS}
libosmgl_la_LIBS = ${XML_LIBS}
libosmgl_HEADERS = @top_srcdir@/include/osmgl/osmgl.hh
