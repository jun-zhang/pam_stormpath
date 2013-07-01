#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([pam_stormpath], [0.1], [lusis.org+github@gmail.com], [pam_stormpath],
        [https://github.com/lusis/pam_stormpath])
AM_INIT_AUTOMAKE([1.11 no-define foreign subdir-objects -Wall])
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
LT_INIT([disable-static])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_CHECK_HEADERS([stdint.h stdlib.h string.h syslog.h])

AC_CHECK_HEADERS([security/pam_appl.h security/pam_modules.h security/pam_ext.h], [],
  [AC_MSG_ERROR([[PAM header files not found, install libpam-dev.]])])

AC_CHECK_HEADERS([curl/curl.h], [],
  [AC_MSG_ERROR([[CURL header files not found, install libcurl-dev.]])])

AC_CHECK_HEADERS([json/json.h], [],
  [AC_MSG_ERROR([[JSON header files not found, install libjson-dev.]])])

AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strdup])

AC_CHECK_LIB([pam], [pam_start])
AC_CHECK_LIB([curl], [curl_global_init])
AC_CHECK_LIB([json], [json_tokener_parse])

AC_ARG_WITH(securedir,
        AS_HELP_STRING([--with-securedir=DIR],[path to location of PAMs @<:@default=$libdir/security@:>@]),
        SECUREDIR=$withval, SECUREDIR=$libdir/security)
AC_SUBST(SECUREDIR)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

AC_MSG_NOTICE([Summary of build options:

  Version:            ${VERSION}
  Host type:          ${host}
  Install prefix:     ${prefix}
  Compiler:           ${CC}
  PAM directory:      ${SECUREDIR}
])

