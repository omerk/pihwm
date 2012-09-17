/* spi_bb_mcp3202.c -- Demo of MCP3202 SPI chip

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

#include <stdio.h>
#include <stdint.h>

#include "pihwm.h"
#include "pi_gpio.h"

/* SPI pins */
#define	SS	18
#define	MOSI	17
#define	MISO	21
#define	SCK	22

/* reference voltage */
#define VREF	3.3

uint16_t val;

double
adc_volt (uint16_t val)
{
  return (val * VREF) / 4096;
}

uint16_t
adc_read (uint8_t channel)
{
  uint16_t result = 0;
  uint8_t cmd = 0xd0;	/* 1'b11010000
			   [start][sgl/diff][odd/sign][msbf][x][x][x][x] */
  uint8_t i, cur, x;

  /* select channel */
  cmd |= (channel << 5);

  /* begin transaction */
  digitalWrite (SS, LOW);

  /* write command bits */
  for (i = 7; i >= 4; i--)
    {
      x = cmd & (1 << i);
      digitalWrite (MOSI, (char *) &x);

      /* clock pulse */
      digitalWrite (SCK, HIGH);
      digitalWrite (SCK, LOW);
    }

  /* ignore null bit */
  digitalWrite (SCK, HIGH);
  digitalWrite (SCK, LOW);

  /* read conversion result */
  for (i = 11; i >= 0; i--)
    {
      cur = digitalRead (MISO);
      result += cur << i;

      /* clock pulse */
      digitalWrite (SCK, HIGH);
      digitalWrite (SCK, LOW);
    }

  /* end transaction */
  digitalWrite (SS, HIGH);

  return result;
}

int
main (void)
{
  uint16_t val;
  uint8_t i;

  printf ("main start\n");

  printf ("direction\n");
  /* set up SPI pins */
  pinMode (SS, OUTPUT);
  pinMode (MOSI, OUTPUT);
  pinMode (MISO, INPUT);
  pinMode (SCK, OUTPUT);
  delay (3000);

  printf ("setup");
  /* disable slave */
  digitalWrite (SS, HIGH);
  digitalWrite (MOSI, LOW);
  digitalWrite (SCK, LOW);
  delay (3000);

  printf ("reading...\n");

  for (;;)
    {
      for (i = 0; i < 2; i++)
	{
	  val = adc_read (i);
	  printf ("Channel %d -- Raw: %4d  Volts: %f\n", i, val,
		  adc_volt (val));
	  delay (50);
	}

      printf ("-------------------\n");
      delay (2000);
    }

  printf ("main end\n");

  return 0;

}
