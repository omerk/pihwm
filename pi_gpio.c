#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "pihwm.h"
#include "pi_gpio.h"


int gpio_init(int pin, char* dir)
{
	FILE *file;
	char filename[35], pinStr[2];

	gpio_release(pin);

	file = fopen("/sys/class/gpio/export", "w");
	if ( file == NULL ){
		debug("[%s] Can't open file (export)\n", __func__);
		return -1;
	}

	sprintf(pinStr, "%d", pin);
	fwrite(pinStr, sizeof(char), strlen(pinStr), file);

	fclose(file);

	sprintf(filename, "/sys/class/gpio/gpio%d/direction", pin);
	file = fopen(filename, "w");
	if ( file == NULL ){
		debug("[%s] Can't open file (direction)\n", __func__);
		return -1;
	}

	fwrite(dir, sizeof(char), 2, file);

	fclose(file);


	return 1;
}


int gpio_write(int pin, char* val)
{
	char filename[35];
	FILE *file;

	sprintf(filename, "/sys/class/gpio/gpio%d/value", pin);
	file = fopen(filename, "w");
	if ( file == NULL ){
		debug("[%s] Can't open file (value): %s\n", __func__, filename);
		return -1;
	}

	fwrite(val, sizeof(char), 1, file);

	fclose(file);

	return 1;
}


int gpio_read(int pin)
{
	char filename[35], valStr[1];
	unsigned int val, frr;
	FILE *file;

	sprintf(filename, "/sys/class/gpio/gpio%d/value", pin);
	file = fopen(filename, "r");
	if ( file == NULL ){
		debug("[%s] Can't open file (value): %s\n", __func__, filename);
		return -1;
	}

	fseek(file, 0, SEEK_SET);
	if( fread(valStr, 1, 1, file) == 1 ){
		val = atoi(valStr);

		printf("valStr: %s, val: %d\n", valStr, val);

		return val;
	} else {
		debug("[%s] Can't read pin value", __func__);
		return -1;
	}

	fclose(file);
}


int gpio_release(int pin)
{
	FILE *file;
	char pinStr[3];
	
	file = fopen("/sys/class/gpio/unexport", "w");
	if ( file == NULL ){
		debug("[%s] Can't open file (unexport)\n", __func__);
		return -1;
	}

	sprintf(pinStr, "%d", pin);
	fwrite(pinStr, sizeof(char), strlen(pinStr), file);

	fclose(file);

	return 1;
}

