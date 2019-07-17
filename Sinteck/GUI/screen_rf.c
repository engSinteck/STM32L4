/*
 * screen_rf.c
 *
 *  Created on: 4 de jul de 2019
 *      Author: rinaldo
 */
#include "Sinteck/GUI/EX15-XT.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "math.h"

static void event_handler_target(lv_obj_t * obj, lv_event_t event);
static void event_handler_swr(lv_obj_t * obj, lv_event_t event);
static void btn_event_prev_rf2(lv_obj_t * btn, lv_event_t event);
static void btn_event_next_rf1(lv_obj_t * btn, lv_event_t event);
static void btn_event_next_rf(lv_obj_t * btn, lv_event_t event);
static void btn_event_esc_rf2(lv_obj_t * btn, lv_event_t event);
static void btn_power(lv_obj_t * btn, lv_event_t event);
static void btn_event_esc_rf1(lv_obj_t * btn, lv_event_t event);
static void btn_event_esc_rf(lv_obj_t * btn, lv_event_t event);
static void update_rf(lv_task_t * param);
static void update_rf_1(lv_task_t * param);


extern char buffer[];
extern uint8_t RFEnable;
extern float forward, max_rfl, target;

static lv_obj_t * Tela_RF;
static lv_obj_t * Tela_RF_1;
static lv_obj_t * Tela_RF_2;
static lv_task_t * Task_RF;
static lv_task_t * Task_RF_1;
static lv_obj_t * img_fundo;
static lv_obj_t * bar_swr[20];
static lv_obj_t * bar_fwd[20];
static lv_obj_t * bar_pwr[10];
static lv_obj_t * imgbtn1[2];
static lv_obj_t *rollerswr[2];
static lv_obj_t *rollertarget[2];
static lv_style_t style_indic_bar;
static lv_style_t style_indic_bar_vd;

const int32_t swr_pos_x[20] = {8, 15, 23, 30, 37, 44, 51, 59, 66, 73,
                               81, 88, 95, 102, 110, 117, 124, 132, 139, 146};

const int32_t fwd_pos_x[20] = {8, 16, 23, 30, 38, 45, 52, 60, 67, 74,
                               82, 89, 96, 103, 111, 118, 125, 133, 140, 147};

const int32_t pwr_pos_x[20] = {6, 14, 22, 30, 37, 45, 53, 61, 68, 76};

#if LV_USE_FILESYSTEM == 0
	LV_IMG_DECLARE(tela_rf);
	LV_IMG_DECLARE(Btn_poweron);
	LV_IMG_DECLARE(Btn_poweron_vd);
	LV_IMG_DECLARE(Btn_poweroff);
	LV_IMG_DECLARE(Btn_poweroff_vm);
	LV_IMG_DECLARE(BtnESC);
	LV_IMG_DECLARE(Btn_next);
	LV_IMG_DECLARE(Btn_prev);
#endif

void screen_rf(void)
{
	// Create a Screen
	Tela_RF = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_RF, NULL);
#if	LV_USE_FILESYSTEM
    lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_rf.bin");
#else
    lv_img_set_src(img_fundo, &tela_rf);
#endif
	lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	lv_obj_set_event_cb(img_fundo, btn_event_esc_rf);
	lv_obj_set_click(img_fundo, 1);

	btn_next_rf();
	create_vumeter_swr();
	prog_swr(max_rfl);
	update_vumeter_swr(max_rfl);
	lv_scr_load(Tela_RF);

	// Task Update Vu-Meter
	Task_RF = lv_task_create(update_rf, 500, LV_TASK_PRIO_MID, NULL);
}

void screen_RF_1(void)
{
	// Create a Screen
	Tela_RF_1 = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_RF_1, NULL);
    lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_rf_1.bin");
	lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	lv_obj_set_event_cb(img_fundo, btn_event_esc_rf1);
	lv_obj_set_click(img_fundo, 1);

	btn_next_rf1();
	create_vumeter_fwd();
	prog_target(target);
	update_vumeter_fwd(target);
	lv_scr_load(Tela_RF_1);

	// Task Update Vu-Meter
	Task_RF_1 = lv_task_create(update_rf_1, 500, LV_TASK_PRIO_MID, NULL);
}

void screen_RF_2(void)
{
	// Create a Screen
	Tela_RF_2 = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_RF_2, NULL);
    lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_rf_2.bin");
	lv_obj_set_protect(img_fundo, LV_PROTECT_POS);

	// POWER ON
	imgbtn1[0] = lv_imgbtn_create(Tela_RF_2, NULL);
	lv_obj_set_user_data(imgbtn1[0], 0);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_poweron.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_poweron_vd.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_poweron.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_poweron.bin");
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_poweron.bin");
#else
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_REL, &Btn_poweron);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_REL, &Btn_poweron_vd);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_TGL_PR, &Btn_poweron);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_PR, &Btn_poweron);
	lv_imgbtn_set_src(imgbtn1[0], LV_BTN_STATE_INA, &Btn_poweron);
#endif

	lv_obj_set_event_cb(imgbtn1[0], btn_power);
	lv_obj_set_pos(imgbtn1[0], 1, 35);
	// POWER OFF
	imgbtn1[1] = lv_imgbtn_create(Tela_RF_2, imgbtn1[0]);
	lv_obj_set_user_data(imgbtn1[1], 1);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_poweroff.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_poweroff_vm.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_poweroff.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_poweroff.bin");
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_poweroff.bin");
#else
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_REL, &Btn_poweroff);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_REL, &Btn_poweroff_vm);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_TGL_PR, &Btn_poweroff);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_PR, &Btn_poweroff);
	lv_imgbtn_set_src(imgbtn1[1], LV_BTN_STATE_INA, &Btn_poweroff);
#endif
	lv_obj_set_pos(imgbtn1[1], 1, 63);

	if(RFEnable) {
		lv_btn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
		lv_btn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_PR);
	}
	else {
		lv_btn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_PR);
		lv_btn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
	}

	btn_prev_rf2();
	create_vumeter_pwr();
	update_vumeter_pwr(forward);
	print_pwr(forward);
	lv_scr_load(Tela_RF_2);
}

void create_vumeter_swr(void)
{
	// Area do Bargraph Frequency
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
		bar_swr[x] = lv_bar_create(Tela_RF, NULL);
		lv_obj_set_size(bar_swr[x], 4, 18);
		lv_bar_set_style(bar_swr[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_swr[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_swr[x], NULL, LV_ALIGN_IN_TOP_LEFT, swr_pos_x[x], 86);
		lv_bar_set_value(bar_swr[x], 100, 0);
	}
}

void create_vumeter_pwr(void)
{
	// Area do Bargraph
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
	for(uint8_t x = 0; x < 10; x++) {
		bar_pwr[x] = lv_bar_create(Tela_RF_2, NULL);
		lv_obj_set_size(bar_pwr[x], 4, 20);
		lv_bar_set_style(bar_pwr[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_pwr[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_pwr[x], NULL, LV_ALIGN_IN_TOP_LEFT, pwr_pos_x[x], 93);
		lv_bar_set_value(bar_pwr[x], 100, 0);
	}
}

void update_vumeter_swr(float swr)
{
	uint32_t y;
	float resul;

	// Create a default bar
	for(uint8_t x = 0; x < 20; x++) {
		lv_bar_set_style(bar_swr[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_swr[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
	}

	if(swr != 0) {
		resul = (uint32_t) (((((swr / 10) / target) * 100) / 2.50f) - 1) ;
		//printf("Debug: Task_SWR Target: %0.1f SWR: %0.1f Resul: %0.1f\n", target, swr, resul);
		if(resul >= 20) resul = 19;

		for(y = 0; y <= resul; y++) {
			lv_bar_set_style(bar_swr[y], LV_BAR_STYLE_BG, &style_indic_bar_vd);
			lv_bar_set_style(bar_swr[y], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
		}
	}
}

void update_vumeter_fwd(float fwd)
{
	uint32_t y;
	uint32_t resul;

	// Create a default bar
	for(uint8_t x = 0; x < 20; x++) {
		lv_bar_set_style(bar_fwd[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_fwd[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
	}

	if(fwd != 0) {
		resul = (uint32_t) ((((((float)fwd) / 15.0f) * 100) / 5.0f) - 1) ;
		if(resul >= 20) resul = 19;
		//printf("Debug: Task_FWD FWD: %0.1f Resul: %ld\n", fwd, resul);

		for(y = 0; y <= resul; y++) {
			lv_bar_set_style(bar_fwd[y], LV_BAR_STYLE_BG, &style_indic_bar_vd);
			lv_bar_set_style(bar_fwd[y], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
		}
	}
}

void update_vumeter_pwr(float fwd)
{
	uint32_t x;
	fwd = 15.0f;
	uint32_t resul = (uint32_t)( (((fwd / 15.0f) * 100) / 10) - 1);

	for(x = 0; x <= resul; x++) {
		lv_bar_set_style(bar_pwr[x], LV_BAR_STYLE_BG, &style_indic_bar_vd);
		lv_bar_set_style(bar_pwr[x], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
	}
}

void print_pwr(float pwr)
{
	// Area de Potencia
	static lv_style_t style_txt_fwd;
	lv_style_copy(&style_txt_fwd, &lv_style_plain);
	style_txt_fwd.text.font = &lv_font_eurostile_22;
	style_txt_fwd.text.letter_space = 1;
	style_txt_fwd.text.line_space = 1;
	style_txt_fwd.text.color = LV_COLOR_CYAN;

	lv_obj_t *txt_fwd = lv_label_create(Tela_RF_2, NULL);
	lv_obj_set_style(txt_fwd, &style_txt_fwd); 							// Configura o estilo criado
	lv_label_set_long_mode(txt_fwd, LV_LABEL_LONG_BREAK); 				// Quebra as linhas longas
	lv_label_set_recolor(txt_fwd, true); 								// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_fwd, LV_ALIGN_IN_BOTTOM_RIGHT); 				// Centraliza linhas alinhadas
	pwr = 15.0f;
	sprintf(buffer, "%0.1f", pwr);
	lv_label_set_text(txt_fwd, buffer);
	lv_obj_set_width(txt_fwd, 200); 									// Configuura o comprimento
	lv_obj_align(txt_fwd, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -30, -13); 		// Alinha ao centro
}

void print_reflected(float swr)
{
	sprintf(buffer, "%0.1f", swr);

	// Area de Refletida
	static lv_style_t style_txt1;
	lv_style_copy(&style_txt1, &lv_style_plain);
	style_txt1.text.font = &lv_font_eurostile_28;
	style_txt1.text.letter_space = 1;
	style_txt1.text.line_space = 1;
	style_txt1.text.color = LV_COLOR_WHITE;

	// Cria um novo rotulo
	lv_obj_t * txt_swr = lv_label_create(Tela_RF, NULL);
	lv_obj_set_style(txt_swr, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_swr, LV_LABEL_LONG_EXPAND); 			// Quebra as linhas longas
	lv_label_set_recolor(txt_swr, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_swr, LV_ALIGN_IN_TOP_LEFT); 				// Centraliza linhas alinhadas
	lv_label_set_text(txt_swr, "1");
	lv_obj_set_width(txt_swr, 300); 								// Configuura o comprimento
	lv_obj_align(txt_swr, NULL, LV_ALIGN_IN_TOP_LEFT, 46, 41); 		// Alinha ao centro

	// Cria um novo rotulo
	lv_obj_t * txt_swr1 = lv_label_create(Tela_RF, NULL);
	lv_obj_set_style(txt_swr1, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_swr1, LV_LABEL_LONG_EXPAND); 		// Quebra as linhas longas
	lv_label_set_recolor(txt_swr1, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_swr1, LV_ALIGN_IN_TOP_LEFT); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_swr1, "2");
	lv_obj_set_width(txt_swr1, 300); 								// Configuura o comprimento
	lv_obj_align(txt_swr1, NULL, LV_ALIGN_IN_TOP_LEFT, 93, 41); 	// Alinha ao centro
}


void print_forward(float fwd)
{
	sprintf(buffer, "%0.1f", fwd);

	// Area de Refletida
	static lv_style_t style_txt1;
	lv_style_copy(&style_txt1, &lv_style_plain);
	style_txt1.text.font = &lv_font_eurostile_28;
	style_txt1.text.letter_space = 1;
	style_txt1.text.line_space = 1;
	style_txt1.text.color = LV_COLOR_WHITE;

	// Cria um novo rotulo
	lv_obj_t * txt_fwd = lv_label_create(Tela_RF, NULL);
	lv_obj_set_style(txt_fwd, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_fwd, LV_LABEL_LONG_EXPAND); 			// Quebra as linhas longas
	lv_label_set_recolor(txt_fwd, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_fwd, LV_ALIGN_IN_TOP_LEFT); 				// Centraliza linhas alinhadas
	lv_label_set_text(txt_fwd, "1");
	lv_obj_set_width(txt_fwd, 300); 								// Configuura o comprimento
	lv_obj_align(txt_fwd, NULL, LV_ALIGN_IN_TOP_LEFT, 46, 41); 		// Alinha ao centro

	// Cria um novo rotulo
	lv_obj_t * txt_fwd1 = lv_label_create(Tela_RF, NULL);
	lv_obj_set_style(txt_fwd1, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_fwd1, LV_LABEL_LONG_EXPAND); 		// Quebra as linhas longas
	lv_label_set_recolor(txt_fwd1, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_fwd1, LV_ALIGN_IN_TOP_LEFT); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_fwd1, "5");
	lv_obj_set_width(txt_fwd1, 300); 								// Configuura o comprimento
	lv_obj_align(txt_fwd1, NULL, LV_ALIGN_IN_TOP_LEFT, 93, 41); 	// Alinha ao centro
}

void create_vumeter_fwd(void)
{
	// Area do Bargraph
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
		bar_fwd[x] = lv_bar_create(Tela_RF, NULL);
		lv_obj_set_size(bar_fwd[x], 4, 19);
		lv_bar_set_style(bar_fwd[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar_fwd[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar_fwd[x], NULL, LV_ALIGN_IN_TOP_LEFT, fwd_pos_x[x], 86);
		lv_bar_set_value(bar_fwd[x], 100, 0);
	}
}

void btn_esc_rf(void)
{
	// Create an Image button
	lv_obj_t * imgbtn1 = lv_imgbtn_create(Tela_RF, NULL);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, "P:/EX15-XT/img/BtnESC.bin");
#else
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, &BtnESC);
#endif
	lv_obj_set_event_cb(imgbtn1, btn_event_esc_rf);
	lv_obj_set_pos(imgbtn1, 143, 2);
}

static void btn_event_esc_rf(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		//printf("TELA RF - Button ESC Released\n");
		lv_task_del(Task_RF);
		lv_obj_del(Tela_RF);
		screen_sel();
	}
}

void btn_esc_rf1(void)
{
	// Create an Image button
	lv_obj_t * imgbtn1 = lv_imgbtn_create(Tela_RF, NULL);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, "P:/EX15-XT/img/BtnESC.bin");
#else
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, &BtnESC);
#endif
	lv_obj_set_event_cb(imgbtn1, btn_event_esc_rf1);
	lv_obj_set_pos(imgbtn1, 143, 2);
}

static void btn_event_esc_rf1(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		//printf("Button ESC  1 Released\n");
		lv_task_del(Task_RF_1);
		lv_obj_del(Tela_RF_1);
		screen_rf();
	}
}


static void btn_power(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		uint32_t id = lv_obj_get_user_data(btn);
		//printf("Button Power %d Released\n" , id);
		if(id == 0) {
			lv_btn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_REL);
			lv_btn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_PR);
			RFEnable = 1;
		}
		else if(id == 1) {
			lv_btn_set_state(imgbtn1[0], LV_BTN_STATE_TGL_PR);
			lv_btn_set_state(imgbtn1[1], LV_BTN_STATE_TGL_REL);
			RFEnable = 0;
		}
		//
		//printf("RFEnable: %d\n", RFEnable);
	}
}


void btn_esc_rf2(void)
{
	// Create an Image button
	lv_obj_t * imgbtn1 = lv_imgbtn_create(Tela_RF, NULL);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, "P:/EX15-XT/img/BtnESC.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, "P:/EX15-XT/img/BtnESC.bin");
#else
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, &BtnESC);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, &BtnESC);
#endif
	lv_obj_set_event_cb(imgbtn1, btn_event_esc_rf2);
	lv_obj_set_pos(imgbtn1, 143, 2);
}

static void btn_event_esc_rf2(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		printf("Button ESC  2 Released\n");
		lv_obj_del(Tela_RF_2);
		screen_RF_1();
	}
}

void btn_next_rf(void)
{
	// Create an Image button
	lv_obj_t * imgbtn1 = lv_imgbtn_create(Tela_RF, NULL);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_next.bin");
#else
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, &Btn_next);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, &Btn_next);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, &Btn_next);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, &Btn_next);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, &Btn_next);
#endif
	lv_obj_set_event_cb(imgbtn1, btn_event_next_rf);
	lv_obj_set_pos(imgbtn1, 112, 18);
}

static void btn_event_next_rf(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		printf("TELA_RF - Button Next Released\n");
		lv_task_del(Task_RF);
		lv_obj_del(Tela_RF);
		screen_RF_1();
	}
}

void btn_next_rf1(void)
{
	// Create an Image button
	lv_obj_t * imgbtn1 = lv_imgbtn_create(Tela_RF_1, NULL);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_next.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_next.bin");
#else
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, &Btn_next);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, &Btn_next);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, &Btn_next);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, &Btn_next);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, &Btn_next);
#endif
	lv_obj_set_event_cb(imgbtn1, btn_event_next_rf1);
	lv_obj_set_pos(imgbtn1, 112, 18);
}

static void btn_event_next_rf1(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		printf("Button Next  1 Released\n");
		lv_task_del(Task_RF_1);
		lv_obj_del(Tela_RF_1);
		screen_RF_2();
	}
}

void btn_prev_rf2(void)
{
	// Create an Image button
	lv_obj_t * imgbtn1 = lv_imgbtn_create(Tela_RF_1, NULL);
#if	LV_USE_FILESYSTEM
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, "P:/EX15-XT/img/Btn_prev.bin");
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, "P:/EX15-XT/img/Btn_prev.bin");
#else
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, &Btn_prev);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, &Btn_prev);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, &Btn_prev);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, &Btn_prev);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_INA, &Btn_prev);
#endif
	lv_obj_set_event_cb(imgbtn1, btn_event_prev_rf2);
	lv_obj_set_pos(imgbtn1, 32, 18);
}

static void btn_event_prev_rf2(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_RELEASED) {
		printf("Button Prev 2 Released\n");
		lv_obj_del(Tela_RF_2);
		screen_RF_1();
	}
}

void prog_swr(float swr)
{
	uint32_t teste, un, ml;

	if( swr > 0) {
		teste = (uint32_t) swr * 10;
		un = teste / 100;
		ml = (teste % 100) / 10;
		//printf("Debug: Prog_SWR SWR: %0.1f Teste: %d  Unidade: %d, Mantissa: %d\n", swr, teste, un, ml);
	}
	else {
		un = 0;
		ml = 0;
	}

	static lv_style_t style_txt;
	lv_style_copy(&style_txt, &lv_style_plain_color);
	style_txt.body.main_color = LV_COLOR_GRAY;
	style_txt.body.grad_color = LV_COLOR_BLACK;

	style_txt.text.font = &lv_font_eurostile_24;
	style_txt.text.letter_space = 2;
	style_txt.text.line_space = 24;
	style_txt.text.color = LV_COLOR_WHITE;

	// Unidade
	rollerswr[0] = lv_roller_create(Tela_RF, NULL);
	lv_obj_set_user_data(rollerswr[0], 5);
    lv_roller_set_options(rollerswr[0], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollerswr[0], 2);
    lv_roller_set_selected(rollerswr[0], un, true);
    lv_roller_set_fix_width(rollerswr[0], 34);
    lv_roller_set_style(rollerswr[0], LV_ROLLER_STYLE_BG, &style_txt);
    lv_roller_set_style(rollerswr[0], LV_ROLLER_STYLE_SEL, &style_txt);
    lv_obj_align(rollerswr[0], NULL, LV_ALIGN_IN_TOP_LEFT, 39, 38);
    lv_obj_set_event_cb(rollerswr[0], event_handler_swr);
    // Mantis
    rollerswr[1] = lv_roller_create(Tela_RF, NULL);
    lv_obj_set_user_data(rollerswr[1], 4);
    lv_roller_set_options(rollerswr[1], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollerswr[1], 2);
    lv_roller_set_selected(rollerswr[1], ml, true);
    lv_roller_set_fix_width(rollerswr[1], 35);
    lv_roller_set_style(rollerswr[1], LV_ROLLER_STYLE_BG, &style_txt);
    lv_roller_set_style(rollerswr[1], LV_ROLLER_STYLE_SEL, &style_txt);
    lv_obj_align(rollerswr[1], NULL, LV_ALIGN_IN_TOP_LEFT, 84, 38);
    lv_obj_set_event_cb(rollerswr[1], event_handler_swr);
}


static void event_handler_swr(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        char buf[32];
        char out[6];

        uint32_t id = lv_obj_get_user_data(obj);

        lv_roller_get_selected_str(obj, buf, sizeof(buf));

        lv_roller_get_selected_str(rollerswr[0], buf, sizeof(buf));
        out[0] = buf[0];
        lv_roller_get_selected_str(rollerswr[1], buf, sizeof(buf));
        out[1] = buf[0];
        out[5] = 0;

        max_rfl = atoi(out);
       // printf("Selected Roller: %d Digit: %s  MAX_RFL: %0.1fW\n", id, buf, max_rfl);
    }
}


void prog_target(float fwd)
{
	uint32_t teste, un, ml;

	if( fwd > 0) {
		teste = (uint32_t) fwd * 10;
		un = teste / 100;
		ml = (teste % 100) / 10;
		//printf("Debug: Prog_TARGET Target: %0.1f Teste: %ld  Unidade: %d, Mantissa: %d\n", fwd, teste, un, ml);
	}
	else {
		un = 0;
		ml = 0;
	}

	static lv_style_t style_txt;
	lv_style_copy(&style_txt, &lv_style_plain_color);
	style_txt.body.main_color = LV_COLOR_GRAY;
	style_txt.body.grad_color = LV_COLOR_BLACK;

	style_txt.text.font = &lv_font_eurostile_24;
	style_txt.text.letter_space = 2;
	style_txt.text.line_space = 24;
	style_txt.text.color = LV_COLOR_WHITE;

	// Milhar
	rollertarget[0] = lv_roller_create(Tela_RF, NULL);
	lv_obj_set_user_data(rollertarget[0], 5);
    lv_roller_set_options(rollertarget[0], "0\n1", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollertarget[0], 2);
    lv_roller_set_selected(rollertarget[0], un, true);
    lv_roller_set_fix_width(rollertarget[0], 34);
    lv_roller_set_style(rollertarget[0], LV_ROLLER_STYLE_BG, &style_txt);
    lv_roller_set_style(rollertarget[0], LV_ROLLER_STYLE_SEL, &style_txt);
    lv_obj_align(rollertarget[0], NULL, LV_ALIGN_IN_TOP_LEFT, 39, 38);
    lv_obj_set_event_cb(rollertarget[0], event_handler_target);
    // Centena
    rollertarget[1] = lv_roller_create(Tela_RF, NULL);
    lv_obj_set_user_data(rollertarget[1], 4);
    lv_roller_set_options(rollertarget[1], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollertarget[1], 2);
    lv_roller_set_selected(rollertarget[1], ml, true);
    lv_roller_set_fix_width(rollertarget[1], 35);
    lv_roller_set_style(rollertarget[1], LV_ROLLER_STYLE_BG, &style_txt);
    lv_roller_set_style(rollertarget[1], LV_ROLLER_STYLE_SEL, &style_txt);
    lv_obj_align(rollertarget[1], NULL, LV_ALIGN_IN_TOP_LEFT, 84, 38);
    lv_obj_set_event_cb(rollertarget[1], event_handler_target);
}


static void event_handler_target(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        char buf[32];
        char out[6];

        uint32_t id = lv_obj_get_user_data(obj);

        lv_roller_get_selected_str(obj, buf, sizeof(buf));


        lv_roller_get_selected_str(rollerswr[0], buf, sizeof(buf));
        out[0] = buf[0];
        lv_roller_get_selected_str(rollerswr[1], buf, sizeof(buf));
        out[1] = buf[0];
        out[5] = 0;

        target = (float)atoi(out);
        //printf("Selected Roller: %d Digit: %s  TARGET: %0.1fW\n", id, buf, target);
    }
}

static void update_rf(lv_task_t * param)
{
	update_vumeter_swr(max_rfl);
}

static void update_rf_1(lv_task_t * param)
{
	update_vumeter_fwd(target);
}

