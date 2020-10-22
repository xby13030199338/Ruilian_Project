#include "init.h"

void spi_config(void)
{
    spi_parameter_struct  spi_init_struct;
    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(LCD_SPI);
	spi_i2s_deinit(FLASH_SPI);
    spi_struct_para_init(&spi_init_struct);

    /* LCD_SPI parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_BDTRANSMIT;		
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_2;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(LCD_SPI, &spi_init_struct);
	
	/* FLASH_SPI parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;		
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_2;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
	spi_init(FLASH_SPI, &spi_init_struct);
	
	/* SPI enable */	
	spi_enable(LCD_SPI);
	spi_enable(FLASH_SPI);	
}


/*!
    \brief      configure different peripheral clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(HC595_GPIO_CLK);
    rcu_periph_clock_enable(LCD_GPIO_CLK);
	rcu_periph_clock_enable(FLASH_GPIO_CLK);
    
    rcu_periph_clock_enable(LCD_SPI_CLK);
	rcu_periph_clock_enable(FLASH_SPI_CLK);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
	/*************************GPIO of 74HC595**************************/
	//init CLK,LOCK,DATA pin
	gpio_init(HC595_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CAM_PWR\
																	| HC595_CLK\
																	| HC595_LOCK\
																	| HC595_DATA);
	
	/****************************GPIO of FLASH******************************/
	//init CLK,MOSI,MISO pin
	gpio_init(FLASH_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, FLASH_CLK\
																| FLASH_MOSI\
																| FLASH_MISO);
	//gpio_init(FLASH_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, FLASH_MISO);
	
	//init CS,RS pin
	gpio_init(FLASH_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, FLASH_CS);		
	
	/*************************GPIO of LCD************************************/
	//init SCK,MOSI pin
    gpio_init(LCD_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, LCD_SCK\
																| LCD_MOSI);
	//init CS,RS pin
	gpio_init(LCD_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LCD_CS\
																| LCD_RS);		
	
//	/*************************GPIO of LED**************************/
//	gpio_init(LCD_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);

	/*********************set SPI CS pin Hight*******************/
	LCD_CS_H;	
	FLASH_CS_H;
	
	/**************************set 595 IO Low************************/
	CAM_PWR_EN;
	HC595_DATA_L;
	HC595_CLK_L;
	HC595_LOCK_L;
}
