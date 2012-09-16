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
  // TODO: Add check here
  fwrite (pinStr, sizeof (char), strlen (pinStr), file);

  fclose (file);

  sprintf (filename, "/sys/class/gpio/gpio%d/direction", pin);
  file = fopen (filename, "w");
  if (file == NULL)
    {
      debug ("[%s] Can't open file (direction)\n", __func__);
      return -1;
    }

  // TODO: Add check here
  fwrite (dir, sizeof (char), strlen (dir), file);

  fclose (file);

  return 1;
}


// Bits from: https://www.ridgerun.com/developer/wiki/index.php/Gpio-int-test.c
int
isr_handler (void *isr)
{
  struct pollfd fdset[2];
  int nfds = 2, gpio_fd, rc, len;
  char *buf[64];

  isr_t i = *(isr_t *) isr;

  if (isr_handler_flag)
    {
      printf ("isr_handler running\n");

      // Get /value fd
      // TODO: Add check here
      gpio_fd = gpio_valfd ((int) i.pin);


      while (1)
	{
	  memset ((void *) fdset, 0, sizeof (fdset));

	  fdset[0].fd = STDIN_FILENO;
	  fdset[0].events = POLLIN;

	  fdset[1].fd = gpio_fd;
	  fdset[1].events = POLLPRI;

	  // poll() with a timeout of a second
	  rc = poll (fdset, nfds, 1000);

	  if (rc < 0)
	    {
	      debug ("\npoll() failed!\n");
	      return -1;
	    }

	  if (rc == 0)
	    {
	      // Timeout 
	      debug ("poll() timeout.\n");
	      if (isr_handler_flag == 0)
		{
		  debug ("exiting isr_handler (timeout)");
		  pthread_exit (NULL);
		}
	    }

	  if (fdset[1].revents & POLLPRI)
	    {
	      // We have an interrupt!
	      len = read (fdset[1].fd, buf, 64);	// looks like we need this.
	      // Call ISR
	      (*i.isr) (i.pin);
	    }

	  if (fdset[0].revents & POLLIN)
	    {
	      (void) read (fdset[0].fd, buf, 1);
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
  // Details of the ISR
  isr_t *i = (isr_t *) malloc (sizeof (isr_t));
  i->pin = pin;
  i->isr = isr;

  // Set up interrupt
  gpio_edge (pin, mode);

  // Set isr_handler flag and create thread
  // TODO: check for errors using retval
  isr_handler_flag = 1;
  pthread_create (&isr_handler_thread, NULL, isr_handler, (void *) i);
  pthread_tryjoin_np (isr_handler_thread, NULL);

  return 1;
}


int
gpio_clear_int (int pin)
{
  // this will terminate isr_handler thread
  isr_handler_flag = 0;

  //TODO: Reset "edge", release pin?
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
  write (file, val, (sizeof (char) * 1));
  close (file);

  return 1;
}


int
gpio_read (int pin)
{
  char valStr[1] = { NULL };
  unsigned int val;
  int file;

  file = gpio_valfd (pin);
  //fseek(file, 0, SEEK_SET);
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
