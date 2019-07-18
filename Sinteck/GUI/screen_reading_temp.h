/*
 * screen_reading_temp.h
 *
 *  Created on: 5 de jul de 2019
 *      Author: rinaldo
 */

#ifndef EX15_XT_SRC_SCREEN_READING_TEMP_H_
#define EX15_XT_SRC_SCREEN_READING_TEMP_H_

void screen_reading_temp(void);
void btn_next_temp(void);
void btn_prev_temp(void);
void print_temp_1(float temp);
void create_vumeter_temperatura(void);
void update_vumeter_temperatura(float temp);
void ButtonEventTelaReading_Temp(uint8_t event, uint8_t tipo, uint8_t id);

#endif /* EX15_XT_SRC_SCREEN_READING_TEMP_H_ */
