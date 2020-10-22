/*!
    \file    gd32e10x_it.c
    \brief   interrupt service routines

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


#include "drv_usbd_int.h"


#define ARRAYSIZE         10
extern uint8_t spi0_send_array[];
extern uint8_t spi2_receive_array[];
extern uint32_t send_n, receive_n;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
}


/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t Rxbuffer = 0x0A;

void USART0_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
		/* receive data */
        Rxbuffer = usart_data_receive(USART0);
		usart_data_transmit(USART0, Rxbuffer);
    }
	
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE)){
		/* transmit data */
        usart_data_transmit(USART0, Rxbuffer);
    }
}

extern __IO uint16_t rxcount; 
extern __IO uint16_t rxflag; 
extern uint8_t rxbuffer[64];
extern uint8_t rx_size;

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART1_IRQHandler(void)
{	
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)){
		/* receive data */
        rxbuffer[rxcount++] = usart_data_receive(USART1);
//		usart_data_transmit(USART0, rxbuffer[rxcount]);
    }
	
	else if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE))
	{
		usart_data_receive(USART1);
//		gd_eval_led_toggle(LED2);
		rxflag = 1;
	}

    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TBE)){
		/* transmit data */
        usart_data_transmit(USART1, Rxbuffer);
		usart_data_transmit(USART0, Rxbuffer);
		gd_eval_led_toggle(LED2);
		usart_interrupt_disable(USART1, USART_INT_TBE);
    }
}


/*!
    \brief      this function handles SPI0 Handler exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SPI0_IRQHandler(void)
{
//    if(RESET != spi_i2s_interrupt_flag_get(SPI0, SPI_I2S_INT_FLAG_TBE)){
//        /* send data */
//        while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));
//        spi_i2s_data_transmit(SPI0, spi0_send_array[send_n++]);

//        if(ARRAYSIZE == send_n){
//            spi_i2s_interrupt_disable(SPI0, SPI_I2S_INT_TBE);
//        }
//    }
}

/*!
    \brief      this function handles SPI2 Handler exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SPI2_IRQHandler(void)
{
    /* receive data */
//    if(RESET != spi_i2s_interrupt_flag_get(SPI2,SPI_I2S_INT_FLAG_RBNE)){
//        spi2_receive_array[receive_n++] = spi_i2s_data_receive(SPI2);
//    }
}



extern void usb_timer_irq (void);

/*!
    \brief      this function handles timer2 interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER2_IRQHandler(void)
{
    usb_timer_irq();
}

extern usb_core_driver cdc_acm;
extern uint32_t usbfs_prescaler;

/*!
    \brief      this function handles USBFS interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void  USBFS_IRQHandler (void)
{
    usbd_isr (&cdc_acm);
}


/*!
    \brief      this function handles USBFS wakeup interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
//void USBFS_WKUP_IRQHandler(void)
//{
//    if (cdc_acm.bp.low_power) {

//        SystemInit();

//        rcu_usb_clock_config(usbfs_prescaler);

//        rcu_periph_clock_enable(RCU_USBFS);

//        usb_clock_active(&cdc_acm);
//    }

//    exti_interrupt_flag_clear(EXTI_18);
//}
