# Makefile.am -- automake configuration file for the libpihwm library
#
# Copyright (C) 2012 Embecosm Limited
# Copyright (C) 2012 Omer Kilic
#
# Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>
# Contributor Omer Kilic <omerkilic@gmail.com>
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


# Process this file with automake to produce a Makefile.in for use with
# autoconf.


# The main library we are building

lib_LTLIBRARIES = libpihwm.la

include_HEADERS = pi_gpio.h \
				  pi_i2c.h  \
				  pi_pwm.h	\
				  pi_spi.h	\
				  pihwm.h

# Details for the library
libpihwm_la_SOURCES  = pi_gpio.c \
					   pi_gpio.h \
					   pi_i2c.c  \
					   pi_i2c.h  \
					   pi_pwm.c  \
					   pi_pwm.h  \
					   pi_spi.c  \
					   pi_spi.h  \
					   pihwm.h	 \
					   pihwm.c
