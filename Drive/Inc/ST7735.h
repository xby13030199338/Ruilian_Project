#ifndef __ST7735_H
#define __ST7735_H

#ifdef cplusplus
 extern "C" {
#endif

/*****************LCD parameter**********************/
//#define min(a,b)	(a<b?a:b)

#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        128
	 
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   		//灰色0  0011 0001 0110 0101
#define GRAY1   0x8410      	//灰色1  0000 0000 0000 0000
#define GRAY2   0x4208      	//灰色2  1111 1111 1101 1111
	 
	 
/* LCD_SPI GPIO config:CS/PA4, SCK/PA5, RS/PA6, MOSI/PA7 */


#define LCD_SPI 				SPI0
#define LCD_SPI_CLK 			RCU_SPI0

#define LCD_GPIO_CLK 			RCU_GPIOA
#define LCD_GPIO_PORT			GPIOA


#define LCD_CS					GPIO_PIN_4
#define LCD_SCK					GPIO_PIN_5
#define LCD_RS					GPIO_PIN_6
#define LCD_MOSI				GPIO_PIN_7

/*operate I/O*/

#define LCD_CS_H 		gpio_bit_set(LCD_GPIO_PORT, LCD_CS)
#define LCD_CS_L		gpio_bit_reset(LCD_GPIO_PORT, LCD_CS)

#define LCD_RS_H 		gpio_bit_set(LCD_GPIO_PORT, LCD_RS)
#define LCD_RS_L		gpio_bit_reset(LCD_GPIO_PORT, LCD_RS)


void LCD_WriteComm(uint8_t comm);

void LCD_WriteData(uint8_t data);

void LCD_WriteData16bit(uint16_t data);

void LCD_Clear(uint16_t Color);

void LCD_init(void);

void LCD_SetXY(uint16_t x,uint16_t y);

void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data);

void ShowImage(const unsigned char *p); //显示40*40 QQ图片




#ifdef cplusplus
}
#endif

#endif

