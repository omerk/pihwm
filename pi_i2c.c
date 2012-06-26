#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "pi_i2c.h"


int i2c_init(char* devname)
{
	int fd = open(devname, O_RDWR);

	if ( fd < 0 ){
		#ifdef DEBUG 
		printf("[Error] Can't open %s : %s\n", devname, strerror(errno));
		#endif
		return -1;
	} else {
		return fd;
	}
}


int i2c_select_device(unsigned int fd, unsigned int addr)
{
	if ( ioctl(fd, I2C_SLAVE, addr) < 0 ){
		#ifdef DEBUG 
		printf("[Error] Can't select device %s: %s\n", addr, strerror(errno));
		#endif
		return -1;
	} else {
		return 1;
	}
}


int i2c_write(unsigned int fd, unsigned int addr, unsigned char* data, unsigned int len)
{
	int i;

	if( i2c_select_device(fd, addr) ){
		if ( write(fd, data, len) != len ) {
			#ifdef DEBUG 
			printf("[Error] I2C write (address: 0x%X) failed: %s\n", addr, strerror(errno));
			#endif
			return -1;
		}
	} else {
		printf("[Error] Can't select I2C device at address: 0x%X, write failed.", addr);
		return -1;
	}

	return 1;
}


int i2c_read(unsigned int fd, unsigned int addr, unsigned char* data, unsigned int len)
{
	char buf[len];

	if( i2c_select_device(fd, addr) ){
		if (read(fd, buf, len) != len) {
			#ifdef DEBUG 
			printf("[Error] I2C read failed: %s\n", strerror(errno));
			#endif
			return -1;
		} else {
			memcpy(data, buf, len);
			return 1;
		}
	} else {
		#ifdef DEBUG
		printf("[Error] Can't select I2C device at address: 0x%X, write failed.", addr);
		#endif
		return -1;
	}
}

