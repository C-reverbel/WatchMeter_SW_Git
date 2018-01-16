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


// SPI driver test
#define CONF_MASTER_SPI_MODULE  SERCOM3
#define CONF_MASTER_SS_PIN      LCD_CS
#define CONF_MASTER_MUX_SETTING SPI_SIGNAL_MUX_SETTING_G
#define CONF_MASTER_PINMUX_PAD0 PINMUX_UNUSED//PINMUX_PA08D_SERCOM2_PAD0
#define CONF_MASTER_PINMUX_PAD1 PINMUX_UNUSED//PINMUX_UNUSED
#define CONF_MASTER_PINMUX_PAD2 PINMUX_PA24C_SERCOM3_PAD2//PINMUX_PA10D_SERCOM2_PAD2
#define CONF_MASTER_PINMUX_PAD3 PINMUX_PA25C_SERCOM3_PAD3//PINMUX_PA11D_SERCOM2_PAD3

#define BUF_LENGTH 20
#define SLAVE_SELECT_PIN CONF_MASTER_SS_PIN

static uint8_t wr_buffer[BUF_LENGTH] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13
};
static uint8_t rd_buffer[BUF_LENGTH];

struct spi_module spi_master_instance;
struct spi_slave_inst slave;

volatile bool transrev_complete_spi_master = false;

void configure_spi_master(void)
{
	struct spi_config config_spi_master;
	struct spi_slave_inst_config slave_dev_config;
	/* Configure and initialize software device instance of peripheral slave */
	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = SLAVE_SELECT_PIN;
	spi_attach_slave(&slave, &slave_dev_config);
	/* Configure, initialize and enable SERCOM SPI module */
	spi_get_config_defaults(&config_spi_master);
	config_spi_master.mux_setting = CONF_MASTER_MUX_SETTING;
	config_spi_master.pinmux_pad0 = CONF_MASTER_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = CONF_MASTER_PINMUX_PAD1;
	config_spi_master.pinmux_pad2 = CONF_MASTER_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = CONF_MASTER_PINMUX_PAD3;
	spi_init(&spi_master_instance, CONF_MASTER_SPI_MODULE, &config_spi_master);
	spi_enable(&spi_master_instance);
}
static void callback_spi_master( struct spi_module *const module)
{
	transrev_complete_spi_master = true;
}
void configure_spi_master_callbacks(void)
{
	spi_register_callback(&spi_master_instance, callback_spi_master,
	SPI_CALLBACK_BUFFER_TRANSCEIVED);
	spi_enable_callback(&spi_master_instance, SPI_CALLBACK_BUFFER_TRANSCEIVED);
}


// SPI driver test



int main (void)
{
	system_init();
	delay_init();
	ioport_init(); 
	/*
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
	*/
	// SPI driver test
	configure_spi_master();
	configure_spi_master_callbacks();
	// SPI driver test
	
	
	
	while(1){
		// SPI driver test
		spi_select_slave(&spi_master_instance, &slave, true);
		spi_transceive_buffer_job(&spi_master_instance, wr_buffer,rd_buffer,BUF_LENGTH);
		while (!transrev_complete_spi_master) {
		}
		transrev_complete_spi_master = false;
		spi_select_slave(&spi_master_instance, &slave, false);
		// SPI driver test
		
		delay_ms(1);
// 		sharp_updateDisplay();
// 		delay_ms(100);
		
	}
}
