/*
 * screen_frequency.h
 *
 *  Created on: 4 de jul de 2019
 *      Author: rinaldo
 */

#ifndef EX15_XT_SRC_SCREEN_FREQUENCY_H_
#define EX15_XT_SRC_SCREEN_FREQUENCY_H_

void screen_freq(void);
void create_vumeter_freq(void);
void update_vumeter(long int freq);
void prog_freq(void);
void btn_prev(void);
void update_style_roller_freq(uint32_t idx);
void ButtonEventTelaFrequencia(uint8_t event, uint8_t tipo, uint8_t id);

#endif /* EX15_XT_SRC_SCREEN_FREQUENCY_H_ */
