AM_CFLAGS = -pipe -O2 -g -W -Wall -Wextra -I$(top_srcdir)/include

check_PROGRAMS = test

nodist_test_SOURCES = test.c test_demosaic_nearest_neighbor.c
test_CFLAGS = $(AM_CFLAGS) $(CUNIT_CFLAGS)
test_LDADD = $(top_builddir)/src/.libs/librpiraw.a $(CUNIT_LIBS)
