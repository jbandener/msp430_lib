/*
 * main.c
 *
 *  Created on: 21.12.2013
 *      Author: jbandener
 *
 *      Toggles both LEDs... used for eclipse settings test
 */


#include  <msp430g2553.h>
#include "lib/basic.h"
//#include "lib/interrupts.h"
#include <signal.h>
#include "lib/exp430.h"
#include "lib/usci.h"
#include "lib/lcd_c0802.h"
#include "lib/str_functions.h"

int counter = 0;
int advalue = 0;
int max_adc = 2500;

void main(void)
{
  stop_watchdog();
  set_clk_16MHz();




  enable_SW_2(1);
  uart_init();

  __enable_interrupt();
  adc_init_p1_4();

  __delay_cycles(5000);

  for (;;)
  {


  }
}

interrupt(PORT1_VECTOR) p1_isr(void) {

	// disable BIT3 interrupt
    P1IE &= ~BIT3;

    while(counter < max_adc) {
    	//adc_init_p1_4();
    	advalue = adc_read_p1_4();
    	uart_tx_int(advalue);
    	uart_tx_string("\n");
    	counter = counter + 1;
    }
    counter = 0;
    // clear interrupt flag & re-enable BIT3 interrupt
    P1IFG &= ~BIT3;
    P1IE |= BIT3;

    // clear vector interrupt flags
    P1IFG = 0x00;

}





