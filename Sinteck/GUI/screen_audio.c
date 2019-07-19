	/*
 * screen_audio.c
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

static void btn_event_prev1(lv_obj_t * btn, lv_event_t event);
static void btn_event_prev2(lv_obj_t * btn, lv_event_t event);
static void btn_event_next(lv_obj_t * btn, lv_event_t event);
static void btn_event_next1(lv_obj_t * btn, lv_event_t event);
static void btn_event_esc(lv_obj_t * btn, lv_event_t event);
static void btn_stereo(lv_obj_t * btn, lv_event_t event);
static void btn_audio(lv_obj_t * btn, lv_event_t event);
static void btn_emphase(lv_obj_t * btn, lv_event_t event);
static void btn_processador(lv_obj_t * btn, lv_event_t event);
static void btn_clipper(lv_obj_t * btn, lv_event_t event);
static void btn_mp3(lv_obj_t * btn, lv_event_t event);

uint32_t TelaAudioProg = 0, IndiceAudio = 0;
uint32_t TelaAudioProg_1 = 0, IndiceAudio_1 = 0;
uint32_t TelaAudioProg_2 = 0, IndiceAudio_2 = 0;

extern uint8_t Cfg_Stereo, Cfg_Audio, Cfg_Processador, Cfg_Clipper, Cfg_Emphase, mp3_status;
extern uint32_t TelaAtiva;

static lv_obj_t * Tela_Audio;
static lv_obj_t * Tela_Audio_1;
static lv_obj_t * Tela_Audio_2;
static lv_obj_t * img_fundo;
static lv_obj_t * imgbtn1[2];
static lv_obj_t * imgbtn2[4];
static lv_obj_t * imgbtn_next[2];
static lv_obj_t * imgbtn_prev[3];
static lv_obj_t * bar[2];
static lv_style_t style_indic_vd;
static lv_style_t style_indic_am;
static lv_style_t style_indic;

#if LV_USE_FILESYSTEM == 0
	LV_IMG_DECLARE(tela_audio);
	LV_IMG_DECLARE(tela_audio_1);
	LV_IMG_DECLARE(tela_audio_2);
	LV_IMG_DECLARE(BtnESC);
	LV_IMG_DECLARE(Btn_prev);
	LV_IMG_DECLARE(Btn_stereo);
	LV_IMG_DECLARE(Btn_stereo_am);
	LV_IMG_DECLARE(Btn_mono);
	LV_IMG_DECLARE(Btn_mono_am);
	LV_IMG_DECLARE(Btn_mpxint);
	LV_IMG_DECLARE(Btn_mpxint_am);
	LV_IMG_DECLARE(Btn_mpxext);
	LV_IMG_DECLARE(Btn_mpxext_am);
	LV_IMG_DECLARE(Btn_mp3);
	LV_IMG_DECLARE(Btn_mp3_am);
	LV_IMG_DECLARE(Btn_mic);
	LV_IMG_DECLARE(Btn_mic_am);
	LV_IMG_DECLARE(Btn_75u);
	LV_IMG_DECLARE(Btn_75u_am);
	LV_IMG_DECLARE(Btn_50u);
	LV_IMG_DECLARE(Btn_50u_am);
	LV_IMG_DECLARE(Btn_on);
	LV_IMG_DECLARE(Btn_on_am);
	LV_IMG_DECLARE(Btn_off);
	LV_IMG_DECLARE(Btn_off_am);
	LV_IMG_DECLARE(Btn_mp3Prev);
	LV_IMG_DECLARE(Btn_mp3Prev_am);
	LV_IMG_DECLARE(Btn_mp3Prev_vd);
	LV_IMG_DECLARE(Btn_mp3Play);
	LV_IMG_DECLARE(Btn_mp3Play_am);
	LV_IMG_DECLARE(Btn_mp3Play_vd);
	LV_IMG_DECLARE(Btn_mp3Pause_vd);
	LV_IMG_DECLARE(Btn_mp3Next);
	LV_IMG_DECLARE(Btn_mp3Next_am);
	LV_IMG_DECLARE(Btn_mp3Next_vd);
	LV_IMG_DECLARE(Btn_mp3Stop);
	LV_IMG_DECLARE(Btn_mp3Stop_am);
	LV_IMG_DECLARE(Btn_mp3Stop_vd);
	LV_IMG_DECLARE(Btn_next);
#endif

void screen_audio(void)
{
	// Create a Screen
	Tela_Audio = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Audio, NULL);
#if	LV_USE_FILESYSTEM
    lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_audio.bin");
#else
    lv_img_set_src(img_fundo, &tela_audio);
#endif
    lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	lv_obj_set_event_cb(img_fundo, btn_event_esc);
	lv_obj_set_click(img_fundo, 1);
	btn_next();

	// Create Bar
	lv_style_copy(&style_indic_vd, &lv_style_pretty);
	style_indic_vd.body.radius = 0;
	style_indic_vd.body.grad_color = LV_COLOR_LIME;
	style_indic_vd.body.main_color = LV_COLOR_LIME;
	style_indic_vd.body.border.color = LV_COLOR_LIME;

	lv_style_copy(&style_indic_am, &lv_style_pretty);
	style_indic_am.body.radius = 0;
	style_indic_am.body.grad_color = LV_COLOR_YELLOW;
	style_indic_am.body.main_color = LV_COLOR_YELLOW;
	style_indic_am.body.border.color = LV_COLOR_YELLOW;

	lv_style_copy(&style_indic, &lv_style_pretty);
	style_indic.body.radius = 0;
	style_indic.body.grad_color = LV_COLOR_BLACK;
	style_indic.body.main_color = LV_COLOR_BLACK;
	style_indic.body.border.color = LV_COLOR_BLACK;

	// Create an Image button
	// Stereo
	imgbtn1[0] = lv_imgbtn_create(Tela_Audio, NULL);
	lv_obj_set_user_data(imgbtn1[0], 0);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_stereo.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_stereo.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_stereo_am.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_stereo_am.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_stereo_am.bin");
#else
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, &Btn_stereo);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_REL, &Btn_stereo);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_PR, &Btn_stereo_am);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_PR, &Btn_stereo_am);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_INA, &Btn_stereo_am);
#endif
	lv_obj_set_event_cb(imgbtn1[0], btn_stereo);
	lv_obj_set_pos(imgbtn1[0], 5, 63);
	// Mono
	imgbtn1[1] = lv_imgbtn_create(Tela_Audio, imgbtn1[0]);
	lv_obj_set_user_data(imgbtn1[1], 1);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_mono.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_mono.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_mono_am.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_mono_am.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_mono_am.bin");
#else
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, &Btn_mono);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_REL, &Btn_mono);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_PR, &Btn_mono_am);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_PR, &Btn_mono_am);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_INA, &Btn_mono_am);
#endif
	lv_obj_set_pos(imgbtn1[1], 5, 83);

	// Bar[0] Indicacao Mono Stereo
	// Bar[1] Indicacao Audio
	bar[0] = lv_bar_create(Tela_Audio, NULL);
	lv_obj_set_size(bar[0], 16, 8);
	lv_bar_set_style(bar[0], LV_BAR_STYLE_BG, &style_indic_vd);
	lv_bar_set_style(bar[0], LV_BAR_STYLE_INDIC, &style_indic_vd);
	lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 88);
	lv_bar_set_value(bar[0], 100, 0);


	// Atualiza Imagem led
	if(Cfg_Stereo) {
		lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 68);
	}
	else {
		lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 88);
	}

	// MPX INT
	imgbtn2[0] = lv_imgbtn_create(Tela_Audio, NULL);
	lv_obj_set_user_data(imgbtn2[0], 0);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_mpxint.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_mpxint.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_mpxint_am.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_mpxint_am.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_mpxint_am.bin");
#else
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_REL, &Btn_mpxint);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_REL, &Btn_mpxint);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_PR, &Btn_mpxint_am);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_PR, &Btn_mpxint_am);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_INA, &Btn_mpxint_am);
#endif
	lv_obj_set_event_cb(imgbtn2[0], btn_audio);
	lv_obj_set_pos(imgbtn2[0], 82, 47);
	// MPX EXT
	imgbtn2[1] = lv_imgbtn_create(Tela_Audio, imgbtn2[0]);
	lv_obj_set_user_data(imgbtn2[1], 1);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_mpxext.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_mpxext.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_mpxext_am.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_mpxext_am.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_mpxext_am.bin");
#else
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_REL, &Btn_mpxext);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_REL, &Btn_mpxext);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_PR, &Btn_mpxext_am);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_PR, &Btn_mpxext_am);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_INA, &Btn_mpxext_am);
#endif
	lv_obj_set_pos(imgbtn2[1], 83, 67);
	// MP3
	imgbtn2[2] = lv_imgbtn_create(Tela_Audio, imgbtn2[0]);
	lv_obj_set_user_data(imgbtn2[2], 2);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_mp3.bin");
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_mp3.bin");
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_mp3_am.bin");
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_mp3_am.bin");
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_mp3_am.bin");
#else
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_REL, &Btn_mp3);
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_TGL_REL, &Btn_mp3);
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_TGL_PR, &Btn_mp3_am);
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_PR, &Btn_mp3_am);
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_INA, &Btn_mp3_am);
#endif
	lv_obj_set_pos(imgbtn2[2], 83, 87);
	// MIC
	imgbtn2[3] = lv_imgbtn_create(Tela_Audio, imgbtn2[0]);
	lv_obj_set_user_data(imgbtn2[3], 3);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_mic.bin");
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_mic.bin");
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_mic_am.bin");
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_mic_am.bin");
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_mic_am.bin");
#else
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_REL, &Btn_mic);
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_TGL_REL, &Btn_mic);
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_TGL_PR, &Btn_mic_am);
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_PR, &Btn_mic_am);
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_INA, &Btn_mic_am);
#endif
	lv_obj_set_pos(imgbtn2[3], 83, 106);

	// Atualiza Imagem led
	// Bar[1] Indicacao Audio
	bar[1] = lv_bar_create(Tela_Audio, NULL);
	lv_obj_set_size(bar[1], 16, 8);
	lv_bar_set_style(bar[1], LV_BAR_STYLE_BG, &style_indic_vd);
	lv_bar_set_style(bar[1], LV_BAR_STYLE_INDIC, &style_indic_vd);
	lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 51);
	lv_bar_set_value(bar[1], 100, 0);

	// Atualiza Configuracao Audio
	switch(Cfg_Audio) {
		case 0:		// MPX INT
			lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 131, 52);
			break;
		case 1:		// MPX EXT
			lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 72);
			break;
		case 2:		// MP3
			lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 92);
			break;
		case 3:		// MIC
			lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 111);
			break;
	}
	lv_scr_load(Tela_Audio);
	TelaAtiva = TelaAudio;
}


void screen_audio_1(void)
{
	// Create a Screen
	Tela_Audio_1 = lv_obj_create(NULL, NULL);

	// Create Bar
	lv_style_copy(&style_indic_vd, &lv_style_pretty);
	style_indic_vd.body.radius = 0;
	style_indic_vd.body.grad_color = LV_COLOR_LIME;
	style_indic_vd.body.main_color = LV_COLOR_LIME;
	style_indic_vd.body.border.color = LV_COLOR_LIME;

	lv_style_copy(&style_indic_am, &lv_style_pretty);
	style_indic_am.body.radius = 0;
	style_indic_am.body.grad_color = LV_COLOR_YELLOW;
	style_indic_am.body.main_color = LV_COLOR_YELLOW;
	style_indic_am.body.border.color = LV_COLOR_YELLOW;

	lv_style_copy(&style_indic, &lv_style_pretty);
	style_indic.body.radius = 0;
	style_indic.body.grad_color = LV_COLOR_BLACK;
	style_indic.body.main_color = LV_COLOR_BLACK;
	style_indic.body.border.color = LV_COLOR_BLACK;

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Audio_1, NULL);
#if LV_USE_FILESYSTEM
    lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_audio_1.bin");
#else
    lv_img_set_src(img_fundo, &tela_audio_1);
#endif
    lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	btn_next1();
	btn_prev1();

	// 75u
	imgbtn1[0] = lv_imgbtn_create(Tela_Audio_1, NULL);
	lv_obj_set_user_data(imgbtn1[0], 0);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_75u.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_75u.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_75u_am.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_75u_am.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_75u_am.bin");
#else
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, &Btn_75u);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_REL, &Btn_75u);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_PR, &Btn_75u_am);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_PR, &Btn_75u_am);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_INA, &Btn_75u_am);
#endif
	lv_obj_set_event_cb(imgbtn1[0], btn_emphase);
	lv_obj_set_pos(imgbtn1[0], 5, 63);
	// 50u
	imgbtn1[1] = lv_imgbtn_create(Tela_Audio_1, imgbtn1[0]);
	lv_obj_set_user_data(imgbtn1[1], 1);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_50u.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_50u.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_50u_am.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_50u_am.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_50u_am.bin");
#else
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, &Btn_50u);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_REL, &Btn_50u);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_PR, &Btn_50u_am);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_PR, &Btn_50u_am);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_INA, &Btn_50u_am);
#endif
	lv_obj_set_pos(imgbtn1[1], 5, 83);

	// Atualiza Imagem led
	bar[0] = lv_bar_create(Tela_Audio, NULL);
	lv_obj_set_size(bar[0], 16, 8);
	lv_bar_set_style(bar[0], LV_BAR_STYLE_BG, &style_indic_vd);
	lv_bar_set_style(bar[0], LV_BAR_STYLE_INDIC, &style_indic_vd);
	lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 67);
	lv_bar_set_value(bar[0], 100, 0);

	if(Cfg_Emphase) {
		lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 68);
	}
	else {
		lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 88);
	}
    // Processador
	// ON
	imgbtn2[0] = lv_imgbtn_create(Tela_Audio_1, NULL);
	lv_obj_set_user_data(imgbtn2[0], 0);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_on.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_on.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_on_am.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_on_am.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_on_am.bin");
#else
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_REL, &Btn_on);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_REL, &Btn_on);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_PR, &Btn_on_am);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_PR, &Btn_on_am);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_INA, &Btn_on_am);
#endif
	lv_obj_set_event_cb(imgbtn2[0], btn_processador);
	lv_obj_set_pos(imgbtn2[0], 83, 63);
	// OFF
	imgbtn2[1] = lv_imgbtn_create(Tela_Audio_1, imgbtn2[0]);
	lv_obj_set_user_data(imgbtn2[1], 1);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_off.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_off.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_off_am.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_off_am.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_off_am.bin");
#else
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_REL, &Btn_off);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_REL, &Btn_off);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_PR, &Btn_off_am);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_PR, &Btn_off_am);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_INA, &Btn_off_am);
#endif
	lv_obj_set_pos(imgbtn2[1], 83, 83);

	// Atualiza Imagem led
	bar[1] = lv_bar_create(Tela_Audio_1, NULL);
	lv_obj_set_size(bar[1], 16, 8);
	lv_bar_set_style(bar[1], LV_BAR_STYLE_BG, &style_indic_vd);
	lv_bar_set_style(bar[1], LV_BAR_STYLE_INDIC, &style_indic_vd);
	lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 68);
	lv_bar_set_value(bar[1], 100, 0);

	if(Cfg_Processador) {
		lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 68);
	}
	else {
		lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 88);
	}

	lv_scr_load(Tela_Audio_1);
	TelaAtiva = TelaAudio_1;
}

void screen_audio_2(void)
{
	// Create a Screen
	Tela_Audio_2 = lv_obj_create(NULL, NULL);

	// Create Bar
	lv_style_copy(&style_indic_vd, &lv_style_pretty);
	style_indic_vd.body.radius = 0;
	style_indic_vd.body.grad_color = LV_COLOR_LIME;
	style_indic_vd.body.main_color = LV_COLOR_LIME;
	style_indic_vd.body.border.color = LV_COLOR_LIME;

	lv_style_copy(&style_indic_am, &lv_style_pretty);
	style_indic_am.body.radius = 0;
	style_indic_am.body.grad_color = LV_COLOR_YELLOW;
	style_indic_am.body.main_color = LV_COLOR_YELLOW;
	style_indic_am.body.border.color = LV_COLOR_YELLOW;

	lv_style_copy(&style_indic, &lv_style_pretty);
	style_indic.body.radius = 0;
	style_indic.body.grad_color = LV_COLOR_BLACK;
	style_indic.body.main_color = LV_COLOR_BLACK;
	style_indic.body.border.color = LV_COLOR_BLACK;

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Audio_2, NULL);
#if LV_USE_FILESYSTEM
    lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_audio_2.bin");
#else
    lv_img_set_src(img_fundo, &tela_audio_2);
#endif
    lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	btn_prev2();
	// ON
	imgbtn1[0] = lv_imgbtn_create(Tela_Audio_2, NULL);
	lv_obj_set_user_data(imgbtn1[0], 0);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_on.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_on.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_on_am.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_on_am.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_on_am.bin");
#else
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, &Btn_on);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_REL, &Btn_on);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_PR, &Btn_on_am);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_PR, &Btn_on_am);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_INA, &Btn_on_am);
#endif
	lv_obj_set_event_cb(imgbtn1[0], btn_clipper);
	lv_obj_set_pos(imgbtn1[0], 5, 63);
	// OFF
	imgbtn1[1] = lv_imgbtn_create(Tela_Audio_2, imgbtn1[0]);
	lv_obj_set_user_data(imgbtn1[1], 1);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_off.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_off.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_off_am.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_off_am.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_off_am.bin");
#else
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, &Btn_off);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_REL, &Btn_off);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_PR, &Btn_off_am);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_PR, &Btn_off_am);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_INA, &Btn_off_am);
#endif
	lv_obj_set_pos(imgbtn1[1], 5, 83);

	// Atualiza Imagem led
	bar[0] = lv_bar_create(Tela_Audio_2, NULL);
	lv_obj_set_size(bar[0], 16, 8);
	lv_bar_set_style(bar[0], LV_BAR_STYLE_BG, &style_indic_vd);
	lv_bar_set_style(bar[0], LV_BAR_STYLE_INDIC, &style_indic_vd);
	lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 67);
	lv_bar_set_value(bar[0], 100, 0);

	if(Cfg_Clipper) {
		lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 68);
	}
	else {
		lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 88);
	}

	// MP3 PREV
	imgbtn2[0] = lv_imgbtn_create(Tela_Audio_2, NULL);
	lv_obj_set_user_data(imgbtn2[0], 0);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_mp3Prev.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_mp3Prev.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_mp3Prev_am.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_mp3Prev_vd.bin");
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_mp3Prev_am.bin");
#else
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_REL, &Btn_mp3Prev);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_REL, &Btn_mp3Prev);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_TGL_PR, &Btn_mp3Prev_am);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_PR, &Btn_mp3Prev_vd);
	lv_imgbtn_set_src(imgbtn2[0], LV_BTN_STATE_INA, &Btn_mp3Prev_am);
#endif
	lv_obj_set_event_cb(imgbtn2[0], btn_mp3);
	lv_obj_set_pos(imgbtn2[0], 83, 48);
	// MP3 PLAY
	imgbtn2[1] = lv_imgbtn_create(Tela_Audio_2, imgbtn2[0]);
	lv_obj_set_user_data(imgbtn2[1], 1);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_mp3Play.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_mp3Play.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_mp3Play_am.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_mp3Play_vd.bin");
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_mp3Pause_vd.bin");
#else
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_REL, &Btn_mp3Play);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_REL, &Btn_mp3Play);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_TGL_PR, &Btn_mp3Play_am);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_PR, &Btn_mp3Play_vd);
	lv_imgbtn_set_src(imgbtn2[1], LV_BTN_STATE_INA, &Btn_mp3Pause_vd);
#endif
	lv_obj_set_pos(imgbtn2[1], 83, 67);
	// MP3 NEXT
	imgbtn2[2] = lv_imgbtn_create(Tela_Audio_2, imgbtn2[0]);
	lv_obj_set_user_data(imgbtn2[2], 2);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_mp3Next.bin");
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_mp3Next.bin");
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_mp3Next_am.bin");
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_mp3Next_vd.bin");
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_mp3Next_am.bin");
#else
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_REL, &Btn_mp3Next);
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_TGL_REL, &Btn_mp3Next);
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_TGL_PR, &Btn_mp3Next_am);
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_PR, &Btn_mp3Next_vd);
	lv_imgbtn_set_src(imgbtn2[2], LV_BTN_STATE_INA, &Btn_mp3Next_am);
#endif
	lv_obj_set_pos(imgbtn2[2], 83, 86);
	// MP3 STOP
	imgbtn2[3] = lv_imgbtn_create(Tela_Audio_2, imgbtn2[0]);
	lv_obj_set_user_data(imgbtn2[3], 3);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_mp3Stop.bin");
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_mp3Stop.bin");
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_mp3Stop_am.bin");
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_mp3Stop_vd.bin");
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_mp3Stop_am.bin");
#else
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_REL, &Btn_mp3Stop);
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_TGL_REL, &Btn_mp3Stop);
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_TGL_PR, &Btn_mp3Stop_am);
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_PR, &Btn_mp3Stop_vd);
	lv_imgbtn_set_src(imgbtn2[3], LV_BTN_STATE_INA, &Btn_mp3Stop_am);
#endif
	lv_obj_set_pos(imgbtn2[3], 83, 105);
	// Atualiza MP3 Status
	switch(mp3_status) {
		case 0:			// STOP
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_PR);
			break;
		case 1:			// Play
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_PR);
			break;
		case 2:			// Pause
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_INA);
			break;
		case 3:			// REV
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_PR);
			break;
		case 4:
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_PR);
			break;		// Next
	}
	lv_scr_load(Tela_Audio_2);
	TelaAtiva = TelaAudio_2;
}


void btn_next(void)
{
	// Create an Image button
	imgbtn_next[0] = lv_imgbtn_create(Tela_Audio, NULL);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn_next[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next[0], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next[0], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next[0], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next[0], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_next.bin");
#else
	lv_imgbtn_set_src(imgbtn_next[0], LV_BTN_STATE_REL, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next[0], LV_BTN_STATE_TGL_REL, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next[0], LV_BTN_STATE_TGL_PR, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next[0], LV_BTN_STATE_PR, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next[0], LV_BTN_STATE_INA, &Btn_next);
#endif
	lv_obj_set_event_cb(imgbtn_next[0], btn_event_next);
	lv_obj_set_pos(imgbtn_next[0], 112, 18);
}

static void btn_event_next(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button next Released\n");
		lv_obj_del(Tela_Audio);
		screen_audio_1();
	}
}

void btn_next1(void)
{
	// Create an Image button
	imgbtn_next[1] = lv_imgbtn_create(Tela_Audio, NULL);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn_next[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next[1], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next[1], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next[1], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn_next[1], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_next.bin");
#else
	lv_imgbtn_set_src(imgbtn_next[1], LV_BTN_STATE_REL, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next[1], LV_BTN_STATE_TGL_REL, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next[1], LV_BTN_STATE_TGL_PR, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next[1], LV_BTN_STATE_PR, &Btn_next);
	lv_imgbtn_set_src(imgbtn_next[1], LV_BTN_STATE_INA, &Btn_next);
#endif
	lv_obj_set_event_cb(imgbtn_next[1], btn_event_next1);
	lv_obj_set_pos(imgbtn_next[1], 112, 18);
}

static void btn_event_next1(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button Next 1 Released\n");
		lv_obj_del(Tela_Audio_1);
		screen_audio_2();
	}
}

void btn_prev1(void)
{
	// Create an Image button
	imgbtn_prev[1] = lv_imgbtn_create(Tela_Audio, NULL);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn_prev[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev[1], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev[1], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev[1], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev[1], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_prev.bin");
#else
	lv_imgbtn_set_src(imgbtn_prev[1], LV_BTN_STATE_REL, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev[1], LV_BTN_STATE_TGL_REL, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev[1], LV_BTN_STATE_TGL_PR, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev[1], LV_BTN_STATE_PR, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev[1], LV_BTN_STATE_INA, &Btn_prev);
#endif
	lv_obj_set_event_cb(imgbtn_prev[1], btn_event_prev1);
	lv_obj_set_pos(imgbtn_prev[1], 32, 18);
}

static void btn_event_prev1(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button Prev 1 Released\n");
		lv_obj_del(Tela_Audio_1);
		screen_audio();
	}
}

void btn_prev2(void)
{
	// Create an Image button
	imgbtn_prev[2] = lv_imgbtn_create(Tela_Audio, NULL);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn_prev[2], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev[2], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev[2], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev[2], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn_prev[2], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_prev.bin");
#else
	lv_imgbtn_set_src(imgbtn_prev[2], LV_BTN_STATE_REL, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev[2], LV_BTN_STATE_TGL_REL, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev[2], LV_BTN_STATE_TGL_PR, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev[2], LV_BTN_STATE_PR, &Btn_prev);
	lv_imgbtn_set_src(imgbtn_prev[2], LV_BTN_STATE_INA, &Btn_prev);
#endif
	lv_obj_set_event_cb(imgbtn_prev[2], btn_event_prev2);
	lv_obj_set_pos(imgbtn_prev[2], 32, 18);
}

static void btn_event_prev2(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button Prev 2 Released\n");
		lv_obj_del(Tela_Audio_2);
		screen_audio_1();
	}
}

void btn_esc(void)
{
	// Create an Image button
	imgbtn_prev[0] = lv_imgbtn_create(Tela_Audio, NULL);
#if LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn_prev[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn_prev[0], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn_prev[0], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn_prev[0], LV_BTN_STATE_PR, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn_prev[0], LV_BTN_STATE_INA, "P:/EX15-XT/img/BtnESC.bin");
#else
	lv_imgbtn_set_src(imgbtn_prev[0], LV_BTN_STATE_REL, &BtnESC);
	lv_imgbtn_set_src(imgbtn_prev[0], LV_BTN_STATE_TGL_REL, &BtnESC);
	lv_imgbtn_set_src(imgbtn_prev[0], LV_BTN_STATE_TGL_PR, &BtnESC);
	lv_imgbtn_set_src(imgbtn_prev[0], LV_BTN_STATE_PR, &BtnESC);
	lv_imgbtn_set_src(imgbtn_prev[0], LV_BTN_STATE_INA, &BtnESC);
#endif
	lv_obj_set_event_cb(imgbtn_prev[0], btn_event_esc);
	lv_obj_set_pos(imgbtn_prev[0], 142, 2);
}

static void btn_event_esc(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button ESC Released\n");
		lv_obj_del(Tela_Audio);
		screen_sel();
	}
}

static void btn_stereo(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		uint32_t id = lv_obj_get_user_data(btn);
		logI("Button Stereo %d Released\n" , id);
		if(id == 0) {
			lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 68);
			Cfg_Stereo = 1;
		}
		else if(id == 1) {
			lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 88);
			Cfg_Stereo = 0;
		}
		logI("Cfg_Stereo: %d\n", Cfg_Stereo);
	}
}

static void btn_audio(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		uint32_t id = lv_obj_get_user_data(btn);
		logI("Button Audio %d Released\n" , id);
		switch(id) {
			case 0:
				lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 131, 52);
				Cfg_Audio = 0;
				break;
			case 1:
				lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 72);
				Cfg_Audio = 1;
				break;
			case 2:
				lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 92);
				Cfg_Audio = 2;
				break;
			case 3:
				lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 111);
				Cfg_Audio = 3;
				break;
		}
		logI("Cfg_Audio: %d\n", Cfg_Audio);
	}
}

static void btn_emphase(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		uint32_t id = lv_obj_get_user_data(btn);
		logI("Button Emphase %d Released\n" , id);
		if(id == 0) {
			lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 68);
			Cfg_Emphase = 1;
		}
		else if(id == 1) {
			lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 88);
			Cfg_Emphase = 0;
		}
		logI("Cfg_Emphase: %d\n", Cfg_Emphase);
	}
}

static void btn_processador(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		uint32_t id = lv_obj_get_user_data(btn);
		logI("Button Processador %d Released\n" , id);
		if(id == 0) {
			lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 68);
			Cfg_Processador = 1;
		}
		else if(id == 1) {
			lv_obj_align(bar[1], NULL, LV_ALIGN_IN_TOP_LEFT, 132, 88);
			Cfg_Processador = 0;
		}
		logI("Cfg_Processador: %d\n", Cfg_Processador);
	}
}

static void btn_clipper(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		uint32_t id = lv_obj_get_user_data(btn);
		logI("Button Clipper %d Released\n" , id);
		if(id == 0) {
			lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 68);
			Cfg_Clipper = 1;
		}
		else if(id == 1) {
			lv_obj_align(bar[0], NULL, LV_ALIGN_IN_TOP_LEFT, 54, 88);
			Cfg_Clipper = 0;
		}
		logI("Cfg_Clipper: %d\n", Cfg_Clipper);
	}
}

static void btn_mp3(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		uint32_t id = lv_obj_get_user_data(btn);
		logI("Button MP3 %d Released\n" , id);
		switch(id) {
			case 0:
				logI("MP3 Function: PREV\n");
				lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_PR);
				mp3_status = 3;
				break;
			case 1:
				logI("MP3 Function: PLAY-PAUSE\n");
				if(mp3_status == 1) {
					mp3_status = 2;
					lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_INA);
				}
				else {
					mp3_status = 1;
					lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_PR);
				}
				break;
			case 2:
				logI("MP3 Function: NEXT\n");
				lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_PR);
				mp3_status = 4;
				break;
			case 3:
				logI("MP3 Function: STOP\n");
				lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_PR);
				mp3_status = 0;
				break;
		}
		logI("MP3 Button: %d\n", id);
	}
}

void update_sel_button_Audio(uint8_t idx)
{
	switch(idx) {
		case 0:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 1:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 2:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 3:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 4:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 5:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 6:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_PR);
			break;
	}
}

void update_sel_button_Audio_1(uint8_t idx)
{
	switch(idx) {
		case 0:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			break;
		case 1:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			break;
		case 2:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			break;
		case 3:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			break;
		case 4:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_PR);
			break;
	}
}

void update_sel_button_Audio_2(uint8_t idx)
{
	switch(idx) {
		case 0:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 1:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 2:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 3:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 4:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 5:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_PR);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_REL);
			break;
		case 6:
			lv_imgbtn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_TGL_REL);
			lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_TGL_PR);
			break;
	}
	// Re-Update MP3 Button
	if(idx < 3) {
		switch(mp3_status) {
			case 0:
				lv_imgbtn_set_state(imgbtn2[3], LV_BTN_STATE_PR);
				break;
			case 1:
				lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_PR);
				break;
			case 2:
				lv_imgbtn_set_state(imgbtn2[1], LV_BTN_STATE_INA);
				break;
			case 3:
				lv_imgbtn_set_state(imgbtn2[0], LV_BTN_STATE_PR);
				break;
			case 4:
				lv_imgbtn_set_state(imgbtn2[2], LV_BTN_STATE_PR);
				break;
		}
	}
}

void ButtonEventTelaAudio(uint8_t event, uint8_t tipo, uint8_t id)
{
	uint32_t x;

	if(event == EVT_PBTN_INPUT) {
		if(tipo == PBTN_SCLK) {	// Single Click
			switch(id) {
				case KEY_DN:
					if(TelaAudioProg == 0) {
						lv_event_send(img_fundo, LV_EVENT_APPLY, NULL);
					}
					else {
						if(IndiceAudio >= 1) IndiceAudio--;
						update_sel_button_Audio(IndiceAudio+1);
					}
					break;
				case KEY_UP:
					if(TelaAudioProg == 0) {
						lv_event_send(imgbtn_next[0], LV_EVENT_APPLY, NULL);
					}
					else {
						IndiceAudio++;
						if(IndiceAudio > 5) IndiceAudio = 0;
						update_sel_button_Audio(IndiceAudio+1);
					}
					break;
				case KEY_ENTER:
					if(TelaAudioProg == 0) {
						TelaAudioProg = 1;
						IndiceAudio = 0;
						update_sel_button_Audio(IndiceAudio+1);
					}
					else if(TelaAudioProg == 1) {
						TelaAudioProg = 2;
						if(IndiceAudio == 0 || IndiceAudio == 1) {
							x = (uint32_t)IndiceAudio;
							lv_obj_set_user_data(imgbtn1[x], x);
							lv_event_send(imgbtn1[x], LV_EVENT_RELEASED, NULL);
						}
						else if(IndiceAudio > 1) {
							x = (uint32_t)IndiceAudio - 2;
							lv_obj_set_user_data(imgbtn2[x], x);
							lv_event_send(imgbtn2[x], LV_EVENT_RELEASED, NULL);
						}
					}
					break;
				case KEY_ESC:
					TelaAudioProg = 0;
					IndiceAudio = 0;
					update_sel_button_Audio(0);
					break;
			}
		}
	}
}

void ButtonEventTelaAudio_1(uint8_t event, uint8_t tipo, uint8_t id)
{
	uint32_t x;

	if(event == EVT_PBTN_INPUT) {
		if(tipo == PBTN_SCLK) {	// Single Click
			switch(id) {
				case KEY_DN:
					if(TelaAudioProg_1 == 0) {
						lv_event_send(imgbtn_prev[1], LV_EVENT_APPLY, NULL);
					}
					else {
						if(IndiceAudio_1 >= 1) IndiceAudio_1--;
						update_sel_button_Audio_1(IndiceAudio_1 + 1);
					}
					break;
				case KEY_UP:
					if(TelaAudioProg_1 == 0) {
						lv_event_send(imgbtn_next[1], LV_EVENT_APPLY, NULL);
					}
					else {
						IndiceAudio_1++;
						if(IndiceAudio_1 > 3) IndiceAudio_1 = 0;
						update_sel_button_Audio_1(IndiceAudio_1 + 1);
					}
					break;
				case KEY_ENTER:
					if(TelaAudioProg_1 == 0) {
						TelaAudioProg_1 = 1;
						IndiceAudio_1 = 0;
						update_sel_button_Audio_1(IndiceAudio_1 + 1);
					}
					else if(TelaAudioProg_1 == 1) {
						TelaAudioProg_1 = 2;
						if(IndiceAudio_1 == 0 || IndiceAudio_1 == 1) {
							x = (uint32_t)IndiceAudio_1;
							lv_obj_set_user_data(imgbtn1[x], x);
							lv_event_send(imgbtn1[x], LV_EVENT_RELEASED, NULL);
						}
						else if(IndiceAudio_1 == 2 || IndiceAudio_1 == 3) {
							x = (uint32_t)IndiceAudio_1 - 2;
							lv_obj_set_user_data(imgbtn2[x], x);
							lv_event_send(imgbtn2[x], LV_EVENT_RELEASED, NULL);
						}
					}
					break;
				case KEY_ESC:
					TelaAudioProg_1 = 0;
					IndiceAudio_1 = 0;
					update_sel_button_Audio_1(0);
					break;
			}
		}
	}
}

void ButtonEventTelaAudio_2(uint8_t event, uint8_t tipo, uint8_t id)
{
	uint32_t x;

	if(event == EVT_PBTN_INPUT) {
		if(tipo == PBTN_SCLK) {	// Single Click
			switch(id) {
				case KEY_DN:
					if(TelaAudioProg_2 == 0) {
						lv_event_send(imgbtn_prev[2], LV_EVENT_APPLY, NULL);
					}
					else {
						if(IndiceAudio_2 >= 1) IndiceAudio_2--;
						update_sel_button_Audio_2(IndiceAudio_2 + 1);
					}
					break;
				case KEY_UP:
					if(TelaAudioProg_2 != 0) {
						IndiceAudio_2++;
						if(IndiceAudio_2 > 5) IndiceAudio_2 = 0;
						update_sel_button_Audio_2(IndiceAudio_2 + 1);
					}
					break;
				case KEY_ENTER:
					logI("Tela Audio_2 - Key_Enter Var: %ld Idx: %ld\n", TelaAudioProg_2, IndiceAudio_2);
					if(TelaAudioProg_2 == 0) {
						TelaAudioProg_2 = 1;
						IndiceAudio_2 = 0;
						update_sel_button_Audio_2(IndiceAudio_2 + 1);
					}
					else if(TelaAudioProg_2 == 1) {
						TelaAudioProg_2 = 2;
						if(IndiceAudio_2 == 0 || IndiceAudio_2 == 1) {
							x = (uint32_t) IndiceAudio_2;
							lv_obj_set_user_data(imgbtn1[x], x);
							lv_event_send(imgbtn1[x], LV_EVENT_RELEASED, &x);
						}
						else if(IndiceAudio_2 >= 2) {
							x = (uint32_t) IndiceAudio_2 - 2;
							lv_obj_set_user_data(imgbtn2[x], x);
							lv_event_send(imgbtn2[x], LV_EVENT_RELEASED, &x);
							logI("Debug: MP3  DBG: %ld\n", x);
							TelaAudioProg_2 = 0;
						}
					}
					break;
				case KEY_ESC:
					TelaAudioProg_2 = 0;
					IndiceAudio_2 = 0;
					update_sel_button_Audio_2(0);
					break;
			}
		}
	}
}
