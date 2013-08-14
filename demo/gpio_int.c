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
#include <unistd.h>

#include "pihwm.h"


#define	INT_PIN	17


void
my_isr (int pin)
{
  printf ("my_isr fired for pin %d\n", pin);
}


int
main (int argc, char **argv, char **envp)
{
  printf ("main start\n");

  /* Init Pin */
  gpio_init (INT_PIN, INPUT);

  /* Init interrupt */
  gpio_set_int (INT_PIN, my_isr, "rising");

  for (;;)
    {
      printf ("doing nothing, waiting for an interrupt.\n");
      sleep (2);
    }

  /* If we didn't have for(;;); */
  gpio_release (INT_PIN);
  printf ("main end\n");
  return 0;
}
