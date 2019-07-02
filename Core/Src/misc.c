/*
 * misc.c
 *
 *  Created on: 10 de mai de 2019
 *      Author: Rinaldo Dos Santos
 *      Sinteck Next
 */


#include "misc.h"
#include "main.h"
#include "log.h"
#include "ST7735.h"
#include "stdio.h"
#include "string.h"

extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim3;

extern uint16_t tft_pwm;

void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{
#if (LV_USE_LOG != 0)
    if(level >= LV_LOG_LEVEL) {
      //Show the log level if you want
      if(level == LV_LOG_LEVEL_TRACE)  {
         HAL_UART_Transmit(&huart1, (uint8_t*)"\nTrace:", 7, HAL_MAX_DELAY);
      }
      else if(level == LV_LOG_LEVEL_INFO) {
    	  HAL_UART_Transmit(&huart1, (uint8_t*)"\nInfo:", 6, HAL_MAX_DELAY);
      }
      else if(level == LV_LOG_LEVEL_WARN) {
    	  HAL_UART_Transmit(&huart1, (uint8_t*)"\nWarn:", 6, HAL_MAX_DELAY);
      }
      else if(level == LV_LOG_LEVEL_ERROR) {
    	  HAL_UART_Transmit(&huart1, (uint8_t*)"\nERROR:", 7, HAL_MAX_DELAY);
      }
      else {
       	  HAL_UART_Transmit(&huart1, (uint8_t*)"\nERRO:", 6, HAL_MAX_DELAY);
      }

      HAL_UART_Transmit(&huart1, (uint8_t*)dsc, strlen(dsc), HAL_MAX_DELAY);
      //You can write 'file' and 'line' too similary if required.
    }
#endif
}

void MicroDelay(uint32_t val)
{
	static uint32_t i, j;
	static uint32_t freq;
	freq = HAL_RCC_GetSysClockFreq() / 1000000;

	for (i = 0; i < val;) {
		for (j = 0; j < freq; ++j) {
			++j;
		}
		++i;
	}
}

void buzzer(uint8_t value)
{
	uint16_t audiobeep = 0;

	switch(value) {
		case 0:
			break;
	    case 1:
	        for(audiobeep = 0; audiobeep < 100; audiobeep++) {
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	        	MicroDelay(500);
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	        	MicroDelay(500);
	        }
	        break;
	    case 2:
	        for(audiobeep = 0; audiobeep < 100; audiobeep++) {
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	        	MicroDelay(250);
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	        	MicroDelay(250);
	        }
	        break;
	    case 3 :
	    	for(audiobeep = 0; audiobeep < 250; audiobeep++) {
	    		//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	    		MicroDelay(125);
	    		//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	    		MicroDelay(125);
	        }
	        break;
	    case 4 :
	    	for(audiobeep = 0; audiobeep < 250; audiobeep++) {
	    		//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	    		MicroDelay(750);
	    		//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	    		MicroDelay(750);
	        }
	        break;
	    case 5 :
	        for(audiobeep = 0; audiobeep < 250; audiobeep++) {
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	        	MicroDelay(1000);
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	        	MicroDelay(1000);
	        }
	        break;
	    case 6 :
	        for(audiobeep = 0; audiobeep < 250; audiobeep++) {
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	        	MicroDelay(750);
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	        	MicroDelay(750);
	        }
	        // Sleep for 150 milliseconds.
	        HAL_Delay(150);
	        for(audiobeep = 0; audiobeep < 250; audiobeep++) {
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	        	MicroDelay(1000);
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	        	MicroDelay(1000);
	        }
	        // Sleep for 300 milliseconds.
	        HAL_Delay(300);
	        break;
	    case 7 :
	        for(int a = 1; a <= 2; a++) {
	          for(audiobeep = 0; audiobeep < 80; audiobeep++) {
	        	 // HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	        	  MicroDelay(250);
	        	  //HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	        	  MicroDelay(250);
	          }
	          HAL_Delay(50 * a);
	        }
	        for(audiobeep = 0; audiobeep < 80; audiobeep++) {
	        	//HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	        	MicroDelay(220);
	           // HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	            MicroDelay(220);
	        }
	        break;
	}
}

void tft_backlight(uint16_t value)
{
	if(value >= 0 && value <= 100) {
		tft_pwm = value * 32;
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, tft_pwm);		// PWM_CH2 = TFT
		logI("I - TFT_PWM: %ld\n\r", tft_pwm);
	}
}

