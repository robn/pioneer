include $(top_srcdir)/Makefile.common

AM_CFLAGS += $(WARN_CFLAGS)
AM_CPPFLAGS += $(WARN_CPPFLAGS)
AM_CXXFLAGS += $(WARN_CXXFLAGS)

INCLUDES = -I$(srcdir)/.. -isystem @top_srcdir@/contrib/gwen/include

noinst_LIBRARIES = libui.a
noinst_HEADERS = \
	InputHandler.h \
	Renderer.h

libui_a_SOURCES = \
	InputHandler.cpp \
	Renderer.cpp
