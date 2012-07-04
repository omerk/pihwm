#ifndef PI_GPIO_H
#define PI_GPIO_H

#define	INPUT	"in"
#define	OUTPUT	"out"
#define	IN		INPUT
#define	OUT		OUTPUT

#define	HIGH	"1"
#define	LOW		"0"
#define	ON		HIGH
#define	OFF		LOW

extern int gpio_init(int pin, char* dir);
extern int gpio_write(int pin, char* val);
extern int gpio_read(int pin);
extern int gpio_release(int pin);

#endif

