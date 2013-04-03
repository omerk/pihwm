/**
* @file   pihwm.h
* @author Omer Kilic <omerkilic@gmail.com>
* @author Jeremy Bennett <jeremy.bennett@embecosm.com>
* @brief  Header for general library functionality
*
* @section LICENSE
* Copyright (C) 2012 Omer Kilic
* Copyright (C) 2012 Embecosm Limited
*
* This file is part of pihwm.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 3 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>. 
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

/* http://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html */
#define a(...)		(unsigned char[])__VA_ARGS__

#define size(a)		(sizeof(a) / sizeof((a)[0]))

typedef struct
{
	int model;
	int rev;
	int mem;
} board_t;

// Function prototypes for pihwm.c
board_t board_info ();
int board_model ();
int board_rev ();
int board_mem ();
int check_kernel_module (char* modulename);

#endif
