#ifndef __GUI_H
#define __GUI_H

#ifdef cplusplus
 extern "C" {
#endif

#define QRcode_MAXPixel		32		

void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Color);

void Gui_DrawRegion(uint16_t X, uint16_t Y, uint8_t Pixel_len, uint16_t Color);

void Gui_DrawQRCode(uint16_t QR_len);


#ifdef cplusplus
}
#endif

#endif

