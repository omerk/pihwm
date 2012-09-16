#ifndef PI_GPIO_H
#define PI_GPIO_H

#include "pihwm.h"


// Aliases for pseudo-Arduino compatibility
#define pinMode(pin, dir)						gpio_init(pin, dir);
#define digitalWrite(pin, val)				gpio_write(pin, val);
#define digitalRead(pin)						gpio_read(pin);
#define attachInterrupt(pin, isr, mode)	gpio_set_int(pin, isr, mode);
#define detachInterrupt(pin)					gpio_clear_int(pin);


// Function prototypes
int gpio_init(int pin, char* dir);
int gpio_set_int(int pin, void (*isr)(int), char* mode);
int gpio_clear_int(int pin);
int gpio_edge(int pin, char* edge);
int gpio_valfd(int pin);
int gpio_write(int pin, char* val);
int gpio_read(int pin);
int gpio_release(int pin);

#endif

