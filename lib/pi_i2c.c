/**
* @file   pi_i2c.c
* @author Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
* @author Jeremy Bennett <jeremy.bennett@embecosm.com> - Embecosm Limited
* @brief  I2C library function implementation.
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

/*! \addtogroup I2C
*  @brief I2C library functions
*  @{
*/

/**
* @brief	Initialises the i2c-dev interface for the I2C peripheral
* exposed on the P1 header (which is dependent on the board revision)
*
* @return 	The file descriptor for the relevant i2c-dev interface, -1
* for failure
*/
int
i2c_init()
{
	int rev;

	if ( check_kernel_module("i2c_dev") < 0 )
	{
		debug("[%s] Kernel module \"i2c_dev\" not loaded.\n", __func__);
		return -1;
	}

	if ( check_kernel_module("i2c_bcm2708") < 0 )
	{
		debug("[%s] Kernel module \"i2c_bcm2708\" not loaded.\n", __func__);
		return -1;
	}

	rev = board_rev();

	if ( rev == REV_1 )
	{
		return i2c_init_name("/dev/i2c-0");
	}
	else if ( rev == REV_2 )
	{
		return i2c_init_name("/dev/i2c-1");
	}
	else
	{
		return -1;
	}

}

/**
* @brief	Initialises the i2c-dev interface for the sysfs entry specified
* by the devname parameter.
*
* @param	devname		The sysfs entry for a particular I2C peripheral
*
* @return 	The file descriptor for the relevant i2c-dev interface, -1
* for failure
*/
int
i2c_init_name (char *devname)
{
	int fd = open (devname, O_RDWR);

	if ( fd < 0 )
	{
		debug("[%s] Can't open %s : %s\n", __func__, devname, strerror (errno));
		return -1;
	}
	else
	{
		return fd;
	}
}

/**
* @brief	Selects a specific I2C slave device
*
* @param	fd		File descriptor for the i2c-dev interface	
* @param	addr	I2C slave device address
*
* @return 	1 for success, -1 for failure
*/
int
i2c_select_device (unsigned int fd, unsigned int addr)
{
	if ( ioctl (fd, I2C_SLAVE, addr) < 0 )
	{
		debug("[%s] Can't select device %s: %s\n", __func__, addr, strerror (errno));
		return -1;
	}
	else
	{
		return 1;
	}
}

/**
* @brief	Initiates an I2C write operation
*
* @param	fd		File descriptor for the i2c-dev interface
* @param	addr	I2C slave device address
* @param	data	Data to write to the device
* @param	len		Length of data
*
* @return 	1 for success, -1 for failure
*/
int
i2c_write (unsigned int fd, unsigned int addr, unsigned char *data,	unsigned int len)
{

	if ( i2c_select_device (fd, addr) )
	{
		if ( write (fd, data, len) != len )
		{
			debug("[%s] I2C write (address: 0x%X) failed: %s\n", __func__, addr, strerror (errno));
			return -1;
		}
	}
	else
	{
		printf ("[%s] Can't select I2C device at address: 0x%X, write failed\n", __func__, addr);
		return -1;
	}

	return 1;
}

/**
* @brief	Initiates an I2C read operation
*
* @param	fd		File descriptor for the i2c-dev interface
* @param	addr	I2C slave device address
* @param	data	Pointer to the read buffer
* @param	len		Length of data
*
* @return 	1 for success, -1 for failure
*/
int
i2c_read (unsigned int fd, unsigned int addr, unsigned char *data,	unsigned int len)
{
	char buf[len];

	if ( i2c_select_device (fd, addr) )
	{
		if ( read (fd, buf, len) != len )
		{
			debug("[%s]: I2C read (address: 0x%X) failed: %s\n", __func__, addr, strerror (errno));
			return -1;
		}
		else
		{
			memcpy(data, buf, len);
			return 1;
		}
	}
	else
	{
		debug("[%s] Can't select I2C device at address: 0x%X, write failed\n", __func__, addr);
		return -1;
	}
}


/*
* FIXME: Test this.
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

/*! @} */