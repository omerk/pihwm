/**
* @file   pihwm.h
* @author Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
* @author Jeremy Bennett <jeremy.bennett@embecosm.com> - Embecosm Limited
* @brief  Header for general library functionality
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

#ifndef PIHWM_H
#define PIHWM_H

#include <stdio.h>
#include <unistd.h>

// Board information
#define MODEL_A	100
#define MODEL_B	200
#define REV_1	10
#define REV_2	20
#define MEM_256	1
#define MEM_512	2

// Useful macros
#ifdef DEBUG
	#define debug(...) printf(__VA_ARGS__)
#else
	#define debug(...) ;
#endif

#define delay(d)				usleep(d*1000); //millisec
#define delayMicroseconds(d)	usleep(d);
#define delaySeconds(d)			sleep(d);

// http://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html
#define a(...)	(unsigned char[])__VA_ARGS__

#define size(a)	(sizeof(a) / sizeof((a)[0]))

typedef struct
{
	int model;
	int rev;
	int mem;
} board_t;

// Function prototypes
board_t board_info ();
int board_model ();
int board_rev ();
int board_mem ();
int check_kernel_module (char* modulename);

#endif
