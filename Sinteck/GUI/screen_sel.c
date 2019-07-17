/*
 * screen_sel.c
 *
 *  Created on: 4 de jul de 2019
 *      Author: rinaldo
 */
#include "Sinteck/GUI/EX15-XT.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "string.h"

static void btn_event_btn1(lv_obj_t * btn, lv_event_t event);
static void btn_event_esc_sel(lv_obj_t * btn, lv_event_t event);

static lv_obj_t * Tela_Sel;
static lv_obj_t * img_fundo;
static lv_obj_t * imgbtn1[4];
static lv_group_t * group;

LV_IMG_DECLARE(tela_sel);
LV_IMG_DECLARE(Btn_Frequency);
LV_IMG_DECLARE(Btn_Frequency_am);
LV_IMG_DECLARE(Btn_Audio);
LV_IMG_DECLARE(Btn_Audio_am);
LV_IMG_DECLARE(Btn_RF);
LV_IMG_DECLARE(Btn_RF_am);
LV_IMG_DECLARE(Btn_Readings);
LV_IMG_DECLARE(Btn_Readings_am);

void screen_sel(void)
{
	// Create a Screen
	Tela_Sel = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Sel, NULL);
#if	LV_USE_FILESYSTEM
	lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_sel.bin");
#else
	lv_img_set_src(img_fundo, &tela_sel);
#endif
	lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	lv_obj_set_user_data(img_fundo, 0);
	lv_obj_set_event_cb(img_fundo, btn_event_esc_sel);
	lv_obj_set_click(img_fundo, 1);
	// Cria Botoes
	create_buttons();

	lv_scr_load(Tela_Sel);
}

void create_buttons(void)
{
	// Create Button Frequency
	imgbtn1[0] = lv_imgbtn_create(Tela_Sel, NULL);
	lv_obj_set_user_data(imgbtn1[0], 0);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_Frequency.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_Frequency.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_Frequency_am.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_Frequency.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_Frequency.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_Frequency.bin");
#else
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, &Btn_Frequency);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_REL, &Btn_Frequency);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_PR, &Btn_Frequency_am);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_PR, &Btn_Frequency);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, &Btn_Frequency);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_INA, &Btn_Frequency);
#endif
	lv_obj_set_event_cb(imgbtn1[0], btn_event_btn1);
	lv_group_add_obj(group, imgbtn1[0]);
	lv_obj_set_pos(imgbtn1[0], 1, 18);

	imgbtn1[1] = lv_imgbtn_create(Tela_Sel, imgbtn1[0]);
	lv_obj_set_user_data(imgbtn1[1], 1);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_Audio.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_Audio.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_Audio_am.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_Audio.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_Audio.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_Audio.bin");
#else
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, &Btn_Audio);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_REL, &Btn_Audio);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_PR, &Btn_Audio_am);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_PR, &Btn_Audio);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, &Btn_Audio);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_INA, &Btn_Audio);
#endif
	lv_group_add_obj(group, imgbtn1[1]);
	lv_obj_set_pos(imgbtn1[1], 1, 45);

	imgbtn1[2] = lv_imgbtn_create(Tela_Sel, imgbtn1[0]);
	lv_obj_set_user_data(imgbtn1[2], 2);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_RF.bin");
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_RF.bin");
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_RF_am.bin");
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_RF.bin");
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_RF.bin");
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_RF.bin");
#else
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_REL, &Btn_RF);
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_TGL_REL, &Btn_RF);
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_TGL_PR, &Btn_RF_am);
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_PR, &Btn_RF);
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_REL, &Btn_RF);
	lv_imgbtn_set_src(imgbtn1[2], LV_BTN_STATE_INA, &Btn_RF);
#endif
	lv_group_add_obj(group, imgbtn1[2]);
	lv_obj_set_pos(imgbtn1[2], 1, 73);

	imgbtn1[3] = lv_imgbtn_create(Tela_Sel, imgbtn1[0]);
	lv_obj_set_user_data(imgbtn1[3], 3);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_Readings.bin");
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_Readings.bin");
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_Readings_am.bin");
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_Readings.bin");
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_Readings.bin");
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_Readings.bin");
#else
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_REL, &Btn_Readings);
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_TGL_REL, &Btn_Readings);
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_TGL_PR, &Btn_Readings_am);
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_PR, &Btn_Readings);
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_REL, &Btn_Readings);
	lv_imgbtn_set_src(imgbtn1[3], LV_BTN_STATE_INA, &Btn_Readings);
#endif
	lv_group_add_obj(group, imgbtn1[3]);
	lv_obj_set_pos(imgbtn1[3], 1, 100);
}

static void btn_event_esc_sel(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		uint32_t id = lv_obj_get_user_data(btn);
		lv_obj_del(Tela_Sel);
		main_screen();
	}
}


static void btn_event_btn1(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		uint32_t id = lv_obj_get_user_data(btn);
		lv_obj_del(Tela_Sel);
		switch(id) {
			case 0:
				screen_freq();
				break;
			case 1:
				screen_audio();
				break;
			case 2:
				screen_rf();
				break;
			case 3:
				screen_readings();
				break;
		}
	}
}