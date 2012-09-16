# Makefile.am -- automake configuration file for demos
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


# Process this file with automake to produce a Makefile.in for use with
# autoconf.


# Build a set of demos and a library.
bin_PROGRAMS = gpio_int       \
	       gpio_test      \
	       i2c_ads1015    \
	       i2c_hmc6352    \
	       i2c_mcp23017   \
	       pwm_demo       \
	       spi_bb_mcp3202

# GPIO interrupt demo
gpio_int_SOURCES = gpio_int.c
gpio_int_LDADD   = ../lib/libpihwm.la -lpthread

# Baseline GPIO demo
gpio_test_SOURCES = gpio_test.c
gpio_test_LDADD   = ../lib/libpihwm.la -lpthread

# ADS1015 i2c demo
i2c_ads1015_SOURCES = i2c_ads1015.c
i2c_ads1015_LDADD   = ../lib/libpihwm.la -lpthread

# HMC6352 i2c demo
i2c_hmc6352_SOURCES = i2c_hmc6352.c
i2c_hmc6352_LDADD   = ../lib/libpihwm.la -lpthread

# MCP23017 i2c demo
i2c_mcp23017_SOURCES = i2c_mcp23017.c
i2c_mcp23017_LDADD   = ../lib/libpihwm.la -lpthread

# PWM demo
pwm_demo_SOURCES = pwm_demo.c
pwm_demo_LDADD   = ../lib/libpihwm.la -lpthread

# SPI demo
spi_bb_mcp3202_SOURCES = spi_bb_mcp3202.c
spi_bb_mcp3202_LDADD   = ../lib/libpihwm.la -lpthread
