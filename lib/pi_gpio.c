/* pi_gpio.c -- gpio library function implementation.

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

#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <pthread.h>

#include "pihwm.h"
#include "pi_gpio.h"


static pthread_t isr_handler_thread;
static int isr_handler_flag;


typedef struct
{
  int pin;
  void (*isr) (int);
} isr_t;


int
gpio_init (int pin, char *dir)
{
  FILE *file;
  char filename[35], pinStr[2];

  gpio_release (pin);

  file = fopen ("/sys/class/gpio/export", "w");
  if (file == NULL)
    {
      debug ("[%s] Can't open file (export)\n", __func__);
      return -1;
    }

  sprintf (pinStr, "%d", pin);
  /* TODO: Add check here */
  fwrite (pinStr, sizeof (char), strlen (pinStr), file);

  fclose (file);

  sprintf (filename, "/sys/class/gpio/gpio%d/direction", pin);
  file = fopen (filename, "w");
  if (file == NULL)
    {
      debug ("[%s] Can't open file (direction)\n", __func__);
      return -1;
    }

  /* TODO: Add check here */
  fwrite (dir, sizeof (char), strlen (dir), file);

  fclose (file);

  return 1;
}


/* Bits from:
   https://www.ridgerun.com/developer/wiki/index.php/Gpio-int-test.c */
static void *
isr_handler (void *isr)
{
  struct pollfd fdset[2];
  int nfds = 2, gpio_fd, rc;
  char *buf[64];

  isr_t i = *(isr_t *) isr;

  if (isr_handler_flag)
    {
      printf ("isr_handler running\n");

      /* Get /value fd
	 TODO: Add check here */
      gpio_fd = gpio_valfd ((int) i.pin);


      while (1)
	{
	  memset ((void *) fdset, 0, sizeof (fdset));

	  fdset[0].fd = STDIN_FILENO;
	  fdset[0].events = POLLIN;

	  fdset[1].fd = gpio_fd;
	  fdset[1].events = POLLPRI;

	  rc = poll (fdset, nfds, 1000);	/* Timeout in ms */

	  if (rc < 0)
	    {
	      debug ("\npoll() failed!\n");
	      return (void *) -1;
	    }

	  if (rc == 0)
	    {
	      debug ("poll() timeout.\n");
	      if (isr_handler_flag == 0)
		{
		  debug ("exiting isr_handler (timeout)");
		  pthread_exit (NULL);
		}
	    }

	  if (fdset[1].revents & POLLPRI)
	    {
	      /* We have an interrupt! */
	      if (-1 == read (fdset[1].fd, buf, 64))
		{
		  debug ("read failed for interrupt");
		  return (void *) -1;
		}

	      (*i.isr) (i.pin);		/* Call the ISR */
	    }

	  if (fdset[0].revents & POLLIN)
	    {
	      if (-1 == read (fdset[0].fd, buf, 1))
		{
		  debug ("read failed for stdin read");
		  return (void *) -1;
		}

	      printf ("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[0]);
	    }

	  fflush (stdout);
	}
    }
  else
    {
      debug ("exiting isr_handler (flag)");
      pthread_exit (NULL);
    }

}


int
gpio_set_int (int pin, void (*isr) (int), char *mode)
{
  /* Details of the ISR */
  isr_t *i = (isr_t *) malloc (sizeof (isr_t));
  i->pin = pin;
  i->isr = isr;

  /* Set up interrupt */
  gpio_edge (pin, mode);

  /* Set isr_handler flag and create thread
     TODO: check for errors using retval */
  isr_handler_flag = 1;
  pthread_create (&isr_handler_thread, NULL, isr_handler, (void *) i);
  pthread_tryjoin_np (isr_handler_thread, NULL);

  return 1;
}


int
gpio_clear_int (int pin)
{
  /* this will terminate isr_handler thread */
  isr_handler_flag = 0;

  /* TODO: Reset "edge", release pin? */
  return  0;			/* Is this a correct return result. */
}


int
gpio_edge (int pin, char *edge)
{
  FILE *file;
  char filename[35];

  sprintf (filename, "/sys/class/gpio/gpio%d/edge", pin);
  file = fopen (filename, "w");
  if (file == NULL)
    {
      debug ("[%s] Can't open file (edge): %s\n", __func__, filename);
      return -1;
    }

  fwrite (edge, sizeof (char), strlen (edge) + 1, file);

  fclose (file);

  return 1;
}


int
gpio_valfd (int pin)
{
  int file;
  char filename[35];

  sprintf (filename, "/sys/class/gpio/gpio%d/value", pin);
  file = open (filename, O_RDWR | O_NONBLOCK);
  if (file < 0)
    {
      debug ("[%s] Can't open file (value): %s\n", __func__, filename);
      return -1;
    }
  else
    {
      return file;
    }

}


int
gpio_write (int pin, char *val)
{
  int file;

  file = gpio_valfd (pin);
  if (-1 == write (file, val, (sizeof (char) * 1)))
    {
      debug ("[%s] Can't write to GPIO", __func__);
      return -1;
    }
  close (file);

  return 1;
}


int
gpio_read (int pin)
{
  char valStr[1] = "";
  unsigned int val;
  int file;

  file = gpio_valfd (pin);
  /* fseek(file, 0, SEEK_SET); */
  if (read (file, &valStr, 1) == 1)
    {
      val = atoi (valStr);

      printf ("valStr: %s, val: %d\n", valStr, val);

      return val;
    }
  else
    {
      debug ("[%s] Can't read pin value", __func__);
      return -1;
    }

  close (file);
}


int
gpio_release (int pin)
{
  FILE *file;
  char pinStr[3];

  file = fopen ("/sys/class/gpio/unexport", "w");
  if (file == NULL)
    {
      debug ("[%s] Can't open file (unexport)\n", __func__);
      return -1;
    }

  sprintf (pinStr, "%d", pin);
  fwrite (pinStr, sizeof (char), strlen (pinStr), file);

  fclose (file);

  return 1;
}


/*! Wrapper for Arduino pin mode.

    @param[in] pin  Pin to intialize
    @param[in] dir  Direction to use in "input" or "output".

    @return  0 on success, -1 on failure. */
int
pinMode (int   pin,
	 char *dir)
{
  return  gpio_init (pin, dir);

}	/* pinMode () */


/*! Wrapper for Arduino write to pin.

    We have to map the Arduino text "HIGH" and "LOW" into "1" and "0" for
    Raspberry Pi.

    @param[in] pin  Pin to write to
    @param[in] val  Value to write, "HIGH" or LOW"

    @return  0 on success, -1 on failure. */
int
digitalWrite (int   pin,
	      char *val)
{
  if (0 == strcmp (val, "HIGH"))
    {
      return gpio_write (pin, HIGH);
    }
  else if (0 == strcmp (val, "LOW"))
    {
      return gpio_write (pin, LOW);
    }
  else
    {
      debug  ("[%s] Unrecognized value: %s: ignored.\n", __func__, val);
      return  -1;
    }
}	/* digitalWrite () */


/*! Wrapper for Arduino read from pin.

    @param[in] pin  Pin to read from

    @return  Value read. */
int
digitalRead (int  pin)
{
  return  gpio_read (pin);

}	/* digitalRead () */


/*! Wrapper for Arduino interrupt attach.

    @param[in] pin   Pin concerned with interrupt
    @param[in] isr   Interrupt service routine
    @param[in] mode  Edge on which to trigger ("rising", "falling", "both");

    @return  0 on success, -1 on failure. */
int
attachInterrupt (int    pin,
		 void (*isr) (int),
		 char  *mode)
{
  return  gpio_set_int (pin, isr, mode);

}	/* attachInterrupt () */


/*! Wrapper for Arduino interrupt detach.

    @param[in] pin   Pin concerned with interrupt

    @return  0 on success, -1 on failure. */
int
detachInterrupt (int  pin)
{
  return  gpio_clear_int (pin);

}	/* detachInterrupt () */
