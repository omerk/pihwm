/* gpio_ledborg.c - A simple GPIO interface for the LedBorg board.

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
#include <string.h>

#include "pihwm.h"
#include "pi_gpio.h"

#define	PIN_RED	17
#define	PIN_BLUE	22
// PIN_GREEN is board revision dependent, sadly.

void
print_usage ()
{
	printf("Usage: \"gpio_ledborg VAL_RED VAL_GREEN VAL_BLUE\"\n");
	printf("VAL_x is 1 for ON or 0 for OFF.\n");
	exit(1);	
}

char*
check_val (char *val_str)
{
	if ( ( strcmp("0", val_str) == 0 ) || ( strcmp("1", val_str) == 0 ) ){
		return val_str;
	} else {
		print_usage();
	}

	return "";
}

int
main (int argc, char *argv[])
{
	unsigned int pin_green;
	char *val_red, *val_green, *val_blue;

	// do we have enough parameters?
	if ( argc < 4 ){
		print_usage();
	}

	// check values (should be 0 or 1)
	val_red = check_val(argv[1]);
	val_green = check_val(argv[2]);
	val_blue = check_val(argv[3]);
	
	// determine which pin to use for the green LED
	if ( board_rev() == REV_1 ){
		pin_green = 21;
	} else if ( board_rev() == REV_2 ) {
		pin_green = 27;
	} else {
		printf("ERROR: Can't get board revision information.\n");
		exit(1);
	}
  
	// initialise pins as outputs
	gpio_init(PIN_RED, OUTPUT);
	gpio_init(pin_green, OUTPUT);
	gpio_init(PIN_BLUE, OUTPUT);

	// let there be light!
	gpio_write(PIN_RED, val_red);
	gpio_write(pin_green, val_green);
	gpio_write(PIN_BLUE, val_blue);

	// release pins so they could be used elsewhere
	gpio_release (PIN_RED);
	gpio_release (PIN_BLUE);
	gpio_release (pin_green);
  
	return 0;

}

