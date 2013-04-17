/**
* @file   pi_gpio.h
* @author Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
* @author Jeremy Bennett <jeremy.bennett@embecosm.com> - Embecosm Limited
* @brief  gpio library headers
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

#ifndef PI_GPIO_H
#define PI_GPIO_H

// Useful constants
#define INPUT	1
#define OUTPUT	0
#define IN		INPUT
#define OUT		OUTPUT

#define HIGH	1
#define LOW		0
#define ON		HIGH
#define OFF		LOW

// Function prototypes 
int gpio_init (unsigned int pin, unsigned int dir);
int gpio_set_int (unsigned int pin, void (*isr) (int), char *mode);
int gpio_clear_int (unsigned int pin);
int gpio_write (unsigned int pin, unsigned int val);
int gpio_read (unsigned int pin);
int gpio_release (unsigned int pin);

// Aliases for pseudo-Arduino compatibility
#define pinMode(pin, dir)					gpio_init(pin, dir);
#define digitalWrite(pin, val)				gpio_write(pin, val);
#define digitalRead(pin)					gpio_read(pin);
#define attachInterrupt(pin, isr, mode)	gpio_set_int(pin, isr, mode);
#define detachInterrupt(pin)				gpio_clear_int(pin);

#endif

