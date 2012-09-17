/* pi_i2c.h -- i2c library headers.

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

#ifndef PI_I2C_H
#define PI_I2C_H

int i2c_init (char *devname);
int i2c_select_device (unsigned int fd, unsigned int addr);
int i2c_write (unsigned int fd, unsigned int addr, unsigned char *data,
	       unsigned int len);
int i2c_read (unsigned int fd, unsigned int addr, unsigned char *data,
	      unsigned int len);

#endif
