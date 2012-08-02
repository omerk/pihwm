#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "pi_i2c.h"

// I2C Address
#define ADDR	0x48

// Registers
#define R_CONV  0x00
#define R_CONF  0x01
#define R_LOTH  0x02
#define R_HITH  0x03

unsigned int ads1015_read(int fd, unsigned int chan)
{
	unsigned char cmd[2], res[2];

	// Set high byte for R_CONF
	switch(chan){
		case 0:
			cmd[0] = 0b01000000;
			break;
		case 1:
			cmd[0] = 0b01010000;
			break;
		case 2:
			cmd[0] = 0b01100000;
			break;
		case 3:
			cmd[0] = 0b01110000;
			break;
		default:
			// ??
		break;
	}

	// Set low byte for R_CONF
	cmd[1] = 0b10000011;

	
	// Set R_CONF
	i2c_write(fd, ADDR, a({R_CONF, cmd[0], cmd[1]}), 3);
	delay(10);

	// Point to R_CONV
	i2c_write(fd, ADDR, a({R_CONV}), 1);
	delay(10);

	// Read conversion result
	i2c_read(fd, ADDR, res, 2);

	return (unsigned int) (res[0] << 4) | (res[1] >> 4);	

}


double ads1015_volt(unsigned int val)
{
	return ((6.144/4096) * val) * 2;
}


int main()
{
	int fd;
	unsigned int res, i, j;
	double volt;

	printf("main() start\n");

	// Init I2C bus
	fd = i2c_init("/dev/i2c-0");
	if ( fd < 0 ){
		return 1;
	}

	for(i=0; i<4; i++){
		printf("Channel %d\n", i);
		for(j=0; j<5; j++){
			res = ads1015_read(fd, i);
			volt = ads1015_volt(res);

			printf("Raw: %4d\tVolt: %1.4f\n", res, volt);
			delay(1000);
		}
		printf("------\n");
	}

	printf("main() end\n");

	return 0;
}

