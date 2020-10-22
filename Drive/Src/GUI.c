#include "init.h"


void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Color)
{
	LCD_SetXY(x,y);
	LCD_WriteData16bit(Color);
}

void Gui_DrawRegion(uint16_t X, uint16_t Y, uint8_t Pixel_len, uint16_t Color)
{
	uint8_t i, j;
	for(i = 0; i < Pixel_len ; i++)
	{
		for(j = 0; j < Pixel_len ; j++)
		{
			Gui_DrawPoint(X+j, Y+i, Color);
		}
	}
}

void Gui_DrawQRCode(uint16_t QR_len)
{	
	uint8_t Pixel_size = 0;	//QRcode Pixel Size
	int QRcode_size = 0;	//QRcode Real Size
	uint8_t Begin_X = 0;	//QRcode Begin PointX
	uint8_t Begin_Y = 0;	//QRcode Begin PointY
	uint8_t i = 0, j = 0;
	uint8_t MAX_PIXEL = 0;

	LCD_Clear(WHITE);		
	MAX_PIXEL = min(X_MAX_PIXEL, Y_MAX_PIXEL);
	/*************************Self-Adaption QRCode Size**********************************/
	//Calculate the pixel size of each block according to the side length of the QR code
	Pixel_size	= MAX_PIXEL / QR_len;	
	printf("MAX_PIXEL = %d\nPixel_size = %d\n", MAX_PIXEL, Pixel_size);
	
	//Calculate the size of the QR code to be display
	QRcode_size = Pixel_size * QR_len;
	
	//Set the starting coordinates and display the QR code in the center
	Begin_X		= (X_MAX_PIXEL-QRcode_size) / 2;
	Begin_Y		= (Y_MAX_PIXEL-QRcode_size) / 2;
	printf("Begin_X = %d\nBegin_Y = %d\nPixel_size = %d\n", Begin_X, Begin_Y, Pixel_size);
	
	/************************Display the QR Code**************************/
	for(i = 0; i < QR_len; i++)
	{
		for(j = 0; j < QR_len; j++)
		{
			if(m_byModuleData[i][j])
				Gui_DrawRegion(Begin_X, Begin_Y, Pixel_size, BLACK);
			
			Begin_X += Pixel_size;
		}
		Begin_X = (X_MAX_PIXEL-QRcode_size) / 2;
		Begin_Y += Pixel_size;
	}
}


