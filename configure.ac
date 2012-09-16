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
AC_INIT([pihwm], [2012-08-16], [omer@kilic.name])
AC_CONFIG_MACRO_DIR([m4])

# Generically use extensions such as _GNU_SOURCE if available.
AC_USE_SYSTEM_EXTENSIONS

# Check we have the execute source file present for sanity. Specify a separate
# config for the testsuite OR1K specific programs, since we'll need different
# tool chain there (the OpenRISC one, rather than the native one).

AC_CANONICAL_TARGET([])
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

AC_CHECK_HEADERS(unistd.h stdlib.h stdarg.h string.h strings.h      \
		 sys/ptem.h sys/pte.h sys/stream.h sys/stropts.h sys/select.h \
		 termcap.h termios.h termio.h sys/file.h locale.h getopt.h    \
		 net/ethernet.h sys/ethernet.h malloc.h inttypes.h libintl.h  \
                 limits.h linux/if_tun.h)
AC_CHECK_FUNCS(strcasecmp select setenv putenv tcgetattr setlocale lstat)
AC_CHECK_FUNCS(grantpt unlockpt ptsname on_exit)
AC_CHECK_FUNCS(basename)

# define options
AC_MSG_CHECKING(whether to enable profiling)
AC_ARG_ENABLE(profiling,
    [  --enable-profiling      generate profiling code], [
    case "$enableval" in
	yes) profile="-pg" ;;
    esac
])
AC_MSG_RESULT(${enable_profiling-no})

# Check for a DejaGNU global config file. If not set, then set it
# ourselves. This stops runtest complaining
DEJAGNU="\$(top_srcdir)/testsuite/global-conf.exp"

AC_SUBST(DEJAGNU)

# The following line will override the default definition of the srcdir, and
# ensure that short names are used for the test names.
RUNTESTDEFAULTFLAGS="--tool \$\$tool"
AC_SUBST(RUNTESTDEFAULTFLAGS)

BUILD_DIR=`pwd`
AC_SUBST(BUILD_DIR)

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

Contributor Omer Kilic <omer@kilic.name>
Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

This file is part of OpenRISC 1000 Architectural Simulator.

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
