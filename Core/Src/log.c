/*
 * log.c
 *
 *  Created on: 4 de mai de 2019
 *      Author: Rinaldo Dos Santos
 */

#include "log.h"

extern UART_HandleTypeDef huart2;

#define PRINT_BUFFER_SIZE 255

/** Custom printf function in order to use HAL_UART_Transmit()
 * @param *fmt String to print
 * @param argp Parameters list
 */
void HAL_printf_valist(const char *fmt, va_list argp)
{
  char string[PRINT_BUFFER_SIZE];

  if (vsprintf(string, fmt, argp) > 0) {
    HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string), HAL_MAX_DELAY); // send message via UART
  } else {
	HAL_UART_Transmit(&huart2, (uint8_t*)"E - Print\n", 14, HAL_MAX_DELAY);
  }
  HAL_Delay(10);
}

/** Custom printf function, only translate to va_list arg HAL_UART.
 * @param *fmt String to print
 * @param ... Data
 */
void HAL_printf(const char *fmt, ...)
{
  va_list argp;

  va_start(argp, fmt);
  HAL_printf_valist(fmt, argp);
  va_end(argp);
}

/** Generic LOG procedure
 * @param Log level
 * @param *fmt String to print
 * @param argp Parameters list
 */
void logUSB(const char *fmt, va_list argp)
{
	//HAL_printf("%d - ", level);
	HAL_printf_valist(fmt, argp);
}

/** LOG procedure - Info
 * @param *fmt String to print
 * @param ... Parameters list
 */
void logI(const char* fmt, ...)
{
	va_list argp;

	va_start(argp, fmt);
	logUSB(fmt, argp);
	va_end(argp);
}

/** LOG procedure - Error
 * @param *fmt String to print
 * @param .. Parameters list
 */
void logE(const char* fmt, ...)
{
  va_list argp;

  va_start(argp, fmt);
  logUSB(fmt, argp);
  va_end(argp);
}


char* concat(int count, ...)
{
    va_list ap;
    int i;

    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}
