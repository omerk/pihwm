/* pi_spi.h -- SPI library headers.

   Copyright (C) 2012 Omer Kilic

   Contributor Omer Kilic <omerkilic@gmail.com>

   This file is part of pihwm.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef PI_SPI_H
#define PI_SPI_H

#include <linux/spi/spidev.h>
#include <inttypes.h>

// Default config for the SPI module
#define SPI_DEFAULT_MODE	0	
#define SPI_DEFAULT_BPW		8
#define SPI_DEFAULT_SPEED	1000000
#define SPI_DEFAULT_DELAY	10

// Function Prototypes
int spi_init (uint8_t channel);

int spi_config (int fd, uint8_t mode, uint8_t bits,
					 uint32_t speed, uint16_t delay);

int spi_config_default (int fd);

int spi_transfer (int fd, uint8_t txbuf[], uint8_t rxbuf[],
					 	uint8_t len);

#endif
