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
uint8_t byCalculateCRC8(uint8_t *pByData, uint8_t byLength);
uint32_t dwSi7020GetHumi (void);
uint32_t dwSi7020GetTemp (void);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/


/**
 * @func    si7020Init
 * @brief   Initialize Si7020
 * @param   None
 * @retval  None
 */
void si7020Init (void)
{
	i2cInit();
	si7020Detect();
}
/**
 * @func    si7020Detect
 * @brief   Detect Electronic Serial Number of Device
 * @param   None
 * @retval  None
 */
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
/**
 * @func    dwSi7020GetHumi
 * @brief   Detect Electronic Serial Number of Device
 * @param   None
 * @retval  Humidity value(2 byte data, 1 byte check Xor,
 *                         1 byte None),
 *                         byte number 1: MSB
 *                         byte number 2: LSB
 *                         byte number 3: Check Xor
 */
uint32_t dwSi7020GetHumi (void)
{
	uint32_t dwHumiValue;
	uint8_t pByRetVal[3] = {0};
	uint8_t byCmdHumi = SI7020_READ_RH;
	uint8_t byCheckSum = 0;

	i2cWriteRead(SI7020_ADDR,&byCmdHumi,1,pByRetVal,3);

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
/**
 * @func    dwSi7020GetTemp
 * @brief   Detect Electronic Serial Number of Device
 * @param   None
 * @retval  Temperature value(2 byte data, 1 byte check Xor,
 *                         1 byte None)
 *                         byte number 1: MSB
 *                         byte number 2: LSB
 *                         byte number 3: Check Xor
 */
uint32_t dwSi7020GetTemp (void)
{
	uint32_t dwTempValue;
	uint8_t pByRetVal[3] = {0};
	uint8_t byCmdTemp = SI7020_READ_TEMP;
	uint8_t byCheckSum = 0;

	i2cWriteRead(SI7020_ADDR,&byCmdTemp,1,pByRetVal,3);

	byCheckSum = byCalculateCRC8(pByRetVal,2);

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
/**
 * @func    si7020Detect
 * @brief   Detect Electronic Serial Number of Device
 * @param   None
 * @retval  Humidity value(2 byte data, 1 byte check Xor,
 *                         1 byte None),
 *                         byte number 1: MSB
 *                         byte number 2: LSB
 *                         byte number 3: Check Xor
 */
uint8_t byCalculateCRC8(uint8_t *pByData, uint8_t byLength)
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

