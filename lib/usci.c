/*
 * usci.c
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#include <msp430.h>
#include <legacymsp430.h>

// defines for UART mode
#define RX	BIT1
#define TX	BIT2

// selections: 	9600 BAUD: 104/0 @ 1MHz CPU clk
//
//#define BAUD_RATE_SELECTION_0 104;
//#define BAUD_RATE_SELECTION_1 0;

#define BAUD_RATE 9600;


void calculate_baud_rate_selection_9600(void) {
	/*
	 * calculation for UCA0BR0 and UCA0BR1: CLK_freq / baud_rate = DEVIDER, UCA0BR0 = 8 LSB of DEVIDER, UCA0BR1 = 8 MSB of DEVIDER
	 * */
	if (BCSCTL1 == CALBC1_1MHZ) {
		UCA0BR0 = 104;     						// set the A0 Baud rate control register 0
		UCA0BR1 = 0;   					       	// set the A0 Baud rate control register 1
	}
	else if (BCSCTL1 == CALBC1_8MHZ) {
		UCA0BR0 = 65;     						// set the A0 Baud rate control register 0
		UCA0BR1 = 3;   					       	// set the A0 Baud rate control register 1
	}
	else if (BCSCTL1 == CALBC1_12MHZ) {
		UCA0BR0 = 226;     						// set the A0 Baud rate control register 0
		UCA0BR1 = 4;    				       	// set the A0 Baud rate control register 1
	}
	else if (BCSCTL1 == CALBC1_16MHZ) {
		UCA0BR0 = 130;     						// set the A0 Baud rate control register 0
		UCA0BR1 = 6;    				       	// set the A0 Baud rate control register 1
	}
}

void uart_set_UCA0BRx(void) {
	int rate = BAUD_RATE;
	if (rate == 9600) {
		calculate_baud_rate_selection_9600();
	}
}


//void (*uart_rx_isr_ptr)(unsigned char c);
void (*uart_isr_rx)(unsigned char c);

void uart_init(void)
{
	uart_set_interrupt(0L);

	UCA0CTL1 |= UCSWRST;						// set USCI software reset (according to USCI initialization procedure description)
	P1SEL  |= RX | TX;						  	// selecting UART mode for P1.1 and P1.2 (PortA special function)
  	P1SEL2 |= RX | TX;							// selecting UART mode for P1.1 and P1.2 (PortA special function)
  	UCA0CTL1 |= UCSSEL_2;                     	// USCI A0 clock source selection -> SMCLK
  	uart_set_UCA0BRx();							// set the UCA0BRx corresponding to the selected SMCLK speed
// 	UCA0BR0 = BAUD_RATE_SELECTION_0;//104;     	// set the A0 Baud rate control register 0 -> 1MHz 9600
// 	UCA0BR1 = BAUD_RATE_SELECTION_1;          	// set the A0 Baud rate control register 1 -> 1MHz 9600
  	UCA0MCTL = UCBRS0;                        	// set the modulation control register -> USCI Second Stage Modulation Select 0
  	UCA0CTL1 &= ~UCSWRST;                     	// clear USCI software reset
  	IE2 |= UCA0RXIE;                          	// enable interrupt vector for UART mode during receive
}


void uart_set_interrupt(void (*interrupt)(unsigned char c))
{
	uart_isr_rx = interrupt;
}


unsigned char uart_rx_char(void)
{
    while (!(IFG2 & UCA0RXIFG));                	// wait for buffer to be ready
	return UCA0RXBUF;
}

void uart_tx_char(unsigned char c)
{
	while (!(IFG2 & UCA0TXIFG));              	// wait for buffer to be ready
  	UCA0TXBUF = c;                    		    // write character to TX buffer
}

void uart_tx_string(const char *str)
{
     while(*str) uart_tx_char(*str++);
}

void uart_tx_int(int value) {
    char str[10];
    itoa(value, str, 10);
    //strcat(str,"\n\r");					// adding a line break after writing each string
    uart_tx_string(str);
}


interrupt(USCIAB0RX_VECTOR) usci_isr(void)	// Interrupt handler for USCI
{
	if(uart_isr_rx != 0L) {
		(uart_isr_rx)(UCA0RXBUF);
	}
}
