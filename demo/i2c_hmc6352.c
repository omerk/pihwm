#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "../pi_i2c.h"

// I2C Address
#define ADDR	0x21

int main()
{
	int fd;
	unsigned char r[2];
	unsigned int res, i;

	printf("main() start\n");

	// Init I2C bus
	fd = i2c_init("/dev/i2c-0");
	if ( fd < 0 ){
		return 1;
	}
	
	for(i=0; i<10; i++){

		// Request Data
		i2c_write(fd, ADDR, "A" , 1);
		sleep(0.1);
		i2c_read(fd, ADDR, r, 2);

		res = (r[0] << 8 | r[1]) / 10;

		printf("res: %d\t(res_h: 0x%02x res_l: 0x%02x)\n", res, r[0], r[1]);
		sleep(1);
	}

	printf("main() end\n");

	return 0;
}

