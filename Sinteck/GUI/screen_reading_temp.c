/*
 * screen_reading_temp.c
 *
 *  Created on: 5 de jul de 2019
 *      Author: rinaldo
 */
#include "main.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "string.h"
#include "key.h"
#include "log.h"
#include "Sinteck/GUI/EX15-XT.h"

static void btn_event_prev_temp(lv_obj_t * btn, lv_event_t event);
static void btn_event_next_temp(lv_obj_t * btn, lv_event_t event);
static void update_reading_temp(lv_task_t * param);

extern float temperatura;
extern char buffer[];
extern uint32_t TelaAtiva;

static lv_obj_t * Tela_Reading_TEMP;
static lv_task_t * Task_Reading_TEMP;
static lv_obj_t * imgbtn_next;
static lv_obj_t * imgbtn_prev;
static lv_obj_t * img_fundo;
static lv_obj_t * bar_t[20];
static lv_style_t style_indic_bar;
static lv_style_t style_indic_bar_vd;
static lv_obj_t * txt_temp;

const int32_t t_pos_x[20] = {6, 14, 21, 28, 36, 43, 50, 57, 65, 72,
                             79, 86, 94, 101, 108, 116, 123, 130, 137, 145};
#if LV_USE_FILESYSTEM == 0
	LV_IMG_DECLARE(tela_reading_temp);
	LV_IMG_DECLARE(Btn_next);
	LV_IMG_DECLARE(Btn_prev);
#endif

void screen_reading_temp(void)
{
    float temp = 43.3f;
    temperatura = temp;

    // Create a Screen
	Tela_Reading_TEMP = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Reading_TEMP, NULL);
#if	LV_USE_FILESYSTEM
    lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_reading_temp.bin");
#else
    lv_img_set_src(img_fundo, &tela_reading_temp);
#endif
    lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	btn_next_temp();
	btn_prev_temp();
	print_temp_1(temp);
	create_vumeter_temperatura();
	update_vumeter_temperatura(temp);

	lv_scr_load(Tela_Reading_TEMP);

	// Task Update Reading Temperatura
	Task_Reading_TEMP = lv_task_create(update_reading_temp, 500, LV_TASK_PRIO_MID, NULL);
	TelaAtiva = TelaReading_Temp;
}

void btn_next_temp(void)
{
	// Create an Image button
	imgbtn_next = lv_imgbtn_create(Tela_Reading_TEMP, NULL);
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
	lv_obj_set_event_cb(imgbtn_next, btn_event_next_temp);
	lv_obj_set_pos(imgbtn_next, 112, 18);
}

static void btn_event_next_temp(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button Next Released SCREEN_READING_VPA\n");
		lv_task_del(Task_Reading_TEMP);
		lv_obj_del(Tela_Reading_TEMP);
		screen_reading_vpa();
	}
}

void btn_prev_temp(void)
{
	// Create an Image button
	imgbtn_prev = lv_imgbtn_create(Tela_Reading_TEMP, NULL);
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
	lv_obj_set_event_cb(imgbtn_prev, btn_event_prev_temp);
	lv_obj_set_pos(imgbtn_prev, 32, 18);
}

static void btn_event_prev_temp(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button ESC Released\n");
        lv_task_del(Task_Reading_TEMP);
		lv_obj_del(Tela_Reading_TEMP);
		screen_reading_mpx();
	}
}


void print_temp_1(float temp)
{
	temp = 43.3f;

	// Area de Refletida
	static lv_style_t style_txt1;
	lv_style_copy(&style_txt1, &lv_style_plain);
	style_txt1.text.font = &lv_font_eurostile_24;					// &lv_font_eurostile_24
	style_txt1.text.letter_space = 1;
	style_txt1.text.line_space = 1;
	style_txt1.text.color = LV_COLOR_CYAN;

	// Cria um novo rotulo
	txt_temp = lv_label_create(Tela_Reading_TEMP, NULL);
	lv_obj_set_style(txt_temp, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_temp, LV_LABEL_LONG_EXPAND); 			// Quebra as linhas longas
	lv_label_set_recolor(txt_temp, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_temp, LV_ALIGN_IN_TOP_RIGHT); 				// Centraliza linhas alinhadas
	sprintf(buffer, "%0.1f", temp);
	lv_label_set_text(txt_temp, buffer);
	lv_obj_set_width(txt_temp, 300); 								// Configuura o comprimento
	lv_obj_align(txt_temp, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 45); 	// Alinha ao centro
}

void create_vumeter_temperatura(void)
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
	for(uint8_t x = 0; x < 20; x++) {
		bar_t[x] = lv_bar_create(Tela_Reading_TEMP, NULL);
		lv_obj_set_size(bar_t[x], 4, 19);
		lv_bar_set_style(bar_t[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_t[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_t[x], NULL, LV_ALIGN_IN_TOP_LEFT, t_pos_x[x], 76);
		lv_bar_set_value(bar_t[x], 100, 0);
	}
}

void update_vumeter_temperatura(float temp)
{
	temp = 43.3f;
	uint32_t x;
	uint32_t resul = (uint32_t)( ((temp - 10.0) / 2.50));

	for(x = 0; x <= resul; x++) {
		lv_bar_set_style(bar_t[x], LV_BAR_STYLE_BG, &style_indic_bar_vd);
		lv_bar_set_style(bar_t[x], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
	}
}

static void update_reading_temp(lv_task_t * param)
{
	sprintf(buffer, "%0.1f", temperatura);
	lv_label_set_text(txt_temp, buffer);
	update_vumeter_temperatura(temperatura);
}

void ButtonEventTelaReading_Temp(uint8_t event, uint8_t tipo, uint8_t id)
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
