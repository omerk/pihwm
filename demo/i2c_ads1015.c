/* i2c_ads1015.c -- Test of ADS1015 I2C ADC chip
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
#include "pi_i2c.h"

/* I2C Address */
#define ADDR	0x48

/* Registers */
#define R_CONV  0x00
#define R_CONF  0x01
#define R_LOTH  0x02
#define R_HITH  0x03

unsigned int
ads1015_read (int fd, unsigned int chan)
{
  unsigned char cmd[2], res[2];

  /* Set high byte for R_CONF */
  switch (chan)
    {
    case 0:
      cmd[0] = 0x40;		/* 8'b01000000 */
      break;
    case 1:
      cmd[0] = 0x50;		/* 8'b01010000 */
      break;
    case 2:
      cmd[0] = 0x60;		/* 8'b01100000 */
      break;
    case 3:
      cmd[0] = 0x70;		/* 8'b01110000 */
      break;
    default:
      cmd[0] = 0x00;		/* TODO: is this correct? */
      break;
    }

  /* Set low byte for R_CONF */
  cmd[1] = 0x83;		/* 8'b10000011 */


  /* Set R_CONF */
  i2c_write (fd, ADDR, (unsigned char[])
			   {
			   R_CONF, cmd[0], cmd[1]}
	     , 3);
  delay (10);

  /* Point to R_CONV */
  i2c_write (fd, ADDR, (unsigned char[])
			   {
			   R_CONV}
	     , 1);
  delay (10);

  /* Read conversion result */
  i2c_read (fd, ADDR, res, 2);

  return (unsigned int) (res[0] << 4) | (res[1] >> 4);

}


double
ads1015_volt (unsigned int val)
{
  return ((6.144 / 4096) * val) * 2;
}


int
main ()
{
  int fd;
  unsigned int res, i, j;
  double volt;

	/* Init I2C bus */
	fd = i2c_init ();
	if ( fd < 0 ) {
		printf("ERROR: Can't initialise I2C bus.\n");
		exit(1);
	}

  for (i = 0; i < 4; i++)
    {
      printf ("Channel %d\n", i);
      for (j = 0; j < 5; j++)
	{
	  res = ads1015_read (fd, i);
	  volt = ads1015_volt (res);

	  printf ("Raw: %4d\tVolt: %1.4f\n", res, volt);
	  delay (1000);
	}
      printf ("------\n");
    }

  return 0;
}
