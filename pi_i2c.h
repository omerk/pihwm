#ifndef PI_I2C_H
#define PI_I2C_H

#include "pihwm.h"

int i2c_init(char* devname);
int i2c_select_device(unsigned int fd, unsigned int addr);
int i2c_write(unsigned int fd, unsigned int addr, unsigned char* data, unsigned int len);
int i2c_read(unsigned int fd, unsigned int addr, unsigned char* data, unsigned int len);

#endif

