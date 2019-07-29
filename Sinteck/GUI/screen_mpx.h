/*
 * screen_mpx.h
 *
 *  Created on: 5 de jul de 2019
 *      Author: rinaldo
 */

#ifndef EX15_XT_SRC_SCREEN_MPX_H_
#define EX15_XT_SRC_SCREEN_MPX_H_

void screen_reading_mpx(void);
void create_style_vumeter(void);
void create_vumeter_m(void);
void create_vumeter_r(void);
void create_vumeter_l(void);
void btn_next_audio(void);
void ButtonEventTelaMpx(uint8_t event, uint8_t tipo, uint8_t id);

#endif /* EX15_XT_SRC_SCREEN_MPX_H_ */
