#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "../pi_i2c.h"

// Registers
#define	IODIRA	0x00
#define	IODIRB	0x01
#define	GPIOA	0x12
#define	GPIOB	0x13

// I2C Addres (A2,A1,A0 tied to ground)
#define ADDR	0x20

int main()
{
	int fd;	

	printf("main() start\n");

	// Init I2C bus
	fd = i2c_init("/dev/i2c-0");
	if ( fd < 0 ){
		return 1;
	}
	
	// Set pin directions
	i2c_write(fd, ADDR, a({IODIRA,0x00}), 2);
	i2c_write(fd, ADDR, a({IODIRB,0x00}), 2);

	// Set pin values
	i2c_write(fd, ADDR, a({GPIOA,0xFF}), 2);
	i2c_write(fd, ADDR, a({GPIOB,0xFF}), 2);

	printf("main() end\n");

	return 0;
}

