#include "init.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

__IO uint32_t FlashID = 0;

uint8_t FLASH_WriteReadByte(uint8_t byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	//Wait Data Transmit Over
	while(RESET == spi_i2s_flag_get(FLASH_SPI, SPI_FLAG_TBE))
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
  
	spi_i2s_data_transmit(FLASH_SPI, byte);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	//Wait Data Receive Over
	while(RESET == spi_i2s_flag_get(FLASH_SPI,SPI_FLAG_RBNE))
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
	
	return spi_i2s_data_receive(FLASH_SPI);	
}

/**
* @brief  ʹ��SPI��ȡһ���ֽڵ�����
* @param  ��
* @retval ���ؽ��յ�������
*/
uint8_t SPI_FLASH_ReadByte(void)
{
  return (FLASH_WriteReadByte(Dummy_Byte));
}

/**
* @brief  ��FLASH���� дʹ�� ����
* @param  none
* @retval none
*/
void SPI_FLASH_WriteEnable(void)
{
  /* ͨѶ��ʼ��CS�� */
  FLASH_CS_L;

  /* ����дʹ������*/
  FLASH_WriteReadByte(W25X_WriteEnable);

  /*ͨѶ������CS�� */
  FLASH_CS_H;
}

/**
* @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
* @param  none
* @retval none
*/
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* ѡ�� FLASH: CS �� */
  FLASH_CS_L;

  /* ���� ��״̬�Ĵ��� ���� */
  FLASH_WriteReadByte(W25X_ReadStatusReg);

  /* ��FLASHæµ����ȴ� */
  do
  {
		/* ��ȡFLASHоƬ��״̬�Ĵ��� */
    FLASH_Status = FLASH_WriteReadByte(Dummy_Byte);	 
  }
  while ((FLASH_Status & WIP_Flag) == SET);  /* ����д���־ */

  /* ֹͣ�ź�  FLASH: CS �� */
  FLASH_CS_H;
}


 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* ��ʼͨѶ��CS�͵�ƽ */
  FLASH_CS_L;

  /* ����JEDECָ���ȡID */
  FLASH_WriteReadByte(W25X_JedecDeviceID);

  /* ��ȡһ���ֽ����� */
  Temp0 = FLASH_WriteReadByte(Dummy_Byte);

  /* ��ȡһ���ֽ����� */
  Temp1 = FLASH_WriteReadByte(Dummy_Byte);

  /* ��ȡһ���ֽ����� */
  Temp2 = FLASH_WriteReadByte(Dummy_Byte);

 /* ֹͣͨѶ��CS�ߵ�ƽ */
  FLASH_CS_H;

  /*�����������������Ϊ�����ķ���ֵ*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/**
* @brief  ��ȡFLASH����
* @param 	pBuffer���洢�������ݵ�ָ��
* @param   ReadAddr����ȡ��ַ
* @param   NumByteToRead����ȡ���ݳ���
* @retval ��
*/
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	/* ѡ��FLASH: CS�͵�ƽ */
	FLASH_CS_L;

	/* ���� �� ָ�� */
	FLASH_WriteReadByte(W25X_ReadData);

	/* ���� �� ��ַ��λ */
	FLASH_WriteReadByte((ReadAddr & 0xFF0000) >> 16);
	/* ���� �� ��ַ��λ */
	FLASH_WriteReadByte((ReadAddr& 0xFF00) >> 8);
	/* ���� �� ��ַ��λ */
	FLASH_WriteReadByte(ReadAddr & 0xFF);

	/* ��ȡ���� */
	while (NumByteToRead--) /* while there is data to be read */
	{
	/* ��ȡһ���ֽ�*/
	*pBuffer = FLASH_WriteReadByte(Dummy_Byte);
	/* ָ����һ���ֽڻ����� */
	pBuffer++;
	}

	/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
	FLASH_CS_H;
}

 /**
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* �������� */
  /* ѡ��FLASH: CS�͵�ƽ */
  FLASH_CS_L;
  /* ������������ָ��*/
  FLASH_WriteReadByte(W25X_SectorErase);
  /*���Ͳ���������ַ�ĸ�λ*/
  FLASH_WriteReadByte((SectorAddr & 0xFF0000) >> 16);
  /* ���Ͳ���������ַ����λ */
  FLASH_WriteReadByte((SectorAddr & 0xFF00) >> 8);
  /* ���Ͳ���������ַ�ĵ�λ */
  FLASH_WriteReadByte(SectorAddr & 0xFF);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  FLASH_CS_H;
  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
  * @retval ��
  */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

  /* ѡ��FLASH: CS�͵�ƽ */
  FLASH_CS_L;
  /* дҳдָ��*/
  FLASH_WriteReadByte(W25X_PageProgram);
  /*����д��ַ�ĸ�λ*/
  FLASH_WriteReadByte((WriteAddr & 0xFF0000) >> 16);
  /*����д��ַ����λ*/
  FLASH_WriteReadByte((WriteAddr & 0xFF00) >> 8);
  /*����д��ַ�ĵ�λ*/
  FLASH_WriteReadByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!"); 
  }

  /* д������*/
  while (NumByteToWrite--)
  {
    /* ���͵�ǰҪд����ֽ����� */
    FLASH_WriteReadByte(*pBuffer);
    /* ָ����һ�ֽ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  FLASH_CS_H;

  /* �ȴ�д�����*/
  SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = SPI_FLASH_PageSize - Addr;
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	/* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0)
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    { 
			/*�Ȱ�����ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				/*��д����ǰҳ*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				
        WriteAddr +=  count;
        pBuffer += count;
				/*��дʣ�������*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			/* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			
			/* ���������ظ���ַ�������� */
      WriteAddr +=  count;
      pBuffer += count;
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}


/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}
   


