/* pwm_demo.c -- Demo of pulse width modulation
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
#include <stdlib.h>

#include "pihwm.h"
#include "pi_pwm.h"


int
main (void)
{
  int i, j;

  // Initialise PWM module
  if ( pwm_init() < 0 ){
    printf("ERROR: Can't initialise PWM module, are you running this as root?\n");
    exit(1);
  }

  for ( i = 0; i < 5; i++ ){
	// Do a little fade in/out
  	printf("Fading in...\n");
	for( j = 0; j < PWM_MAX; j++ ){
   	pwm_value(j);
		delay(2);
	}

	  printf("Fading out...\n");
	  for( j = PWM_MAX; j > 0; j-- ){
	    pwm_value(j);
	    delay(2);
	 }
  }

  // Unmap memory and disable the PWM module
  pwm_release();
  
  return 0;

}

