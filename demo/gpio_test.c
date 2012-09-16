/* gpio_test.c -- Test of GPIO output.

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

#include <stdio.h>

#include "pihwm.h"
#include "pi_gpio.h"

int
main (void)
{
  unsigned int val, i;

  printf ("main() start\n");

  gpio_init (17, OUTPUT);
  gpio_init (18, INPUT);

  for (i = 0; i < 5; i++)
    {
      val = gpio_read (18);
      printf ("Pin 18 Val: %d\n", val);

      gpio_write (17, HIGH);
      sleep (1);
      gpio_write (17, LOW);
      sleep (1);
    }

  gpio_release (17);
  gpio_release (18);

  printf ("main() end\n");

  return 0;
}
