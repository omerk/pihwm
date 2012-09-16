#include <stdio.h>
#include <unistd.h>
#include "../pi_gpio.h"


#define	INT_PIN	18


void my_isr(int pin)
{
	printf("my_isr fired for pin %d\n", pin);
}


int main(int argc, char **argv, char **envp)
{
	printf("main start\n");
	
	// Init Pin
	gpio_init(INT_PIN, INPUT);

	// Init interrupt
	gpio_set_int(INT_PIN, my_isr, "rising");

	for(;;){
		printf("doing nothing, waiting for an interrupt.\n");
		sleep(2);
	}	
	
	// If we didn't have for(;;);
	gpio_release(INT_PIN);
	printf("main end\n");
	return 0;
}

