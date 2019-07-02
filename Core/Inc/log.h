/*
 * log.h
 *
 *  Created on: 4 de mai de 2019
 *      Author: rinaldo
 */

#ifndef INC_LOG_H_
#define INC_LOG_H_

#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void HAL_printf_valist(const char *fmt, va_list argp);
void HAL_printf(const char *fmt, ...);
void logUSB(const char *fmt, va_list argp);
void logI(const char* fmt, ...);
void logE(const char* fmt, ...);
char* concat(int count, ...);

#endif /* INC_LOG_H_ */
