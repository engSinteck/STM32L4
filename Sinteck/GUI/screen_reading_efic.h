/*
 * screen_reading_efic.h
 *
 *  Created on: 5 de jul de 2019
 *      Author: rinaldo
 */

#ifndef EX15_XT_SRC_SCREEN_READING_EFIC_H_
#define EX15_XT_SRC_SCREEN_READING_EFIC_H_

void screen_reading_efic(void);
void btn_next_efic(void);
void btn_prev_efic(void);
void create_vumeter_eficiencia(void);
void update_vumeter_eficiencia(void);
void print_eficiencia(void);
void ButtonEventTelaReading_Efic(uint8_t event, uint8_t tipo, uint8_t id);

#endif /* EX15_XT_SRC_SCREEN_READING_EFIC_H_ */
