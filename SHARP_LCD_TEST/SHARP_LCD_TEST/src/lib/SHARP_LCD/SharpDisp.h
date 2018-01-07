/*
 * SharpDisp.h
 *
 * Created: 05/01/2018 18:16:26
 *  Author: carlos
 */ 


#ifndef SHARPDISP_H_
#define SHARPDISP_H_

#include <ioport.h>
#include <delay.h>

// LCD commands - Note: the bits are reversed per the memory LCD data
// sheets because of the order the bits are shifted out in the SPI
// port.
#define MLCD_WRITE_LINE_CMD 0x80 //MLCD write line command
#define MLCD_CLEAR_MEMORY_CMD 0x20 //MLCD clear memory command
#define MLCD_NOP_CMD 0x00 //MLCD NOP command (used to switch VCOM)
//defines the VCOM bit in the command word that goes to the LCD
#define VCOM_HI 0x40
#define VCOM_LO 0x00

struct SharpLCD {
	ioport_pin_t _cs;
	ioport_pin_t _mosi;
	ioport_pin_t _clk;
	uint8_t _width;
	uint8_t _height;
	uint8_t _rotation;
	
	uint8_t *sharplcd_buffer;
};
// variable to keep track of VCOM bit
uint16_t vcom;

// private functions
void sendbyte(uint8_t data);
void sendbyteLSB(uint8_t data);

// public functions
void sharp_drawPixel(uint8_t x, uint8_t y, bool color);
uint8_t sharp_getPixel(uint8_t x, uint8_t y);

// create SharpLCD struct and initialize it
bool sharp_begin(ioport_pin_t cs, ioport_pin_t mosi, ioport_pin_t clk, uint8_t width, uint8_t height);
// clear the display
void sharp_clearDisplay(void);
// refresh actual display content
void sharp_refreshDisplay(void);
// toggle vcom via serial
void sharp_toggleVcom(void);
// load buffered data to the display
void sharp_updateDisplay(void);



#endif /* SHARPDISP_H_ */