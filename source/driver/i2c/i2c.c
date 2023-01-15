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
 * File name: i2c.h
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
#ifndef SOURCE_MIDDLE_I2C_I2C_C_
#define SOURCE_MIDDLE_I2C_I2C_C_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "source/driver/i2c/i2c.h"

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
void i2cInit(void);
void i2cWriteRead(uint16_t wSlaveAddress, uint8_t *pbyWrite,
				uint8_t byNumBytesOfWrite, uint8_t *pbyRead,
				uint8_t byNumBytesOfRead);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 * @func    i2cInit
 * @brief   Initialize of ADC
 * @param   None
 * @retval  None
 */
void i2cInit(void)
{
	CMU_ClockEnable(cmuClock_I2C0, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	// Use default settings
	I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

	GPIO_PinModeSet(I2C_PORT_SCL,I2C_PIN_SCL, gpioModeWiredAndPullUpFilter, 1);
	GPIO_PinModeSet(I2C_PORT_SDA, I2C_PIN_SDA, gpioModeWiredAndPullUpFilter, 1);

	// Route I2C pins to GPIO
	GPIO->I2CROUTE[0].SDAROUTE = (GPIO->I2CROUTE[0].SDAROUTE & ~_GPIO_I2C_SDAROUTE_MASK)
						| (I2C_PORT_SDA << _GPIO_I2C_SDAROUTE_PORT_SHIFT
						| (I2C_PIN_SDA << _GPIO_I2C_SDAROUTE_PIN_SHIFT));
	GPIO->I2CROUTE[0].SCLROUTE = (GPIO->I2CROUTE[0].SCLROUTE & ~_GPIO_I2C_SCLROUTE_MASK)
						| (I2C_PORT_SCL << _GPIO_I2C_SCLROUTE_PORT_SHIFT
						| (I2C_PIN_SCL << _GPIO_I2C_SCLROUTE_PIN_SHIFT));
	GPIO->I2CROUTE[0].ROUTEEN = GPIO_I2C_ROUTEEN_SDAPEN | GPIO_I2C_ROUTEEN_SCLPEN;

	// Initialize the I2C
	I2C_Init(I2C0, &i2cInit);


	// Enable automatic STOP on NACK
	I2C0->CTRL = I2C_CTRL_AUTOSN;


	// Re-enable I2C

	I2C_Enable(I2C0, true);
}
/**
 * @func    i2cWriteRead
 * @brief
 * @param
	 * wSlaveAddress:
	 * *pbyWrite:
	 * byNumBytesOfWrite:
	 * *pbyRead:
	 * byNumBytesOfRead:
 * @retval  None
 */
void i2cWriteRead(uint16_t wSlaveAddress, uint8_t *pbyWrite,
				uint8_t byNumBytesOfWrite, uint8_t *pbyRead,
				uint8_t byNumBytesOfRead)
{
	  // Transfer structure
	  I2C_TransferSeq_TypeDef i2cTransfer;
	  I2C_TransferReturn_TypeDef result;


	  // Initialize I2C transfer
	  i2cTransfer.addr          = wSlaveAddress <<1;
	  i2cTransfer.flags         = I2C_FLAG_WRITE_READ; // must write target address before reading
	  i2cTransfer.buf[0].data   = pbyWrite;
	  i2cTransfer.buf[0].len    = byNumBytesOfWrite;
	  i2cTransfer.buf[1].data   = pbyRead;
	  i2cTransfer.buf[1].len    = byNumBytesOfRead;

	  result = I2C_TransferInit(I2C0, &i2cTransfer);

	  // Send data
	  while (result == i2cTransferInProgress) {
	    result = I2C_Transfer(I2C0);
	  }
	  if (result != i2cTransferDone) {
	      // Infinite while loop to indicate I2C transmission problem
	      while(1);
	  }
}


#endif /* SOURCE_MIDDLE_I2C_I2C_C_ */
