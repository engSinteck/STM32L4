/*
 * key.c
 *
 *  Created on: 4 de mai de 2019
 *      Author: Rinaldo Dos Santos
 *
 */
#include "key.h"
#include "log.h"
#include "misc.h"

extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim3;

extern uint16_t tft_pwm, pwm_rf_value;
extern long int frequencia;
extern uint8_t audio_select, stereo_sel, processador_sel, emphase_sel, clipper_sel;

// Estrututa Botoes
pushbtn_param btenter;
pushbtn bt[6];

// Inicia base de Dados Leitura de Teclado
void KeyboardInit(uint8_t mask)
{
	int i ;
	uint8_t x;

	for(x = 0; x < 6; x++) {
		// clear data
		bt[x]->old_state = bt[x]->new_state = 0;
		bt[x]->mask = mask;
		bt[x]->mode = 0;
		bt[x]->flag = 0;

		// clear log
		for(i = 0; i < 8; i++) {
			ClearLog(x, i);
		}
	}
}

/** Clear the duration parameter and the click count parameter of the button.
 */
void ClearLog(uint8_t key, uint8_t index)
{
	if(index < 8)
	{
		bt[key]->click[index] = bt[key]->duration[index] = 0;
	}
}

void KeyboardSetMode(uint8_t key, uint8_t mode, bool flag)
{
	int i;

	// clear data
	bt[key]->old_state = bt[key]->new_state = 0;
	// this looks weird but correct
	bt[key]->flag = !flag;

	// clear log
	for(i = 0; i < 8; i++)
	{
		ClearLog(key, i);
	}

	// change mode
	bt[key]->mode = mode;
}

/** Clear the duration parameter and the click count parameter of the button.
 */
void Enter_ClearLog(uint8_t index)
{
	if(index < 8)
	{
		bt[2]->click[index] = bt[2]->duration[index] = 0;
	}
}

void Enter_SetMode(uint8_t mode, bool flag)
{
	int i;

	// clear data
	bt[2]->old_state = bt[2]->new_state = 0;
	// this looks weird but correct
	bt[2]->flag = !flag;

	// clear log
	for(i = 0; i < 8; i++)
	{
		Enter_ClearLog(i);
	}

	// change mode
	bt[2]->mode = mode;
}

void KeyEnter_Routine(uint8_t key)
{
	int i;
	uint8_t diff_state;
	//uint8_t evento;

	bt[key]->new_state = PushButton_Read(key);

	// difference in the button state
	diff_state = bt[key]->old_state ^ bt[key]->new_state;

	i = 0;
	// up-down mode
	if(((bt[key]->mode >> i) & 0x01) == PUSHBTN_MODE_UDOWN) {
		// the button pressed
		if(((bt[key]->new_state >> i) & 0x01) == 0x01) {
			//logI("I - KEY Evento: State Down [0]: %d [1]: %d [2]: %d \n\r", EVT_PBTN_INPUT, (uint8_t)(i+1), PBTN_DOWN);
		}
		// button released
		else {
			// actually it has just risen
			if(((bt[key]->old_state >> i) & 0x01) == 0x01) {
				Enter_SetMode(PUSHBTN_MODE_CLICK, true);
				//logI("I - KEY Evento: Change Up [0]: %d [1]: %d [2]: %d \n\r", EVT_PBTN_INPUT, (uint8_t)(i+1), PBTN_ENDN);
			}
		}
	}
	// click mode
	else {
		// the button state changed
		if((diff_state >> i) & 0x01) {
			// (re)start duration count
			bt[key]->duration[i] = 1;

			// the button released
			if(((bt[key]->new_state >> i) & 0x01) == 0x00) {
				if(bt[key]->flag) {
					bt[key]->flag = false;
					bt[key]->duration[i] = 0;
				}
				else {
					// increase click count
					bt[key]->click[i]++;
				}
			}
		}
		// button state not changed
		else {
			// increase duration count
			if((bt[key]->duration[i] > 0) && (bt[key]->duration[i] < PUSHBTN_TO_MAX)) {
				bt[key]->duration[i]++;
			}
		}
		// triple click
		if(bt[key]->click[i] >= 3) {
			// triple click event
			//logI("I - KEY Evento: Triple Click [0]: %d [1]: %d [2]: %d \n\r", EVT_PBTN_INPUT, (uint8_t)(i+1), PBTN_TCLK);
			// clear log
			//Enter_ClearLog(i);
			bt[key]->click[i] = 0;
			bt[key]->duration[i] = 0;
			//ClearLog(key, i);
			Enter_SetMode(PUSHBTN_MODE_UDOWN, true);
		}
		// button relased and short timeout passed
		else if((bt[key]->duration[i] > PUSHBTN_TO_SHORT) &&	(((bt[key]->new_state >> i) & 0x01) == 0x00)) {
			// double click
			if(bt[key]->click[i] == 2) {
				// double click event
				//evento = PBTN_DCLK;
				//logI("I - KEY Evento: Double Click [0]: %d [1]: %d [2]: %d \n\r", EVT_PBTN_INPUT, (uint8_t)(i+1), evento);
			}
			// single click
			else {
				// single click event
				//evento = PBTN_SCLK;
				//logI("I - KEY Evento: Single Click [0]: %d [1]: %d [2]: %d \n\r", EVT_PBTN_INPUT, (uint8_t)(i+1), evento);
			}
			// clear log
			//Enter_ClearLog(i);
			bt[key]->click[i] = 0;
			bt[key]->duration[i] = 0;
			//ClearLog(key, i);
		}
		// button pressed and long timeout passed
		else if((bt[key]->duration[i] > PUSHBTN_TO_LONG) && (((bt[key]->new_state >> i) & 0x01) == 0x01)) {
			// long click event
			//logI("I - KEY Evento: Long Click [0]: %d [1]: %d [2]: %d \n\r", EVT_PBTN_INPUT, (uint8_t)(i+1), PBTN_LCLK);
			// clear log
			//Enter_ClearLog(i);
			bt[key]->click[i] = 0;
			bt[key]->duration[i] = 0;
			//ClearLog(key, i);
			// raise flag: this will prevent false detect after long click
			bt[key]->flag = true;
		}
	}
	// update pin state
	bt[key]->old_state = bt[key]->new_state;
}

void KeyboardRead(uint8_t key)
{
	int i;
	uint8_t diff_state;
//	uint8_t evento;
	uint8_t event[EVT_QWIDTH];

	bt[key]->new_state = PushButton_Read(key);

	// difference in the button state
	diff_state = bt[key]->old_state ^ bt[key]->new_state;

	i = 0;
	// up-down mode
	if(((bt[key]->mode >> i) & 0x01)  == PUSHBTN_MODE_UDOWN) {
		// the button pressed
		if(((bt[key]->new_state >> i) & 0x01) == 0x01) {
			//logI("I - KEY Evento: State Down Key[%d]  Evt: [0]: %d [1]: %d [2]: %d \n\r", key, EVT_PBTN_INPUT, (uint8_t)(i+1), PBTN_DOWN);
			event[0] = EVT_PBTN_INPUT;
			event[1] = (uint8_t)key;
			event[2] = PBTN_DOWN;
			Evt_EnQueue(event);
			//buzzer(3);
		}
		// button released
		else {
			// actually it has just risen
			if(((bt[key]->old_state >> i) & 0x01) == 0x01) {
				KeyboardSetMode(key, PUSHBTN_MODE_CLICK, true);
				//logI("I - KEY Evento: Change Up Key[%d]  Evt: [0]: %d [1]: %d [2]: %d \n\r", key, EVT_PBTN_INPUT, (uint8_t)(i+1), PBTN_ENDN);
				event[0] = EVT_PBTN_INPUT;
				event[1] = (uint8_t)key;
				event[2] = PBTN_ENDN;
				Evt_EnQueue(event);
				//buzzer(3);
			}
		}
	}
	// click mode
	else {
		// the button state changed
		if((diff_state >> i) & 0x01) {
			// (re)start duration count
			bt[key]->duration[i] = 1;

			// the button released
			if(((bt[key]->new_state >> i) & 0x01) == 0x00) {
				if(bt[key]->flag) {
					bt[key]->flag = false;
					bt[key]->duration[i] = 0;
				}
				else {
					// increase click count
					bt[key]->click[i]++;
				}
			}
		}
		// button state not changed
		else {
			// increase duration count
			if((bt[key]->duration[i] > 0) && (bt[key]->duration[i] < PUSHBTN_TO_MAX)) {
				bt[key]->duration[i]++;
			}
		}
		// triple click
		if(bt[key]->click[i] >= 3) {
			// triple click event
			event[0] = EVT_PBTN_INPUT;
			event[1] = (uint8_t)key;
			event[2] = PBTN_TCLK;
			Evt_EnQueue(event);
			//buzzer(3);
			//logI("I - KEY Evento: Triple Click Key[%d]  Evt: [0]: %d [1]: %d [2]: %d \n\r", key, EVT_PBTN_INPUT, (uint8_t)(i+1), PBTN_TCLK);
			// clear log
			//ClearLog(key, i);
			bt[key]->click[i] = 0;
			bt[key]->duration[i] = 0;
			//KeyboardSetMode(key, PUSHBTN_MODE_UDOWN, true);
		}
		// button relased and short timeout passed
		else if((bt[key]->duration[i] > PUSHBTN_TO_SHORT) &&	(((bt[key]->new_state >> i) & 0x01) == 0x00)) {
			// double click
			if(bt[key]->click[i] == 2) {
				// double click event
				//evento = PBTN_DCLK;
				event[0] = EVT_PBTN_INPUT;
				event[1] = (uint8_t)key;
				event[2] = PBTN_DCLK;
				Evt_EnQueue(event);
				//buzzer(3);
				//logI("I - KEY Evento: Double Click Key[%d]  Evt: [0]: %d [1]: %d [2]: %d \n\r", key, EVT_PBTN_INPUT, (uint8_t)(i+1), event[2]);
			}
			// single click
			else {
				// single click event
				//evento = PBTN_SCLK;
				event[0] = EVT_PBTN_INPUT;
				event[1] = (uint8_t)key;
				event[2] = PBTN_SCLK;
				Evt_EnQueue(event);
				//buzzer(3);
				//logI("I - KEY Evento: Single Click Key[%d]  Evt: [0]: %d [1]: %d [2]: %d \n\r", key, EVT_PBTN_INPUT, (uint8_t)(i+1), event[2]);
			}
			// clear log
			//ClearLog(key, i);
			bt[key]->click[i] = 0;
			bt[key]->duration[i] = 0;
		}
		// button pressed and long timeout passed
		else if((bt[key]->duration[i] > PUSHBTN_TO_LONG) && (((bt[key]->new_state >> i) & 0x01) == 0x01)) {
			// long click event
			//evento = PBTN_SCLK;
			event[0] = EVT_PBTN_INPUT;
			event[1] = (uint8_t)key;
			event[2] = PBTN_LCLK;
			Evt_EnQueue(event);
			//buzzer(3);
			//logI("I - KEY Evento: Long Click Key[%d]  Evt: [0]: %d [1]: %d [2]: %d \n\r", key, EVT_PBTN_INPUT, (uint8_t)(i+1), PBTN_LCLK);
			// clear log
			//ClearLog(key, i);
			bt[key]->click[i] = 0;
			bt[key]->duration[i] = 0;
			// raise flag: this will prevent false detect after long click
			bt[key]->flag = true;
		}
	}
	// update pin state
	bt[key]->old_state = bt[key]->new_state;
}


uint8_t PushButton_Read(uint8_t key)
{
	uint8_t ret = 0x00;

	switch(key) {
		case 0:			// Down
			ret = !(HAL_GPIO_ReadPin(KEY_DN_GPIO_Port, KEY_DN_Pin));
			break;
		case 1:			// UP
			ret = !(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin));
			break;
		case 2:			// Enter
			ret = !(HAL_GPIO_ReadPin(KEY_ENTER_GPIO_Port, KEY_ENTER_Pin));
			break;
		case 3:			// Esc
			ret = !(HAL_GPIO_ReadPin(KEY_ESC_GPIO_Port, KEY_ESC_Pin));
			break;
		default:
			break;
	}
	return ret;
}

void Key_Read(void)
{
	uint8_t x;
	for(x = 0; x < 6; x++) {
		KeyboardRead(x);
	}
}

/* Rotina de Evento do Teclado */
extern void HAL_SuspendTick(void);
extern void HAL_ResumeTick(void);

#define ADVANCE_QPTR(x)     ((x+1) % EVT_QDEPTH)

static struct
{
	uint8_t buff[EVT_QDEPTH][EVT_QWIDTH];
	uint8_t head;
	uint8_t tail;
} evt_queue;


/**
 * Append a new event at the end of the queue. If the queue is full, then
 * the event is ignored and it returns with false.
 *
 * \param  event data in an array of uint8_t
 * \return false if the queue is full
 */
bool Evt_EnQueue(uint8_t *event)
{
	unsigned i;
	uint8_t next = ADVANCE_QPTR(evt_queue.head);

	// queue is full
	if(next == evt_queue.tail)
	{
		// event will be lost
		//logI("I - Evt_EnQueue Full\n\r");
		return false;
	}

	// copy event bytes into the buffer
	for(i = 0; i < EVT_QWIDTH; i++)
	{
		evt_queue.buff[evt_queue.head][i] = event[i];
	}
	// move to the next positition
	evt_queue.head = next;

	//logI("I - Evt_EnQueue OK\n\r");
	return true;
}

/**
 * Retrieve the oldest event from the queue. If the return value is false
 * the retrieved event data should be ignored. Note that the access of the
 * queue is protected by HAL_SuspendTick / Hal_ResumeTick. If any other
 * interrupt service routine were to access the queue through Evt_EnQueue,
 * corresponding interrupt should be suspended here.
 *
 * \param  event data in an array of uint8_t
 * \return false if the queue is empty
 */
bool Evt_DeQueue(uint8_t *event)
{
	uint8_t i;
	bool flag = false;

	// disable all timers
	//UsrTimer_Enable(false);

	// queue is not empty
	if(evt_queue.tail != evt_queue.head)
	{
		// copy event bytes into the buffer
		for(i = 0; i < EVT_QWIDTH; i++)
		{
			event[i] = evt_queue.buff[evt_queue.tail][i];
		}
		// move to the next position
		evt_queue.tail = ADVANCE_QPTR(evt_queue.tail);
		// set flag
		flag = true;
	}

	// enable all timers
	//UsrTimer_Enable(true);

	// return with the flag
	return flag;
}

/**
 * The tail and the head pointers are set to zero. This will invalidate all
 * the data in the queue.
 */
void Evt_InitQueue(void)
{
	// clear queue by resetting the pointers
	evt_queue.head = evt_queue.tail = 0;
}

// Eventos Teclado
void KeyboardEvent(void)
{
	uint8_t event[EVT_QWIDTH];
	const char* teclas[] = {"KEY-DOWN", "KEY-UP", "KEY-ENTER", "KEY-ESC", "KEY-ROTARY", "KEY-USER"};

	// check event queue
	if(Evt_DeQueue(event)) {
		switch(event[0]) {
			// pushbutton event ================================================
			// event[1]: button id
			// event[2]: PBTN_SCLK, _DCLK, _TCLK, _LCLK, _DOWN, _ENDN
			case EVT_PBTN_INPUT:
				if(event[2] == PBTN_SCLK) {
					if(event[1] == 0) {
						// KEY_DN FREQ--

					}
					else if(event[1] == 1) {
						// KEY_UP FREQ++

					}
					else if(event[1] == 2) {
						// KEY_ENTER PWM--

					}
					else if(event[1] == 3) {
						// KEY_ESC PWM++

					}
					logI("\r\nButton %s: single click.", teclas[event[1]]);
				}
				else if(event[2] == PBTN_LCLK) {
					if(event[1] == 0) {

					}
					else if(event[1] == 1) {
					}
					else if(event[1] == 2) {

					}
					else if(event[1] == 3) {

					}
					logI("\r\nButton %s: long click.", teclas[event[1]]);
				}
				else if(event[2] == PBTN_DCLK) {

					logI("\r\nButton %s: double click.", teclas[event[1]]);
				}
				else if(event[2] == PBTN_TCLK) {
					logI("\r\nButton %s: triple click.", teclas[event[1]]);
					//PushButton_SetMode(PUSHBTN_MODE_UDOWN, true);
					//logI("\r\n --> Switch to up-down mode.");
				}
				else if(event[2] == PBTN_DOWN) {
					logI("\r\nButton %s: is being pressed.", teclas[event[1]]);
				}
				else if(event[2] == PBTN_ENDN) {
					logI("\r\nButton %s: has been released.", teclas[event[1]]);
					//PushButton_SetMode(PUSHBTN_MODE_CLICK, true);
					//logI("\r\n --> Switch to click mode.");
				}
				break;
		}
	}
}
