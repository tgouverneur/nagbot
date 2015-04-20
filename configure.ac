
# nagbot: Nagios IRC bot
# Copyright (C) 2008-2009 Gouverneur Thomas <wildcat@espix.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([nagbot], [svn], [wildcat@espix.org])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX

#AC_DISABLE_STATIC
#AC_LIBTOOL_DLOPEN
#AC_DEFUN([AC_PROG_CXX], [])
#AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_TIME
# Put here every libs needed
AC_CHECK_HEADERS([])

# BSD specific header files
#AC_CHECK_HEADERS([sys/endian.h])

# Linux specidic header files
#AC_CHECK_HEADERS([mcheck.h endian.h])


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
#AC_TYPE_UINT16_T
#AC_TYPE_UINT32_T
#AC_TYPE_UINT8_T

# Checks for library functions.
#AC_PROG_GCC_TRADITIONAL
#AC_FUNC_MALLOC
#AC_FUNC_REALLOC
AC_TYPE_SIGNAL
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([bzero gettimeofday inet_ntoa memmove memset strerror])

AC_CHECK_LIB([nsl], [gethostbyname])
AC_CHECK_LIB([socket], [socket])

# Checks for packages.
AC_CONFIG_FILES([Makefile
                 src/Makefile])

# #define LIB_DIR
LIB_DIR='${libdir}'
AC_SUBST(LIB_DIR)

AC_OUTPUT


