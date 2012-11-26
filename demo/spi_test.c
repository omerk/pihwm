/* spi_test.c -- SPI loopback test

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

#include <stdio.h>
#include <stdlib.h>

#include "pihwm.h"
#include "pi_spi.h"

int main(void)
{
	int fd, i;

	// Dummy data and RX buffer
	uint8_t tx[] = {
		0xDE, 0xAD, 0xBE, 0xEF,
		0xFE, 0xED, 0x00, 0x42
	};
	uint8_t rx[size(tx)] = { 0 };
	
	// Initialise SPI module, use CE0
	fd = spi_init(0);
	if ( fd < 0 ){
		printf("ERROR: Can't initialise SPI\n");
		exit(1);
	}

	// Transfer data
	if ( spi_transfer(fd, tx, rx, size(tx))  ){
		// Print RX buffer
		for (i = 0; i < size(tx); i++) {
			if ( (i % 4) == 0 ){
				printf("\n");
			}
			printf("%.2X ", rx[i]);
		}
		printf("\n");
	} else {
		printf("ERROR: Transfer failed.\n");
	}

	return 0;

}

