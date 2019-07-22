/*
 * screen_rf.h
 *
 *  Created on: 4 de jul de 2019
 *      Author: rinaldo
 */

#ifndef EX15_XT_SRC_SCREEN_RF_H_
#define EX15_XT_SRC_SCREEN_RF_H_

#include "lvgl/lvgl.h"

void screen_rf(void);
void screen_RF_1(void);
void screen_RF_2(void);
void create_vumeter_swr(void);
void create_vumeter_pwr(void);
void update_vumeter_swr(uint32_t swr);
void update_vumeter_fwd(float fwd);
void update_vumeter_pwr(float fwd);
void print_pwr(float pwr);
void print_reflected(float swr);
void print_forward(float fwd);
void create_vumeter_fwd(void);
void screen_rf_1(void);
void screen_rf_2(void);
void btn_esc_rf(void);
void btn_esc_rf1(void);
void btn_esc_rf2(void);
void btn_next_rf(void);
void btn_next_rf1(void);
void btn_prev_rf2(void);
void prog_swr(uint32_t swr);
void prog_target(float fwd);
void update_style_roller_rf(uint32_t idx);
void update_style_roller_rf_1(uint32_t idx);
void ButtonEventTelaRF(uint8_t event, uint8_t tipo, uint8_t id);
void ButtonEventTelaRF_1(uint8_t event, uint8_t tipo, uint8_t id);
void ButtonEventTelaRF_2(uint8_t event, uint8_t tipo, uint8_t id);

#endif /* EX15_XT_SRC_SCREEN_RF_H_ */
