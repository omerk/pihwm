/* check_kernel_modules.c

   Copyright (C) 2012 Omer Kilic

   Contributor Omer Kilic <omerkilic@gmail.com>

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
#include "pihwm.h"

int main()
{

	printf("Kernel Modules:\n");

	printf("spi_bcm2708\t");
	if ( check_kernel_module("spi_bcm2708") > 0 ){
		printf("OK\n");
	} else {
		printf("Not loaded!\n");
	} 
	
	printf("i2c_bcm2708\t");
	if ( check_kernel_module("i2c_bcm2708") > 0 ){
		printf("OK\n");
	} else {
		printf("Not loaded!\n");
	} 
	
	printf("spidev\t\t");
	if ( check_kernel_module("spidev") > 0 ){
		printf("OK\n");
	} else {
		printf("Not loaded!\n");
	} 
	
	printf("i2c_dev\t\t");
	if ( check_kernel_module("i2c_dev") > 0 ){
		printf("OK\n");
	} else {
		printf("Not loaded!\n");
	}

	return 0;

}

