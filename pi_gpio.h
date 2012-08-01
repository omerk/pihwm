#ifndef PI_GPIO_H
#define PI_GPIO_H

#include "pihwm.h"

extern int gpio_init(int pin, char* dir);
extern int gpio_write(int pin, char* val);
extern int gpio_read(int pin);
extern int gpio_release(int pin);

#endif

