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
//#include "lib/exp430.h"
#include "lib/usci.h"
#include "lib/lcd_c0802.h"
#include "lib/str_functions.h"

//#include "edid.c"

 unsigned int i = 0;

 int counter = 0;
 //unsigned char data[2] = {0x30};
 unsigned char data_in[1];
 unsigned char data_out[1] = {'AD'};
 unsigned char device_addr = 0x50;
 unsigned char write_byte[2];
 unsigned int write_counter = 0;

 unsigned char received_byte;


void dumpEEPROM(void) {
	unsigned int k = 0;
	uart_tx_string("\n\r");
	while(k<256) {
		i2c_writeSingleByte(k,device_addr);
		__delay_cycles(5);
		received_byte = i2c_receiveSingleByte(device_addr);
		__delay_cycles(5);
		uart_tx_string(int2HEXcharArray(received_byte));
		uart_tx_string(" ");
		k=k+1;
	}
	__delay_cycles(5);
	uart_tx_string("\n\r");

}


void main(void)
{
  stop_watchdog();
  set_clk_8MHz();

  __enable_interrupt();

  lcd_init();
  uart_init();
  i2c_master_init();

  dumpEEPROM();


}

interrupt(PORT1_VECTOR) p1_isr(void) {

	// disable BIT3 interrupt
    P1IE &= ~BIT3;

    // clear interrupt flag & re-enable BIT3 interrupt
    P1IFG &= ~BIT3;
    P1IE |= BIT3;

    // clear vector interrupt flags
    P1IFG = 0x00;

}





