/*
 * misc.h
 *
 *  Created on: 10 de mai de 2019
 *      Author: rinaldo
 */

#ifndef INC_MISC_H_
#define INC_MISC_H_

#include "stm32l4xx_hal.h"
#include "lvgl/lvgl.h"

void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc);
void buzzer(uint8_t value);
void MicroDelay(uint32_t val);
void print_ad(void);
void tft_backlight(uint16_t value);
uint16_t Read_Encoder(void);

#endif /* INC_MISC_H_ */
