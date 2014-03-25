/*
 * i2c_display_uart.c
 *
 *  Created on: 24.03.2014
 *      Author: jbandener
 *		
 *      reads a register from an I2C eeprom and prints the value on a Display and on UART
 */


#include  <msp430g2553.h>
#include "lib/basic.h"
#include <signal.h>
#include "lib/exp430.h"
#include "lib/usci.h"
#include "lib/lcd_c0802.h"
#include "lib/str_functions.h"


 unsigned int i = 0;

 int counter = 0;
 unsigned char data_in[1];
 unsigned char data_out[1] = {'AD'};

void main(void)
{
  stop_watchdog();
  set_clk_8MHz();


  interrupts_enable();
  lcd_init();

  uart_init();
  uart_tx_string("Register Value:");

  unsigned char i2creceive[8];
  i2c_master_init();
  unsigned char data[1] = {0x6F};
  lcd_goto(1,0);
  lcd_print("R: ");
  lcd_print(int2HEXcharArray(*data));
  lcd_goto(2,0);
  lcd_print("V: ");
  i2c_writeByte(1,data,0x50);
  __delay_cycles(300);  // delay some time to get a better view on the signals on the scope
  unsigned char rx_byte = i2c_receiveSingleByte(0x50);
  __delay_cycles(300);
  uart_tx_string(int2HEXcharArray(rx_byte));
  lcd_print(int2HEXcharArray(rx_byte));

  __delay_cycles(300);
  uart_tx_string("\n");


  for (;;)
  {

  }
}






