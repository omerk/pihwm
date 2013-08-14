/* gpio_test.c -- Test of GPIO output.
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

#include "pihwm.h"

#define	BTN0	17
#define	BTN1	22	
#define	LED0	23
#define	LED1	24

int
main (void)
{
  unsigned int val, i;

  printf("main() start\n");

  gpio_init(LED0, OUTPUT);
  gpio_init(LED1, OUTPUT);
  gpio_init(BTN0, INPUT);
  gpio_init(BTN1, INPUT);

  printf("Pushbuttons:\n");
  for (i = 0; i < 10; i++)
    {
      val = gpio_read(BTN0);
      printf(" GPIO%d: %d  ", BTN0, val);
      val = gpio_read(BTN1);
      printf("GPIO%d: %d\n", BTN1, val);

      gpio_write(LED0, HIGH);
      gpio_write(LED1, HIGH);
      sleep(1);
      gpio_write(LED0, LOW);
      gpio_write(LED1, LOW);
      sleep(1);
    }

  gpio_release(LED0);
  gpio_release(LED1);
  gpio_release(BTN0);
  gpio_release(BTN1);

  printf("main() end\n");

  return 0;
}

