AM_CFLAGS = -pipe -O2 -g -W -Wall -Wextra -I$(top_srcdir)/include

check_PROGRAMS = test

nodist_test_SOURCES = test.c \
                      test_demosaic_nearest_neighbor.c test_histogram.c \
                      test_statistic.c test_luminance.c
if HAVE_PNG
    nodist_test_SOURCES += test_file_png.c
endif

test_CFLAGS = $(AM_CFLAGS) $(CUNIT_CFLAGS) $(OPENMP_CFLAGS) $(PNG_CFLAGS)
test_LDADD = $(top_builddir)/src/.libs/librpiraw.a \
             $(CUNIT_LIBS) $(OPENMP_LIBS) $(M_LIBS) $(PNG_LIBS)
