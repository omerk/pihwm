/* test_board.c -- Test of board identification functions.
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
