#ifndef PIHWM_C
#define PIHWM_C

// Useful constants
#define INPUT	"in"
#define OUTPUT	"out"
#define IN		INPUT
#define OUT		OUTPUT

#define HIGH	"1"
#define LOW		"0"
#define ON		HIGH
#define OFF		LOW


// Useful macros
#define a(...) (unsigned char[])__VA_ARGS__

#ifdef DEBUG
	#define debug(...) printf(__VA_ARGS__)
#else
	#define debug(...) ;
#endif


#endif

