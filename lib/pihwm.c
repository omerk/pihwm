/**
* @file   pihwm.c
* @author Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
* @author Jeremy Bennett <jeremy.bennett@embecosm.com> - Embecosm Limited
* @brief  Implementation for general library functions
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

#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
#include "pihwm.h"

/*! \addtogroup General
*  @brief General library functions
*  @{
*/

/**
* @mainpage Documentation

pihwm is a collection of lightweight drivers for the hardware peripherals on the
Raspberry Pi computer.

* @section build Building and installing pihwm

Grab the latest copy of the library from Github:
<pre>
git clone git://github.com/omerk/pihwm.git
cd pihwm
mkdir build
cd build
</pre>

Configure the software using the configure script in the main directory.
<pre>
../configure
</pre>

There are several options available, many of which are standard to GNU configure 
scripts. Use configure --help to see all the options. The most useful is --prefix 
to specify a directory for installation of the library.

Build the tool with:
<pre>
make all
</pre>

Install the tool with:
<pre>
make install
</pre>

This will install the library in prefix/lib, the demo programs in prefix/bin,
headers in prefix/include and the documentation in prefix/share, where prefix is
the prefix specified when configuring, if any.

The documentation may be created and installed in alternative formats (PDF,
Postscript,DVI, HTML) with for example:
<pre>
make pdf
make install-pdf
</pre>

* @section usage Using the library
The library may be used by adding -lpihwm when compiling on the Raspberry Pi.
Programs should include pihwm.h and any other hardware specific headers from 
the include directory as required. Note that if a prefix has been specified 
when configuring, it may be necessary to specify the location of the library 
(using the -L flag) and the headers (using the -I flag).

For example, running on the Raspberry Pi:
<pre>
gcc -I/opt/pihwm/include demo.c -L/opt/pihwm/lib -l pihwm -o demo
</pre>

A number of examples can be found under the /demo directory.

* @section bugs Reporting errors/bugs
Please report bugs via the <a href="https://github.com/omerk/pihwm/issues">
issue tracker</a> on the Github project page. Pull requests are also welcome!

*/

/**
* @brief Return board information (Model, PCB revision and Memory)
*
* @return board information
*/
board_t
board_info ()
{
	FILE *info;
	char rev_hex[5];
	unsigned int rev_int = 0;

	board_t board;
	board.model = -1;
	board.rev = -1;
	board.mem = -1;

	char *cmd = "cat /proc/cpuinfo | grep 'Revision' | awk '{print $3}'";

	if( !(info = popen(cmd, "r")) )
	{
		return board;
	}

	if ( fgets(rev_hex, 5, info) != NULL )
	{
		sscanf(rev_hex, "%x", &rev_int);
	}
	else
	{
		return board;
	}

	// Data from: http://raspberryalphaomega.org.uk/?p=428 
	switch(rev_int){
		case 2:
		case 3:
			board.model = MODEL_B;
			board.rev = REV_1;
			board.mem = MEM_256;
			break;

		case 4:
		case 5:
		case 6:
			board.model = MODEL_B;
			board.rev = REV_2;
			board.mem = MEM_256;
		break;

		case 7:
		case 8:
		case 9:
			board.model = MODEL_A;
			board.rev = REV_2;
			board.mem = MEM_256;
		break;

		case 13:
		case 14:
		case 15:
			board.model = MODEL_B;
			board.rev = REV_2;
			board.mem = MEM_512;
		break;

	default:
		// Default values (-1) already set.
		break;
	}

	return board;

}

/**
* @brief Return board model
*
* @return MODEL_A or MODEL_B
*/
int
board_model ()
{
	board_t b = board_info();
	return b.model;
}

/**
* @brief Return board revision
*
* @return REV_1 or REV_2
*/
int
board_rev ()
{
	board_t b = board_info();
	return b.rev;
}

/**
* @brief Return the amount of system memory
*
* @return MEM_256 or MEM_512
*/
int
board_mem ()
{
	board_t b = board_info();
	return b.mem;
}

/**
* @brief Check if the kernel module specified is loaded.
*
* @param 	name of the kernel module
*
* @return 	1 for success, -1 for failure
*/
int
check_kernel_module (char* modulename)
{
	FILE *lsmod;
	char cmd[100];
	char modcount[2];
	unsigned int modcount_int = 0;

	sprintf(cmd, "lsmod | grep %s | wc -l", modulename); 

	if( !(lsmod = popen(cmd, "r")) )
	{
		return -1;
	}

	if ( fgets(modcount, 2, lsmod) != NULL )
	{
		modcount_int = atoi(modcount);
	}
	else
	{
		return -1;
	}


	if ( modcount_int > 0 )
	{
		return 1;
	}
	else
	{
		return -1;
	}

}

/*! @} */
