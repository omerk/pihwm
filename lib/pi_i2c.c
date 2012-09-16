/* pi_i2c.c -- i2c library function implementation.

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

#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>

#include "pihwm.h"
#include "pi_i2c.h"


int
i2c_init (char *devname)
{
  int fd = open (devname, O_RDWR);

  if (fd < 0)
    {
      debug ("[%s] Can't open %s : %s\n", __func__, devname,
	     strerror (errno));
      return -1;
    }
  else
    {
      return fd;
    }
}


int
i2c_select_device (unsigned int fd, unsigned int addr)
{
  if (ioctl (fd, I2C_SLAVE, addr) < 0)
    {
      debug ("[%s] Can't select device %s: %s\n", __func__, addr,
	     strerror (errno));
      return -1;
    }
  else
    {
      return 1;
    }
}


int
i2c_write (unsigned int fd, unsigned int addr, unsigned char *data,
	   unsigned int len)
{

  if (i2c_select_device (fd, addr))
    {
      if (write (fd, data, len) != len)
	{
	  debug ("[%s] I2C write (address: 0x%X) failed: %s\n", __func__,
		 addr, strerror (errno));
	  return -1;
	}
    }
  else
    {
      printf ("[%s] Can't select I2C device at address: 0x%X, write failed\n",
	      __func__, addr);
      return -1;
    }

  return 1;
}


int
i2c_read (unsigned int fd, unsigned int addr, unsigned char *data,
	  unsigned int len)
{
  char buf[len];

  if (i2c_select_device (fd, addr))
    {
      if (read (fd, buf, len) != len)
	{
	  debug ("[%s]: I2C read (address: 0x%X) failed: %s\n", __func__,
		 addr, strerror (errno));
	  return -1;
	}
      else
	{
	  memcpy (data, buf, len);
	  return 1;
	}
    }
  else
    {
      debug ("[%s] Can't select I2C device at address: 0x%X, write failed\n",
	     __func__, addr);
      return -1;
    }
}
