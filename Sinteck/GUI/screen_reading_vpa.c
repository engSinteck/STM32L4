/*
 * screen_reading_vpa.c
 *
 *  Created on: 5 de jul de 2019
 *      Author: Rinaldo Dos Santos
 *      Sinteck Next
 */
#include "main.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "string.h"
#include "key.h"
#include "log.h"
#include "Sinteck/GUI/EX15-XT.h"

extern char buffer[];
extern uint32_t TelaAtiva;

static void btn_event_next_vpa(lv_obj_t * btn, lv_event_t event);
static void btn_event_prev_vpa(lv_obj_t * btn, lv_event_t event);
static void update_screen_vpa(lv_task_t * param);

static lv_obj_t * Tela_Reading_VPA;
static lv_task_t * Task_Reading_VPA;

static lv_obj_t * img_fundo;
static lv_obj_t * imgbtn_next;
static lv_obj_t * imgbtn_prev;
static lv_obj_t * txt_vpa;
static lv_obj_t * txt_ipa;
static lv_obj_t * bar_vpa[21];
static lv_obj_t * bar_ipa[21];
static lv_style_t style_indic_bar;
static lv_style_t style_indic_bar_vd;

const int32_t vpa_pos_x[21] = {8, 15, 22, 29, 36, 43, 50, 57, 64, 70, 77,
                               85, 92, 98, 105, 112, 119, 126, 133, 140, 147};


const int32_t ipa_pos_x[21] = {8, 15, 22, 29, 36, 43, 50, 57, 64, 70, 77,
                               85, 92, 98, 105, 112, 119, 126, 133, 140, 147};

#if LV_USE_FILESYSTEM == 0
	LV_IMG_DECLARE(tela_reading_vpa);
	LV_IMG_DECLARE(Btn_prev);
	LV_IMG_DECLARE(Btn_next);
#endif

void screen_reading_vpa(void)
{
	// Create a Screen
	Tela_Reading_VPA = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Reading_VPA, NULL);
#if	LV_USE_FILESYSTEM
	lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_reading_vpa.bin");
#else
	lv_img_set_src(img_fundo, &tela_reading_vpa);
#endif
	lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	btn_next_vpa();
	btn_prev_vpa();
	create_vumeter_vpa_1();
	create_vumeter_ipa_1();
	print_vpa_1(2000);
	print_ipa_1(2000);
	update_vumeter_vpa_1(2000);
	update_vumeter_ipa_1(2000);

    lv_scr_load(Tela_Reading_VPA);

	// Task Update Main Screen
    Task_Reading_VPA = lv_task_create(update_screen_vpa, 500, LV_TASK_PRIO_MID, NULL);
    TelaAtiva = TelaReading_Vpa;
}

void btn_next_vpa(void)
{
	// Create an Image button
	imgbtn_next = lv_imgbtn_create(Tela_Reading_VPA, NULL);
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
	lv_obj_set_event_cb(imgbtn_next, btn_event_next_vpa);
	lv_obj_set_pos(imgbtn_next, 112, 18);
}

static void btn_event_next_vpa(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button Next Released SCREEN_READING_VPA\n");
		lv_task_del(Task_Reading_VPA);
		lv_obj_del(Tela_Reading_VPA);
		screen_reading_efic();
	}
}

void btn_prev_vpa(void)
{
	// Create an Image button
	imgbtn_prev = lv_imgbtn_create(Tela_Reading_VPA, NULL);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_prev.bin");
#else
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_REL, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_TGL_REL, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_TGL_PR, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_PR, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev, LV_BTN_STATE_INA, &Btn_prev);
#endif
	lv_obj_set_event_cb(imgbtn_prev, btn_event_prev_vpa);
	lv_obj_set_pos(imgbtn_prev, 32, 18);
}

static void btn_event_prev_vpa(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button ESC Released\n");
		lv_task_del(Task_Reading_VPA);
		lv_obj_del(Tela_Reading_VPA);
		screen_reading_temp();
	}
}

void print_vpa_1(uint32_t vpa)
{
	vpa = 3900;
	float resul = (float) ( vpa * (3.30 / 4095.0)) * 5;
	sprintf(buffer, "%0.1f", resul);

	// Area de VPA
	static lv_style_t style_txt1;
	lv_style_copy(&style_txt1, &lv_style_plain);
	style_txt1.text.font = &lv_font_eurostile_24;
	style_txt1.text.letter_space = 1;
	style_txt1.text.line_space = 1;
	style_txt1.text.color = LV_COLOR_CYAN;

	// Cria um novo rotulo
	txt_vpa = lv_label_create(Tela_Reading_VPA, NULL);
	lv_obj_set_style(txt_vpa, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_vpa, LV_LABEL_LONG_EXPAND); 			// Quebra as linhas longas
	lv_label_set_recolor(txt_vpa, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_vpa, LV_ALIGN_IN_TOP_RIGHT); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_vpa, buffer);
	lv_obj_set_width(txt_vpa, 300); 								// Configuura o comprimento
	lv_obj_align(txt_vpa, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 34); 	// Alinha ao centro
}

void print_ipa_1(uint32_t ipa)
{
	ipa = 2000;
	float resul = (float) ( ipa * (3.30 / 4095.0)) * 0.98;
	sprintf(buffer, "%0.1f", resul);

	// Area de VPA
	static lv_style_t style_txt1;
	lv_style_copy(&style_txt1, &lv_style_plain);
	style_txt1.text.font = &lv_font_eurostile_24;
	style_txt1.text.letter_space = 1;
	style_txt1.text.line_space = 1;
	style_txt1.text.color = LV_COLOR_CYAN;

	// Cria um novo rotulo
	txt_ipa = lv_label_create(Tela_Reading_VPA, NULL);
	lv_obj_set_style(txt_ipa, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_ipa, LV_LABEL_LONG_EXPAND); 			// Quebra as linhas longas
	lv_label_set_recolor(txt_ipa, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_ipa, LV_ALIGN_IN_TOP_RIGHT); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_ipa, buffer);
	lv_obj_set_width(txt_ipa, 300); 								// Configuura o comprimento
	lv_obj_align(txt_ipa, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 77); 	// Alinha ao centro
}

void create_vumeter_vpa_1(void)
{
	// Area do Barguraph
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
		bar_vpa[x] = lv_bar_create(Tela_Reading_VPA, NULL);
		lv_obj_set_size(bar_vpa[x], 4, 13);
		lv_bar_set_style(bar_vpa[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_vpa[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_vpa[x], NULL, LV_ALIGN_IN_TOP_LEFT, vpa_pos_x[x], 57);
		lv_bar_set_value(bar_vpa[x], 100, 0);
	}
}

void create_vumeter_ipa_1(void)
{
	// Area do Barguraph
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
		bar_ipa[x] = lv_bar_create(Tela_Reading_VPA, NULL);
		lv_obj_set_size(bar_ipa[x], 4, 13);
		lv_bar_set_style(bar_ipa[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_ipa[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_ipa[x], NULL, LV_ALIGN_IN_TOP_LEFT, ipa_pos_x[x], 99);
		lv_bar_set_value(bar_ipa[x], 100, 0);
	}
}

void update_vumeter_vpa_1(uint32_t value)
{
	uint32_t x;
	value = 3900;
	float resul = (float) ( (value * (3.30 / 4095.0)) * 5) / 0.75;

	for(x = 0; x <= resul; x++) {
		lv_bar_set_style(bar_vpa[x], LV_BAR_STYLE_BG, &style_indic_bar_vd);
		lv_bar_set_style(bar_vpa[x], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
	}
}

void update_vumeter_ipa_1(uint32_t value)
{
	uint32_t x;
	value = 2000;
	float resul = (float) (( value * (3.30 / 4095.0)) * 0.98) / 0.10;

	for(x = 0; x <= resul; x++) {
		lv_bar_set_style(bar_ipa[x], LV_BAR_STYLE_BG, &style_indic_bar_vd);
		lv_bar_set_style(bar_ipa[x], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
	}
}

static void update_screen_vpa(lv_task_t * param)
{
	uint32_t vpa = 3900;
	float resul = (float) ( vpa * (3.30 / 4095.0)) * 5;
	sprintf(buffer, "%0.1f", resul);
	lv_label_set_text(txt_vpa, buffer);
	update_vumeter_vpa_1(vpa);

	uint32_t ipa = 2000;
	resul = (float) ( ipa * (3.30 / 4095.0)) * 0.98;
	sprintf(buffer, "%0.1f", resul);
	lv_label_set_text(txt_ipa, buffer);
	update_vumeter_ipa_1(ipa);
}

void ButtonEventTelaReading_Vpa(uint8_t event, uint8_t tipo, uint8_t id)
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
