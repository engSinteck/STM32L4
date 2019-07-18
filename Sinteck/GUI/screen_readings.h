/*
 * screen_readings.h
 *
 *  Created on: 4 de jul de 2019
 *      Author: rinaldo
 */

#ifndef EX15_XT_SRC_SCREEN_READINGS_H_
#define EX15_XT_SRC_SCREEN_READINGS_H_

void screen_readings(void);
void create_vumeter_swr_1(void);
void create_vumeter_fwd_1(void);
void update_vumeter_swr_1(float swr);
void update_vumeter_fwd_1(float fwd);
void btn_next_r(void);
void ButtonEventTelaReadings(uint8_t event, uint8_t tipo, uint8_t id);

#endif /* EX15_XT_SRC_SCREEN_READINGS_H_ */
