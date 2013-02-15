/* i2c_mcp23017.c -- Test of MCP23008 I2C Port Expander

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
#include "pi_i2c.h"

// Registers
#define	IODIR		0x00
#define	IPOL		0x01
#define	GPINTEN	0x02
#define	DEFVAL	0x03
#define	INTCON	0x04
#define	IOCON		0x05
#define	GPPU		0x06
#define	INTF		0x07
#define	INTCAP	0x08
#define	GPIO		0x09
#define	OLAT		0x0a

// I2C Address (A2,A1,A0 tied to ground)
#define	ADDR		0x20

int
main ()
{
	int fd, count;
	unsigned char pattern[2], val[2];

	printf ("main() start\n");

	// Initalise I2C bus
	fd = i2c_init ();
	if ( fd < 0 ) {
		printf("ERROR: Can't initialise I2C bus.\n");
		exit(1);
	}

	// Set pin directions
	i2c_write(fd, ADDR, a({IODIR, 0x0F}), 2);

	// Reset pin values
	i2c_write(fd, ADDR, a({GPIO, 0}), 2);

	for (count=0; count <16; count++){

		// Set LED state
		pattern[0] = GPIO;
		pattern[1] = count<<4;
		i2c_write(fd, ADDR, pattern, 2);
		delay(1000);

		// Read DIP switches
		i2c_write (fd, ADDR, a({GPIO}), 1);
		delay (10);
		i2c_read (fd, ADDR, val, 1);
		printf("GPIO=%02X : GP0: %x  GP1: %x  GP2: %x  GP3: %x\n", val[0], val[0]&1, (val[0]&2)>>1, (val[0]&4)>>2, (val[0]&8)>>3);
	}

	// Reset pin values
	i2c_write(fd, ADDR, a({GPIO, 0}), 2);
	
	printf ("main() end\n");

	return 0;
}
