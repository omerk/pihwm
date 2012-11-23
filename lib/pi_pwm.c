/* pi_pwm.c -- Pulse width modulation library function implementation.

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

/* Based on Gert Jan van Loo's example code. */

#include "config.h"

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
// TODO: These are already defined in .h, remove?
volatile unsigned int *gpio, *pwm, *clk;

static int
open_memory ()
{
  int mem_fd;

  mem_fd = open ("/dev/mem", O_RDWR | O_SYNC);

  if ( mem_fd < 0 ){
    debug ("[%s] Can't open /dev/mem. Are you root?\n", __func__);
    return -1;
  } else {
    return mem_fd;
  }

}

static volatile unsigned int*
map_register (int mem_fd, unsigned int addr)
{
  char *mem;
  unsigned char *map;

  // Allocate a chunk of memory
  if ( (mem = malloc(BLOCK_SIZE + (PAGE_SIZE - 1))) == NULL ){
      debug ("[%s] Can't allocate memory.\n", __func__);
      return 0;
  }

  // Align?
  if ( (unsigned long) mem % PAGE_SIZE ){
    mem += PAGE_SIZE - ((unsigned long) mem % PAGE_SIZE);
  }
    
  // Map relevant register address to the allocated memory pointer
  map = (unsigned char *) mmap ((caddr_t) mem, 
            BLOCK_SIZE, PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_FIXED, mem_fd, addr);

  if ( (long) map < 0 ){
    debug ("[%s] Can't mmap memory.\n", __func__);
    return 0;
  } else {
    return (volatile unsigned int*) map;
  }

}

int
pwm_init (){

  int mem_fd;

  mem_fd = open_memory();
  if ( mem_fd < 0){
    debug ("[%s] Can't open /dev/mem.\n", __func__);
    return -1;
  }

  // FIXME: Check return values?
  gpio = map_register(mem_fd, GPIO_BASE);
  clk = map_register(mem_fd, CLOCK_BASE);
  pwm = map_register(mem_fd, PWM_BASE);

  /* GPIO18= PWM channel-A       Funct. 5
     Setup GPIO18 as PWM output */
  INP_GPIO (18);
  SET_GPIO_ALT (18, 5);

    /* Derive PWM clock direct from X-tal
     thus any system auto-slow-down-clock-to-save-power does not effect it
     The values below depends on the X-tal frequency! */
  PWMCLK_DIV = 0x5A000000 | (32 << 12);/* set pwm div to 32 (19.2/3 = 600KHz) */
  PWMCLK_CNTL = 0x5A000011; /* Source=osc and enable */

  /* Make sure it is off and that the B driver (GPIO4) is low */
  GPIO_CLR0 = 1 << 4;   /* Set GPIO 4 LOW */
  
  PWM_CONTROL = 0;
  usleep(100);

  /* I use 1024 steps for the PWM
     (Just a nice value which I happen to like) */
  PWM0_RANGE = PWM_MAX;
  usleep(100);

  PWM_CONTROL = PWM0_ENABLE;
  usleep(100);

  return 1;

}

void
pwm_mode (unsigned int mode)
{
  PWM_CONTROL = mode;
}


/* Set PWM value

   This routine does not wait for the value to arrive If a new value comes in
   before it is picked up by the chip it will definitely be too fast for the
   motor to respond to it */
void
pwm_value (unsigned int value)
{

  if ( value < 0 ){
    value = 0;
  } 
  
  if ( value > PWM_MAX ){
    value = PWM_MAX;
  }

  PWM0_DATA = value;
}


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

