/*
 * interrupts.c
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#include <msp430g2553.h>
#include <legacymsp430.h>
#include "basic.h"
#include <signal.h>
#include "usci.h"

/*
 -----------------------------------------------------------------------------------------------------------------------
 Handling the ISR (Interrupt Service Routines) for main application
 -----------------------------------------------------------------------------------------------------------------------
 */

void interrupts_enable() {
	__enable_interrupt();

}

// This will be called when timer counts to TA0CCR1.
interrupt(TIMER0_A1_VECTOR) ta0_isr(void)
{
	//clear_interrupt_flag();
    TA0CCTL1 &= ~CCIFG;
}

// This will be called when timer counts to TA1CCR1.
interrupt(TIMER1_A1_VECTOR) ta1_isr(void)
{
	//clear_interrupt_flag();
    TA1CCTL1 &= ~CCIFG;
}

// This will be called when Button on BIT3 is pressed
//interrupt(PORT1_VECTOR) p1_isr(void) {
void spacer(void) {
	// disable BIT3 interrupt
    P1IE &= ~BIT3;


    //P1OUT ^= (BIT0 | BIT6);
    //__delay_cycles(90000);
    uart_tx_string((char *)"USCI INITIALIZED\n\r");

    // clear interrupt flag & re-enable BIT3 interrupt
    P1IFG &= ~BIT3;
    P1IE |= BIT3;

    // clear vector interrupt flags
    P1IFG = 0x00;

}

interrupt(PORT2_VECTOR) p2_isr(void) {

	// clear vector interrupt flags
	P2IFG = 0x00;

}

// This will be called when ADC Conversion is running
interrupt(ADC10_VECTOR) adc_isr(void) {

}

