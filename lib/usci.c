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


/*
 * UART functionality
 *
 * */

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




/*
 * I2C functionality
 *
 * */

#define SCL BIT6
#define SDA BIT7
#define I2C_BAUD_SCALE_LOW 40  // set to approx. 200 kHz
#define I2C_BAUD_SCALE_HIGH 0  // set to 200 kHz



void i2c_master_init() {

	P1OUT |= SCL | SDA;
	P1SEL  |= SCL | SDA;					// selecting UART mode for P1.6 and P1.7 (PortA special function)
	P1SEL2 |= SCL | SDA;
	UCB0CTL1 |= UCSWRST;					// bring MSP to SW reset
	UCB0CTL0 = UCMODE_3 + UCSYNC + UCMST;	// I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;			// Use SMCLK, keep SW reset
	UCB0BR0 = I2C_BAUD_SCALE_LOW;
	UCB0BR1 = I2C_BAUD_SCALE_HIGH;
	//UCB0I2COA = 0x11;						// set a slave address for MSP
	UCB0CTL1 &= ~UCSWRST;					// Clear SW reset

	//__delay_cycles(10000);
}

/*
 * function for sending bytes to an I2C slave
 * */
void i2c_writeByte(unsigned int byteCount, unsigned char *data, unsigned char addr) {
	int counter = 0;
	UCB0I2CSA = addr;				// set slave address
	UCB0CTL1 |= UCTR + UCTXSTT ;

	while(counter < byteCount) {
		while (!(IFG2 & UCB0TXIFG));
			UCB0TXBUF = *data;
			data++;
			counter++;
	}
	while (!(IFG2 & UCB0TXIFG));
		UCB0CTL1 |= UCTXSTP;
	while(UCB0CTL1 & UCTXSTP);
}


/*
 * function for sending bytes to an I2C slave
 * */
void i2c_writeSingleByte(unsigned char data, unsigned char addr) {
	//int counter = 0;
	UCB0I2CSA = addr;				// set slave address
	UCB0CTL1 |= UCTR + UCTXSTT ;

	//while(counter < byteCount) {
		while (!(IFG2 & UCB0TXIFG));
			UCB0TXBUF = data;
	//		data++;
	//		counter++;
	//}
	while (!(IFG2 & UCB0TXIFG));
		UCB0CTL1 |= UCTXSTP;
	while(UCB0CTL1 & UCTXSTP);
}


/*
 * Function for sending a register address with up to 16Bit as an Integer. (required for EEPROM with more than 512 pages)
 * */
void i2c_writeInt(unsigned int byteCount, unsigned char *data, unsigned char addr, unsigned int register_value) {
	int counter = 0;
	UCB0I2CSA = addr;				// set slave address
	UCB0CTL1 |= UCTR + UCTXSTT ;

	while (!(IFG2 & UCB0TXIFG));
	UCB0TXBUF = register_value;
	counter++;

	while(counter < byteCount) {
		while (!(IFG2 & UCB0TXIFG));
			UCB0TXBUF = *data;
			data++;
			counter++;
	}
	while (!(IFG2 & UCB0TXIFG));
		UCB0CTL1 |= UCTXSTP;
	while(UCB0CTL1 & UCTXSTP);
}


/*
 * Function for receiving bytes from an I2C slave
 * */
unsigned int i2c_rx_counter;
unsigned int i2c_rx_max;
unsigned char *i2c_rx_data[];
unsigned int i2c_active_marker;
unsigned char *data2[8];
//unsigned char *data[];
// TODO: change name of data and make it global. Pointing to that variable might bring some issues. RX function has not yet been tested if pointer is pointing on the right variable

static int received_byte;
unsigned char i2c_receiveSingleByte(unsigned char addr) {
	received_byte=0;
	UCB0I2CSA = addr;
	UCB0CTL1 &= ~UCTR;
	UCB0CTL1 |= UCTXSTT;
	__delay_cycles(400);
	UCB0CTL1 |= UCTXSTP;
	while (!(IFG2 & UCB0RXIFG));
	received_byte = UCB0RXBUF;
	UCB0CTL1 |= UCTXSTP;
	if(!UCTXSTP){
	    	UCB0CTL1 |= UCTXSTP;
	    }
	while(UCB0CTL1 & UCTXSTP);
	//received_byte=255;
	return received_byte;

}


unsigned char *i2c_receiveByte(unsigned int byteCount, unsigned char addr) {
	unsigned char *data[byteCount];
	int counter = 0;
	*data2 = "";



	UCB0I2CSA = addr;
	UCB0CTL1 &= ~UCTR;
	UCB0CTL1 |= UCTXSTT;


	if(byteCount == 1) {						// handling if only 1 byte shall be received
		__delay_cycles(600);
		UCB0CTL1 |= UCTXSTP;
		while (!(IFG2 & UCB0RXIFG));
		data2[counter] = UCB0RXBUF;
	}
    if(byteCount != 1) {						// handling for 2 or more bytes
		while (counter < byteCount) {
			while (!(IFG2 & UCB0RXIFG));		// wait until IFG is set and data are in Buffer
			counter++;
			data2[counter-1] = UCB0RXBUF;		// write buffer to array
			__delay_cycles(100);
			if(counter+1 == byteCount) {		// send stop condition for only receiving one more byte
				UCB0CTL1 |= UCTXSTP;
			}
		}
    }

    if(!UCTXSTP){
    	UCB0CTL1 |= UCTXSTP;
    }
	while(UCB0CTL1 & UCTXSTP);




	return data2;
}



/*
 * Interrupt handler for USCI module (UART and I2C)
 * */

interrupt(USCIAB0RX_VECTOR) usci_isr(void)	// Interrupt handler for USCI
{
	if(uart_isr_rx != 0L) {
		(uart_isr_rx)(UCA0RXBUF);
	}


}

interrupt(USCIAB0TX_VECTOR) usci_isr_tx(void) {

}
