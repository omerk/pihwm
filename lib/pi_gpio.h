/* pi_gpio.h -- gpio library headers

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

#ifndef PI_GPIO_H
#define PI_GPIO_H

/* Aliases for pseudo-Arduino compatibility */
#define pinMode(pin, dir)		gpio_init(pin, dir);
#define digitalWrite(pin, val)		gpio_write(pin, val);
#define digitalRead(pin)		gpio_read(pin);
#define attachInterrupt(pin, isr, mode)	gpio_set_int(pin, isr, mode);
#define detachInterrupt(pin)		gpio_clear_int(pin);

/* Function prototypes */
int gpio_init (int pin, char *dir);
int gpio_set_int (int pin, void (*isr) (int), char *mode);
int gpio_clear_int (int pin);
int gpio_edge (int pin, char *edge);
int gpio_valfd (int pin);
int gpio_write (int pin, char *val);
int gpio_read (int pin);
int gpio_release (int pin);

#endif
