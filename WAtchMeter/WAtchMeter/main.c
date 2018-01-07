/*
 * WAtchMeter.c
 *
 * Created: 20/12/2017 16:00:00
 * Author : carlos
 */ 

#include <asf.h>

//#define RANGE_IN1 IOPORT_CREATE_PIN(IOPORT_PORTB, 10)

struct dac_module dac_instance;


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
// 	delay_init();
// 	ioport_init();
// 	ioport_set_pin_dir(RANGE_IN1,IOPORT_DIR_OUTPUT);
// 	ioport_set_pin_level(RANGE_IN1,1);

	
	uint16_t i = 0;
    while (1) 
    {

// 		delay_ms(10);
// 		ioport_toggle_pin_level(RANGE_IN1);
    }
}
