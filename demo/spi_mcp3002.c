/* spi_mcp3002.c -- Demo of MCP3202 SPI ADC chip
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
#include <stdint.h>
#include <linux/types.h>

#include "pihwm.h"

int
mcp3002_read (int fd, int channel)
{
	uint8_t tx[3];
	uint8_t rx[3];

	if ( channel == 0 || channel == 1 ){
		tx[0] = 1; 							// start bit
		tx[1] = (2 + channel) << 6;	// (sgl/diff) (odd/sign) (msbf)
												// odd/sign select channel in single ended mode
		tx[2] = 0;							// dummy data to shift result

		// CH0: 0000 0001  1000 0000  0000 0000
		// CH1: 0000 0001  1100 0000  0000 0000
		spi_transfer(fd, tx, rx, 3);
		
		return ((rx[1] & 31) << 6) + (rx[2] >> 2);
	} else {
		return -1;
	}
}

int
main ()
{
	int fd = spi_init(0);
	int i;

	spi_config_default(fd);

	for( i=0; i< 10; i++){
		printf("Channel 0: %d, Channel 1: %d\n", mcp3002_read(fd, 0), mcp3002_read(fd, 1));
		delay(1000);
	}

	return 0;

}

