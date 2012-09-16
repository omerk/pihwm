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
extern int gpio_init (int pin, char *dir);
extern int gpio_set_int (int pin, void (*isr) (int), char *mode);
extern int gpio_clear_int (int pin);
extern int gpio_edge (int pin, char *edge);
extern int gpio_valfd (int pin);
extern int gpio_write (int pin, char *val);
extern int gpio_read (int pin);
extern int gpio_release (int pin);

#endif
