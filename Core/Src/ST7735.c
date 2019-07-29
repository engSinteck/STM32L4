/*
 * st7735.c
 *
 *  Created on: May 10, 2019
 *      Author: Rinaldo Dos Santos
 *      Sinteck Next
 */

#include "string.h" // For memcpy
#include "stdio.h"
#include "ST7735.h"
#include "font5x7.h"
#include "lvgl/lvgl.h"
#include "log.h"

extern char buffer[];

uint16_t scr_width;
uint16_t scr_height;
//uint8_t buf_tft[3300] = {0};
static lv_disp_drv_t * disp_p;

void ST7735_write(uint8_t data)
{
	HAL_SPI_Transmit(&_SPI_PORT, (uint8_t *)&data, 1, HAL_MAX_DELAY);
}

void ST7735_write_color(uint16_t color)
{
	uint8_t tmp[2];

	tmp[0] = color >> 8;
	tmp[1] = color;
	HAL_SPI_Transmit(&_SPI_PORT, (uint8_t *)&tmp[0], 2, HAL_MAX_DELAY);
}

void ST7735_cmd(uint8_t cmd) {
	A0_L();
	ST7735_write(cmd);
}

void ST7735_data(uint8_t data) {
	A0_H();
	ST7735_write(data);
}

uint16_t RGB565(uint8_t R,uint8_t G,uint8_t B) {
	return ((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3);
}

void ST7735_Init(void) {
	// Reset display
	CS_H();
	RST_H();
	HAL_Delay(5);
	RST_L();
	HAL_Delay(5);
	RST_H();
	HAL_Delay(5);
	CS_L();

	ST7735_cmd(0x11); 	// Sleep out & booster on
	HAL_Delay(25); 		// Datasheet says what display wakes about 120ms (may be much faster actually)

	ST7735_cmd(0xb1);   // In normal mode (full colors):
	A0_H();
	ST7735_write(0x05); //   RTNA set 1-line period: RTNA2, RTNA0
	ST7735_write(0x3c); //   Front porch: FPA5,FPA4,FPA3,FPA2
	ST7735_write(0x3c); //   Back porch: BPA5,BPA4,BPA3,BPA2

	ST7735_cmd(0xb2);   // In idle mode (8-colors):
	A0_H();
	ST7735_write(0x05); //   RTNB set 1-line period: RTNAB, RTNB0
	ST7735_write(0x3c); //   Front porch: FPB5,FPB4,FPB3,FPB2
	ST7735_write(0x3c); //   Back porch: BPB5,BPB4,BPB3,BPB2

	ST7735_cmd(0xb3);   // In partial mode + full colors:
	A0_H();
	ST7735_write(0x05); //   RTNC set 1-line period: RTNC2, RTNC0
	ST7735_write(0x3c); //   Front porch: FPC5,FPC4,FPC3,FPC2
	ST7735_write(0x3c); //   Back porch: BPC5,BPC4,BPC3,BPC2
	ST7735_write(0x05); //   RTND set 1-line period: RTND2, RTND0
	ST7735_write(0x3c); //   Front porch: FPD5,FPD4,FPD3,FPD2
	ST7735_write(0x3c); //   Back porch: BPD5,BPD4,BPD3,BPD2

	ST7735_cmd(0xB4);   // Display dot inversion control:
	ST7735_data(0x03);  //   NLB,NLC

	ST7735_cmd(0x3a);   // Interface pixel format
//	ST7735_data(0x03);  // 12-bit/pixel RGB 4-4-4 (4k colors)
	ST7735_data(0x05);  // 16-bit/pixel RGB 5-6-5 (65k colors)
//	ST7735_data(0x06);  // 18-bit/pixel RGB 6-6-6 (256k colors)

//	ST7735_cmd(0x36);   // Memory data access control:
						//   MY MX MV ML RGB MH - -
//	ST7735_data(0x00);  //   Normal: Top to Bottom; Left to Right; RGB
//	ST7735_data(0x80);  //   Y-Mirror: Bottom to top; Left to Right; RGB
//	ST7735_data(0x40);  //   X-Mirror: Top to Bottom; Right to Left; RGB
//	ST7735_data(0xc0);  //   X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
//	ST7735_data(0x20);  //   X-Y Exchange: X and Y changed positions
//	ST7735_data(0xA0);  //   X-Y Exchange,Y-Mirror
//	ST7735_data(0x60);  //   X-Y Exchange,X-Mirror
//	ST7735_data(0xE0);  //   X-Y Exchange,X-Mirror,Y-Mirror

	ST7735_cmd(0x20);   // Display inversion off
//	ST7735_cmd(0x21);   // Display inversion on

	ST7735_cmd(0x13);   // Partial mode off

	ST7735_cmd(0x26);   // Gamma curve set:
	ST7735_data(0x01);  //   Gamma curve 1 (G2.2) or (G1.0)
//	ST7735_data(0x02);  //   Gamma curve 2 (G1.8) or (G2.5)
//	ST7735_data(0x04);  //   Gamma curve 3 (G2.5) or (G2.2)
//	ST7735_data(0x08);  //   Gamma curve 4 (G1.0) or (G1.8)

	ST7735_cmd(0x29);   // Display on

	CS_H();

	ST7735_Orientation(scr_CCW);
}

void ST7735_Orientation(ScrOrientation_TypeDef orientation) {
	CS_L();
	ST7735_cmd(0x36); // Memory data access control:
	switch(orientation) {
	case scr_CW:
		scr_width  = TFT_H;
		scr_height = TFT_W;
		ST7735_data(0xA0); // X-Y Exchange,Y-Mirror
		break;
	case scr_CCW:
		scr_width  = TFT_H;
		scr_height = TFT_W;
		ST7735_data(0x60); // X-Y Exchange,X-Mirror
		break;
	case scr_180:
		scr_width  = TFT_W;
		scr_height = TFT_H;
		ST7735_data(0xc0); // X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
		break;
	default:
		scr_width  = TFT_W;
		scr_height = TFT_H;
		ST7735_data(0x00); // Normal: Top to Bottom; Left to Right; RGB
		break;
	}
	CS_H();
}

void ST7735_AddrSet(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE) {
	ST7735_cmd(0x2a); // Column address set
	A0_H();
	ST7735_write(XS >> 8);
	ST7735_write(XS);
	ST7735_write(XE >> 8);
	ST7735_write(XE);

	ST7735_cmd(0x2b); // Row address set
	A0_H();
	ST7735_write(YS >> 8);
	ST7735_write(YS);
	ST7735_write(YE >> 8);
	ST7735_write(YE);

	ST7735_cmd(0x2c); // Memory write
}

void ST7735_Clear(uint16_t color) {
	uint16_t i;
	uint8_t  CH,CL;

	CH = color >> 8;
	CL = (uint8_t)color;

	CS_L();
	ST7735_AddrSet(0, 0, scr_width - 1, scr_height - 1);
	A0_H();
	for (i = 0; i < scr_width * scr_height; i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_Flush_3(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
	uint16_t size;
	uint8_t tmp[2];
	uint16_t teste = 0;

    size = ( ((area->x2 - area->x1) + 1)  * ((area->y2 - area->y1) + 1) );

	CS_L();
    ST7735_cmd(0x2a); // Column address set
	A0_H();
	ST7735_write(area->x1 >> 8);
	ST7735_write(area->x1);
	ST7735_write(area->x2 >> 8);
	ST7735_write(area->x2);

	ST7735_cmd(0x2b); // Row address set
	A0_H();
	ST7735_write(area->y1 >> 8);
	ST7735_write(area->y1);
	ST7735_write(area->y2 >> 8);
	ST7735_write(area->y2);

	ST7735_cmd(0x2c); // Memory write
	A0_H();
	//
#if USE_NODMA
	for(uint16_t x = 0; x <= size-1; x++) {
		tmp[0] = color_p->full >> 8;
		tmp[1] = color_p->full;
		HAL_SPI_Transmit(&_SPI_PORT, (uint8_t *)&tmp[0], 2, HAL_MAX_DELAY);
		color_p++;
		teste++;
	}
#else
    /*SPI transmit data with DMA*/
	disp_p = disp_drv;
	if ( HAL_SPI_Transmit_DMA(&_SPI_PORT, (uint8_t *)color_p, size * 2) != HAL_OK)
    {
        //while(1);	/*Halt on error*/
    }
#endif

//	for(uint16_t x = 0; x <= size-1; x++) {
//		buf_tft[(x*2) + 0] = color_p->full >> 8;
//		buf_tft[(x*2) + 1] = color_p->full;
//		color_p++;
//		teste++;
//	}
//	HAL_SPI_Transmit(&_SPI_PORT, (uint8_t *)&buf_tft[0], (size-1)*2, HAL_MAX_DELAY);

	CS_H();

//	sprintf(buffer, "ST7735-Flush3 X1: %d X2: %d Y1: %d Y2: %d Size: %d Teste: %d\n", area->x1, area->x2, area->y1, area->y2, size, teste);
//    logI(buffer);
#if USE_NODMA
	lv_disp_flush_ready(disp_drv);                  /* Tell you are ready with the flushing*/
#endif
}

void ST7735_Flush_2(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
	uint16_t size;
	uint16_t teste = 0;
	uint8_t tmp[2];

    size = ( ((area->x2 - area->x1) + 1)  * ((area->y2 - area->y1) + 1) );

	CS_L();
    ST7735_cmd(0x2a); // Column address set
	A0_H();
	ST7735_write(area->x1 >> 8);
	ST7735_write(area->x1);
	ST7735_write(area->x2 >> 8);
	ST7735_write(area->x2);

	ST7735_cmd(0x2b); // Row address set
	A0_H();
	ST7735_write(area->y1 >> 8);
	ST7735_write(area->y1);
	ST7735_write(area->y2 >> 8);
	ST7735_write(area->y2);

	ST7735_cmd(0x2c); // Memory write
	A0_H();
	//
	for(uint16_t x = 0; x <= size-1; x++) {
		tmp[0] = color_p->full >> 8;
		tmp[1] = color_p->full;
		HAL_SPI_Transmit(&_SPI_PORT, (uint8_t *)&tmp[0], 2, HAL_MAX_DELAY);
		color_p++;
		teste++;
	}
	CS_H();

//	sprintf(buffer, "ST7735-Flush2 X1: %d X2: %d Y1: %d Y2: %d Size: %d Teste: %d", area->x1, area->x2, area->y1, area->y2, size, teste);
//    LV_LOG_ERROR(buffer);

	lv_disp_flush_ready(disp_drv);                  /* Tell you are ready with the flushing*/
}

void ST7735_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    int32_t x;
    int32_t y;
//    int32_t size;
    int32_t teste = 0;

//    size = ( ((area->x2 - area->x1) + 1)  * ((area->y2 - area->y1) + 1) );
    CS_L();
    for(y = area->y1; y <= area->y2; y++) {
    	for(x = area->x1; x <= area->x2; x++) {
    		/* Put a pixel to the display. For example: */
    		/* put_px(x, y, *color_p)*/
    		ST7735_AddrSet(x, y, x, y);
    		A0_H();
    		ST7735_write_color(color_p->full);
    		color_p++;
    		teste++;
    	}
    }
	CS_H();

//    sprintf(buffer, "ST7735-Flush X1: %d X2: %d Y1: %d Y2: %d Teste: %ld", area->x1, area->x2, area->y1, area->y2, size);
//    LV_LOG_ERROR(buffer);

	lv_disp_flush_ready(disp_drv);                  /* Tell you are ready with the flushing*/
}

void ST7735_Pixel(uint16_t X, uint16_t Y, uint16_t color) {
    CS_L();
    ST7735_AddrSet(X,Y,X,Y);
    A0_H();
    ST7735_write_color(color);
    CS_H();
}

void ST7735_HLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color) {
    uint16_t i;
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

    CS_L();
    ST7735_AddrSet(X1,Y,X2,Y);
	A0_H();
	for (i = 0; i <= (X2 - X1); i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_VLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color) {
    uint16_t i;
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

    CS_L();
    ST7735_AddrSet(X,Y1,X,Y2);
	A0_H();
	for (i = 0; i <= (Y2 - Y1); i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_Line(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t color) {
	int16_t dX = X2-X1;
	int16_t dY = Y2-Y1;
	int16_t dXsym = (dX > 0) ? 1 : -1;
	int16_t dYsym = (dY > 0) ? 1 : -1;

	if (dX == 0) {
		if (Y2>Y1) ST7735_VLine(X1,Y1,Y2,color); else ST7735_VLine(X1,Y2,Y1,color);
		return;
	}
	if (dY == 0) {
		if (X2>X1) ST7735_HLine(X1,X2,Y1,color); else ST7735_HLine(X2,X1,Y1,color);
		return;
	}

	dX *= dXsym;
	dY *= dYsym;
	int16_t dX2 = dX << 1;
	int16_t dY2 = dY << 1;
	int16_t di;

	if (dX >= dY) {
		di = dY2 - dX;
		while (X1 != X2) {
			ST7735_Pixel(X1,Y1,color);
			X1 += dXsym;
			if (di < 0) {
				di += dY2;
			} else {
				di += dY2 - dX2;
				Y1 += dYsym;
			}
		}
	} else {
		di = dX2 - dY;
		while (Y1 != Y2) {
			ST7735_Pixel(X1,Y1,color);
			Y1 += dYsym;
			if (di < 0) {
				di += dX2;
			} else {
				di += dX2 - dY2;
				X1 += dXsym;
			}
		}
	}
	ST7735_Pixel(X1,Y1,color);
}

void ST7735_Rect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color) {
	ST7735_HLine(X1,X2,Y1,color);
	ST7735_HLine(X1,X2,Y2,color);
	ST7735_VLine(X1,Y1,Y2,color);
	ST7735_VLine(X2,Y1,Y2,color);
}

void ST7735_FillRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color) {
	uint16_t i;
	uint16_t FS = (X2 - X1 + 1) * (Y2 - Y1 + 1);
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

	CS_L();
	ST7735_AddrSet(X1,Y1,X2,Y2);
	A0_H();
	for (i = 0; i < FS; i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_PutChar5x7(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color) {
	uint16_t i,j;
	uint8_t buffer[5];
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

	memcpy(buffer,&Font5x7[(chr - 32) * 5],5);

	CS_L();
	ST7735_AddrSet(X,Y,X + 4,Y + 6);
	A0_H();
	for (j = 0; j < 7; j++) {
        for (i = 0; i < 5; i++) {
    		if ((buffer[i] >> j) & 0x01) {
    			ST7735_write(CH);
    			ST7735_write(CL);
    		} else {
    			ST7735_write(0x00);
    			ST7735_write(0x00);
    		}
    	}
    }
	CS_H();
}

void ST7735_PutStr5x7(uint8_t X, uint8_t Y, char *str, uint16_t color) {
    while (*str) {
        ST7735_PutChar5x7(X,Y,*str++,color);
        if (X < scr_width - 6) { X += 6; } else if (Y < scr_height - 8) { X = 0; Y += 8; } else { X = 0; Y = 0; }
    };
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    logI("debug HAL_SPI_TxCpltCallback\n");
	lv_disp_flush_ready(disp_p);
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	logI("debug SPI ERROR\n");
}

