/* pi_i2c.c -- I2C library function implementation.

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

#include "config.h"

#include <linux/i2c.h>
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
i2c_init()
{
	int rev = board_rev();

	if ( check_kernel_module("i2c_dev") < 0 ){
		debug ("[%s] Kernel module \"i2c_dev\" not loaded.\n", __func__);
		return -1;
	}

	if ( rev == REV_1 ){
		return i2c_init_name("/dev/i2c-0");
	} else if ( rev == REV_2 ){
		return i2c_init_name("/dev/i2c-1");
	} else {
		return -1;
	}
}


int
i2c_init_name (char *devname)
{
	int fd = open (devname, O_RDWR);

	if ( fd < 0 ){
		debug ("[%s] Can't open %s : %s\n", __func__, devname, strerror (errno));
		return -1;
	} else {
		return fd;
	}
}


int
i2c_select_device (unsigned int fd, unsigned int addr)
{
	if ( ioctl (fd, I2C_SLAVE, addr) < 0 ){
		debug ("[%s] Can't select device %s: %s\n", __func__, addr,
		strerror (errno));
		return -1;
	} else {
		return 1;
	}
}


int
i2c_write (unsigned int fd, unsigned int addr, unsigned char *data, unsigned int len)
{
	if ( i2c_select_device(fd, addr) ){
		if ( write (fd, data, len) != len ){
			debug ("[%s] I2C write (address: 0x%X) failed: %s\n", __func__, addr, strerror (errno));
			return -1;
		}
	} else {
		printf ("[%s] Can't select I2C device at address: 0x%X, write failed\n",  __func__, addr);
		return -1;
	}

	return 1;
}


int
i2c_read (unsigned int fd, unsigned int addr, unsigned char *data, unsigned int len)
{
	char buf[len];

	if ( i2c_select_device (fd, addr) ){
		if ( read (fd, buf, len) != len ){
			debug ("[%s]: I2C read (address: 0x%X) failed: %s\n", __func__, addr, strerror (errno));
			return -1;
		} else {
			memcpy (data, buf, len);
			return 1;
		}
	} else {
		debug ("[%s] Can't select I2C device at address: 0x%X, read failed\n", __func__, addr);
		return -1;
	}
}

/*
int
i2c_read_write (unsigned int fd, unsigned int addr, unsigned char *write, unsigned char *read)
{

	struct i2c_msg	msgs[2]; 
	struct i2c_rdwr_ioctl_data	rdwr; 
	int rc; 

	msgs[0].addr  = ADDR; 
	msgs[0].flags = 0; 
	msgs[0].buf   = write_buf;
	msgs[0].len   = count(write_buf);

	msgs[1].addr  = ADDR; 
	msgs[1].flags = I2C_M_RD; 
	msgs[1].buf   = read_buf; 
	msgs[1].len   = count(read_buf); 

	rdwr.msgs  = msgs; 
	rdwr.nmsgs = 2; 

	if ( i2c_select_device (fd, addr) ){
		// FIXME: add error checking here
		ioctl( fd , I2C_RDWR , &rdwr );
	} else {
		debug ("[%s] Can't select I2C device at address: 0x%X, read failed\n", __func__, addr);
		return -1;
	}

	return 1;
}
*/
