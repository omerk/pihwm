#include <stdio.h>
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
}				// quick_pwm_demo



int
main (void)
{
  // Map the I/O sections
  setup_io ();

  // Set up PWM module
  setup_pwm ();

  // Run PWM demo
  quick_pwm_demo ();

  // make sure everything is off!
  pwm_off ();
  restore_io ();

  return 0;
}				// main
