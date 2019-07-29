/*
 * st7735.h
 *
 *  Created on: May 10, 2019
 *      Author: Rinaldo Dos Santos
 *      Sinteck Next
 */

#ifndef INC_ST7735_H_
#define INC_ST7735_H_

#include "stm32l4xx_hal.h"
#include "main.h"
#include "lvgl/lvgl.h"


#define USE_NODMA  1

extern SPI_HandleTypeDef hspi3;

/* Which SPI use */
#define _SPI_PORT hspi3 // Edit

// Screen resolution in normal orientation
#define TFT_W         128
#define TFT_H         160

// ST7735 A0 (Data/Command select) pin
#define ST7735_A0_PORT     TFT_DC_GPIO_Port
#define ST7735_A0_PIN      TFT_DC_Pin 		// GPIO_Pin_PD1    // PB4 EDIT

// ST7735 RST (Reset) pin
#define ST7735_RST_PORT    TFT_RST_GPIO_Port
#define ST7735_RST_PIN     TFT_RST_Pin    // PB6 NOT USED / connected to hardware reset

// ST7735 CS (Chip Select) pin
#define ST7735_CS_PORT     TFT_CS_GPIO_Port
#define ST7735_CS_PIN      TFT_CS_Pin 	// GPIO_Pin_7    // PB7 EDIT

// CS pin macros
#define CS_L() HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET)
#define CS_H() HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET)

// A0 pin macros
#define A0_L() HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET)
#define A0_H() HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET)

// RESET pin macros
#define RST_L() HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_RESET)
#define RST_H() HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET)

typedef enum {
	scr_normal = 0,
	scr_CW     = 1,
	scr_CCW    = 2,
	scr_180    = 3
} ScrOrientation_TypeDef;


extern uint16_t scr_width;
extern uint16_t scr_height;


void ST7735_write(uint8_t data);
void ST7735_write_color(uint16_t color);
void ST7735_Flush_2(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void ST7735_Flush_3(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

uint16_t RGB565(uint8_t R,uint8_t G,uint8_t B);

void ST7735_Init(void);
void ST7735_AddrSet(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE);
void ST7735_Orientation(ScrOrientation_TypeDef orientation);
void ST7735_Clear(uint16_t color);

void ST7735_Pixel(uint16_t X, uint16_t Y, uint16_t color);
void ST7735_HLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color);
void ST7735_VLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color);
void ST7735_Line(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t color);
void ST7735_Rect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color);
void ST7735_FillRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color);
void ST7735_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

void ST7735_PutChar5x7(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color);
void ST7735_PutStr5x7(uint8_t X, uint8_t Y, char *str, uint16_t color);

#endif /* INC_ST7735_H_ */
