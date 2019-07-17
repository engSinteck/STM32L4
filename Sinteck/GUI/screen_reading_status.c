/*
 * screen_reading_status.c
 *
 *  Created on: 5 de jul de 2019
 *      Author: rinaldo
 */
#include "Sinteck/GUI/EX15-XT.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "string.h"

#define MAX_RFL	1.5f

extern char buffer[];
extern uint8_t pll_status;
extern float temperatura, reflected;

static void btn_event_prev_status(lv_obj_t * btn, lv_event_t event);
static void update_screen_status(lv_task_t * param);

static lv_obj_t * Tela_Reading_STS;
static lv_obj_t * img_fundo;
static lv_obj_t * txt_pll;
static lv_obj_t * txt_temp;
static lv_obj_t * txt_vswr;
static lv_task_t * Task_Status;

LV_IMG_DECLARE(tela_status);
LV_IMG_DECLARE(Btn_prev);

void screen_reading_status(void)
{
	// Create a Screen
	Tela_Reading_STS = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Reading_STS, NULL);
#if	LV_USE_FILESYSTEM
    lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_reading_status.bin");
#else
    lv_img_set_src(img_fundo, &tela_status);
#endif
    lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	btn_prev_status();
	print_reading_status();

    lv_scr_load(Tela_Reading_STS);

	// Task Update Main Screen
	Task_Status = lv_task_create(update_screen_status, 500, LV_TASK_PRIO_MID, NULL);
}

void btn_prev_status(void)
{
	// Create an Image button
	lv_obj_t * imgbtn2 = lv_imgbtn_create(Tela_Reading_STS, NULL);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_prev.bin");
#else
	lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_REL, &Btn_prev);
	lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_TGL_REL, &Btn_prev);
	lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_TGL_PR, &Btn_prev);
	lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_PR, &Btn_prev);
	lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_INA, &Btn_prev);
#endif
	lv_obj_set_event_cb(imgbtn2, btn_event_prev_status);
	lv_obj_set_pos(imgbtn2, 32, 18);
}

static void btn_event_prev_status(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		printf("Button ESC Released\n");
		lv_task_del(Task_Status);
		lv_obj_del(Tela_Reading_STS);
		screen_reading_efic();
	}
}


void print_reading_status(void)
{
	// Area de versao
	static lv_style_t style_txt1;
	lv_style_copy(&style_txt1, &lv_style_plain);
	style_txt1.text.font = &lv_font_eurostile_10;
	style_txt1.text.letter_space = 1;
	style_txt1.text.line_space = 1;
	style_txt1.text.color = LV_COLOR_WHITE;

	// PLL LOCK
	if(pll_status)
		sprintf(buffer, "OFF");
	else
		sprintf(buffer, "ON");

	txt_pll = lv_label_create(Tela_Reading_STS, NULL);
	lv_obj_set_style(txt_pll, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_pll, LV_LABEL_LONG_EXPAND); 			// Quebra as linhas longas
	lv_label_set_recolor(txt_pll, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_pll, LV_ALIGN_IN_TOP_RIGHT); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_pll, buffer);
	lv_obj_set_width(txt_pll, 300); 								// Configuura o comprimento
	lv_obj_align(txt_pll, NULL, LV_ALIGN_IN_TOP_RIGHT, -18, 40); 	// Alinha ao centro

	// Temperatura
	sprintf(buffer, "%0.1f°C", temperatura);
	txt_temp = lv_label_create(Tela_Reading_STS, NULL);
	lv_obj_set_style(txt_temp, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_temp, LV_LABEL_LONG_EXPAND); 		// Quebra as linhas longas
	lv_label_set_recolor(txt_temp, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_temp, LV_ALIGN_IN_TOP_RIGHT); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_temp, buffer);
	lv_obj_set_width(txt_temp, 300); 								// Configuura o comprimento
	lv_obj_align(txt_temp, NULL, LV_ALIGN_IN_TOP_RIGHT, -18, 64); 	// Alinha ao centro

	// VSWR
	if((float)reflected >= MAX_RFL)
		sprintf(buffer, "Falha");
	else
		sprintf(buffer, "OK");

	txt_vswr = lv_label_create(Tela_Reading_STS, NULL);
	lv_obj_set_style(txt_vswr, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_vswr, LV_LABEL_LONG_EXPAND); 		// Quebra as linhas longas
	lv_label_set_recolor(txt_vswr, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_vswr, LV_ALIGN_IN_TOP_RIGHT); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_vswr, buffer);
	lv_obj_set_width(txt_vswr, 300); 								// Configuura o comprimento
	lv_obj_align(txt_vswr, NULL, LV_ALIGN_IN_TOP_RIGHT, -18, 87); 	// Alinha ao centro

	// Versao
	sprintf(buffer, "1.0.0/2019");
	lv_obj_t * txt_version = lv_label_create(Tela_Reading_STS, NULL);
	lv_obj_set_style(txt_version, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_version, LV_LABEL_LONG_EXPAND); 			// Quebra as linhas longas
	lv_label_set_recolor(txt_version, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_version, LV_ALIGN_IN_TOP_RIGHT); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_version, buffer);
	lv_obj_set_width(txt_version, 300); 								// Configuura o comprimento
	lv_obj_align(txt_version, NULL, LV_ALIGN_IN_TOP_RIGHT, -8, 110); 	// Alinha ao centro
}

static void update_screen_status(lv_task_t * param)
{
	// PLL LOCK
	if(pll_status)
		sprintf(buffer, "OFF");
	else
		sprintf(buffer, "ON");

	lv_label_set_text(txt_pll, buffer);

	// Temperatura
	sprintf(buffer, "%0.1f°C", temperatura);
	lv_label_set_text(txt_temp, buffer);

	// VSWR
	if((float)reflected >= MAX_RFL)
		sprintf(buffer, "Falha");
	else
		sprintf(buffer, "OK");

	lv_label_set_text(txt_vswr, buffer);
}
