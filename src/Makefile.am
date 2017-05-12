AM_CFLAGS = -pipe -O2 -g -W -Wall -Wextra -I$(top_srcdir)/include \
            $(OPENMP_CFLAGS) $(PNG_CFLAGS)

lib_LTLIBRARIES = librpiraw.la

librpiraw_la_SOURCES = raw.c convert.c \
                       demosaic_edge.c demosaic_nearest_neighbor.c \
                       histogram.c histogram_equalize.c
if HAVE_PNG
    librpiraw_la_SOURCES += file_png.c
endif
