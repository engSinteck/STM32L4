/*
 * main_screen.c
 *
 *  Created on: 3 de jul de 2019
 *      Author: Rinaldo Dos Santos
 *      Sinteck Next
 */

#include "main.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "string.h"
#include "log.h"
#include "Sinteck/GUI/EX15-XT.h"

//static void main_screen_event(lv_obj_t * obj, lv_event_t event);
static void update_main_screen(lv_task_t * param);
static void update_vumeter_mpx(uint32_t value);

const char *IndexAudio[] = {"MPX INT", "MPX EXT", "MP3 USB", "MIC EXT"};
static lv_obj_t * Tela_Principal;
static lv_obj_t * img_stereo;
static lv_obj_t * bar[20];

static lv_obj_t *txt_fwd;
static lv_obj_t *txt_pot;
static lv_obj_t *txt_ref;
static lv_obj_t *txt_swr;
static lv_obj_t * txt_temp;
static lv_obj_t *cod_falha;
static lv_obj_t *desc_falha;

static lv_style_t style_fundo;
static lv_style_t style_indic_off;
static lv_style_t style_indic_vd;
static lv_style_t style_indic_cy;
static lv_style_t style_indic_am;
static lv_style_t style_indic_vm;
static lv_task_t * Task_Principal;
#if LV_USE_BACKGROUND
	static lv_obj_t * img_fundo;
#endif

// Variaveis Externa
extern char buffer[500];
extern long int frequencia;
extern uint8_t Cfg_Stereo, Cfg_Audio, Cfg_Processador, Cfg_Clipper, Cfg_Emphase, RFEnable, mp3_status, sent_hor;
extern float temperatura, forward, reflected;
extern uint32_t TelaAtiva, falha, mpx, Last_mpx;

#if LV_USE_FILESYSTEM == 0
	LV_IMG_DECLARE(tela_0);
	LV_IMG_DECLARE(stereo_vd);
	LV_IMG_DECLARE(stereo_off);
#endif

const int32_t vumeter_pos_x[20] = {6, 12, 18, 23, 29,
		                           35, 41, 47, 52, 58,
                                   64, 70, 76, 81, 87,
								   93, 99, 104, 110, 116};
void main_screen(void)
{
	// Create a Screen
	Tela_Principal = lv_obj_create(NULL, NULL);
	lv_style_copy(&style_fundo, &lv_style_plain_color);
	style_fundo.body.main_color = LV_COLOR_BLACK;
	style_fundo.body.grad_color = LV_COLOR_BLACK;
	lv_obj_set_style(Tela_Principal, &style_fundo); 					// Configura o estilo criado

#if LV_USE_BACKGROUND
	// Imagem de Fundo
	img_fundo = lv_img_create(Tela_Principal, NULL);

#if	LV_USE_FILESYSTEM
    lv_img_set_src(img_fundo, "P:/EX15-XT/img/tela_0.bin");
#else
    lv_img_set_src(img_fundo, &tela_0);
#endif
	lv_obj_set_protect(img_fundo, LV_PROTECT_POS);
//	lv_obj_set_event_cb(img_fundo, main_screen_event);
//	lv_obj_set_click(img_fundo, 1);
#endif

	print_frequencia(frequencia);
	print_audio(Cfg_Stereo, Cfg_Audio);
	print_temperatura(temperatura);
	print_status(falha);
	print_pot(forward, reflected);
	create_vumeter();
	lv_scr_load(Tela_Principal);

	// Task Update Main Screen
	Task_Principal = lv_task_create(update_main_screen, 500, LV_TASK_PRIO_MID, NULL);
	TelaAtiva = TelaPrincipal;
}

void print_frequencia(long int frequencia)
{
	uint16_t q0, q1, q2, q3, q4;

	sprintf(buffer, "%ld", frequencia);
	if(strlen(buffer) > 4) {
		q0 = buffer[0] - '0';
		q1 = buffer[1] - '0';
		q2 = buffer[2] - '0';
		q3 = buffer[3] - '0';
		q4 = buffer[4] - '0';
		sprintf(buffer, "%d%d%d.%d%d%d Mhz", q0, q1, q2, q3, q4, 0);
	}
	else {
		q0 = buffer[0] - '0';
		q1 = buffer[1] - '0';
		q2 = buffer[2] - '0';
		q3 = buffer[3] - '0';
		sprintf(buffer, "%d%d.%d%d%d Mhz", q0, q1, q2, q3, 0);
	}
	// Area de Frequencia
	static lv_style_t style_txt1;
	lv_style_copy(&style_txt1, &lv_style_plain);
	style_txt1.text.font = &lv_font_eurostile_24;
	style_txt1.text.letter_space = 0;
	style_txt1.text.line_space = 0;
	style_txt1.text.color = LV_COLOR_WHITE;
	// Cria um novo rotulo
	lv_obj_t * txt_freq = lv_label_create(Tela_Principal, NULL);
	lv_obj_set_style(txt_freq, &style_txt1); 					// Configura o estilo criado
	lv_label_set_long_mode(txt_freq, LV_LABEL_LONG_EXPAND); 	// Quebra as linhas longas
	lv_label_set_recolor(txt_freq, true); 						// Ativa recolorizar por comandos no texto
	lv_label_set_align(txt_freq, LV_ALIGN_IN_TOP_MID); 			// Centraliza linhas alinhadas
	lv_label_set_text(txt_freq, buffer);
	lv_obj_set_width(txt_freq, 300); 							// Configuura o comprimento
	lv_obj_align(txt_freq, NULL, LV_ALIGN_IN_TOP_MID, 0, 3); 	// Alinha ao centro
}

void print_pot(float fwd, float swr)
{
	if(!falha) {
		// Area de Potencia
		static lv_style_t style_txt_fwd;
		lv_style_copy(&style_txt_fwd, &lv_style_plain);
		style_txt_fwd.text.font = &lv_font_eurostile_24;
		style_txt_fwd.text.letter_space = 1;
		style_txt_fwd.text.line_space = 1;
		style_txt_fwd.text.color = LV_COLOR_CYAN;

		txt_fwd = lv_label_create(Tela_Principal, NULL);
		lv_obj_set_style(txt_fwd, &style_txt_fwd); 							// Configura o estilo criado
		lv_label_set_long_mode(txt_fwd, LV_LABEL_LONG_EXPAND); 				// Quebra as linhas longas
		lv_label_set_recolor(txt_fwd, true); 								// Ativa recolorizar por comandos no texto
		lv_label_set_align(txt_fwd, LV_ALIGN_IN_BOTTOM_LEFT); 				// Centraliza linhas alinhadas
		lv_label_set_text(txt_fwd, "FWD:");
		lv_obj_set_width(txt_fwd, 200); 									// Configuura o comprimento
		lv_obj_align(txt_fwd, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 5, -25); 		// Alinha ao centro

		txt_pot = lv_label_create(Tela_Principal, NULL);
		lv_obj_set_style(txt_pot, &style_txt_fwd); 							// Configura o estilo criado
		lv_label_set_long_mode(txt_pot, LV_LABEL_LONG_BREAK); 				// Quebra as linhas longas
		lv_label_set_recolor(txt_pot, true); 								// Ativa recolorizar por comandos no texto
		lv_label_set_align(txt_pot, LV_ALIGN_IN_BOTTOM_RIGHT); 				// Centraliza linhas alinhadas
		sprintf(buffer, "%0.1fW", fwd);
		lv_label_set_text(txt_pot, buffer);
		lv_obj_set_width(txt_pot, 200); 									// Configuura o comprimento
		lv_obj_align(txt_pot, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -8, -25); 	// Alinha ao centro

		// Area de Refletida
		static lv_style_t style_txt_ref;
		lv_style_copy(&style_txt_ref, &lv_style_plain);
		style_txt_ref.text.font = &lv_font_eurostile_24;
		style_txt_ref.text.letter_space = 1;
		style_txt_ref.text.line_space = 1;
		style_txt_ref.text.color = LV_COLOR_WHITE;

		txt_ref = lv_label_create(Tela_Principal, NULL);
		lv_obj_set_style(txt_ref, &style_txt_ref); 							// Configura o estilo criado
		lv_label_set_long_mode(txt_ref, LV_LABEL_LONG_EXPAND); 				// Quebra as linhas longas
		lv_label_set_recolor(txt_ref, true); 								// Ativa recolorizar por comandos no texto
		lv_label_set_align(txt_ref, LV_ALIGN_IN_BOTTOM_LEFT); 				// Centraliza linhas alinhadas
		lv_label_set_text(txt_ref, "REF:");
		lv_obj_set_width(txt_ref, 300); 									// Configuura o comprimento
		lv_obj_align(txt_ref, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 5, -2); 		//Alinha ao centro

		txt_swr = lv_label_create(Tela_Principal, NULL);
		lv_obj_set_style(txt_swr, &style_txt_ref); 							// Configura o estilo criado
		lv_label_set_long_mode(txt_swr, LV_LABEL_LONG_BREAK); 				// Quebra as linhas longas
		lv_label_set_recolor(txt_swr, true); 								// Ativa recolorizar por comandos no texto
		lv_label_set_align(txt_swr, LV_ALIGN_IN_BOTTOM_RIGHT); 				// Centraliza linhas alinhadas
		sprintf(buffer, "%0.1fW", swr);
		lv_label_set_text(txt_swr, buffer);
		lv_obj_set_width(txt_swr, 300); 									// Configuura o comprimento
		lv_obj_align(txt_swr, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -8, -2); 		//Alinha ao centro
	}
}

void print_audio(u_int8_t stereo, u_int8_t audio)
{
	// Create a new style
	static lv_style_t style_txt;
	lv_style_copy(&style_txt, &lv_style_plain);
	style_txt.text.font = &lv_font_eurostile_10;
	style_txt.text.letter_space = 1;
	style_txt.text.line_space = 1;
	style_txt.text.color = LV_COLOR_WHITE;

	// Print Stereo-Mono
	img_stereo = lv_img_create(Tela_Principal, NULL); 					// Crate an image object
	lv_obj_t * txt_stereo = lv_label_create(Tela_Principal, NULL);
	lv_obj_set_style(txt_stereo, &style_txt);                    		// Set the created style
	lv_label_set_long_mode(txt_stereo, LV_LABEL_LONG_EXPAND);     		// Break the long lines
	lv_label_set_recolor(txt_stereo, true);                      		// Enable re-coloring by commands in the text
	lv_label_set_align(txt_stereo, LV_ALIGN_IN_TOP_LEFT);       		// Center aligned lines
	if(stereo) {
		lv_label_set_text(txt_stereo, "ST");
#if	LV_USE_FILESYSTEM
		lv_img_set_src(img_stereo, "P:/EX15-XT/img/stereo_vd.bin");  	// Set the created file as image (a red flower icon)
#else
		lv_img_set_src(img_stereo, &stereo_vd);
#endif
		lv_obj_set_pos(img_stereo, 28, 59); 							// Set the positions
	}
	else {
		lv_label_set_text(txt_stereo, "Mo");
#if	LV_USE_FILESYSTEM
		lv_img_set_src(img_stereo, "P:/EX15-XT/img/stereo_off.bin");  	// Set the created file as image (a red flower icon)
#else
		lv_img_set_src(img_stereo, &stereo_off);
#endif
		lv_obj_set_pos(img_stereo, 28, 59); 							// Set the positions
	}
	lv_obj_set_width(txt_stereo, 300);                           		// Set a width
	lv_obj_align(txt_stereo, NULL, LV_ALIGN_IN_TOP_LEFT, 6, 61);      	// Align to center
	// Print Audio
	lv_obj_t * txt_audio = lv_label_create(Tela_Principal, NULL);
	lv_obj_set_style(txt_audio, &style_txt);                    		// Set the created style
	lv_label_set_long_mode(txt_audio, LV_LABEL_LONG_EXPAND);     		// Break the long lines
	lv_label_set_recolor(txt_audio, true);                      		// Enable re-coloring by commands in the text
	lv_label_set_align(txt_audio, LV_ALIGN_IN_TOP_LEFT);       			// Center aligned lines
	lv_label_set_text(txt_audio, IndexAudio[audio]);
	lv_obj_set_width(txt_audio, 300);                           		// Set a width
	lv_obj_align(txt_audio, NULL, LV_ALIGN_IN_TOP_LEFT, 55, 61);      	// Align to center
}

void print_temperatura(float temp)
{
	temp = 43.3f;
	// Create a new style
	static lv_style_t style_txt;
	lv_style_copy(&style_txt, &lv_style_plain);
	style_txt.text.font = &lv_font_eurostile_10;
	style_txt.text.letter_space = 1;
	style_txt.text.line_space = 1;
	style_txt.text.color = LV_COLOR_WHITE;

	// Print Temperatura
	txt_temp = lv_label_create(Tela_Principal, NULL);
	lv_obj_set_style(txt_temp, &style_txt);                    		// Set the created style
	lv_label_set_long_mode(txt_temp, LV_LABEL_LONG_EXPAND);     	// Break the long lines
	lv_label_set_recolor(txt_temp, true);                      		// Enable re-coloring by commands in the text
	lv_label_set_align(txt_temp, LV_ALIGN_IN_TOP_LEFT);       		// Center aligned lines
	sprintf(buffer, "TP:%0.1f°", temp);
	lv_label_set_text(txt_temp, buffer);
	lv_obj_align(txt_temp, NULL, LV_ALIGN_IN_TOP_LEFT, 109, 61);    // Align to center
}

void print_status(uint32_t falha)
{
	lv_obj_t * cont;

	// Create a new style
	static lv_style_t style_txt;
	lv_style_copy(&style_txt, &lv_style_plain);
	style_txt.text.font = &lv_font_eurostile_10;
	style_txt.text.letter_space = 1;
	style_txt.text.line_space = 1;
	style_txt.text.color = LV_COLOR_WHITE;
	style_txt.body.main_color = LV_COLOR_BLUE;
	style_txt.body.grad_color = LV_COLOR_BLUE;

	cont = lv_cont_create(Tela_Principal, NULL);
	lv_obj_set_style(cont, &style_txt);
	lv_obj_set_size(cont, 30, 12);
	//lv_obj_set_event_cb(cont, main_screen_event);   // Assign an event callback
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 127, 45);
	lv_cont_set_layout(cont, LV_LAYOUT_COL_M);

	lv_obj_t * label;
	label = lv_label_create(cont, NULL);
	if(!falha) {
		lv_label_set_text(label, "OK");
	}
	else {
		style_txt.body.main_color = LV_COLOR_RED;
		style_txt.body.grad_color = LV_COLOR_RED;
		lv_label_set_text(label, "FAIL");
		print_alarme(falha);
	}
}

void print_alarme(uint32_t falha)
{
	lv_obj_t * cont;

	// Create a new style
	static lv_style_t style_cont;
	lv_style_copy(&style_cont, &lv_style_plain);
	style_cont.text.font = &lv_font_eurostile_24;
	style_cont.text.letter_space = 1;
	style_cont.text.line_space = 1;
	style_cont.text.color = LV_COLOR_WHITE;
	style_cont.body.main_color = LV_COLOR_RED;
	style_cont.body.grad_color = LV_COLOR_RED;
	style_cont.body.opa = LV_OPA_50;

	cont = lv_cont_create(Tela_Principal, NULL);
	lv_obj_set_style(cont, &style_cont);
	lv_obj_set_size(cont, 154, 50);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 3, 75);
	lv_cont_set_layout(cont, LV_LAYOUT_OFF);
	cod_falha = lv_label_create(cont, NULL);
	lv_label_set_long_mode(cod_falha, LV_LABEL_LONG_EXPAND);     	// Break the long lines

	lv_label_set_recolor(cod_falha, true); 							// Ativa recolorizar por comandos no texto
	lv_obj_set_width(cod_falha, 150);
	lv_obj_align(cod_falha, cont, LV_ALIGN_IN_BOTTOM_MID, -10, -24 );

	desc_falha = lv_label_create(cont, NULL);
	lv_label_set_long_mode(desc_falha, LV_LABEL_LONG_SROLL_CIRC);     	// Break the long lines
	lv_label_set_recolor(desc_falha, true); 							// Ativa recolorizar por comandos no texto
	lv_obj_set_width(desc_falha, 150);
	lv_obj_align(desc_falha, cont, LV_ALIGN_IN_BOTTOM_MID, 0, -2);

    switch(falha) {
    	case FAIL_PLLLOCK:
    		lv_label_set_text(cod_falha, "Fail 01");
    		lv_label_set_text(desc_falha, "PLL NO LOCK");
    		break;
    	case FAIL_RFL:
    		lv_label_set_text(cod_falha, "Fail 02");
    		lv_label_set_text(desc_falha, "Reflected High");
    		break;
    	case FAIL_FOLD:
    		lv_label_set_text(cod_falha, "Fail 03");
    		lv_label_set_text(desc_falha, "Foldback");
    		break;
    	case FAIL_FOLDIPA:
    		lv_label_set_text(cod_falha, "Fail 04");
    		lv_label_set_text(desc_falha, "Foldback IPA");
    		break;
    	case FAIL_IPA:
    		lv_label_set_text(cod_falha, "Fail 05");
    		lv_label_set_text(desc_falha, "IPA High");
    		break;
    	case FAIL_VIN:
    		lv_label_set_text(cod_falha, "Fail 06");
    		lv_label_set_text(desc_falha, "VIN Fail");
    		break;
    	case FAIL_SWR:
    		lv_label_set_text(cod_falha, "Fail 07");
    		lv_label_set_text(desc_falha, "No Antenna");
    		break;
    }
}


void create_vumeter(void)
{
	// Area do VU MPX
	// Indicador OFF
	lv_style_copy(&style_indic_off, &lv_style_pretty);
	style_indic_off.body.radius = 0;
	style_indic_off.body.grad_color = LV_COLOR_MAKE(105, 105, 105);
	style_indic_off.body.main_color = LV_COLOR_MAKE(105, 105, 105);
	style_indic_off.body.border.color = LV_COLOR_MAKE(105, 105, 105);
	// Indicador Verde
	lv_style_copy(&style_indic_vd, &lv_style_pretty);
	style_indic_vd.body.radius = 0;
	style_indic_vd.body.grad_color = LV_COLOR_LIME;
	style_indic_vd.body.main_color = LV_COLOR_LIME;
	style_indic_vd.body.border.color = LV_COLOR_LIME;
	// Indicador CYAN
	lv_style_copy(&style_indic_cy, &lv_style_pretty);
	style_indic_cy.body.radius = 0;
	style_indic_cy.body.grad_color = LV_COLOR_CYAN;
	style_indic_cy.body.main_color = LV_COLOR_CYAN;
	style_indic_cy.body.border.color = LV_COLOR_CYAN;
	// Indicador Amarelo
	lv_style_copy(&style_indic_am, &lv_style_pretty);
	style_indic_am.body.radius = 0;
	style_indic_am.body.grad_color = LV_COLOR_YELLOW;
	style_indic_am.body.main_color = LV_COLOR_YELLOW;
	style_indic_am.body.border.color = LV_COLOR_YELLOW;
	// Indicador Vermelho
	lv_style_copy(&style_indic_vm, &lv_style_pretty);
	style_indic_vm.body.radius = 0;
	style_indic_vm.body.grad_color = LV_COLOR_RED;
	style_indic_vm.body.main_color = LV_COLOR_RED;
	style_indic_vm.body.border.color = LV_COLOR_RED;

	// Create a default bar
	for(uint8_t x = 0; x < 20; x++) {
		bar[x] = lv_bar_create(Tela_Principal, NULL);
		lv_obj_set_size(bar[x], 3, 12);
		lv_bar_set_style(bar[x], LV_BAR_STYLE_BG, &style_indic_off);
		lv_bar_set_style(bar[x], LV_BAR_STYLE_INDIC, &style_indic_off);
		lv_obj_align(bar[x], NULL, LV_ALIGN_IN_TOP_LEFT, vumeter_pos_x[x], 43);
		lv_bar_set_value(bar[x], 100, 0);
	}
}

void ButtonEventTelaPrincipal(uint8_t event, uint8_t tipo, uint8_t id)
{
	if(event == 0x10) {
		if(tipo == 0x01) {	// Single Click
			//lv_event_send(img_fundo, LV_EVENT_APPLY, NULL);
            lv_task_del(Task_Principal);
    		lv_obj_del(Tela_Principal);
    		screen_sel();
		}
	}
}

/*
static void main_screen_event(lv_obj_t * obj, lv_event_t event)
{
    switch(event) {
        case LV_EVENT_APPLY:
            //printf("Released\n");
            lv_task_del(Task_Principal);
    		lv_obj_del(Tela_Principal);
    		screen_sel();
            break;
    }
}
*/

static void update_vumeter_mpx(uint32_t value)
{
	uint32_t x;

		if(value > Last_mpx) {
			for(x = Last_mpx; x <= value; x++) {
				if(x == 13) {
					lv_bar_set_style(bar[x], LV_BAR_STYLE_BG, &style_indic_cy);
					lv_bar_set_style(bar[x], LV_BAR_STYLE_INDIC, &style_indic_cy);
				}
				else if(x == 14 || x == 15 || x == 16) {
					lv_bar_set_style(bar[x], LV_BAR_STYLE_BG, &style_indic_am);
					lv_bar_set_style(bar[x], LV_BAR_STYLE_INDIC, &style_indic_am);
				}
				else if(x == 17 || x == 18 || x == 19) {
					lv_bar_set_style(bar[x], LV_BAR_STYLE_BG, &style_indic_vm);
					lv_bar_set_style(bar[x], LV_BAR_STYLE_INDIC, &style_indic_vm);
				}
				else {
					lv_bar_set_style(bar[x], LV_BAR_STYLE_BG, &style_indic_vd);
					lv_bar_set_style(bar[x], LV_BAR_STYLE_INDIC, &style_indic_vd);
				}
			}
		}
		else {
			for(x = value; x <= Last_mpx; x++) {
				lv_bar_set_style(bar[x], LV_BAR_STYLE_BG, &style_indic_off);
				lv_bar_set_style(bar[x], LV_BAR_STYLE_INDIC, &style_indic_off);
			}
		}
		Last_mpx = value;
}

static void update_main_screen(lv_task_t * param)
{
	if(!falha) {
		// Potencia Direta
		lv_label_set_text(txt_fwd, "FWD:");
		sprintf(buffer, "%0.1fW", forward);
		lv_label_set_text(txt_pot, buffer);
		// Refletida
		lv_label_set_text(txt_ref, "REF:");
		sprintf(buffer, "%0.1fW", reflected);
		lv_label_set_text(txt_swr, buffer);
	}
	// Temperatura
	sprintf(buffer, "TP:%0.1f°", temperatura);
//	lv_label_set_text(txt_temp, buffer);

	// Status
    switch(falha) {
    	case FAIL_PLLLOCK:
    		lv_label_set_text(cod_falha, "Fail 01");
    		lv_label_set_text(desc_falha, "PLL NO LOCK");
    		break;
    	case FAIL_RFL:
    		lv_label_set_text(cod_falha, "Fail 02");
    		lv_label_set_text(desc_falha, "Reflected High");
    		break;
    	case FAIL_FOLD:
    		lv_label_set_text(cod_falha, "Fail 03");
    		lv_label_set_text(desc_falha, "Foldback");
    		break;
    	case FAIL_FOLDIPA:
    		lv_label_set_text(cod_falha, "Fail 04");
    		lv_label_set_text(desc_falha, "Foldback IPA");
    		break;
    	case FAIL_IPA:
    		lv_label_set_text(cod_falha, "Fail 05");
    		lv_label_set_text(desc_falha, "IPA High");
    		break;
    	case FAIL_VIN:
    		lv_label_set_text(cod_falha, "Fail 06");
    		lv_label_set_text(desc_falha, "VIN Fail");
    		break;
    	case FAIL_SWR:
    		lv_label_set_text(cod_falha, "Fail 07");
    		lv_label_set_text(desc_falha, "No Antenna");
    		break;
    }
	// Vu-Meter MPX
    update_vumeter_mpx(mpx);
	if(!sent_hor) {
		mpx++;
		if(mpx >= 20) {
			sent_hor = 1;
			mpx--;
		}
	}
	else {
		mpx--;
		if(mpx == 0) {
			sent_hor = 0;
		}
	}
}
