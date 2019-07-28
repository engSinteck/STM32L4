/*
 * screen_mpx.c
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

static void btn_event_next_audio(lv_obj_t * btn, lv_event_t event);
static void btn_event_esc_audio(lv_obj_t * btn, lv_event_t event);
static void update_mpx_screen(lv_task_t * param);
static void update_vumeter_mpx2(uint32_t value);
static void update_vumeter_audio_l(uint32_t audio_l);
static void update_vumeter_audio_r(uint32_t audio_r);

extern uint32_t TelaAtiva;
extern uint8_t sent_mpx;
extern uint32_t mpx2, Last_mpx2;
extern uint32_t audio_l, Last_audio_l;
extern uint32_t audio_r, Last_audio_r;

static lv_obj_t * Tela_Reading_MPX;
static lv_obj_t * img_fundo;
static lv_obj_t * imgbtn_next;
static lv_obj_t * imgbtn_prev;
static lv_obj_t * bar_m[24];
static lv_obj_t * bar_l[24];
static lv_obj_t * bar_r[24];
static lv_style_t style_indic_bar;
static lv_style_t style_indic_bar_vd;
static lv_task_t * Task_MPX;

const int32_t m_pos_x[24] = {14, 20, 26, 32, 38, 44, 50, 56, 62, 68, 74, 80,
                             86, 92, 98, 104, 110, 116, 122, 128, 134, 140, 146, 152};


const int32_t r_pos_x[24] = {14, 20, 26, 32, 38, 44, 50, 56, 62, 68, 74, 80,
                             86, 92, 98, 104, 110, 116, 122, 128, 134, 140, 146, 152};

const int32_t l_pos_x[24] = {14, 20, 26, 32, 38, 44, 50, 56, 62, 68, 74, 80,
                             86, 92, 98, 104, 110, 116, 122, 128, 134, 140, 146, 152};

#if LV_USE_FILESYSTEM == 0
	LV_IMG_DECLARE(tela_reading_audio);
	LV_IMG_DECLARE(BtnESC);
	LV_IMG_DECLARE(Btn_next);
#endif

void screen_reading_mpx(void)
{
	// Create a Screen
	Tela_Reading_MPX = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Reading_MPX, NULL);
#if	LV_USE_FILESYSTEM
    lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_reading_audio.bin");
#else
    lv_img_set_src(img_fundo, &tela_reading_audio);
#endif
    lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	lv_obj_set_event_cb(img_fundo, btn_event_esc_audio);
	lv_obj_set_click(img_fundo, 1);

	create_vumeter_m();
	create_vumeter_r();
	create_vumeter_l();
	btn_next_audio();
	lv_scr_load(Tela_Reading_MPX);

	// Task Update MPX Screen
	Task_MPX = lv_task_create(update_mpx_screen, 500, LV_TASK_PRIO_MID, NULL);
	TelaAtiva = TelaMpx;
}

void create_vumeter_m(void)
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
	for(uint8_t x = 0; x < 24; x++) {
		bar_m[x] = lv_bar_create(Tela_Reading_MPX, NULL);
		lv_obj_set_size(bar_m[x], 4, 11);
		lv_bar_set_style(bar_m[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_m[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_m[x], NULL, LV_ALIGN_IN_TOP_LEFT, m_pos_x[x], 48);
		lv_bar_set_value(bar_m[x], 100, 0);
	}
}

void create_vumeter_r(void)
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
	for(uint8_t x = 0; x < 24; x++) {
		bar_r[x] = lv_bar_create(Tela_Reading_MPX, NULL);
		lv_obj_set_size(bar_r[x], 4, 21);
		lv_bar_set_style(bar_r[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_r[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_r[x], NULL, LV_ALIGN_IN_TOP_LEFT, r_pos_x[x], 71);
		lv_bar_set_value(bar_r[x], 100, 0);
	}
}

void create_vumeter_l(void)
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
	for(uint8_t x = 0; x < 24; x++) {
		bar_l[x] = lv_bar_create(Tela_Reading_MPX, NULL);
		lv_obj_set_size(bar_l[x], 4, 21);
		lv_bar_set_style(bar_l[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_l[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_l[x], NULL, LV_ALIGN_IN_TOP_LEFT, l_pos_x[x], 103);
		lv_bar_set_value(bar_l[x], 100, 0);
	}
}

void btn_next_audio(void)
{
	// Create an Image button
	imgbtn_next = lv_imgbtn_create(Tela_Reading_MPX, NULL);
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
	lv_obj_set_event_cb(imgbtn_next, btn_event_next_audio);
	lv_obj_set_pos(imgbtn_next, 112, 18);
}

static void btn_event_next_audio(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button Next Released SCREEN_READING_TEMP\n");
		lv_task_del(Task_MPX);
		lv_obj_del(Tela_Reading_MPX);
		screen_reading_temp();
	}
}

void btn_esc_audio(void)
{
	// Create an Image button
	imgbtn_prev = lv_imgbtn_create(Tela_Reading_MPX, NULL);
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
	lv_obj_set_event_cb(imgbtn_prev, btn_event_esc_audio);
	lv_obj_set_pos(imgbtn_prev, 2, 18);
}

static void btn_event_esc_audio(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button ESC Released\n");
		lv_task_del(Task_MPX);
		lv_obj_del(Tela_Reading_MPX);
		screen_readings();
	}
}

static void update_vumeter_mpx2(uint32_t value)
{
	uint32_t x;

	if(value > Last_mpx2) {
		for(x = Last_mpx2; x <= value; x++) {
			lv_bar_set_style(bar_m[x], LV_BAR_STYLE_BG, &style_indic_bar_vd);
			lv_bar_set_style(bar_m[x], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
		}
	}
	else {
		for(x = value; x <= Last_mpx2; x++) {
			lv_bar_set_style(bar_m[x], LV_BAR_STYLE_BG, &style_indic_bar);
			lv_bar_set_style(bar_m[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		}
	}
	Last_mpx2 = value;
}

static void update_vumeter_audio_l(uint32_t value)
{
	uint32_t x;

	if(value > Last_audio_l) {
		for(x = Last_audio_l; x <= value; x++) {
			lv_bar_set_style(bar_l[x], LV_BAR_STYLE_BG, &style_indic_bar_vd);
			lv_bar_set_style(bar_l[x], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
		}
	}
	else {
		for(x = value; x <= Last_audio_l; x++) {
			lv_bar_set_style(bar_l[x], LV_BAR_STYLE_BG, &style_indic_bar);
			lv_bar_set_style(bar_l[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		}
	}
	Last_audio_l = value;
}

static void update_vumeter_audio_r(uint32_t value)
{
	uint32_t x;

	if(value > Last_audio_r) {
		for(x = Last_audio_r; x <= value; x++) {
			lv_bar_set_style(bar_r[x], LV_BAR_STYLE_BG, &style_indic_bar_vd);
			lv_bar_set_style(bar_r[x], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
		}
	}
	else {
		for(x = value; x <= Last_audio_r; x++) {
			lv_bar_set_style(bar_r[x], LV_BAR_STYLE_BG, &style_indic_bar);
			lv_bar_set_style(bar_r[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		}
	}
	Last_audio_r = value;
}

static void update_mpx_screen(lv_task_t * param)
{
	update_vumeter_mpx2(mpx2);
	update_vumeter_audio_l(audio_l);
	update_vumeter_audio_r(audio_r);
	if(!sent_mpx) {
		mpx2++;
		audio_l++;
		audio_r++;
		if(mpx2 >= 24) {
			sent_mpx = 1;
			mpx2--;
			audio_l--;
			audio_r--;
		}
	}
	else {
		mpx2--;
		audio_l--;
		audio_r--;
		if(mpx2 == 0) {
			sent_mpx = 0;
		}
	}
}

void ButtonEventTelaMpx(uint8_t event, uint8_t tipo, uint8_t id)
{
	if(event == EVT_PBTN_INPUT) {
		if(tipo == PBTN_SCLK) {	// Single Click
			switch(id) {
				case KEY_DN:
					lv_event_send(img_fundo, LV_EVENT_APPLY, NULL);
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
