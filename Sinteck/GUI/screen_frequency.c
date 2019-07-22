/*
 * screen_frequency.c
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

static void btn_event_esc_freq(lv_obj_t * btn, lv_event_t event);

extern long int frequencia;
extern char buffer[];
extern uint32_t TelaAtiva;

uint32_t TelaProgFREQ = 0, IndiceFREQ = 0;
uint16_t freq1, freq2, freq3, freq4, freq5, freq6;

static lv_obj_t * Tela_Freq;
static lv_obj_t * img_fundo;
static lv_obj_t * bar[21];
static lv_obj_t *rollerfreq[6];
static lv_style_t style_indic_bar;
static lv_style_t style_indic_bar_vd;
static lv_style_t style_roller_anim;
static lv_style_t style_roller;
static lv_style_t style_roller_bg;
static lv_style_t style_roller_s;
static lv_anim_t sf;

const int32_t freq_pos_x[21] = {8, 15, 22, 29, 36, 43, 50, 57, 64, 71,
                                78, 85, 92, 99, 106, 113, 120, 127, 134, 141, 148};
#if LV_USE_FILESYSTEM == 0
	LV_IMG_DECLARE(tela_freq);
#endif

void screen_freq(void)
{
	// Create a Screen
	Tela_Freq = lv_obj_create(NULL, NULL);

	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Freq, NULL);
#if	LV_USE_FILESYSTEM
	lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_freq.bin");
#else
	lv_img_set_src(img_fundo, &tela_freq);
#endif
	lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
	lv_obj_set_event_cb(img_fundo, btn_event_esc_freq);
	lv_obj_set_click(img_fundo, 1);
	create_vumeter_freq();
	update_vumeter(frequencia);
	prog_freq();
	lv_scr_load(Tela_Freq);
	TelaAtiva = TelaFrequencia;
}

void create_vumeter_freq(void)
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
	for(uint8_t x = 0; x < 21; x++) {
		bar[x] = lv_bar_create(Tela_Freq, NULL);
		lv_obj_set_size(bar[x], 4, 20);
		lv_bar_set_style(bar[x], LV_BAR_STYLE_BG, &style_indic_bar);
		lv_bar_set_style(bar[x], LV_BAR_STYLE_INDIC, &style_indic_bar);
		lv_obj_align(bar[x], NULL, LV_ALIGN_IN_TOP_LEFT, freq_pos_x[x], 90);
		lv_bar_set_value(bar[x], 100, 0);
	}
}

void print_freq(long int frequencia)
{
	uint16_t q0, q1, q2, q3, q4, q5;
	q0 = 1;
	q1 = 0;
	q2 = 3;
	// Decimal
	q3 = 1;
	q4 = 0;
	q5 = 0;

	sprintf(buffer, "%d%d%d", q0, q1, q2);

	// Area de Frequencia
	static lv_style_t style_txt1;
	lv_style_copy(&style_txt1, &lv_style_plain);
	style_txt1.text.font = &lv_font_eurostile_24;
	style_txt1.text.letter_space = 9;
	style_txt1.text.line_space = 2;
	style_txt1.text.color = LV_COLOR_WHITE;

	// Cria um novo rotulo
	lv_obj_t * txt_freq = lv_label_create(Tela_Freq, NULL);
	lv_obj_set_style(txt_freq, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_freq, LV_LABEL_LONG_EXPAND); 		// Quebra as linhas longas
	lv_label_set_recolor(txt_freq, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_freq, LV_ALIGN_IN_TOP_LEFT); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_freq, buffer);
	lv_obj_set_width(txt_freq, 300); 								// Configuura o comprimento
	lv_obj_align(txt_freq, NULL, LV_ALIGN_IN_TOP_LEFT, 11, 41); 	// Alinha ao centro

	sprintf(buffer,"%d%d%d" , q3, q4, q5);
	// Cria um novo rotulo
	lv_obj_t * txt_freq2 = lv_label_create(Tela_Freq, NULL);
	lv_obj_set_style(txt_freq2, &style_txt1); 						// Configura o estilo criado
	lv_label_set_long_mode(txt_freq2, LV_LABEL_LONG_EXPAND); 		// Quebra as linhas longas
	lv_label_set_recolor(txt_freq2, true); 							// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_freq2, LV_ALIGN_IN_TOP_LEFT); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_freq2, buffer);
	lv_obj_set_width(txt_freq2, 300); 								// Configuura o comprimento
	lv_obj_align(txt_freq2, NULL, LV_ALIGN_IN_TOP_LEFT, 85, 41); 	// Alinha ao centro

}

void update_vumeter(long int freq)
{
	style_indic_bar_vd.body.grad_color = LV_COLOR_MAKE(0, 255, 0);
	style_indic_bar_vd.body.main_color = LV_COLOR_MAKE(0, 255, 0);
	style_indic_bar_vd.body.border.color = LV_COLOR_MAKE(0, 255, 0);

	uint32_t pos = ((freq / 100) - 87) - 1;
	lv_bar_set_style(bar[pos], LV_BAR_STYLE_BG, &style_indic_bar_vd);
	lv_bar_set_style(bar[pos], LV_BAR_STYLE_INDIC, &style_indic_bar_vd);
}

static void btn_event_esc_freq(lv_obj_t * btn, lv_event_t event)
{
	if(event == LV_EVENT_APPLY) {
		//printf("Button ESC Released\n");
		lv_obj_del(Tela_Freq);
		screen_sel();
	}
}

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        char buf[32];
        char out[6];

        uint32_t id = lv_obj_get_user_data(obj);

        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        if(id == 4 && (atoi(buf) == 8 || atoi(buf) == 9)) {
        	lv_roller_set_options(rollerfreq[5], "0", LV_ROLLER_MODE_INIFINITE);
        	lv_roller_set_selected(rollerfreq[5], 0, true);
        }
        if(id == 4 &&  atoi(buf) == 0) {
        	lv_roller_set_options(rollerfreq[5], "0\n1", LV_ROLLER_MODE_INIFINITE);
        	lv_roller_set_selected(rollerfreq[5], 1, true);
        }

        lv_roller_get_selected_str(rollerfreq[5], buf, sizeof(buf));
        out[0] = buf[0];
        lv_roller_get_selected_str(rollerfreq[4], buf, sizeof(buf));
        out[1] = buf[0];
        lv_roller_get_selected_str(rollerfreq[3], buf, sizeof(buf));
        out[2] = buf[0];
        lv_roller_get_selected_str(rollerfreq[2], buf, sizeof(buf));
        out[3] = buf[0];
        lv_roller_get_selected_str(rollerfreq[1], buf, sizeof(buf));
        out[4] = buf[0];
        out[5] = 0;

        //printf("Selected Roller: %d Digit: %s  Frequencia: %d\n", id, buf, atoi(out));
    }
}

void prog_freq(void)
{
	lv_style_copy(&style_roller, &lv_style_plain_color);
	style_roller.body.main_color = LV_COLOR_BLACK;
	style_roller.body.grad_color = LV_COLOR_BLACK;
	style_roller.text.font = &lv_font_eurostile_24;
	style_roller.text.letter_space = 2;
	style_roller.text.line_space = 24;
	style_roller.text.color = LV_COLOR_WHITE;

	lv_style_copy(&style_roller_anim, &lv_style_plain_color);
	style_roller_anim.body.main_color = LV_COLOR_BLACK;
	style_roller_anim.body.grad_color = LV_COLOR_BLACK;
	style_roller_anim.text.font = &lv_font_eurostile_24;
	style_roller_anim.text.letter_space = 2;
	style_roller_anim.text.line_space = 24;
	style_roller_anim.text.color = LV_COLOR_WHITE;

	lv_style_copy(&style_roller_bg, &lv_style_plain_color);
	style_roller_bg.body.main_color = LV_COLOR_YELLOW;
	style_roller_bg.body.grad_color = LV_COLOR_YELLOW;
	style_roller_bg.text.font = &lv_font_eurostile_24;
	style_roller_bg.text.letter_space = 2;
	style_roller_bg.text.line_space = 24;
	style_roller_bg.text.color = LV_COLOR_BLACK;

	lv_style_copy(&style_roller_s, &lv_style_plain_color);
	style_roller_s.body.main_color = LV_COLOR_BLACK;
	style_roller_s.body.grad_color = LV_COLOR_BLACK;
	style_roller_s.text.font = &lv_font_eurostile_24;
	style_roller_s.text.letter_space = 2;
	style_roller_s.text.line_space = 24;
	style_roller_s.text.color = LV_COLOR_LIME;

	// Animate the new style
	lv_style_anim_init(&sf);
	lv_style_anim_set_styles(&sf, &style_roller_anim, &style_roller, &style_roller_bg);
	lv_style_anim_set_time(&sf, 500, 500);
	lv_style_anim_set_playback(&sf, 500);
	lv_style_anim_set_repeat(&sf, 500);
	lv_style_anim_create(&sf);

	// Milhar
	rollerfreq[5] = lv_roller_create(Tela_Freq, NULL);
	lv_obj_set_user_data(rollerfreq[5], 5);
    lv_roller_set_options(rollerfreq[5], "0\n1", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollerfreq[5], 1);
    lv_roller_set_selected(rollerfreq[5], 1, true);
    lv_roller_set_fix_width(rollerfreq[5], 19);
    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_BG, &style_roller);
    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_SEL, &style_roller);
    lv_obj_align(rollerfreq[5], NULL, LV_ALIGN_IN_TOP_LEFT, 10, 48);
    lv_obj_set_event_cb(rollerfreq[5], event_handler);
    // Centena
    rollerfreq[4] = lv_roller_create(Tela_Freq, NULL);
    lv_obj_set_user_data(rollerfreq[4], 4);
    lv_roller_set_options(rollerfreq[4], "0\n8\n9", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollerfreq[4], 1);
    lv_roller_set_selected(rollerfreq[4], 0, true);
    lv_roller_set_fix_width(rollerfreq[4], 19);
    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_BG, &style_roller);
    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_SEL, &style_roller);
    lv_obj_align(rollerfreq[4], NULL, LV_ALIGN_IN_TOP_LEFT, 33, 48);
    lv_obj_set_event_cb(rollerfreq[4], event_handler);
    // Dezena
    rollerfreq[3] = lv_roller_create(Tela_Freq, NULL);
    lv_obj_set_user_data(rollerfreq[3], 3);
    lv_roller_set_options(rollerfreq[3], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollerfreq[3], 1);
    lv_roller_set_selected(rollerfreq[3], 3, true);
    lv_roller_set_fix_width(rollerfreq[3], 19);
    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_BG, &style_roller);
    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_SEL, &style_roller);
    lv_obj_align(rollerfreq[3], NULL, LV_ALIGN_IN_TOP_LEFT, 57, 48);
    lv_obj_set_event_cb(rollerfreq[3], event_handler);
    // Casa 1
    rollerfreq[2] = lv_roller_create(Tela_Freq, NULL);
    lv_obj_set_user_data(rollerfreq[2], 2);
    lv_roller_set_options(rollerfreq[2], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollerfreq[2], 1);
    lv_roller_set_selected(rollerfreq[2], 1, true);
    lv_roller_set_fix_width(rollerfreq[2], 19);
    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_BG, &style_roller);
    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_SEL, &style_roller);
    lv_obj_align(rollerfreq[2], NULL, LV_ALIGN_IN_TOP_LEFT, 84, 48);
    lv_obj_set_event_cb(rollerfreq[2], event_handler);
    // Casa 2
    rollerfreq[1] = lv_roller_create(Tela_Freq, NULL);
    lv_obj_set_user_data(rollerfreq[1], 1);
    lv_roller_set_options(rollerfreq[1], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollerfreq[1], 1);
    lv_roller_set_selected(rollerfreq[1], 0, true);
    lv_roller_set_fix_width(rollerfreq[1], 19);
    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_BG, &style_roller);
    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_SEL, &style_roller);
    lv_obj_align(rollerfreq[1], NULL, LV_ALIGN_IN_TOP_LEFT, 107, 48);
    lv_obj_set_event_cb(rollerfreq[1], event_handler);
    // Casa 3
    rollerfreq[0] = lv_roller_create(Tela_Freq, NULL);
    lv_obj_set_user_data(rollerfreq[0], 0);
    lv_roller_set_options(rollerfreq[0], "0", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollerfreq[0], 1);
    lv_roller_set_selected(rollerfreq[0], 0, true);
    lv_roller_set_fix_width(rollerfreq[0], 19);
    lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_BG, &style_roller);
    lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_SEL, &style_roller);
    lv_obj_align(rollerfreq[0], NULL, LV_ALIGN_IN_TOP_LEFT, 131, 48);
    lv_obj_set_event_cb(rollerfreq[0], event_handler);
}

void update_style_roller_freq(uint32_t idx)
{
	switch(idx) {
		case 0:
			lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_SEL, &style_roller);
			break;
		case 1:
			lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_BG, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_SEL, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_SEL, &style_roller);
			break;
		case 2:
			lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_BG, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_SEL, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_SEL, &style_roller);
			break;
		case 3:
			lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_BG, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_SEL, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_SEL, &style_roller);
			break;
		case 4:
			lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_BG, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_SEL, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_SEL, &style_roller);
			break;
		case 5:
			lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_BG, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_SEL, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_SEL, &style_roller);
			break;
		case 6:
			lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[0], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[1], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[2], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[3], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_BG, &style_roller);
		    lv_roller_set_style(rollerfreq[4], LV_ROLLER_STYLE_SEL, &style_roller);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_BG, &style_roller_bg);
		    lv_roller_set_style(rollerfreq[5], LV_ROLLER_STYLE_SEL, &style_roller_bg);
		    break;
	}
}

void ButtonEventTelaFrequencia(uint8_t event, uint8_t tipo, uint8_t id)
{
	uint8_t x;
	char buf[32];
	char out[8];

	if(event == EVT_PBTN_INPUT) {
		if(tipo == PBTN_SCLK) {	// Single Click
			switch(id) {
				case KEY_DN:
					if(TelaProgFREQ == 0) {
						lv_event_send(img_fundo, LV_EVENT_APPLY, NULL);
					}
					else if(TelaProgFREQ == 1) {
						IndiceFREQ++;
						if(IndiceFREQ > 5) IndiceFREQ = 5;
						logI("TelaFrequencia - KEY_DN - IndiceFREQ: %ld\n", IndiceFREQ);
						update_style_roller_freq(IndiceFREQ + 1);
					}
					break;
				case KEY_UP:
					if(TelaProgFREQ == 0) {
					}
					else if(TelaProgFREQ == 1) {
						if(IndiceFREQ > 1) IndiceFREQ--;
						logI("TelaFrequencia - KEY_UP - IndiceFREQ: %ld\n", IndiceFREQ);
						update_style_roller_freq(IndiceFREQ + 1);
					}
					break;
				case KEY_ENTER:
					if(TelaProgFREQ == 0) {
						TelaProgFREQ = 1;
						IndiceFREQ = 5;
						update_style_roller_freq(IndiceFREQ + 1);
					}
					else if(TelaProgFREQ == 1) {
						TelaProgFREQ = 2;
						lv_roller_set_style(rollerfreq[IndiceFREQ], LV_ROLLER_STYLE_BG, &style_roller_anim);
						lv_roller_set_style(rollerfreq[IndiceFREQ], LV_ROLLER_STYLE_SEL, &style_roller_anim);
					}
					else if(TelaProgFREQ == 2) {
						logI("Salva Frequencia\n");
						TelaProgFREQ = 0;
						IndiceFREQ = 0;
						update_style_roller_freq(0);
						for(x = 0; x < 6; x++) {
							lv_roller_set_style(rollerfreq[x], LV_ROLLER_STYLE_BG, &style_roller_s);
						    lv_roller_set_style(rollerfreq[x], LV_ROLLER_STYLE_SEL, &style_roller_s);
						}
				        lv_roller_get_selected_str(rollerfreq[5], buf, sizeof(buf));
				        out[0] = buf[0];
				        lv_roller_get_selected_str(rollerfreq[4], buf, sizeof(buf));
				        out[1] = buf[0];
				        lv_roller_get_selected_str(rollerfreq[3], buf, sizeof(buf));
				        out[2] = buf[0];
				        lv_roller_get_selected_str(rollerfreq[2], buf, sizeof(buf));
				        out[3] = buf[0];
				        lv_roller_get_selected_str(rollerfreq[1], buf, sizeof(buf));
				        out[4] = buf[0];
				        out[5] = 0;

				        logI("Selected Roller: %d Digit: %s  Frequencia: %d\n", id, buf, atoi(out));

					}
					break;
				case KEY_ESC:
					TelaProgFREQ = 0;
					IndiceFREQ = 0;
					update_style_roller_freq(0);
					break;
			}
		}
	}
}

