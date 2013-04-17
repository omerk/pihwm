/**
* @file   pi_pwm.c
* @author Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
* @author Jeremy Bennett <jeremy.bennett@embecosm.com> - Embecosm Limited
* @brief  Pulse width modulation library function implementation.
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

/* Based on Gert van Loo's example code. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include "pihwm.h"
#include "pi_pwm.h"

// Registers
// FIXME: These are already defined in .h, remove?
volatile unsigned int *gpio, *pwm, *clk;

/*! \addtogroup PWM
*  @brief PWM library functions
*  @{
*/

/**
* @brief	Opens /dev/mem and returns the file descriptor
*
* @return 	File descriptor for /dev/mem, -1 for failure
*/
static int
open_memory ()
{
	int mem_fd;

	mem_fd = open("/dev/mem", O_RDWR | O_SYNC);

	if ( mem_fd < 0 )
	{
		debug("[%s] Can't open /dev/mem. Are you root?\n", __func__);
		return -1;
	} else {
		return mem_fd;
	}

}

/**
* @brief	Maps a specified register in memory for manipulation
*
* @param	mem_fd		File descriptor for /dev/mem
* @param	addr		Address of register
*
* @return 	Pointer for specified register in memory, -1 for failure
*/
static volatile unsigned int*
map_register (int mem_fd, unsigned int addr)
{
	char *mem;
	unsigned char *map;

	// Allocate a chunk of memory
	if ( (mem = malloc(BLOCK_SIZE + (PAGE_SIZE - 1))) == NULL )
	{
		debug("[%s] Can't allocate memory.\n", __func__);
		return 0;
	}

	// Align?
	if ( (unsigned long) mem % PAGE_SIZE )
	{
		mem += PAGE_SIZE - ((unsigned long) mem % PAGE_SIZE);
	}

	// Map relevant register address to the allocated memory pointer
	map = (unsigned char *) mmap ((caddr_t) mem, 
		BLOCK_SIZE, PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_FIXED, mem_fd, addr);

	if ( (long) map < 0 )
	{
		debug("[%s] Can't mmap memory.\n", __func__);
		return 0;
	} else {
		return (volatile unsigned int*) map;
	}

}

/**
* @brief	Initialises the PWM peripheral
*
* @return 	1 for success, -1 for failure
*/
int
pwm_init ()
{

	int mem_fd;

	mem_fd = open_memory();
	if ( mem_fd < 0)
	{
		debug("[%s] Can't open /dev/mem.\n", __func__);
		return -1;
	}

	// FIXME: Check return values?
	gpio = map_register(mem_fd, GPIO_BASE);
	clk = map_register(mem_fd, CLOCK_BASE);
	pwm = map_register(mem_fd, PWM_BASE);

	/* GPIO18= PWM channel-A       Funct. 5
	Setup GPIO18 as PWM output */
	INP_GPIO(18);
	SET_GPIO_ALT(18, 5);

	/* Derive PWM clock direct from X-tal
	thus any system auto-slow-down-clock-to-save-power does not effect it
	The values below depends on the X-tal frequency! */
	PWMCLK_DIV = 0x5A000000 | (32 << 12);/* set pwm div to 32 (19.2/3 = 600KHz) */
	PWMCLK_CNTL = 0x5A000011; /* Source=osc and enable */

	/* Make sure it is off and that the B driver (GPIO4) is low */
	GPIO_CLR0 = 1 << 4;   /* Set GPIO 4 LOW */

	PWM_CONTROL = 0;
	usleep(100);

	/* 1024 steps for the PWM */
	PWM0_RANGE = PWM_MAX;
	usleep(100);

	PWM_CONTROL = PWM0_ENABLE;
	usleep(100);

	return 1;

}

/**
* @brief	Sets mode for the PWM peripheral
*
* @param	mode		Mode for the PWM peripheral
*
* @return 	none
*/
void
pwm_mode (unsigned int mode)
{
	PWM_CONTROL = mode;
}


/**
* @brief	Sets PWM value
*
* @param	value		PWM value
*
* @return 	1 for success, -1 for failure
*/
void
pwm_value (unsigned int value)
{

	if ( value < 0 )
	{
		value = 0;
	} 

	if ( value > PWM_MAX )
	{
		value = PWM_MAX;
	}

	PWM0_DATA = value;
}


/**
* @brief	Releases the PWM peripheral and unmaps the memory
*
* @return 	none
*/
void
pwm_release ()
{

	GPIO_CLR0 = 1 << 4;
	pwm_value(0);
	PWM_CONTROL = 0;

	// FIXME: Check this.
	//munmap(pwm, BLOCK_SIZE);
	//munmap(gpio, BLOCK_SIZE);
	//munmap(clk, BLOCK_SIZE);
}

/*! @} */