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
		printf("<???>");
	}

	if ( rev == REV_1 ) {
		printf(", Revision 1 board");
	} else if( rev == REV_2 ){
		printf(", Revision 2 board");
	} else {
		printf("<???>");
	}

	if ( mem == MEM_256 ){
		printf(" with 256");
	} else if ( mem == MEM_512 ) {
		printf(" with 512");
	} else {
		printf(" with <???>");
	}

	printf("MB of RAM.\n");

}
