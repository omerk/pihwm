/* pi_pwm.h -- Pulse width modulation library headers.

   Copyright (C) 2012 Omer Kilic
   Copyright (C) 2012 Embecosm Limited

   Contributor Omer Kilic <omer@kilic.name>
   Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

   This file is part of pihwm.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef PI_PWM_H
#define PI_PWM_H


#define BCM2708_PERI_BASE        0x20000000
#define CLOCK_BASE               (BCM2708_PERI_BASE + 0x101000)	/* Clocks */
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000)	/* GPIO   */
#define PWM_BASE                 (BCM2708_PERI_BASE + 0x20C000)	/* PWM    */

/* PWM Registers */
#define GPIO_SET0   *(gpio+7)	/* Set GPIO high bits 0-31 */
#define GPIO_SET1   *(gpio+8)	/* Set GPIO high bits 32-53 */

#define GPIO_CLR0   *(gpio+10)	/* Set GPIO low bits 0-31 */
#define GPIO_CLR1   *(gpio+11)	/* Set GPIO low bits 32-53 */

#define PWMCLK_CNTL  *(clk+40)
#define PWMCLK_DIV   *(clk+41)

#define PWM_CONTROL *pwm
#define PWM_STATUS  *(pwm+1)
#define PWM0_RANGE  *(pwm+4)
#define PWM1_RANGE  *(pwm+8)
#define PWM0_DATA   *(pwm+5)
#define PWM1_DATA   *(pwm+9)

/* PWM Control register bits */
#define PWM1_MS_MODE    0x8000	/* Run in MS mode */
#define PWM1_USEFIFO    0x2000	/* Data from FIFO */
#define PWM1_REVPOLAR   0x1000	/* Reverse polarity */
#define PWM1_OFFSTATE   0x0800	/* Ouput Off state */
#define PWM1_REPEATFF   0x0400	/* Repeat last value if FIFO empty */
#define PWM1_SERIAL     0x0200	/* Run in serial mode */
#define PWM1_ENABLE     0x0100	/* Channel Enable */

#define PWM0_MS_MODE    0x0080	/* Run in MS mode */
#define PWM0_USEFIFO    0x0020	/* Data from FIFO */
#define PWM0_REVPOLAR   0x0010	/* Reverse polarity */
#define PWM0_OFFSTATE   0x0008	/* Ouput Off state */
#define PWM0_REPEATFF   0x0004	/* Repeat last value if FIFO empty */
#define PWM0_SERIAL     0x0002	/* Run in serial mode */
#define PWM0_ENABLE     0x0001	/* Channel Enable */

#define PWM_CLRFIFO     0x0040	/* Clear FIFO (Self clearing bit) */

/* PWM status bits I need */
#define PWMS_BUSERR     0x0100	/* Register access was too fast */
/* (Write to clear it) */


#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)


/* GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or
   SET_GPIO_ALT(x,y) */
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET0     *(gpio+7)		/* Set GPIO high bits 0-31 */
#define GPIO_SET1     *(gpio+8)		/* Set GPIO high bits 32-53 */

#define GPIO_CLR0     *(gpio+10)	/* Set GPIO low bits 0-31 */
#define GPIO_CLR1     *(gpio+11)	/* Set GPIO low bits 32-53 */
#define GPIO_PULL     *(gpio+37)	/* Pull up/pull down */
#define GPIO_PULLCLK0 *(gpio+38)	/* Pull up/pull down clock */


/* I/O access */
extern volatile unsigned *gpio;
extern volatile unsigned *pwm;
extern volatile unsigned *clk;


extern void setup_io ();
extern void restore_io ();
extern void setup_pwm ();
extern void set_pwm (int v);
extern void force_pwm (int v, int mode);
extern void pwm_off ();

#endif
