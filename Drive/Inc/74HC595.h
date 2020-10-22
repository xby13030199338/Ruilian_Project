#ifndef __74HC595_H
#define __74HC595_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32e10x.h"
	 
/* 74HC595 GPIO config:CLK/PC1, LOCK/PC2, DATA/PC3 */
	 
#define HC595_GPIO_CLK 			RCU_GPIOC
#define HC595_GPIO_PORT			GPIOC

#define CAM_PWR					GPIO_PIN_0
#define HC595_CLK				GPIO_PIN_1
#define HC595_LOCK				GPIO_PIN_2
#define HC595_DATA				GPIO_PIN_3
	 
/*Operate I/O*/

#define CAM_PWR_DIS 		gpio_bit_set(GPIOC, CAM_PWR)
#define CAM_PWR_EN			gpio_bit_reset(GPIOC, CAM_PWR)

#define HC595_CLK_H 		gpio_bit_set(HC595_GPIO_PORT, HC595_CLK)
#define HC595_CLK_L			gpio_bit_reset(HC595_GPIO_PORT, HC595_CLK)

#define HC595_LOCK_H 		gpio_bit_set(HC595_GPIO_PORT, HC595_LOCK)
#define HC595_LOCK_L		gpio_bit_reset(HC595_GPIO_PORT, HC595_LOCK)

#define HC595_DATA_H 		gpio_bit_set(HC595_GPIO_PORT, HC595_DATA)
#define HC595_DATA_L		gpio_bit_reset(HC595_GPIO_PORT, HC595_DATA)

#define CLK_UP			HC595_CLK_L;\
                        __NOP();\
						__NOP();\
                        HC595_CLK_H;\
                        __NOP();\
						__NOP();\
						HC595_CLK_L
						
#define LOCK_UP			HC595_LOCK_L;\
                        __NOP();\
						__NOP();\
						HC595_LOCK_H;\
                        __NOP();\
						__NOP();\
						HC595_LOCK_L
				

/*************************Contrl the ExGPIO*****************************/
extern uint8_t ExGpioStatus;

#define Setbit(x, n)		x |= (0x01 << n)
#define Clrbit(x, n)		x &= ~(0x01 << n)

typedef enum{
	PWR_4G		= 0,
	LD4			= 1,
	LD3			= 2,
	LD2			= 3,
	LD1			= 4,
	LCD_EN		= 5,
	LCD_RESET	= 6,
	AUDIO_MUTE	= 7,		
}ExGPIO_e;
						
void HC595_SendData(uint8_t data);

void ExGpio_Resetbit(ExGPIO_e GPIO);

void ExGpio_Setbit(ExGPIO_e GPIO);
	 
#ifdef cplusplus
}
#endif

#endif
