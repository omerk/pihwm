/* i2c_hmc6352.c -- Test of HMC6352 i2c chip

   Copyright (C) 2012 Omer Kilic
   Copyright (C) 2012 Embecosm Limited

   Contributor Omer Kilic <omer@kilic.name>
   Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

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

#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include "pihwm.h"
#include "pi_i2c.h"

/* I2C Address */
#define ADDR	0x21

int
main ()
{
  int fd;
  unsigned char r[2];
  unsigned int res, i;

  printf ("main() start\n");

  /* Init I2C bus */
  fd = i2c_init ("/dev/i2c-0");
  if (fd < 0)
    {
      return 1;
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

  printf ("main() end\n");

  return 0;
}
