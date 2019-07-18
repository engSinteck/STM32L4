/*
 * screen_readings.c
 *
 *  Created on: 4 de jul de 2019
 *      Author: rinaldo
 */

#include "main.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "string.h"
#include "key.h"
#include "log.h"
#include "Sinteck/GUI/EX15-XT.h"

static void btn_event_next_r(lv_obj_t * btn, lv_event_t event);
static void btn_event_esc_r(lv_obj_t * btn, lv_event_t event);
static void update_reading(lv_task_t * param);
void print_fwd_1(float fwd);
void print_swr_1(float swr);

extern float forward, reflected;
extern char buffer[];
extern uint32_t TelaAtiva;

static lv_obj_t * Tela_Readings;
static lv_obj_t * img_fundo;
static lv_task_t * Task_Readings;
static lv_obj_t * imgbtn_next;
static lv_obj_t * imgbtn_prev;
static lv_obj_t * bar_swr[21];
static lv_obj_t * bar_fwd[21];
static lv_obj_t * txt_fwd;
static lv_obj_t * txt_swr;
static lv_style_t style_indic_bar;
static lv_style_t style_indic_bar_vd;

const int32_t swr1_pos_x[21] = {8, 15, 23, 30, 37, 44, 50, 57, 64, 71,
                               78, 85, 92, 99, 106, 113, 120, 127, 134, 141, 147};

const int32_t fwd1_pos_x[21] = {8, 15, 23, 30, 37, 44, 50, 57, 64, 71,
                               78, 85, 92, 99, 106, 113, 120, 127, 134, 141, 147};

#if LV_USE_FILESYSTEM == 0
	LV_IMG_DECLARE(tela_reading_rf);
	LV_IMG_DECLARE(Btn_next);
	LV_IMG_DECLARE(BtnESC);
#endif

void screen_readings(void)
{
	// Create a Screen
	Tela_Readings = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Readings, NULL);
#if	LV_USE_FILESYSTEM
	lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_reading_rf.bin");
#else
	lv_img_set_src(img_fundo, &tela_reading_rf);
#endif
	lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	lv_obj_set_event_cb(img_fundo, btn_event_esc_r);
	lv_obj_set_click(img_fundo, 1);

	forward = 15.0f;
	reflected = 1.2f;

	create_vumeter_swr_1();
	create_vumeter_fwd_1();
	print_fwd_1(forward);
	print_swr_1(reflected);
	update_vumeter_swr_1(forward);
	update_vumeter_fwd_1(reflected);
	btn_next_r();
	lv_scr_load(Tela_Readings);

	// Task Update Reading RF
	Task_Readings = lv_task_create(update_reading, 500, LV_TASK_PRIO_MID, NULL);
	TelaAtiva = TelaReadings;
}

void create_vumeter_swr_1(void)
{
	// Area do Barguraph Frequency
	// Indicador OFF
	lv_style_copy(&style_indic_bar, &lv_style_pretty);
	style_indic_bar.body.radius = 0;
	style_indic_bar.body.grad_color = LV_COLOR_MAKE(105, 105, 105);
	style_indic_bar.body.main_color = LV_COLOR_MAKE(105, 105, 105);
	style_indic_bar.body.border.color = LV_COLOR_MAKE(105, 105, 105);

	lv_style_copy(&style_indic_bar_vd, &lv_style_pretty);
	style_indic_bar_vd.body.radius = 0;
	style_indic_bar_vd.body.grad_color = LV_COLOR_MAKE(0, 255, 0);
	style_indic_bar_vd.body.main_color = LV_COLOR_MAKE(0, 255, 0);
	style_indic_bar_vd.body.border.color = LV_COLOR_MAKE(0, 255, 0);

	// Create a default bar
	for(uint8_t x = 0; x < 21; x++) {
		bar_swr[x] = lv_bar_create(Tela_Readings, NULL);
		lv_obj_set_size(bar_swr[x], 4, 14);
		lv_bar_set_style(bar_swr[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_swr[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_swr[x], NULL, LV_ALIGN_IN_TOP_LEFT, swr1_pos_x[x], 99);
		lv_bar_set_value(bar_swr[x], 100, 0);
	}
}

void create_vumeter_fwd_1(void)
{
	// Area do Barguraph Frequency
	// Indicador OFF
	lv_style_copy(&style_indic_bar, &lv_style_pretty);
	style_indic_bar.body.radius = 0;
	style_indic_bar.body.grad_color = LV_COLOR_MAKE(105, 105, 105);
	style_indic_bar.body.main_color = LV_COLOR_MAKE(105, 105, 105);
	style_indic_bar.body.border.color = LV_COLOR_MAKE(105, 105, 105);

	lv_style_copy(&style_indic_bar_vd, &lv_style_pretty);
	style_indic_bar_vd.body.radius = 0;
	style_indic_bar_vd.body.grad_color = LV_COLOR_MAKE(0, 255, 0);
	style_indic_bar_vd.body.main_color = LV_COLOR_MAKE(0, 255, 0);
	style_indic_bar_vd.body.border.color = LV_COLOR_MAKE(0, 255, 0);

	// Create a default bar
	for(uint8_t x = 0; x < 21; x++) {
		bar_fwd[x] = lv_bar_create(Tela_Readings, NULL);
		lv_obj_set_size(bar_fwd[x], 4, 14);
		lv_bar_set_style(bar_fwd[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_fwd[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_fwd[x], NULL, LV_ALIGN_IN_TOP_LEFT, fwd1_pos_x[x], 56);
		lv_bar_set_value(bar_fwd[x], 100, 0);
	}
}

void print_fwd_1(float fwd)
{
	// Area de Refletida
	static lv_style_t style_txt1;
	lv_style_copy(&style_txt1, &lv_style_plain);
	style_txt1.text.font = &lv_font_eurostile_24;
	style_txt1.text.letter_space = 1;
	style_txt1.text.line_space = 1;
	style_txt1.text.color = LV_COLOR_CYAN;

	// Cria um novo rotulo
	txt_fwd = lv_label_create(Tela_Readings, NULL);
	lv_obj_set_style(txt_fwd, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_fwd, LV_LABEL_LONG_EXPAND); 			// Quebra as linhas longas
	lv_label_set_recolor(txt_fwd, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_fwd, LV_ALIGN_IN_TOP_RIGHT); 			// Centraliza linhas alinhadas
	sprintf(buffer, "%0.1fW", fwd);
	lv_label_set_text(txt_fwd, buffer);
	lv_obj_set_width(txt_fwd, 300); 								// Configuura o comprimento
	lv_obj_align(txt_fwd, NULL, LV_ALIGN_IN_TOP_RIGHT, -41, 33); 	// Alinha ao centro
}

void print_swr_1(float swr)
{
	// Area de Refletida
	static lv_style_t style_txt1;
	lv_style_copy(&style_txt1, &lv_style_plain);
	style_txt1.text.font = &lv_font_eurostile_24;
	style_txt1.text.letter_space = 1;
	style_txt1.text.line_space = 1;
	style_txt1.text.color = LV_COLOR_CYAN;

	// Cria um novo rotulo
	txt_swr = lv_label_create(Tela_Readings, NULL);
	lv_obj_set_style(txt_swr, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_swr, LV_LABEL_LONG_EXPAND); 			// Quebra as linhas longas
	lv_label_set_recolor(txt_swr, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_swr, LV_ALIGN_IN_TOP_RIGHT); 				// Centraliza linhas alinhadas
	sprintf(buffer, "%0.1fW", swr);
	lv_label_set_text(txt_swr, buffer);
	lv_obj_set_width(txt_swr, 300); 								// Configuura o comprimento
	lv_obj_align(txt_swr, NULL, LV_ALIGN_IN_TOP_RIGHT, -41, 76); 	// Alinha ao centro
}

void update_vumeter_swr_1(float swr)
{
	uint32_t x;
	swr = 1.2f;
	uint32_t resul = (uint32_t)( (((swr / 15.0f) * 100) / 5) );

	for(x = 0; x <= resul; x++) {
		lv_bar_set_style(bar_swr[x], LV_BAR_STYLE_BG, &style_indic_bar_vd);
		lv_bar_set_style(bar_swr[x], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
	}
}

void update_vumeter_fwd_1(float fwd)
{
	uint32_t x;
	fwd = 15.0f;
	uint32_t resul = (uint32_t)( (((fwd / 15.0f) * 100) / 5) - 1);

	for(x = 0; x <= resul; x++) {
		lv_bar_set_style(bar_fwd[x], LV_BAR_STYLE_BG, &style_indic_bar_vd);
		lv_bar_set_style(bar_fwd[x], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
	}
}

void btn_next_r(void)
{
	// Create an Image button
	imgbtn_next = lv_imgbtn_create(Tela_Readings, NULL);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn_next, LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next, LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next, LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next, LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next, LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_next.bin");
#else
	lv_imgbtn_set_src(imgbtn_next, LV_BTN_STATE_REL, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next, LV_BTN_STATE_TGL_REL, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next, LV_BTN_STATE_TGL_PR, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next, LV_BTN_STATE_PR, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next, LV_BTN_STATE_INA, &Btn_next);
#endif
	lv_obj_set_event_cb(imgbtn_next, btn_event_next_r);
	lv_obj_set_pos(imgbtn_next, 112, 18);
}

static void btn_event_next_r(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button Next Released SCREEN_READING_MPX\n");
        lv_task_del(Task_Readings);
		lv_obj_del(Tela_Readings);
		screen_reading_mpx();
	}
}

void btn_esc_r(void)
{
	// Create an Image button
	imgbtn_prev = lv_imgbtn_create(Tela_Readings, NULL);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_REL, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_PR, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_INA, "P:/EX15-XT/img/BtnESC.bin");
#else
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_REL, &BtnESC);
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_TGL_REL, &BtnESC);
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_TGL_PR, &BtnESC);
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_PR, &BtnESC);
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_INA, &BtnESC);
#endif
	lv_obj_set_event_cb(imgbtn_prev, btn_event_esc_r);
	lv_obj_set_pos(imgbtn_prev, 2, 18);
	lv_obj_set_click(imgbtn_prev, 1);
}

static void btn_event_esc_r(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button ESC Released\n");
		lv_task_del(Task_Readings);
		lv_obj_del(Tela_Readings);
		screen_sel();
	}
}

void bargraph_fwd_off(uint8_t value)
{
	for(uint8_t x = value; x < 21; x++) {
		lv_bar_set_style(bar_fwd[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_fwd[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
	}
}

void bargraph_swr_off(uint8_t value)
{
	for(uint8_t x = value; x < 21; x++) {
		lv_bar_set_style(bar_swr[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_swr[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
	}
}


static void update_reading(lv_task_t * param)
{
	// Potencia Direta
	sprintf(buffer, "%0.1fW", forward);
	lv_label_set_text(txt_fwd, buffer);
	// Refletida
	sprintf(buffer, "%0.1fW", reflected);
	lv_label_set_text(txt_swr, buffer);
	// Update Vu-Meter FWD
	update_vumeter_fwd_1(forward);
	// Update Vu-Meter REF
	update_vumeter_swr_1(reflected);
}

void ButtonEventTelaReadings(uint8_t event, uint8_t tipo, uint8_t id)
{
	if(event == EVT_PBTN_INPUT) {
		if(tipo == PBTN_SCLK) {	// Single Click
			switch(id) {
				case KEY_DN:
					lv_event_send(imgbtn_prev, LV_EVENT_APPLY, NULL);
					break;
				case KEY_UP:
					lv_event_send(imgbtn_next, LV_EVENT_APPLY, NULL);
					break;
				case KEY_ENTER:
					break;
				case KEY_ESC:
					break;
			}
		}
	}
}
