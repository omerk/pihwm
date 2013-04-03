/* pi_spi.c -- SPI library function implementation.

Copyright (C) 2012 Omer Kilic

Contributor Omer Kilic <omerkilic@gmail.com>

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

//#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "pihwm.h"
#include "pi_spi.h"

int
spi_init (uint8_t channel)
{
	int fd;

	if ( check_kernel_module("spidev") < 0 )
	{
		debug ("[%s] Kernel module \"spidev\" not loaded.\n", __func__);
		return -1;
	}

	if ( check_kernel_module("spi_bcm2708") < 0 )
	{
		debug ("[%s] Kernel module \"spi_bcm2708\" not loaded.\n", __func__);
		return -1;
	}

	if ( channel == 0 )
	{
		fd = open("/dev/spidev0.0", O_RDWR);
	}
	else if ( channel == 1 )
	{
		fd = open("/dev/spidev0.1", O_RDWR);
	}
	else
	{
		debug ("[%s] Invalid SPI channel: %d\n", __func__, channel);
		fd = -1;
	}

	if ( fd < 0 )
	{
		debug ("[%s] Can't open SPI device.\n", __func__);
		return -1;
	}
	else
	{
		// device open, config default values
		if ( spi_config_default(fd) )
		{
			return fd;
		}
		else
		{
			debug ("[%s] Can't set default SPI config.\n", __func__);
			return -1;
		}
	}
}

int
spi_config(int fd, uint8_t mode, uint8_t bits, uint32_t speed, uint16_t delay)
{
	int ret;

	//spi mode
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if ( ret == -1 )
	{
		debug ("[%s] Can't set SPI mode.\n", __func__);
		return -1;
	}

	// bits per word
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if ( ret == -1 )
	{
		debug ("[%s] Can't set SPI bits per word.\n", __func__);
		return -1;
	}

	// max speed hz
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if ( ret == -1 )
	{
		debug ("[%s] Can't set SPI max speed.\n", __func__);
		return -1;
	}

	return 1;
}

int
spi_config_default(int fd)
{
	int ret;

	ret = spi_config (fd, SPI_DEFAULT_MODE, SPI_DEFAULT_BPW, SPI_DEFAULT_SPEED, SPI_DEFAULT_DELAY);

	return ret;
}

int
spi_transfer (int fd, uint8_t txbuf[], uint8_t rxbuf[], uint8_t len)
{
	int ret;

	struct spi_ioc_transfer transfer = {
		.tx_buf = (unsigned long)txbuf,
		.rx_buf = (unsigned long)rxbuf,
		.len = len,
		.delay_usecs = SPI_DEFAULT_DELAY,
		.speed_hz = SPI_DEFAULT_SPEED,
		.bits_per_word = SPI_DEFAULT_BPW,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
	if ( ret < 1 )
	{
		debug ("[%s] Can't send SPI message.\n", __func__);
		return -1;
	}
	else
	{
		return 1;
	}
}

