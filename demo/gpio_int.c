/* gpio_test.c -- Test of GPIO output.

   Copyright (C) 2012 Omer Kilic
   Copyright (C) 2012 Embecosm Limited

   Contributor Omer Kilic <omerkilic@gmail.com>
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
#include <unistd.h>

#include "pihwm.h"
#include "pi_gpio.h"


#define	INT_PIN	18


void
my_isr (int pin)
{
  printf ("my_isr fired for pin %d\n", pin);
}


int
main (int argc, char **argv, char **envp)
{
  printf ("main start\n");

  /* Init Pin */
  gpio_init (INT_PIN, INPUT);

  /* Init interrupt */
  gpio_set_int (INT_PIN, my_isr, "rising");

  for (;;)
    {
      printf ("doing nothing, waiting for an interrupt.\n");
      sleep (2);
    }

  /* If we didn't have for(;;); */
  gpio_release (INT_PIN);
  printf ("main end\n");
  return 0;
}
