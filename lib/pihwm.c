/* pihwm.c -- general library implementation

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
#include <linux/types.h>
#include "pihwm.h"


struct _board_info {
	uint8_t model;
	uint8_t rev;
	uint8_t mem;
};
   

/* http://www.element14.com/community/docs/DOC-50776 */
board_info get_board_info()
{
	FILE *info;
	char rev_hex[5];
	unsigned int rev_int = 0;
	board_info board;

	char *cmd = "cat /proc/cpuinfo | grep 'Revision' | awk '{print $3}'";

	if( !(info = popen(cmd,"r")) ){
		printf("Error!");
	}

	fgets(rev_hex, 5, info);
	sscanf(rev_hex, "%x", &rev_int);

	switch(rev_int){
		case 2:
		case 3:
			board.model = MODEL_B;
			board.rev = REV_1;
			board.mem = MEM_256;
		break;

		case 4:
		case 5:
		case 6:
			board.model = MODEL_B;
			board.rev = REV_2;
			board.mem = MEM_256;
		break;

		case 13:
		case 14:
		case 15:
			board.model = MODEL_B;
			board.rev = REV_2;
			board.mem = MEM_512;
		break;
		
		default:
			board.model = -1;
			board.rev = -1;
			board.mem = -1;
		break;
	}
	
	return board;

}

uint8_t board_model()
{
	board_info b = get_board_info();
	return b.model;
}

uint8_t board_rev()
{
	board_info b = get_board_info();
	return b.rev;
}

uint8_t board_mem()
{
	board_info b = get_board_info();
	return b.mem;
}

