/* gpio_ledborg.c - A simple GPIO interface for the LedBorg board.
*
* Copyright (C) 2013 Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
* Copyright (C) 2013 Jeremy Bennett <jeremy.bennett@embecosm.com> - Embecosm Limited
*
* This file is part of pihwm <http://omerk.github.io/pihwm>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pihwm.h"

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

unsigned int
check_val (char* val_str)
{
	unsigned int val = atoi(val_str);

	if ( ( val == 0 ) || ( val == 1 ) ){
		return val;
	} else {
		print_usage();
	}

	return 0;
}

int
main (int argc, char *argv[])
{
	unsigned int pin_green;
	unsigned int val_red, val_green, val_blue;

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
	gpio_release(PIN_RED);
	gpio_release(PIN_BLUE);
	gpio_release(pin_green);
  
	return 0;

}

