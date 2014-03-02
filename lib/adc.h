/*
 * adc.h
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#ifndef ADC_H_
#define ADC_H_


void adc_init_p1_4();
void adc_init_p1_5();

int adc_read_p1_4();            // read ADC value (0...1024) on PIN 1.4
int adc_read_p1_5();            // read ADC value (0...1024) on PIN 1.5


#endif /* ADC_H_ */
