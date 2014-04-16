/*
 * adc.c
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#include <msp430g2553.h>
#include <legacymsp430.h>

#define ADC_DELAY 1  // for 16 MHz CLK speed and max speed on sampling / min # of averaging (max 200kps)
//#define ADC_DELAY 100  // for 1 MHz CLK speed



void adc_init_p1_4() {
	P1DIR &= ~BIT4;
	// Initialize
	ADC10CTL1 = INCH_4 + ADC10DIV_0 + ADC10SSEL1;         // Channel 4, ADC10CLK/1, ADC10CLK = MCLK
	ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON + ADC10IE;  //Vcc & Vss as reference
	ADC10AE0 |= BIT4;                         //P1.4 ADC option
}
int adc_read_p1_4() {
    /*
     --------------------------------------------------------------------
     ADC10 PIN 1.4 READ VALUE    range(0...1024) (0V...Vcc)
     --------------------------------------------------------------------
     */

    // Read value
    int value0 = 0;
    //__delay_cycles(ADC_DELAY);
    ADC10CTL0 |= ENC + ADC10SC;
    __delay_cycles(ADC_DELAY);
    value0 = ADC10MEM;
    //__delay_cycles(ADC_DELAY);

    ADC10CTL1 &= ~(INCH_4 + ADC10DIV_3);
    ADC10AE0 &= ~BIT4;
    ADC10CTL0 &= ~(ENC + ADC10SC);
    return value0;
}


void adc_init_p1_5() {
	P1DIR &= ~BIT5;
	// Initialize
	ADC10CTL1 = INCH_5 + ADC10DIV_3 ;         // Channel 5, ADC10CLK/4
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  //Vcc & Vss as reference
	ADC10AE0 |= BIT5;                         //P1.5 ADC option

}

int adc_read_p1_5() {
    /*
     --------------------------------------------------------------------
     ADC10 PIN 1.5 READ VALUE    range(0...1024) (0V...Vcc)
     --------------------------------------------------------------------
     */


    // Read value
    int value1 = 0;
    //__delay_cycles(ADC_DELAY);
    ADC10CTL0 |= ENC + ADC10SC;
    __delay_cycles(ADC_DELAY);
    value1 = ADC10MEM;
    //__delay_cycles(ADC_DELAY);

    ADC10CTL1 &= ~(INCH_5 + ADC10DIV_3);
    ADC10AE0 &= ~BIT5;
    ADC10CTL0 &= ~(ENC + ADC10SC);
    return value1;
}
