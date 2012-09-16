#include <stdio.h>
#include "../pi_gpio.h"

int main(void)
{
	unsigned int val, i;

	printf("main() start\n");
	
	gpio_init(17, OUTPUT);
	gpio_init(18, INPUT);

	for (i=0; i<5; i++){
		val = gpio_read(18);
		printf("Pin 18 Val: %d\n", val);

		gpio_write(17, HIGH);
		sleep(1);
		gpio_write(17, LOW);
		sleep(1);
	}
	
	gpio_release(17);
	gpio_release(18);

	printf("main() end\n");

	return 0;
}

