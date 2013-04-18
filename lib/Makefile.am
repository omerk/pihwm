# Makefile.am -- automake configuration file for the libpihwm library
#
# Copyright (C) 2013 Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
# Copyright (C) 2013 Jeremy Bennett <jeremy.bennett@embecosm.com> - Embecosm Limited
#
# This file is part of pihwm <http://omerk.github.io/pihwm>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
