/* pwm_demo.c -- Dem oof pulse width modulation

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
#include "pi_pwm.h"


void
quick_pwm_demo ()
{
  int r, s;
  for (r = 0; r < 1; r++)
    {
      printf ("\n>>> ");
      fflush (stdout);
      GPIO_CLR0 = 1 << 4;
      force_pwm (0, PWM0_ENABLE);
      for (s = 0x100; s <= 0x400; s += 0x10)
	{
	  usleep (10000);
	  set_pwm (s);
	  putchar ('+');
	  fflush (stdout);
	}
      for (s = 0x400; s >= 0x100; s -= 0x10)
	{
	  usleep (10000);
	  set_pwm (s);
	  putchar ('-');
	  fflush (stdout);
	}
      // same in reverse direction
      GPIO_SET0 = 1 << 4;
      force_pwm (0, PWM0_ENABLE | PWM0_REVPOLAR);
      printf ("\n<<< ");
      for (s = 0x100; s <= 0x400; s += 0x10)
	{
	  usleep (10000);
	  set_pwm (s);
	  putchar ('+');
	  fflush (stdout);
	}
      for (s = 0x400; s >= 0x100; s -= 0x10)
	{
	  usleep (10000);
	  set_pwm (s);
	  putchar ('-');
	  fflush (stdout);
	}
    }
  pwm_off ();
  putchar ('\n');
}	/* quick_pwm_demo */



int
main (void)
{
  /* Map the I/O sections */
  setup_io ();

  /* Set up PWM module */
  setup_pwm ();

  /* Run PWM demo */
  quick_pwm_demo ();

  /* make sure everything is off! */
  pwm_off ();
  restore_io ();

  return 0;
}	/* main */
