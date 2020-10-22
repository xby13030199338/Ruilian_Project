#ifndef __INIT_H
#define __INIT_H

#ifdef cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "drv_usb_hw.h"
#include "cdc_acm_core.h"


//#include "qr_encode.h"
//#include "qr_consts.h"	 
	 
#include "gd32e10x.h"
#include "74HC595.h"
#include "ST7735.h"
#include "W25Q16.h"


#include "GUI.h"


#include "QR_Encode.h"

	 
	 
void rcu_config(void);

void spi_config(void);

void gpio_config(void);

	 

#ifdef cplusplus
}
#endif

#endif


