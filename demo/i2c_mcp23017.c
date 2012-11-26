/* i2c_mcp23017.c -- Test of MCP23017 i2c chip

   Copyright (C) 2012 Omer Kilic
   Copyright (C) 2012 Embecosm Limited

   Contributor Omer Kilic <omerkilic@gmail.com>
   Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

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

/* Registers */
#define	IODIRA	0x00
#define	IODIRB	0x01
#define	GPIOA		0x12
#define	GPIOB		0x13

/* I2C Addres (A2,A1,A0 tied to ground) */
#define	ADDR		0x20

int
main ()
{
  int fd;

  printf ("main() start\n");

	/* Init I2C bus */
	fd = i2c_init ();
	if ( fd < 0 ) {
		printf("ERROR: Can't initialise I2C bus.\n");
		exit(1);
	}

  /* Set pin directions */
  i2c_write (fd, ADDR, (unsigned char [])
			   {
			   IODIRA, 0x00}
	     , 2);
  i2c_write (fd, ADDR, (unsigned char [])
			   {
			   IODIRB, 0x00}
	     , 2);

  /* Set pin values */
  i2c_write (fd, ADDR, (unsigned char [])
			   {
			   GPIOA, 0xFF}
	     , 2);
  i2c_write (fd, ADDR, (unsigned char [])
			   {
			   GPIOB, 0xFF}
	     , 2);

  printf ("main() end\n");

  return 0;
}
