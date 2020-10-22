/*!
    \file    main.c
    \brief   master send and slave receive data use interrupt mode 

    \version 2017-12-26, V1.0.0, firmware for GD32E10x
    \version 2020-09-30, V1.1.0, firmware for GD32E10x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32e10x_it.h"
#include "gd32e10x_eval.h"
#include "init.h"

usb_core_driver cdc_acm;

void delay_us(uint16_t nus);

void delay_ms(uint16_t nms);

void delay_init(uint8_t SYSCLK);

uint8_t rxbuffer[64];
uint8_t rx_size = 64;
__IO uint16_t rxcount = 0;
__IO uint16_t rxflag = 0;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
	/* peripheral clock enable */
	rcu_config();
	usb_rcu_config();
	/* USB Timer Configure */
	usb_timer_init();
	/* GPIO configure */
	gpio_config();
	/* SPI configure */
	spi_config();

	/* USART1 interrupt configuration */

	gd_eval_com_init(EVAL_COM0, 115200);
	gd_eval_com_init(EVAL_COM1, 115200);
	//	gpio_init(GPIO_PIN_2, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	//	usart_interrupt_enable(USART0, USART_INT_TBE);
	usart_interrupt_enable(USART1, USART_INT_RBNE);
	usart_interrupt_enable(USART1, USART_INT_IDLE);
	//	usart_interrupt_enable(USART1, USART_INT_TBE);

	nvic_irq_enable(USART0_IRQn, 0, 0);
	nvic_irq_enable(USART1_IRQn, 0, 1);

	gd_eval_led_init(LED1);
	gd_eval_led_init(LED2);

	usbd_init(&cdc_acm, USB_CORE_ENUM_FS, &cdc_desc, &cdc_class);

	usb_intr_config();

#ifdef USE_IRC48M
	/* CTC peripheral clock enable */
	rcu_periph_clock_enable(RCU_CTC);

	/* CTC configure */
	ctc_config();

	while (ctc_flag_get(CTC_FLAG_CKOK) == RESET)
	{
		printf("CTC_FLAG_CKOK ERROR!\n");
	}
#endif

	delay_init(120);

	//	HC595_SendData(0x40);
	//	delay_ms(100);
	//	HC595_SendData(0x00);
	//	delay_ms(100);
	//	HC595_SendData(0x00);
	//	delay_ms(50);

	//ExGpio_Setbit(D1);
	//HC595_SendData(0xff);
	//printf("111\n");
	LCD_init();

	//	uint8_t buf[100];
	//	CreatQRCode("123456", buf);
	//	EncodeData("sdgfgsdfgsadfgsdfgsdfgsdf");
	//	printf("m_nSymbleSize = %d\n", m_nSymbleSize);
	//	Gui_DrawQRCode(m_nSymbleSize);
	//	DisplayQRCode(CreatQRCode("123456", buf), buf);
	//	for(uint8_t i = 0; i < sizeof(buf); i++)
	//		printf("0x%x ", buf[i]);

	delay_ms(1000);

	//	   LCD_Clear(BLACK);

	//	printf("111\n");

	extern __IO uint32_t FlashID;
	FlashID = SPI_FLASH_ReadID();
	if (FlashID == sFLASH_ID)
	{
		gpio_bit_set(LCD_GPIO_PORT, GPIO_PIN_1);
		delay_ms(1000);
	}

	uint8_t FlashTestData = 0;
	//	SPI_FLASH_BufferWrite(&FlashTestData, SPI_FLASH_PageSize*0, 1);
	//	FlashTestData = 0;
	//
	uint8_t QRbuf[64];
	SPI_FLASH_BufferRead(&FlashTestData, SPI_FLASH_PageSize * 0, 1);
	if (FlashTestData == 0x55)
	{
		SPI_FLASH_BufferRead(QRbuf, SPI_FLASH_PageSize * 1, 64);
		printf("\nQRbuf:%s\nm_nSymbleSize = %d\n", QRbuf, m_nSymbleSize);
		EncodeData((char *)QRbuf);
		Gui_DrawQRCode(m_nSymbleSize);
	}

	gd_eval_led_on(LED1);
	gd_eval_led_on(LED2);
	delay_ms(1000);
	gd_eval_led_off(LED1);
	gd_eval_led_off(LED2);
	gd_eval_led_toggle(LED2);

	printf("\ninit success!\n");
	while (1)
	{
		delay_ms(1000);
		gd_eval_led_toggle(LED1);
		if (rxflag)
		{

			strcpy(QRbuf, rxbuffer);
			printf("rxcount = %d\n", rxcount);
			memset(rxbuffer, '\0', rxcount);
			rxcount = 0;
			rxflag = 0;

			printf("receive QRcode success!\n");
			printf("%s\n", QRbuf);
			printf("m_nSymbleSize = %d\n", m_nSymbleSize);
			gd_eval_led_toggle(LED2);
			EncodeData((char *)QRbuf);
			Gui_DrawQRCode(m_nSymbleSize);

			SPI_FLASH_BufferWrite(QRbuf, SPI_FLASH_PageSize * 1, sizeof(QRbuf));
			SPI_FLASH_BufferWrite((uint8_t *)&m_nSymbleSize, SPI_FLASH_PageSize * 2, 1);

			FlashTestData = 0x55;
			SPI_FLASH_BufferWrite(&FlashTestData, SPI_FLASH_PageSize * 0, 1);
		}

		if (USBD_CONFIGURED == cdc_acm.dev.cur_status)
		{
			if (0U == cdc_acm_check_ready(&cdc_acm))
				cdc_acm_data_receive(&cdc_acm);
			else
				cdc_acm_data_send(&cdc_acm);
		}

		//		usart_data_transmit(USART1, 0xaa);
		////		usart_interrupt_enable(USART1, USART_INT_TBE);
		//		usart_data_transmit(USART0, 0xaa);
	}
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
	usart_data_transmit(EVAL_COM0, (uint8_t)ch);
	while (RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE))
		;

	return ch;
}

static uint8_t fac_us = 0;	//us delay count
static uint16_t fac_ms = 0; //ms delay count

void delay_init(uint8_t SYSCLK)
{
	SysTick->CTRL &= 0xfffffffb; 
	fac_us = SYSCLK / 8;
	fac_ms = (uint16_t)fac_us * 1000;
}

void delay_us(uint16_t nus)
{
	uint32_t temp;
	SysTick->LOAD = (uint32_t)nus * fac_us; 
	SysTick->VAL = 0x00;					
	SysTick->CTRL = 0x01;					
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp & (1 << 16))); 
	SysTick->CTRL = 0x00;						  
	SysTick->VAL = 0X00;						  
}

void delay_ms(uint16_t nms)
{
	uint32_t temp;
	SysTick->LOAD = (uint32_t)nms * fac_ms; 
	SysTick->VAL = 0x00;					
	SysTick->CTRL = 0x01;					
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp & (1 << 16))); 
	SysTick->CTRL = 0x00;						  
	SysTick->VAL = 0X00;						  
}
