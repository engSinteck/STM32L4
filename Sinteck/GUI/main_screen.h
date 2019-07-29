/*
 * main_screen.h
 *
 *  Created on: 3 de jul de 2019
 *      Author: rinaldo
 */

#ifndef EX15_XT_MAIN_SCREEN_H_
#define EX15_XT_MAIN_SCREEN_H_

#include <stdlib.h>
#include <unistd.h>

#define FAIL_PLLLOCK	1
#define	FAIL_RFL		2
#define FAIL_FOLD		3
#define FAIL_FOLDIPA	4
#define	FAIL_IPA		5
#define	FAIL_VIN		6
#define	FAIL_SWR		7

void main_screen(void);
void print_frequencia(long int freq);
void print_pot(float fwd, float swr);
void print_audio(u_int8_t stereo, u_int8_t audio);
void print_temperatura(float temp);
void print_status(u_int32_t falha);
void create_vumeter(void);
void print_alarme(u_int32_t falha);
void ButtonEventTelaPrincipal(uint8_t event, uint8_t tipo, uint8_t id);


#endif /* EX15_XT_MAIN_SCREEN_H_ */
