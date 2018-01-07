/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "lib/SHARP_LCD/SharpDisp.h"

#define LCD_CS IOPORT_CREATE_PIN(IOPORT_PORTA, 22)	// pin 43
#define LCD_DI IOPORT_CREATE_PIN(IOPORT_PORTA, 23)	// pin 44
#define LCD_CLK IOPORT_CREATE_PIN(IOPORT_PORTA, 24)	// pin 45


int main (void)
{
	system_init();
	delay_init();
	ioport_init(); 
	sharp_begin(LCD_CS,LCD_DI,LCD_CLK,144,168);
	
	// pin configuration
	ioport_set_pin_dir(LCD_CS,IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LCD_DI,IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LCD_CLK,IOPORT_DIR_OUTPUT);
	// set pin level
	ioport_set_pin_level(LCD_CS,0);
	ioport_set_pin_level(LCD_DI,0);
	ioport_set_pin_level(LCD_CLK,0);
	
	
	// TEST LCD LIB
	for(int j = 0; j < 168; j++){
		for(int i = 0; i < 144; i++){
			sharp_drawPixel(i,j,(i==j?1:0));
		}
	}
	
	while(1){
		sharp_updateDisplay();
		delay_ms(100);
	}
}
