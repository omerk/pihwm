# Makefile.am -- automake configuration file for demos
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

# Build a set of demos and a library.
bin_PROGRAMS = board_info \
           check_kernel_modules \
	       gpio_int       \
	       gpio_test      \
		   gpio_ledborg   \
	       i2c_ads1015    \
	       i2c_hmc6352    \
	       i2c_mcp23008   \
	       pwm_demo       \
		   spi_test		 \
	       spi_bb_mcp3202 \
	       spi_mcp3002

# Kernel modules test
check_kernel_modules_SOURCES = check_kernel_modules.c
check_kernel_modules_LDADD   = ../lib/libpihwm.la -lpthread

# Board info demo
board_info_SOURCES = board_info.c
board_info_LDADD   = ../lib/libpihwm.la -lpthread

# GPIO interrupt demo
gpio_int_SOURCES = gpio_int.c
gpio_int_LDADD   = ../lib/libpihwm.la -lpthread

# Baseline GPIO demo
gpio_test_SOURCES = gpio_test.c
gpio_test_LDADD   = ../lib/libpihwm.la -lpthread

# LedBorg demo
gpio_ledborg_SOURCES = gpio_ledborg.c
gpio_ledborg_LDADD   = ../lib/libpihwm.la -lpthread

# ADS1015 i2c demo
i2c_ads1015_SOURCES = i2c_ads1015.c
i2c_ads1015_LDADD   = ../lib/libpihwm.la -lpthread

# HMC6352 i2c demo
i2c_hmc6352_SOURCES = i2c_hmc6352.c
i2c_hmc6352_LDADD   = ../lib/libpihwm.la -lpthread

# MCP23017 i2c demo
i2c_mcp23008_SOURCES = i2c_mcp23008.c
i2c_mcp23008_LDADD   = ../lib/libpihwm.la -lpthread

# PWM demo
pwm_demo_SOURCES = pwm_demo.c
pwm_demo_LDADD   = ../lib/libpihwm.la -lpthread

# MCP3002 SPI demo
spi_mcp3002_SOURCES = spi_mcp3002.c
spi_mcp3002_LDADD   = ../lib/libpihwm.la -lpthread

# Bitbanged SPI demo
spi_bb_mcp3202_SOURCES = spi_bb_mcp3202.c
spi_bb_mcp3202_LDADD   = ../lib/libpihwm.la -lpthread

# SPI loopback test
spi_test_SOURCES = spi_test.c
spi_test_LDADD   = ../lib/libpihwm.la -lpthread
