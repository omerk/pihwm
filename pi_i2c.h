
/* http://www.kernel.org/doc/Documentation/i2c/dev-interface */

#ifndef PI_I2C_H
#define PI_I2C_H

#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>


#define a(...) (unsigned char[])__VA_ARGS__


extern int i2c_init(char* devname);
extern int i2c_select_device(unsigned int fd, unsigned int addr);
extern int i2c_write(unsigned int fd, unsigned int addr, unsigned char* data, unsigned int len);
extern int i2c_read(unsigned int fd, unsigned int addr, unsigned char* data, unsigned int len);

#endif

