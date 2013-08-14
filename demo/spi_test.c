/* spi_test.c -- SPI loopback test
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

#include "pihwm.h"

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

