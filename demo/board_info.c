/* test_board.c -- Test of board identification functions.

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
#include <inttypes.h>
#include "pihwm.h"

int main()
{
	uint8_t model, rev, mem;

	model = board_model();
	rev = board_rev();
	mem = board_mem();

	printf("It appears that you are using a ");

	if( model == MODEL_A ){
		printf("Model A");
	} else if ( model == MODEL_B ){
		printf("Model B");
	} else {
		printf("???");
	}

	if ( rev == REV_1 ) {
		printf(", Revision 1 board");
	} else if( rev == REV_2 ){
		printf(", Revision 2 board");
	} else {
		printf("???");
	}

	if ( mem == MEM_256 ){
		printf(" with 256");
	} else if ( mem == MEM_512 ) {
		printf(" with 512");
	} else {
		printf(" with ???");
	}

	printf("MB of RAM.\n");

	return 0;

}
