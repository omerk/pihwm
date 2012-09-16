CC			= gcc
CFLAGS	= -Wall -lpthread -g -DDEBUG 

MODULES	= pi_gpio pi_pwm pi_i2c
EXAMPLES = $(patsubst %.c,%,$(wildcard demo/*.c))
#EXAMPLES = demo/gpio_test demo/gpio_int

.PHONY:	clean library

all: library $(EXAMPLES)

library: $(MODULES)

$(MODULES):
	@echo Building library: $@...
	$(CC) $(CFLAGS) -c $@.c 

$(EXAMPLES): $(addsuffix .o, $(MODULES))
	@echo Building example: $@...
	$(CC) $(CFLAGS) -o $@ $@.c $^

clean:
	rm -f $(EXAMPLES) *.o
