/*
 * EPD_Display_controller.h
 *
 *  Created on: Apr 29, 2024
 *      Author: Shamil Valiullin
 */

#ifndef EXAMPLES_EPD_DISPLAY_CONTROLLER_H_
#define EXAMPLES_EPD_DISPLAY_CONTROLLER_H_

#include "main.h"

typedef struct EPD_Controller_params{

	uint8_t screen_state;
	uint8_t screen_substate;
	uint8_t display_need_update;

	uint8_t prev_screen_state;
	uint8_t prev_screen_substate;

};

extern struct EPD_Controller_params EPD_params;

void EPD_init_states_machine(void);
uint8_t EPD_Draw_Screen(void);
void EPD_diplay_screen_info(void);
void check_if_display_state_changed(void);
void EPD_change_state(void);

void EPD_Draw_Lock_Screen(void);
void EPD_Draw_Lock_Keyboard_Screen(void);
void EPD_Draw_Remember_Phrase_Screen(void);
void EPD_Draw_QR_Screen(void);

// Screen states
void EPD_Draw_Logo_Screen(void);

#endif /* EXAMPLES_EPD_DISPLAY_CONTROLLER_H_ */
