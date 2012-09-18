# configure.ac -- Top level autoconf configuration file
#
# Copyright (C) 2012 Embecosm Limited
#
# Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>
#
# This file is part of pihwm.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 3 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.


# Process this file with autoconf to produce a configure script.


# Standard place for local macros
ACLOCAL_AMFLAGS = -I m4

# Use a full version number (x.y.z, possibly with "rc<n>" as a suffix) when
# preparing a release, otherwise use a the date version (yyyy-mm-dd).
AC_INIT([pihwm], [2012-09-18], [omerkilic@gmail.com])
AC_CONFIG_MACRO_DIR([m4])

# Generically use extensions such as _GNU_SOURCE if available.
AC_USE_SYSTEM_EXTENSIONS

# Set up to use libtool and automake as well and create a config header
AC_PROG_LIBTOOL
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])

# make sure we are using a recent autoconf version
AC_PREREQ(2.68)

# Set the CFLAGS we want. We put the user flags last, so that if the user
# changes the optimization level, that will take precedence.
CFLAGS="-g -Wall -Werror -O2"

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_TYPE_SIGNAL

# Use AC_CHECK_HEADERS and AC_CHECK_FUNCS here as needed.

# define options
AC_MSG_CHECKING(whether to enable profiling)
AC_ARG_ENABLE(profiling,
    [  --enable-profiling      generate profiling code], [
    case "$enableval" in
	yes) CFLAGS="$CFLAGS -pg" ;;
    esac
])
AC_MSG_RESULT(${enable_profiling-no})

# We can set and AC_SUBST DEJAGNU and RUNTESTDEFAULTFLAGS here if needed.

# Export flags and any local flags used.
AC_SUBST(CFLAGS)
AC_SUBST(LOCAL_CFLAGS)
AC_SUBST(LOCAL_LDFLAGS)
AC_SUBST(LOCAL_DEFS)

# Ensure we can find the library headers when building the demos and tests.
INCLUDES="$INCLUDES -I\${top_srcdir}/lib"
AC_SUBST(INCLUDES)

AC_CONFIG_FILES([Makefile             \
                 demo/Makefile        \
                 doc/Makefile         \
                 lib/Makefile         \
                 testsuite/Makefile])

# Generate the output
AC_COPYRIGHT([Copyright (C) 2012 Omer Kilic
Copyright (C) 2012 Embecosm Limited

Contributor Omer Kilic <omerkilic@gmail.com>
Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

This file is part of pihwm.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.])

AC_OUTPUT
