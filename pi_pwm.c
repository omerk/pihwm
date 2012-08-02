
// Based (very) heavily on Gert Jan van Loo's example code.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include "pi_pwm.h"


int  mem_fd;
char *clk_mem,  *clk_map;
char *gpio_mem, *gpio_map;
char *pwm_mem,  *pwm_map;

// I/O access
volatile unsigned *gpio;
volatile unsigned *pwm;
volatile unsigned *clk;


// Set up memory regions to access the peripherals.
// This is a bit of 'magic' which you should not touch.
// It it also the part of the code which makes that
// you have to use 'sudo' to run this program.
//
void setup_io()
{

   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("Can't open /dev/mem\n");
      printf("Did you forgot to use 'sudo .. ?'\n");
      exit (-1);
   }

   /*
    * mmap clock
    */
   if ((clk_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }
   if ((unsigned long)clk_mem % PAGE_SIZE)
     clk_mem += PAGE_SIZE - ((unsigned long)clk_mem % PAGE_SIZE);

   clk_map = (unsigned char *)mmap(
      (caddr_t)clk_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      CLOCK_BASE
   );

   if ((long)clk_map < 0) {
      printf("clk mmap error %d\n", (int)clk_map);
      exit (-1);
   }
   clk = (volatile unsigned *)clk_map;


   /*
    * mmap GPIO
    */
   if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }
   if ((unsigned long)gpio_mem % PAGE_SIZE)
     gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);

   gpio_map = (unsigned char *)mmap(
      (caddr_t)gpio_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      GPIO_BASE
   );

   if ((long)gpio_map < 0) {
      printf("gpio mmap error %d\n", (int)gpio_map);
      exit (-1);
   }
   gpio = (volatile unsigned *)gpio_map;

   /*
    * mmap PWM
    */
   if ((pwm_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }
   if ((unsigned long)pwm_mem % PAGE_SIZE)
     pwm_mem += PAGE_SIZE - ((unsigned long)pwm_mem % PAGE_SIZE);

   pwm_map = (unsigned char *)mmap(
      (caddr_t)pwm_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      PWM_BASE
   );

   if ((long)pwm_map < 0) {
      printf("pwm mmap error %d\n", (int)pwm_map);
      exit (-1);
   }
   pwm = (volatile unsigned *)pwm_map;


   // GPIO18= PWM channel-A       Funct. 5
   // Setup GPIO18 as PWM output
   INP_GPIO(18);  SET_GPIO_ALT(18,5);

} // setup_io



// Undo what we did above
//
void restore_io()
{
  munmap(pwm_map,BLOCK_SIZE);
  munmap(gpio_map,BLOCK_SIZE);
  munmap(clk_map,BLOCK_SIZE);
} // restore_io


//
// Setup the Pulse Width Modulator
// It needs a clock and needs to be off
//
void setup_pwm()
{
   // Derive PWM clock direct from X-tal
   // thus any system auto-slow-down-clock-to-save-power does not effect it
   // The values below depends on the X-tal frequency!
   PWMCLK_DIV  = 0x5A000000 | (32<<12); // set pwm div to 32 (19.2/3 = 600KHz)
   PWMCLK_CNTL = 0x5A000011; // Source=osc and enable

   // Make sure it is off and that the B driver (GPIO4) is low
   GPIO_CLR0 = 1<<4; // Set GPIO 4 LOW
   PWM_CONTROL = 0; usleep(100); 

   // I use 1024 steps for the PWM
   // (Just a nice value which I happen to like)
   PWM0_RANGE = 0x400;  usleep(100);

} // setup_pwm


// Set PWM value
// This routine does not wait for the value to arrive
// If a new value comes in before it is picked up by the chip
// it will definitely be too fast for the motor to respond to it
//
void set_pwm(int v)
{ // make sure value is in safe range
  if (v<0) v=0;
  if (v>0x400) v=0x400;
  PWM0_DATA = v;
} // set_pwm


// Force PWM value update
// This routine makes sure the new value goes in.
// This is done by dis-abling the PWM, write the value
// and enable it again. This routine is weak as it
// uses a delay which is tested (but not guaranteed)
// Controls channel 0 only.
//
void force_pwm(int v,int mode)
{
  // disable
  PWM_CONTROL  = 0;
  // wait for this command to get to the PWM clock domain
  // that depends on PWN clock speed
  // unfortunately there is no way to know when this has happened :-(
  usleep(100);
  // make sure value is in safe range
  if (v<0) v=0;
  if (v>0x400) v=0x400;
  PWM0_DATA = v;
  usleep(100);

  PWM_CONTROL  = mode;
  usleep(100);
} // force_pwm


// Turn off PWM
void pwm_off()
{
  GPIO_CLR0 = 1<<4;
  force_pwm(0,0);
}

