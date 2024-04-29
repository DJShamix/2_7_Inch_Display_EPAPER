/*
 * qr_test.c
 *
 *  Created on: Apr 29, 2024
 *      Author: Shamil Valiullin
 */


#include "qr_test.h"
//#include "GUI/GUI_Paint.h"
//#include "e-Paper/EPD_2in7b.h"
//#include "Examples/EPD_Test.h"

void create_test_qr(int qr_offset_x, int qr_offset_y){

//	char* text = "Hello world!";
	char* text = "https://t.me/DJSham";
	QRGenerator(qr_offset_x, qr_offset_y, text);

}



