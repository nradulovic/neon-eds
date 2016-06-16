#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Event Driven System], [2.0], [nenad.b.radulovic@gmail.com])
AC_CONFIG_SRCDIR([include/neon_eds.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects])
LT_INIT

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
