#include "74HC595.h"

static uint8_t ExGpioStatus = 0;

void HC595_SendData(uint8_t data)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if ((data<<i) & 0x80)
			HC595_DATA_H;
		else
			HC595_DATA_L;
		
		CLK_UP;		//Put up 595 CLK
	}
	
	LOCK_UP;		//Put up 595 LOCK
	
	HC595_DATA_L;	
}


void ExGpio_Setbit(ExGPIO_e GPIO)
{
	Setbit(ExGpioStatus, GPIO);
	HC595_SendData(ExGpioStatus);
//	printf("%c", ExGpioStatus);
}

void ExGpio_Resetbit(ExGPIO_e GPIO)
{
	Clrbit(ExGpioStatus, GPIO);
	HC595_SendData(ExGpioStatus);
//	printf("%c", ExGpioStatus);
}
