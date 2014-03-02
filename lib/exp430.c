/*
 * exp430.c
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#include <msp430g2553.h>
#include <legacymsp430.h>

void enable_SW_2(int enable) {
	if (enable == 1) {
		P1DIR &= ~BIT3;
		P1REN |= BIT3;  // P1.3 Resistor enable (Pull up or Pull down)
		P1OUT |= BIT3;  // SET PULL_UP for P1.3
		P1IE |= BIT3; // P1.3 interrupt enable
		P1IFG &= ~BIT3; // P1.3 IFG cleared
		//P1IES ^= BIT3; // watch on EDGE and not on state
	}
	else {
		P1DIR |= BIT3;
		P1REN &= ~BIT3;  // P1.3 Resistor enable (Pull up or Pull down)
		P1OUT &= ~BIT3;  // SET PULL_UP for P1.3
		P1IE &= ~BIT3; // P1.3 interrupt enable
		P1IFG &= ~BIT3; // P1.3 IFG cleared
		//P1IES ^= BIT3; // watch on EDGE and not on state
	}

}
