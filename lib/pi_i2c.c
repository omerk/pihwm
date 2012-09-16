#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "pihwm.h"
#include "pi_i2c.h"


int i2c_init(char* devname)
{
	int fd = open(devname, O_RDWR);

	if ( fd < 0 ){
		debug("[%s] Can't open %s : %s\n", __func__, devname, strerror(errno));
		return -1;
	} else {
		return fd;
	}
}


int i2c_select_device(unsigned int fd, unsigned int addr)
{
	if ( ioctl(fd, I2C_SLAVE, addr) < 0 ){
		debug("[%s] Can't select device %s: %s\n", __func__, addr, strerror(errno));
		return -1;
	} else {
		return 1;
	}
}


int i2c_write(unsigned int fd, unsigned int addr, unsigned char* data, unsigned int len)
{

	if( i2c_select_device(fd, addr) ){
		if ( write(fd, data, len) != len ) {
			debug("[%s] I2C write (address: 0x%X) failed: %s\n", __func__, addr, strerror(errno));
			return -1;
		}
	} else {
		printf("[%s] Can't select I2C device at address: 0x%X, write failed\n", __func__, addr);
		return -1;
	}

	return 1;
}


int i2c_read(unsigned int fd, unsigned int addr, unsigned char* data, unsigned int len)
{
	char buf[len];

	if( i2c_select_device(fd, addr) ){
		if (read(fd, buf, len) != len) {
			debug("[%s]: I2C read (address: 0x%X) failed: %s\n", __func__, addr, strerror(errno));
			return -1;
		} else {
			memcpy(data, buf, len);
			return 1;
		}
	} else {
		debug("[%s] Can't select I2C device at address: 0x%X, write failed\n", __func__, addr);
		return -1;
	}
}

