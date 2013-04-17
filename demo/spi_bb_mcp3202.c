/* spi_bb_mcp3202.c -- Demo of MCP3202 SPI ADC chip
*
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
      digitalWrite (MOSI, x);

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
