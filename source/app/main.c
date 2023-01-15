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
 * File name: main.c
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
#include "af.h"
#include "source/middle/Si7020-temp-humi/si7020.h"
#include "source/driver/adc/adc.h"
#include "source/middle/Kalman_filter/kalman_filter.h"


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
EmberEventControl eventScanTempAndHumi;
EmberEventControl eventLightSensorAdcPollingRead;

// Raw IADC conversion result
static volatile IADC_Result_t g_sample;

// Result converted to light value
static volatile double g_dwLightValue;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/

void emberAfMainInitCallback(void)
{
	si7020Init();
	adcInit();
	KalmanFilterInit(2,2,0.001);
	emberEventControlSetActive(eventScanTempAndHumi);
	emberEventControlSetActive(eventLightSensorAdcPollingRead);

}
void eventScanTempAndHumiHandler(void)
{
	emberEventControlSetInactive(eventScanTempAndHumi);

	uint8_t byTemp  = (uint8_t)(Si7020_GetTemp()/100);
	uint8_t byHumi  = (uint8_t)(Si7020_GetHumi()/100);
	emberAfCorePrintln("TempValue = %d",byTemp);
	emberAfCorePrintln("HumiValue = %d",byHumi);

	emberEventControlSetDelayMS(eventScanTempAndHumi,10000);

}

void eventLightSensorAdcPollingReadHandler(void)
{

	emberEventControlSetInactive(eventLightSensorAdcPollingRead);
	IADC_command(IADC0, iadcCmdStartSingle);

	while(!(IADC0->STATUS & IADC_IEN_SINGLEFIFODVL));
	//At least one result in the single FIFO is ready to read.
	  // Read a result from the FIFO
	g_sample = IADC_pullSingleFifoResult(IADC0);
	g_dwLightValue = (((g_sample.data) * 2420)/ 4095);
	emberAfCorePrintln("LightValue: %d",\
			  (uint16_t)g_dwLightValue);
	    // Enter EM2 sleep, wait for IADC interrupt
	    //EMU_EnterEM2(true);
    emberEventControlSetDelayMS(eventLightSensorAdcPollingRead,5000);
}


