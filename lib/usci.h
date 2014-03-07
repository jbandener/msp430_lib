/*
 * usci.h
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#ifndef USCI_H_
#define USCI_H_


void uart_init(void);
void uart_set_interrupt(void (*interrupt)(unsigned char c));
unsigned char uart_rx_char();
void uart_tx_char(unsigned char c);
void uart_tx_string(const char *str);
void uart_tx_int(int value);


void i2c_master_init();
void i2c_writeByte(unsigned int byteCount, unsigned char *data, unsigned char addr);
unsigned char *i2c_receiveByte(unsigned int byteCount, unsigned char addr);


#endif /* USCI_H_ */
