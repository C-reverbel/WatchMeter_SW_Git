/*
 * SharpDisp.c
 *
 * Created: 05/01/2018 18:16:13
 *  Author: carlos
 */ 

#include "SharpDisp.h"

#define TOGGLE_VCOM vcom = (vcom ? VCOM_LO : VCOM_HI)
#define SPI_HW 1

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif
#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b) { uint16_t t = a; a = b; b = t; }
#endif


// declare sharp memory struct
struct SharpLCD	lcd;
uint8_t clkDelay_us = 20;
// 1<<n is a costly operation -- table usu. smaller & faster
static const uint8_t set[] = {  1,  2,  4,  8,  16,  32,  64,  128 };
static const uint8_t clr[] = { (uint8_t)~1 , (uint8_t)~2 , (uint8_t)~4 , (uint8_t)~8, (uint8_t)~16, (uint8_t)~32, (uint8_t)~64, (uint8_t)~128 };



// ===== private functions =====
void sendbyte(uint8_t data){ // LCD expects LSB first
#if SPI_HW

#else
	uint8_t i = 0;

	for (i=0; i<8; i++) {
		// Make sure clock starts low
		ioport_set_pin_level(lcd._clk, 0);
		delay_us(clkDelay_us);
		if (data & 0x80) // 1000 0000
			ioport_set_pin_level(lcd._mosi, 1);
		else
			ioport_set_pin_level(lcd._mosi, 0);
		// Clock is active high
		ioport_set_pin_level(lcd._clk, 1);
		data <<= 1;
		//delay_ms(1);
		delay_us(clkDelay_us);
	}
	// Make sure clock ends low
	ioport_set_pin_level(lcd._clk, 0);
#endif
}
void sendbyteLSB(uint8_t data){// LCD expects LSB first
#if SPI_HW

#else
	uint8_t i = 0;

	for (i=0; i<8; i++)
	{
		// Make sure clock starts low
		ioport_set_pin_level(lcd._clk, 0);
		delay_us(clkDelay_us);
		if (data & 0x01) // 0000 0001
			ioport_set_pin_level(lcd._mosi, 1);
		else
			ioport_set_pin_level(lcd._mosi, 0);
		// Clock is active high
		ioport_set_pin_level(lcd._clk, 1);
		data >>= 1;
		//delay_ms(1);
		delay_us(clkDelay_us);
	}
	// Make sure clock ends low
	ioport_set_pin_level(lcd._clk, 0);
#endif
}



// ===== public functions =====
// create SharpLCD struct and initialize it
bool sharp_begin(ioport_pin_t cs, ioport_pin_t mosi, ioport_pin_t clk, uint8_t width, uint8_t height){
	vcom = 0;
	
	lcd._cs = cs;
	lcd._clk = clk;
	lcd._mosi = mosi;
	lcd._width = width;
	lcd._height = height;
	lcd._rotation = 0;
	// allocate space for the display buffer
	lcd.sharplcd_buffer = (uint8_t*)malloc((lcd._width*lcd._height)/8);
	if(!lcd.sharplcd_buffer)
	return false;
	return true;
}
// draw a pixel to the lcd buffer
void sharp_drawPixel(uint8_t x, uint8_t y, bool color)
{
	if((x < 0) || (x >= lcd._width) || (y < 0) || (y >= lcd._height)) return;

	switch(lcd._rotation) {
		case 1:
		_swap_int16_t(x, y);
		x = lcd._width  - 1 - x;
		break;
		case 2:
		x = lcd._width  - 1 - x;
		y = lcd._height - 1 - y;
		break;
		case 3:
		_swap_int16_t(x, y);
		y = lcd._height - 1 - y;
		break;
	}

	if(color)
		lcd.sharplcd_buffer[(y * lcd._width + x) / 8] |= set[x & 7];
	else
		lcd.sharplcd_buffer[(y * lcd._width + x) / 8] &= clr[x & 7];
}
// load buffered data to the display
void sharp_updateDisplay(void){
	uint16_t i, totalbytes, currentline, oldline;
	totalbytes = (lcd._width * lcd._height) / 8;

	// Send the write command
	ioport_set_pin_level(lcd._cs, 1);
	sendbyte(MLCD_WRITE_LINE_CMD | vcom);
	TOGGLE_VCOM;

	// Send the address for line 1
	oldline = currentline = 1;
	sendbyteLSB(currentline);

	// Send image buffer
	for (i=0; i<totalbytes; i++)
	{
		sendbyteLSB(lcd.sharplcd_buffer[i]);
		currentline = ((i+1)/(lcd._width/8)) + 1;
		if(currentline != oldline)
		{
			// Send end of line and address bytes
			sendbyteLSB(0x00);
			if (currentline <= lcd._height)
				sendbyteLSB(currentline);
			oldline = currentline;
		}
	}
	
	// Send another trailing 8 bits for the last line (total of 16 zeros)
	sendbyteLSB(0x00);
	ioport_set_pin_level(lcd._cs, 0);
}

uint8_t sharp_getPixel(uint8_t x, uint8_t y){return 0;}
// clear the display
void sharp_clearDisplay(void){}
// refresh actual display content
void sharp_refreshDisplay(void){}
// toggle vcom via serial
void sharp_toggleVcom(void){}