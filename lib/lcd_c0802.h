/*
 * lcd_c0802.h
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#ifndef LCD_C0802_H_
#define LCD_C0802_H_

#define LCD_DATA_D	P2DIR  		//direction register
#define LCD_OUTPUT	P2OUT   	//output register

// pin configuration for data and controll lines
#define LCD_D0		BIT0
#define LCD_D1		BIT1
#define LCD_D2		BIT2
#define LCD_D3		BIT3
#define LCD_RS		BIT4 		//RS Portpin
#define LCD_E		BIT5 		//Enable Portpin

// display specific configuration according to datasheet (display controller datasheet HD44780 compatible)
#define LCD_CH		0			//character height  (8px = 0 , 10px = 1) -> pollin display has 8px height

#define LCD_FIRSTLINE	0x00 	// lcd controller addr. of first char on first line
#define LCD_SECONDLINE	0x40	// lcd controller addr. of first char on second line

#define DELAY_TOGGLE 16 		// delay between enable set HIGH to enable set LOW
#define DELAY_FIXED 8000 		// default delay time between operations / commands



void lcd_setCommandMode();
void lcd_setDataMode();
void lcd_setDataPins(unsigned char data);
void lcd_toggleEnable(void);
void lcd_send4Bit(unsigned char halfbyte);
void lcd_sendByte(unsigned char fullbyte);
void lcd_sendCommand(unsigned char cmd);
void lcd_sendData(unsigned char data);
void lcd_init();
void lcd_print(char* string);
void lcd_goto(int line,int pos);
void lcd_clear();
void lcd_home();
void lcd_setConfiguration(int on,int cursor,int blink);
void lcd_softClear();


#endif /* LCD_C0802_H_ */
