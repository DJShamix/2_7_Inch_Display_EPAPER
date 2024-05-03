/*
 * EPD_Display_controller.c
 *
 *  Created on: Apr 29, 2024
 *      Author: Shamil Valiullin
 */


#include "EPD_Display_controller.h"

#include "../Examples/EPD_Test.h"
#include "../e-Paper/EPD_2in7b.h"

#include "portable.h"

struct EPD_Controller_params EPD_params;

#define MAX_STATES 4
#define MAX_INPUT_VAL 2
typedef void (*func_type)(void);
func_type functions[MAX_STATES][MAX_INPUT_VAL] = {
		{ &EPD_Draw_Logo_Screen, NULL },
		{ &EPD_Draw_Lock_Screen, &EPD_Draw_Lock_Keyboard_Screen },
		{ &EPD_Draw_QR_Screen, &EPD_Draw_Remember_Phrase_Screen },
		{ &EPD_Draw_Lock_Screen, NULL },
		//...
};


void EPD_init_states_machine(void){

	EPD_params.screen_state = 0;
	EPD_params.screen_substate = 0;
	EPD_params.display_need_update = 1;

}


uint8_t EPD_Draw_Screen(void){

	// Skip display update if non needed
	if(!EPD_params.display_need_update)
		return -1;

	// Wakeup EPD
	EPD_init();

	//Create a new image cache named IMAGE_BW and fill it with white
	UBYTE *BlackImage, *RedImage;
	UWORD Imagesize = ((EPD_2IN7B_WIDTH % 8 == 0)? (EPD_2IN7B_WIDTH / 8 ): (EPD_2IN7B_WIDTH / 8 + 1)) * EPD_2IN7B_HEIGHT;
	//UWORD Imagesize = EPD_2IN7B_WIDTH * EPD_2IN7B_HEIGHT;

	if((BlackImage = (UBYTE *)pvPortMalloc(Imagesize)) == NULL) {
		printf("Failed to apply for black memory...\r\n");
		return -1;
	}
	if((RedImage = (UBYTE *)pvPortMalloc(Imagesize)) == NULL) {
		printf("Failed to apply for red memory...\r\n");
		return -1;
	}
	printf("NewImage:BlackImage and RedImage\r\n");
	Paint_NewImage(BlackImage, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0, WHITE); // rot - 270
	Paint_NewImage(RedImage, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0, WHITE);

	// Clear Screen
	static uint8_t clear_screen = 1;
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);
	Paint_SelectImage(RedImage);
	Paint_Clear(WHITE);
	if(clear_screen){
		EPD_2IN7B_Display(BlackImage, RedImage);
		DEV_Delay_ms(3000);
		clear_screen = 0;
	}
	//EPD_2IN7B_Display(BlackImage, RedImage);
	//DEV_Delay_ms(100);

	// Prepare Display to load information
	Paint_SelectImage(BlackImage);
	Paint_SetRotate(0);


	/* Display screen information,
	   according to selected mode
	*/
	EPD_diplay_screen_info();


	// Display information on display
	EPD_2IN7B_Display(BlackImage, RedImage);


	// Free allocated RAM memory
	vPortFree(BlackImage);
	BlackImage = NULL;
	vPortFree(RedImage);
	RedImage = NULL;

	// Enter EPD sleep mode
	EPD_2IN7B_Sleep();
	DEV_Module_Exit();

	EPD_params.display_need_update = 0;

	return 0;
}


void EPD_diplay_screen_info(void){

	func_type func_ptr = functions[EPD_params.screen_state][EPD_params.screen_substate];
	if (func_ptr != NULL) {
	    func_ptr(); // Call the function through the pointer
	} else {
	    printf("Function pointer is NULL.\n");
	}

}
//------------------------


void EPD_change_state(void){

	static uint32_t last_display_update;

	if(EPD_params.display_need_update){
		last_display_update = HAL_GetTick();
		return;
	}

	if(last_display_update + 15000 <= HAL_GetTick()){

		switch(EPD_params.screen_state){

			case 0:
				EPD_params.screen_state = 1;
				EPD_params.screen_substate = 0;
				break;

			case 1:{

				switch(EPD_params.screen_substate){
					case 0:
						EPD_params.screen_state = 1;
						EPD_params.screen_substate = 1;
						break;

					case 1:
						EPD_params.screen_state = 2;
						EPD_params.screen_substate = 0;
						break;
					}

				break;
			}

		case 2:
			switch(EPD_params.screen_substate){
				case 0:
					EPD_params.screen_state = 2;
					EPD_params.screen_substate = 1;
					break;

				case 1:
					EPD_params.screen_state = 3;
					EPD_params.screen_substate = 0;
					break;
				}

			break;

		case 3:
			//EPD_params.screen_state = 3;
			//EPD_params.screen_substate = 0;
			break;

		}

		check_if_display_state_changed();

		last_display_update = HAL_GetTick();
	}

}


void check_if_display_state_changed(void){

	if(EPD_params.screen_state != EPD_params.prev_screen_state || \
			EPD_params.screen_substate != EPD_params.prev_screen_substate){

		EPD_params.prev_screen_state = EPD_params.screen_state;
		EPD_params.prev_screen_substate = EPD_params.screen_substate;
		EPD_params.display_need_update = 1;

	}
}



void EPD_Draw_Logo_Screen(void){

	Paint_SetRotate(0);
	Paint_DrawBitMap_Paste(gImage_Loading_logo, 0, 0, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0);

}
//------------------------


void EPD_Draw_Lock_Screen(void){

	Paint_SetRotate(0);
	Paint_DrawBitMap_Paste(gImage_Lock_screen_promnt, 0, 0, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0);

}
//------------------------


void EPD_Draw_Lock_Keyboard_Screen(void){

	Paint_SetRotate(0);
	Paint_DrawBitMap_Paste(gImage_Lock_screen_promnt_keyboard, 0, 0, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0);

}
//------------------------


void EPD_Draw_Remember_Phrase_Screen(void){

	Paint_SetRotate(0);
	Paint_DrawBitMap_Paste(gImage_Lock_screen_promnt_remember_phrase, 0, 0, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0);

}
//------------------------

void EPD_Draw_QR_Screen(void){

	//Display QR
	Paint_SetRotate(0);
	Paint_DrawBitMap_Paste(gImage_QR_Scan, 0, 0, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0);
	create_test_qr(26, 75);

}
//------------------------
