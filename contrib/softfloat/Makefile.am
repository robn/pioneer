#  Process this file with automake to produce Makefile.in
include $(top_srcdir)/Makefile.common

AM_CFLAGS += $(SDL_CFLAGS)

noinst_LIBRARIES = libsoftfloat.a
libsoftfloat_a_SOURCES = softfloat.cpp
noinst_HEADERS = softfloat.h

check_PROGRAMS = softfloat_test
softfloat_test_SOURCES = \
	softfloat_test.cpp
softfloat_test_LDADD = \
	libsoftfloat.a
