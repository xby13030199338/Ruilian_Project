#ifndef __W25Q16_H
#define __W25Q16_H

#ifdef cplusplus
 extern "C" {
#endif

#define  sFLASH_ID              0xEF4015	 //W25Q16


#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		  0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF
/*命令定义-结尾*******************************/


#define FLASH_SPI 				SPI1
#define FLASH_SPI_CLK 			RCU_SPI1

#define FLASH_GPIO_CLK 			RCU_GPIOB
#define FLASH_GPIO_PORT			GPIOB

#define FLASH_CS					GPIO_PIN_12
#define FLASH_CLK					GPIO_PIN_13
#define FLASH_MISO					GPIO_PIN_14
#define FLASH_MOSI					GPIO_PIN_15

#define FLASH_CS_H 		gpio_bit_set(FLASH_GPIO_PORT, FLASH_CS)
#define FLASH_CS_L		gpio_bit_reset(FLASH_GPIO_PORT, FLASH_CS)


/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)

uint32_t SPI_FLASH_ReadID(void);

void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);


void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);


void SPI_FLASH_SectorErase(uint32_t SectorAddr);

#ifdef cplusplus
}
#endif

#endif
