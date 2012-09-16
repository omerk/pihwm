#ifndef PI_I2C_H
#define PI_I2C_H

#include "pihwm.h"

extern int i2c_init (char *devname);
extern int i2c_select_device (unsigned int fd, unsigned int addr);
extern int i2c_write (unsigned int fd, unsigned int addr, unsigned char *data,
		      unsigned int len);
extern int i2c_read (unsigned int fd, unsigned int addr, unsigned char *data,
		     unsigned int len);

#endif
