/*
 * screen_audio.h
 *
 *  Created on: 4 de jul de 2019
 *      Author: Rinaldo Dos Santos
 *      Sinteck Next
 */

#ifndef EX15_XT_SRC_SCREEN_AUDIO_H_
#define EX15_XT_SRC_SCREEN_AUDIO_H_

void screen_audio(void);
void btn_next(void);
void btn_next1(void);
void btn_prev1(void);
void btn_prev2(void);
void update_sel_button_Audio(uint8_t idx);
void update_sel_button_Audio_1(uint8_t idx);
void update_sel_button_Audio_2(uint8_t idx);
void ButtonEventTelaAudio(uint8_t event, uint8_t tipo, uint8_t id);
void ButtonEventTelaAudio_1(uint8_t event, uint8_t tipo, uint8_t id);
void ButtonEventTelaAudio_2(uint8_t event, uint8_t tipo, uint8_t id);

#endif /* EX15_XT_SRC_SCREEN_AUDIO_H_ */
