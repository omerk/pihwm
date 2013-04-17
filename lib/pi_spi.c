/**
* @file   pi_spi.c
* @author Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
* @brief  SPI library function implementation.
*
* @description
*
* @section LICENSE
* Copyright (C) 2013 Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
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

/*! \addtogroup SPI
*  @brief SPI library functions
*  @{
*/

/**
* @brief	Initialises the spidev interface
*
* @param	channel	SPI channel to use (0 or 1)
*
* @return	1 for success, -1 for failure
*/
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


/**
* @brief	Configures the spidev interface
*
* @param	fd		File descriptor to use, can be acquired via spi_init()
* @param	mode	SPI mode	
* @param	bits	Number of bits
* @param	speed	Bus speed
* @param	delay	Amount of delay
*
* @return	1 for success, -1 for failure
*/
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

/**
* @brief	Configures the spidev interface with default values
*
* @param	fd	File descriptor to use, can be acquired via spi_init()
*
* @return	1 for success, -1 for failure
*/
int
spi_config_default(int fd)
{
	int ret;

	ret = spi_config (fd, SPI_DEFAULT_MODE, SPI_DEFAULT_BPW, SPI_DEFAULT_SPEED, SPI_DEFAULT_DELAY);

	return ret;
}

/**
* @brief	Initiates SPI transfers
*
* @param	fd		File descriptor to use, can be acquired via spi_init()
* @param	txbuf	Transmit buffer array
* @param	rxbuf	Receive buffer array
* @param	len		Length of transfer
*
* @return	1 for success, -1 for failure
*/
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

/*! @} */