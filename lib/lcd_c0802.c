/*
 * lcd_c0802.c
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#include "lcd_c0802.h"
#include <msp430g2553.h>

/*
 * library for Pollin LCD module C0802-04 (Part.code.:120 622)
 *
 * */



void lcd_setCommandMode() {
	LCD_OUTPUT &= ~LCD_RS;
}

void lcd_setDataMode() {
	LCD_OUTPUT |= LCD_RS;
}


void lcd_setDataPins(unsigned char data) {
	if (data & 1) {
		LCD_OUTPUT |= LCD_D0;
	}
	else {
		LCD_OUTPUT &= ~LCD_D0;
	}
	if (data & 2) {
		LCD_OUTPUT |= LCD_D1;
	}
	else {
		LCD_OUTPUT &= ~LCD_D1;
	}
	if (data & 4) {
		LCD_OUTPUT |= LCD_D2;
	}
	else {
		LCD_OUTPUT &= ~LCD_D2;
	}
	if (data & 8) {
		LCD_OUTPUT |= LCD_D3;
	}
	else {
		LCD_OUTPUT &= ~LCD_D3;
	}
}

void lcd_toggleEnable() {
	LCD_OUTPUT |= LCD_E;
	__delay_cycles(DELAY_TOGGLE);				// 2us @ 8MHz
	LCD_OUTPUT &= ~LCD_E;
}


/*
 * Send 4LSB bits to the display
 * */
void lcd_send4Bit(unsigned char halfbyte) {
	lcd_setDataPins(halfbyte);
	lcd_toggleEnable();
}

/*
 * send one byte to the display as 2x4bit (LSB first)
 * */
void lcd_sendByte(unsigned char fullbyte) {
	lcd_send4Bit(fullbyte>>4);
	lcd_send4Bit(fullbyte);
}

/*
 * send command to display
 * */
void lcd_sendCommand(unsigned char cmd) {
	lcd_setCommandMode();
	__delay_cycles(4*DELAY_FIXED);
	lcd_sendByte(cmd);
}

/*
 * send data to display
 * */
void lcd_sendData(unsigned char data) {
	lcd_setDataMode();
	__delay_cycles(4*DELAY_FIXED);
	lcd_sendByte(data);
}

void lcd_init() {
	// set port direction to output
	LCD_DATA_D |= LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3 | LCD_E | LCD_RS;
	__delay_cycles(DELAY_FIXED);

	// set display mode to 4 bit
	lcd_send4Bit(0x3); //8-Bit Mode
	__delay_cycles(DELAY_FIXED);
	lcd_send4Bit(0x3); //8-Bit Mode
	__delay_cycles(DELAY_FIXED);
	lcd_send4Bit(0x3); //8-Bit Mode
	__delay_cycles(DELAY_FIXED);
	lcd_send4Bit(0x2); //4-Bit Mode
	__delay_cycles(DELAY_FIXED);

	// send initialization commands
	unsigned char cmd = 0x20;
		cmd |= 0x8;
	if (LCD_CH) {
		cmd |= 0x4;
	}
	lcd_sendCommand(cmd);
	__delay_cycles(DELAY_FIXED);
	lcd_sendCommand(0xC);			// Display on, no cursor, no blink
	__delay_cycles(DELAY_FIXED);
	lcd_sendCommand(0x1);			// Clear Display and Cursor home
	__delay_cycles(DELAY_FIXED);

}

void lcd_print(char* string) {
	while(*string) {
		lcd_sendData(*string);
		string++;
	}
}

void lcd_goto(int line,int pos) {
	if (line==2)  {
		lcd_sendCommand(128+LCD_SECONDLINE+pos);
	}
	else {
		lcd_sendCommand(128+LCD_FIRSTLINE+pos);
	}
	__delay_cycles(DELAY_FIXED);
}

void lcd_clear() {
	lcd_sendCommand(0x1);
	__delay_cycles(DELAY_FIXED);
}

void lcd_home() {
	lcd_sendCommand(0x2);
	__delay_cycles(DELAY_FIXED);
}

void lcd_setConfiguration(int active,int cursor,int blink) {
	unsigned char cmd = 8;
	if (active == 1) {
		cmd |= 4;
	}
	if (cursor == 1) {
		cmd |= 2;
	}
	if (blink == 1) {
		cmd |= 1;
	}
	lcd_sendCommand(cmd);
}

void lcd_softClear() {
	lcd_home();
	lcd_print("        ");
	lcd_goto(2,0);
	lcd_print("        ");
	lcd_home();
}
