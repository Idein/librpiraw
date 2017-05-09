#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([librpiraw], [1.0], [ysugi@idein.jp])
AM_INIT_AUTOMAKE([foreign -W -Wall])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# pkgconfig
AC_ARG_WITH(pkgconfigdir,
            AC_HELP_STRING([--with-pkgconfigdir=DIR],
                           [pkgconfig dir [default=LIBDIR/pkgconfig]]),
            [pkgconfigdir=${withval}],
            [pkgconfigdir=${libdir}/pkgconfig])
AC_MSG_NOTICE([pkgconfig directory is ${pkgconfigdir}])
AC_SUBST([pkgconfigdir])

# Checks for programs.
AC_PROG_CC
AM_PROG_AR

# Checks for libraries.
PKG_CHECK_MODULES(CUNIT, [cunit >= 2.0], , [AC_MSG_ERROR("missing -lcunit")])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC

LT_INIT
AC_CONFIG_FILES([Makefile include/Makefile src/Makefile test/Makefile
                 librpiraw.pc])
AC_OUTPUT
