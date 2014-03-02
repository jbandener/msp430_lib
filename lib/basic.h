/*
 * basic.h
 *
 *  Created on: 02.03.2014
 *      Author: jbandener
 */

#ifndef BASIC_H_
#define BASIC_H_

void stop_watchdog(void);
void set_clk_1MHz(void);
void set_clk_8MHz(void);
void set_clk_12MHz(void);
void set_clk_16MHz(void);
void set_SMCLK_div_1(void);
void set_SMCLK_div_2(void);
void set_SMCLK_div_4(void);
void set_SMCLK_div_8(void);
void init_timerA_smclk_up(int TACCR0_max);
void timerA_interrupt_enable_taccr1(int taccr1_max);
void clear_interrupt_flag(void);
void goto_powermode_LPM0(void);
void main_init(void);
void easy_TimerA_pwm_smclk_50(int period, int onTime, int smclk_div);

#endif /* BASIC_H_ */
