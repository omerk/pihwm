#include <stdio.h>
#include <stdint.h>
#include "../pi_gpio.h"

// SPI pins
#define	SS	18
#define	MOSI	17
#define	MISO	21
#define	SCK	22

// reference voltage
#define VREF	3.3

uint16_t val;

double adc_volt(uint16_t val)
{
  return (val * VREF) / 4096;
}

uint16_t adc_read(uint8_t channel)
{
  uint16_t result = 0;
  uint8_t cmd = 0b11010000; // [start][sgl/diff][odd/sign][msbf][x][x][x][x]
  uint8_t i, cur, x;

  // select channel
  cmd |= (channel << 5);

  // begin transaction
  digitalWrite(SS, LOW);
  
  // write command bits
  for (i=7; i>=4; i--){
	x = cmd & (1 << i);
    digitalWrite(MOSI, x);

    // clock pulse
    digitalWrite(SCK, HIGH);
    digitalWrite(SCK, LOW);
  }

  // ignore null bit
  digitalWrite(SCK, HIGH);
  digitalWrite(SCK, LOW);

  // read conversion result
  for (i=11; i>=0; i--){
    cur = digitalRead(MISO);
    result += cur << i;
    
    // clock pulse
    digitalWrite(SCK, HIGH);
    digitalWrite(SCK, LOW);
  }
  
  // end transaction
  digitalWrite(SS, HIGH);
  
  return result;
}

int main(void)
{
	uint16_t val;
	uint8_t i;

	printf("main start\n");

	printf("direction\n");
	// set up SPI pins
	pinMode(SS, OUTPUT);
	pinMode(MOSI, OUTPUT);
	pinMode(MISO, INPUT);
	pinMode(SCK, OUTPUT);
 	delay(3000);

	printf("setup");
	// disable slave
	digitalWrite(SS, HIGH);
	digitalWrite(MOSI, LOW);
	digitalWrite(SCK, LOW);
	delay(3000);

	printf("reading...\n");

	for(;;){ 
		for(i=0; i<2; i++){
			val = adc_read(i);
			printf("Channel %d -- Raw: %4d  Volts: %f\n", i, val, adc_volt(val));
			delay(50);
		}
  
		printf("-------------------\n");
		delay(2000);
	}

	printf("main end\n");

	return 0;

}

