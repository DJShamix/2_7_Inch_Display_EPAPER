/*****************************************************************************
* | File      	:   EPD_2IN7b_test.c
* | Author      :   Waveshare team
* | Function    :   2.7inch e-paper b test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-11
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_Test.h"
#include "../e-Paper/EPD_2in7b.h"

int EPD_init(){

	DEV_Module_Init();

	printf("e-Paper Init and Clear...\r\n");
	EPD_2IN7B_Init();
//	EPD_2IN7B_Clear();
	DEV_Delay_ms(500);

	return 0;

}

int EPD_test(void)
{
    printf("EPD_2IN7B_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN7B_Init();
    EPD_2IN7B_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *BlackImage, *RedImage;
    UWORD Imagesize = ((EPD_2IN7B_WIDTH % 8 == 0)? (EPD_2IN7B_WIDTH / 8 ): (EPD_2IN7B_WIDTH / 8 + 1)) * EPD_2IN7B_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((RedImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
    printf("NewImage:BlackImage and RedImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 270, WHITE);
    Paint_NewImage(RedImage, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 270, WHITE);

    //Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);

#if 1   // show image for array   
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_DrawBitMap(gImage_2in7b_Black);
    Paint_SelectImage(RedImage);
    Paint_DrawBitMap(gImage_2in7b_Red);
    EPD_2IN7B_Display(BlackImage, RedImage);
    DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
    /*Horizontal screen*/
    //1.Draw black image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 110, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "΢ѩ����", &Font24CN, WHITE, BLACK);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    //2.Draw red image
    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);
    Paint_DrawCircle(160, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(210, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_CN(130, 0,"���abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);

    printf("EPD_Display\r\n");
    EPD_2IN7B_Display(BlackImage, RedImage);
    DEV_Delay_ms(2000);
#endif

    printf("Clear...\r\n");
    EPD_2IN7B_Clear();

    printf("Goto Sleep...\r\n");
    EPD_2IN7B_Sleep();
    free(BlackImage);
    BlackImage = NULL;

    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}


enum screen_modes{

	screen_loading,
	screen_log_in_promnt,
	screen_warning_unknown_contract,
	screen_wallets_list,
	screen_display_wallet_info,
	screen_enter_password,
	screen_scan_qr,
	screen_remember_password

};



uint8_t EPD_Draw_Enter_PIN_Screen(void){

	//Create a new image cache named IMAGE_BW and fill it with white
	UBYTE *BlackImage, *RedImage;
	UWORD Imagesize = ((EPD_2IN7B_WIDTH % 8 == 0)? (EPD_2IN7B_WIDTH / 8 ): (EPD_2IN7B_WIDTH / 8 + 1)) * EPD_2IN7B_HEIGHT;
	//UWORD Imagesize = EPD_2IN7B_WIDTH * EPD_2IN7B_HEIGHT;

	if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
		printf("Failed to apply for black memory...\r\n");
		return -1;
	}
	if((RedImage = (UBYTE *)malloc(Imagesize)) == NULL) {
		printf("Failed to apply for red memory...\r\n");
		return -1;
	}
	printf("NewImage:BlackImage and RedImage\r\n");
	Paint_NewImage(BlackImage, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0, WHITE); // rot - 270
	Paint_NewImage(RedImage, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0, WHITE);

//	// Clear Screen
//	Paint_SelectImage(BlackImage);
//	Paint_Clear(WHITE);
//	Paint_SelectImage(RedImage);
//	Paint_Clear(WHITE);
//	EPD_2IN7B_Display(BlackImage, RedImage);
//	//EPD_2IN7B_Sleep();
//	DEV_Delay_ms(5000);
//
//	// Display Intro screen
//	Paint_SelectImage(BlackImage);
//	Paint_SetRotate(0);
//	Paint_DrawBitMap_Paste(gImage_Loading_logo, 0, 0, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0);
//	EPD_2IN7B_Display(BlackImage, RedImage);
//	DEV_Delay_ms(5000);
//
//	// Clear Screen
//	Paint_SelectImage(BlackImage);
//	Paint_Clear(WHITE);
//	Paint_SelectImage(RedImage);
//	Paint_Clear(WHITE);
//	EPD_2IN7B_Display(BlackImage, RedImage);
//	DEV_Delay_ms(2000);
//
//	// Display Lock screen
//	Paint_SelectImage(BlackImage);
//	Paint_SetRotate(0);
//	Paint_DrawBitMap_Paste(gImage_Lock_screen_promnt, 0, 0, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0);
//	EPD_2IN7B_Display(BlackImage, RedImage);
//	DEV_Delay_ms(2000);
//
//	// Display Keyboard screen
//	Paint_SelectImage(BlackImage);
//	Paint_SetRotate(0);
//	Paint_DrawBitMap_Paste(gImage_Lock_screen_promnt_keyboard, 0, 0, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0);
//	EPD_2IN7B_Display(BlackImage, RedImage);
//	//EPD_2IN7B_Sleep();
//	DEV_Delay_ms(2000);
//
//	// Display Remember Phrase screen
//	Paint_SelectImage(BlackImage);
//	Paint_SetRotate(0);
//	Paint_DrawBitMap_Paste(gImage_Lock_screen_promnt_remember_phrase, 0, 0, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0);
//	EPD_2IN7B_Display(BlackImage, RedImage);
//	EPD_2IN7B_Sleep();
//	DEV_Delay_ms(5000);


//	// Clear Screen
//	Paint_SelectImage(BlackImage);
//	Paint_Clear(WHITE);
//	Paint_SelectImage(RedImage);
//	Paint_Clear(WHITE);
//	EPD_2IN7B_Display(BlackImage, RedImage);
//	DEV_Delay_ms(500);
//
//	//Display QR
//	Paint_SelectImage(BlackImage);
//	Paint_SetRotate(0);
//	Paint_DrawBitMap_Paste(gImage_QR_Scan, 0, 0, EPD_2IN7B_WIDTH, EPD_2IN7B_HEIGHT, 0);
//	Paint_SelectImage(BlackImage);
//	create_test_qr(26, 75);
//	EPD_2IN7B_Display(BlackImage, RedImage);

	free(BlackImage);
	BlackImage = NULL;
	free(RedImage);
	RedImage = NULL;

	Paint_Clear(WHITE);
	//HAL_Delay(5000);
	EPD_2IN7B_Sleep();
	DEV_Module_Exit();

	return 0;
}


