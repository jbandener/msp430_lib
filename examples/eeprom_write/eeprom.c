/*
 * eeprom.c
 *
 *  Created on: 25.03.2014
 *      Author: jbandener
 *
 *      Test application for accessing a EEPROM via I2C. 
 * 		This application will receive 2kbytes via UART and write them to the EEPROM on Address 0x50
 * 		Register values are incremented from 0x00 to 0xFF
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

void main(void)
{
  stop_watchdog();
  set_clk_8MHz();

  //interrupts_enable();

  lcd_init();
  uart_init();


  /*
   * Read a regiser (data[1]) and print register and value on the display
   * */
  unsigned char i2creceive[8];
  i2c_master_init();
  unsigned char data[1] = {0x6F};
  lcd_goto(1,0);
  lcd_print("R: ");
  lcd_print(int2HEXcharArray(*data));
  lcd_goto(2,0);
  lcd_print("V: ");
  i2c_writeByte(1,data,0x50);
  __delay_cycles(300);  // delay some time to get a better view on the signals
  unsigned char rx_byte = i2c_receiveSingleByte(0x50);
  __delay_cycles(300);
  lcd_print(int2HEXcharArray(rx_byte));


  /*
   * receive 256 register values via UART and write them to the EEPROM (incrementing address from 0x00 to 0xFF)
   * */

  ///*
  unsigned char write_byte[2];
  unsigned int write_counter = 0;

  while(write_counter < 255) {
	  lcd_goto(2,0);
	  write_byte[0] = write_counter;
	  write_byte[1] = uart_rx_char();
	  lcd_print(int2HEXcharArray(write_byte[1]));
	  i2c_writeByte(2,write_byte,0x50);
	  write_counter = write_counter + 1;
  }
  //*/


  /*
   * Read out a register value and print it on the display to verify the write process
   * */
  lcd_goto(2,0);
  lcd_print("V: ");
  i2c_writeByte(1,data,0x50);
  rx_byte = i2c_receiveSingleByte(0x50);
  __delay_cycles(300);
  lcd_print(int2HEXcharArray(rx_byte));
  __delay_cycles(300);

  for (;;)
  {

  }
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





