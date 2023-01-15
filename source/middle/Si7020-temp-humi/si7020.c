/*******************************************************************************
 *				 _ _                                             _ _
				|   |                                           (_ _)
				|   |        _ _     _ _   _ _ _ _ _ _ _ _ _ _   _ _
				|   |       |   |   |   | |    _ _     _ _    | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |_ _ _  |   |_ _|   | |   |   |   |   |   | |   |
				|_ _ _ _ _| |_ _ _ _ _ _| |_ _|   |_ _|   |_ _| |_ _|
								(C)2021 Lumi
 * Copyright (c) 2021
 * Lumi, JSC.
 * All Rights Reserved
 *
  File name: si7020.c
 *
 * Description:
 *
 * Author: CuuNV
 *
 * Last Changed By:  $Author: CuuNV $
 * Revision:         $Revision: $
 * Last Changed:     $Date: Jan 13, 2023
 *

 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "app/framework/include/af.h"
#include "source/middle/Si7020-temp-humi/si7020.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void si7020Init (void);
void si7020Detect (void);
uint8_t CalculateCRC8(uint8_t *pByData, uint8_t byLength);
uint32_t Si7020_GetHumi (void);
uint32_t Si7020_GetTemp (void);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/


/**
 * @func    Si7020_Init
 * @brief   Initialize Si7020
 * @param   None
 * @retval  None
 */
void si7020Init (void)
{
	i2cInit();
	si7020Detect();
}
void si7020Detect (void)
{
	uint8_t pbyBufWrite[2];
	uint8_t pbyBufRead[3];
	uint8_t byNumWrite =2;
	uint8_t byNumRead = 3;

	pbyBufWrite[0] = SI7020_READ_ID_1;
	pbyBufWrite[1] = SI7020_READ_ID_2;

	i2cWriteRead(SI7020_ADDR,pbyBufWrite,byNumWrite,pbyBufRead,byNumRead);

	for(uint8_t i = 0;i<byNumRead;i++)
	{
		emberAfCorePrintln("Seri: %d",pbyBufWrite[i]);
	}
}
uint32_t Si7020_GetHumi (void)
{
	uint32_t dwHumiValue;
	uint8_t pByRetVal[3] = {0};
	uint8_t cmdHumi = SI7020_READ_RH;
	uint8_t byCheckSum = 0;

	i2cWriteRead(SI7020_ADDR,&cmdHumi,1,pByRetVal,3);

	byCheckSum = CalculateCRC8(pByRetVal,2);

	if(byCheckSum == pByRetVal[2])
	{
		//Ban tin nhan ve khong loi
	}else
	{
		//Ban tin nhan ve loi
	}
	dwHumiValue = (pByRetVal[0]<<8) + pByRetVal[1];
	dwHumiValue = ((dwHumiValue*12500)/65536) - 600;

	return dwHumiValue;
}
uint32_t Si7020_GetTemp (void)
{
	uint32_t dwTempValue;
	uint8_t pByRetVal[3] = {0};
	uint8_t cmdTemp = SI7020_READ_TEMP;
	uint8_t byCheckSum = 0;

	i2cWriteRead(SI7020_ADDR,&cmdTemp,1,pByRetVal,3);

	byCheckSum = CalculateCRC8(pByRetVal,2);

	if(byCheckSum == pByRetVal[2])
	{
		//Ban tin nhan ve khong loi
	}else
	{
		//Ban tin nhan ve loi
	}
	dwTempValue = (pByRetVal[0]<<8) + pByRetVal[1];
	dwTempValue = ((dwTempValue*17572)/65536) - 4685;

	return dwTempValue;
}
uint8_t CalculateCRC8(uint8_t *pByData, uint8_t byLength)
{
	uint8_t i, j = 0;
	uint16_t wCRC = 0;
	uint8_t *pByCurrData = pByData;

	for(j = 0; j< byLength; j++, pByCurrData++)
	{
		wCRC ^= ((uint16_t)(*pByCurrData)<<8);
		for(i = 0; i<8; i++)
		{
			if((wCRC & 0x8000)!= 0)
				wCRC ^=(0x1310 <<3);
			wCRC <<=1;
		}
	}
	return ((wCRC>>8)& 0xFF);
}

