/*
 * pwm.c
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#include <msp430g2553.h>
#include <legacymsp430.h>

#define TIMER_A0_OUTPUT_PIN BIT6;
#define TIMER_A1_OUTPUT_PIN BIT1;

void pwm_init() {

	/*
	     --------------------------------------------------------------------
	     TIMER A 0 CONFIGURATION
	     --------------------------------------------------------------------
	     */

	    // set TA0CCRO register -> count to
		TA0CCR0 = 100;
		// select TimerA0 source SMCLK, set mode to up-counting
		TA0CTL = TASSEL_2 | MC_1;
	    // set output to Port P1.6
	    P1SEL |= TIMER_A0_OUTPUT_PIN;
	    // OUTMOD_2 = Toggles output when counts to TA0CCR1, Resets when counts to TA0CCR0
		// --> not used... only calls interrupt when count to TA0CCR1
		// CCIE = Interrupt when timer counts to TA0CCR1
		TA0CCTL1 = OUTMOD_7 | CCIE;
		// Interrupt called when counter reaches TA0CCR1
		// set up counter_limit for interrupt
		TA0CCR1 = 0;



	    /*
	     --------------------------------------------------------------------
	     TIMER A 1 CONFIGURATION
	     --------------------------------------------------------------------
	     */

	    TA1CCR0 = 100;
		// select TimerA1 source SMCLK, set mode to up-counting
		TA1CTL = TASSEL_2 | MC_1;
	    // set output to Port P2.1
	    P2SEL |= TIMER_A1_OUTPUT_PIN;
	    // OUTMOD_2 = Toggles output when counts to TA1CCR1, Resets when counts to TA1CCR0
		// --> not used... only calls interrupt when count to TACCR1
		// CCIE = Interrupt when timer counts to TACCR1
		TA1CCTL1 = OUTMOD_7 | CCIE;
		// Interrupt called when counter reaches TA1CCR1
		// set up counter_limit for interrupt
		TA1CCR1 = 0;


}


void set_pwm_a0(int percent) {
    if(percent == 100) {
        TA0CCR1 = 100;
    }
    else {
        TA0CCR1 = percent%100;
    }
}

void set_pwm_a1(int percent) {
    if(percent == 100) {
        TA1CCR1 = 100;
    }
    else {
        TA1CCR1 = percent%100;
    }
}
