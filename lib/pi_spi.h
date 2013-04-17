/**
* @file   pi_spi.h
* @author Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
* @brief  SPI library headers.
*
* @description
*
* @section LICENSE
* Copyright (C) 2013 Omer Kilic <omerkilic@gmail.com> - Erlang Solutions
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

#ifndef PI_SPI_H
#define PI_SPI_H

#include <linux/spi/spidev.h>
#include <inttypes.h>

// Default config for the SPI module
#define SPI_DEFAULT_MODE	0	
#define SPI_DEFAULT_BPW		8
#define SPI_DEFAULT_SPEED	1000000
#define SPI_DEFAULT_DELAY	10

// Function Prototypes
int spi_init (uint8_t channel);
int spi_config (int fd, uint8_t mode, uint8_t bits, uint32_t speed, uint16_t delay);
int spi_config_default (int fd);
int spi_transfer (int fd, uint8_t txbuf[], uint8_t rxbuf[], uint8_t len);

#endif
