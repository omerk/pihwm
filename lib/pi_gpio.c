/**
* @file   pi_gpio.c
* @author Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
* @author Jeremy Bennett <jeremy.bennett@embecosm.com> - Embecosm Limited
* @brief  gpio library function implementation
*
* @description
*
* @section LICENSE
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


/*! \addtogroup GPIO
*  @brief GPIO library functions
*  @{
*/

/**
* @brief	Return the file desriptor for the /edge sysfs entry for a pin.
* Also sets the interrupt condition for the pin.
*
* @param	pin		Pin number
* @param	edge	Interrupt condition
*
* @return 	1 for success, -1 for failure
*/
static int
gpio_edge (int pin, char *edge)
{
	FILE *file;
	char filename[35];

	sprintf(filename, "/sys/class/gpio/gpio%d/edge", pin);
	file = fopen(filename, "w");
	if ( file == NULL )
	{
		debug("[%s] Can't open file (edge): %s\n", __func__, filename);
		return -1;
	}

	fwrite(edge, sizeof (char), strlen (edge) + 1, file);

	fclose(file);

	return 1;
}

/**
* @brief	Return the file desriptor for the /value sysfs entry for a pin
*
* @param	pin		Pin number
*
* @return 	1 for success, -1 for failure
*/
static int
gpio_valfd (int pin)
{
	int file;
	char filename[35];

	sprintf(filename, "/sys/class/gpio/gpio%d/value", pin);
	file = open(filename, O_RDWR | O_NONBLOCK);
	if ( file < 0 )
	{
		debug("[%s] Can't open file (value): %s\n", __func__, filename);
		return -1;
	}
	else
	{
		return file;
	}

}

/**
* @brief	Initialize the GPIO interface for the pin numbered pin on the 
* Raspberry Pi P1 header in the direction specified by dir. dir should be
* either "in" or "out", for which the defined constants INPUT, IN, OUTPUT
* or OUT may be used.
*
* @param	pin		Pin number		
* @param	dir		Direction of pin (input or output)
*
* @return 	1 for success, -1 for failure
*/
int
gpio_init (unsigned int pin, unsigned int dir)
{
	FILE *file;
	char filename[35], pinStr[2];

	gpio_release(pin);

	file = fopen("/sys/class/gpio/export", "w");
	if ( file == NULL )
	{
		debug("[%s] Can't open file (export)\n", __func__);
		return -1;
	}

	sprintf(pinStr, "%d", pin);
	/* TODO: Add check here */
	fwrite(pinStr, sizeof (char), strlen (pinStr), file);

	fclose(file);

	sprintf(filename, "/sys/class/gpio/gpio%d/direction", pin);
	file = fopen(filename, "w");
	if ( file == NULL )
	{
		debug("[%s] Can't open file (direction)\n", __func__);
		return -1;
	}

  int ret = 1;
	if ( dir == 0 )
	{
		fwrite("out", sizeof (char), 3, file);
	} 
	else if ( dir == 1 )
	{
		fwrite("in", sizeof (char), 2, file);
	} 
	else
	{
		debug("[%s] Can't set pin direction.\n", __func__);
    ret = -1;
	}

	fclose(file);

	return ret;
}


/**
* @brief	Set up the listener thread for the interrupt service routine
*
* @param	isr		Interrupt service routine
*
* @return 	1 for success, -1 for failure
*/
static void *
isr_handler (void *isr)
{
	struct pollfd fdset[2];
	int nfds = 2, gpio_fd, rc;
	char *buf[64];

	isr_t i = *(isr_t *) isr;

	if ( isr_handler_flag )
	{
		printf("isr_handler running\n");

		/* Get /value fd
		TODO: Add check here */
		gpio_fd = gpio_valfd((int) i.pin);


		while ( 1 )
		{
			memset((void *) fdset, 0, sizeof (fdset));

			fdset[0].fd = STDIN_FILENO;
			fdset[0].events = POLLIN;

			fdset[1].fd = gpio_fd;
			fdset[1].events = POLLPRI;

			rc = poll(fdset, nfds, 1000);	/* Timeout in ms */

			if ( rc < 0 )
			{
				debug("\npoll() failed!\n");
				return (void *) -1;
			}

			if ( rc == 0 )
			{
				debug("poll() timeout.\n");
				if ( isr_handler_flag == 0 )
				{
					debug("exiting isr_handler (timeout)");
					pthread_exit(NULL);
				}
			}

			if ( fdset[1].revents & POLLPRI )
			{
				/* We have an interrupt! */
				if ( -1 == read(fdset[1].fd, buf, 64) )
				{
					debug("read failed for interrupt");
					return (void *) -1;
				}

				(*i.isr) (i.pin);		/* Call the ISR */
			}

			if ( fdset[0].revents & POLLIN )
			{
				if ( -1 == read(fdset[0].fd, buf, 1) )
				{
					debug("read failed for stdin read");
					return (void *) -1;
				}

				printf("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[0]);
			}

			fflush(stdout);
		}
	}
	else
	{
		debug("exiting isr_handler (flag)");
		pthread_exit(NULL);
	}

}

/**
* @brief	Set <b>isr</b> as the interrupt service routine (ISR) for pin numbered
* <b>pin</b> on the Raspberry Pi P1 header. mode should be one of the strings "rising",
* "falling" or "both" to indicate which edge(s) the ISR is to be triggered on.
* The function isr is called whenever the edge specified occurs, receiving as 
* argument the number of the pin which triggered the interrupt.
*
* @param	pin		Pin number to attach interrupt to
* @param	isr		Interrupt service routine to call
* @param	mode	Interrupt mode
*
* @return 	Returns 1 on success. Never fails
*/
int
gpio_set_int (unsigned int pin, void (*isr) (int), char *mode)
{
	/* Details of the ISR */
	isr_t *i = (isr_t *) malloc(sizeof (isr_t));
	i->pin = pin;
	i->isr = isr;

	/* Set up interrupt */
	gpio_edge(pin, mode);

	/* Set isr_handler flag and create thread
	TODO: check for errors using retval */
	isr_handler_flag = 1;
	pthread_create(&isr_handler_thread, NULL, isr_handler, (void *) i);
	//pthread_tryjoin_np(isr_handler_thread, NULL);

	return 1;
}

/**
* @brief	Clears any interrupt service routine (ISR) set on the pin numbered
* pin on the Raspberry Pi P1 header. The ISR will have been set originally by
* a call to gpio_set_int.
*
* @param	Pin number which has an interrupt attached
*
* @return 	Returns 1 on success. Never fails
*/
int
gpio_clear_int (unsigned int pin)
{
	/* this will terminate isr_handler thread */
	isr_handler_flag = 0;

	/* TODO: Reset "edge", release pin? */
	return  0;			/* Is this a correct return result. */
}

/**
* @brief	Set the pin numbered pin on the Raspberry Pi P1 header to the value specified by val. dir should be either "0" or "1". The defined constants LOW or OFF may be used instead of 0 and HIGH or ON instead of 1/
*
* @param	pin		Pin number to write to
* @param	val		Value to set (0 or 1)
*
* @return 	1 for success, -1 for failure
*/
int
gpio_write (unsigned int pin, unsigned int val)
{
	int file;
  int ret = 1;

	file = gpio_valfd (pin);

	if ( val == 0 )
	{
		if ( write(file, "0", (sizeof(char) * 1)) == -1 )
		{
			debug("[%s] Can't write to GPIO pin", __func__);
      ret = -1;
		}
	} 
	else if ( val == 1 )
	{
		if ( write(file, "1", (sizeof(char) * 1)) == -1 )
		{
			debug("[%s] Can't write to GPIO pin", __func__);
      ret = -1;
		}
	} 
	else 
	{
		debug("[%s] Wrong value for the GPIO pin", __func__);
    ret = -1;
	}

	close(file);

	return ret;
}

/**
* @brief	Read the value on the pin numbered pin on the Raspberry Pi
* P1 header.
*
* @param	pin		Pin number to read from
*
* @return 	On success, returns 0 or 1 depending on whether the pin is
* low or high respectively. On failure returns -1.
*/
int
gpio_read (unsigned int pin)
{
  char valStr[1] = "";
  unsigned int val;
  int file;

  file = gpio_valfd (pin);
  /* fseek(file, 0, SEEK_SET); */
  if (read (file, &valStr, 1) == 1)
    {
      val = atoi (valStr);
      debug ("[%s] valStr: %s, val: %d\n", __func__, valStr, val);
    }
  else
    {
      debug ("[%s] Can't read pin value", __func__);
      val = -1;
    }

  close (file);
  return val;
}

/**
* @brief	The inverse of gpio_init. Frees the pin numbered pin on the
* Raspberry Pi P1 header, so it can be used for other purposes.
*
* @param	pin		Pin number to release
*
* @return 	1 for success, -1 for failure
*/
int
gpio_release (unsigned int pin)
{
	FILE *file;
	char pinStr[3];

	file = fopen("/sys/class/gpio/unexport", "w");
	if ( file == NULL )
	{
		debug("[%s] Can't open file (unexport)\n", __func__);
		return -1;
	}

	sprintf(pinStr, "%d", pin);
	fwrite(pinStr, sizeof (char), strlen (pinStr), file);

	fclose(file);

	return 1;
}

/*! @} */
