/* i2c_hmc6352.c -- Test of HMC6352 I2C digital compass chip
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

/* I2C Address */
#define ADDR	0x21

int
main ()
{
  int fd;
  unsigned char r[2];
  unsigned int res, i;

	/* Init I2C bus */
	fd = i2c_init ();
	if ( fd < 0 ) {
		printf("ERROR: Can't initialise I2C bus.\n");
		exit(1);
	}

  for (i = 0; i < 10; i++)
    {

      /* Request Data */
      i2c_write (fd, ADDR, (unsigned char *) "A", 1);
      sleep (0.1);
      i2c_read (fd, ADDR, r, 2);

      res = (r[0] << 8 | r[1]) / 10;

      printf ("res: %d\t(res_h: 0x%02x res_l: 0x%02x)\n", res, r[0], r[1]);
      sleep (1);
    }

  return 0;
}
