/* check_kernel_modules.c -- Check if the necessary kernel modules are loaded or not.
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

