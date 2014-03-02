/*
 * basic.c
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#include <msp430g2553.h>
#include <legacymsp430.h>
//#include "uart.h"
#include <string.h>
#include <stdlib.h>

/*
 --------------------------------------------------------------------------------
 Basic functions for MSP430 series uC
 -------------------------------------------------------------------------------
 */

void stop_watchdog(void) {
	WDTCTL = WDTPW + WDTHOLD;
}

void set_clk_1MHz(void) {
	DCOCTL= 0;
	BCSCTL1= CALBC1_1MHZ;
	DCOCTL= CALDCO_1MHZ;
}
void set_clk_8MHz(void) {
	DCOCTL= 0;
	BCSCTL1= CALBC1_8MHZ;
	DCOCTL= CALDCO_8MHZ;
}
void set_clk_12MHz(void) {
	DCOCTL= 0;
	BCSCTL1= CALBC1_12MHZ;
	DCOCTL= CALDCO_12MHZ;
}
void set_clk_16MHz(void) {
	DCOCTL= 0;
	BCSCTL1= CALBC1_16MHZ;
	DCOCTL= CALDCO_16MHZ;
}


/*
set the sub-system clock used by the peripheral modules to a divider by 1,2,4 or 8
*/
void set_SMCLK_div_1(void) {
	BCSCTL2 |= DIVS_0;
}
void set_SMCLK_div_2(void) {
	BCSCTL2 |= DIVS_1;
}
void set_SMCLK_div_4(void) {
	BCSCTL2 |= DIVS_2;
}
void set_SMCLK_div_8(void) {
	BCSCTL2 |= DIVS_3;
}

/*
initializes the Timer A with up-counting mode, counting to TACCR0_max
*/
void init_timerA_smclk_up(int TACCR0_max)
{
	// set TACCRO register -> count to
	TACCR0 = TACCR0_max;
	// select TimerA source SMCLK, set mode to up-counting
	TACTL = TASSEL_2 | MC_1;
}

/*
enables the TimerA interrupt with calling the interrupt when counter reaches
taccr1_max value
NOTE: interrupt Vector for msp-gcc is TIMER0_A1_VECTOR
*/
void timerA_interrupt_enable_taccr1(int taccr1_max) {
	// OUTMOD_2 = Toggles output when counts to TACCR1, Resets when counts to TACCR0
	// --> not used... only calls interrupt when count to TACCR1
	// CCIE = Interrupt when timer counts to TACCR1
	TACCTL1 = OUTMOD_2 | CCIE;
	// Interrupt called when counter reaches TACCR1
	// set up counter_limit for interrupt
	TACCR1 = taccr1_max;
    __enable_interrupt();
}

/*
clear up the interrupt flag
*/
void clear_interrupt_flag(void) {
	TACCTL1 &= ~CCIFG;
}

/*
LPM0 (shut down the CPU) but leave interrupts enable
*/
void goto_powermode_LPM0(void) {
	__bis_SR_register(CPUOFF | GIE);
}

/*
main init routine for ti-msp430 applications stopping the watchdog and selecting 1MHz as the main clk
*/
void main_init(void) {
	stop_watchdog();
	//set_clk_1MHz();
	set_clk_8MHz();
}


/*

*/
void easy_TimerA_pwm_smclk_50(int period, int onTime, int smclk_div) {
	TACCR0 = period;
	TACCR1 = onTime;

	if (smclk_div == 2) {
		set_SMCLK_div_2();
	}
	else if (smclk_div == 4) {
		set_SMCLK_div_4();
	}
	else if (smclk_div == 8) {
		set_SMCLK_div_8();
	}
	else {
		set_SMCLK_div_1();
	}

	TACTL = TASSEL_2 | MC_1;
	TACCTL1 = OUTMOD_2 | CCIE;


	//timerA_interrupt_enable_taccr1(onTime);
}


