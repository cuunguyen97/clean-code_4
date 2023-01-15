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
 * File name: led.c
 *
 * Description:
 *
 * Author: CuuNV
 *
 * Last Changed By:  $Author: CuuNV $
 * Revision:         $Revision: $
 * Last Changed:     $Date: Jan 10, 2023
 *

 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "source/middle/led/led.h"


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typedef struct
{
	GPIO_Port_TypeDef	port;
	unsigned int		pin;
}Rgb_t;

static Rgb_t   rgbArray[LED_RGB_COUNT][LED_RGB_ELEMENT] = {LED_RGB_1, LED_RGB_2} ;
static LedAction_t ledAction[LED_RGB_COUNT];
EmberEventControl led1ToggleEventControl,led2ToggleEventControl;
EmberEventControl *ledEventControl[LED_RGB_COUNT];

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void ledInit(void);
void onLed(LedNumber ledIndex, LedColor color);
void offLed(LedNumber ledIndex);
void toggleLed(LedNumber ledIndex,LedColor color,uint8_t byToggleCount,uint32_t dwTimeOnMs,uint32_t dwTimeOffMs);
void led1ToggleEventControlHandler(void);
void led2ToggleEventControlHandler(void);
void toggleLedHandle(LedNumber ledIndex);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/
void ledInit(void)
{
	ledEventControl[LED_1] = (EmberEventControl *)&led1ToggleEventControl;
	ledEventControl[LED_2] = (EmberEventControl *)&led2ToggleEventControl;

	CMU_ClockEnable(cmuClock_GPIO,true);

	for(int i = 0; i<LED_RGB_COUNT; i++)
	{
		for(int j = 0; j<LED_RGB_ELEMENT; j++)
		{
			GPIO_PinModeSet(rgbArray[i][j].port, rgbArray[i][j].pin,gpioModePushPull,0);
		}

	}
	offLed(0);
	offLed(1);
}
void onLed(LedNumber ledIndex, LedColor color)
{
	for(int i = 0; i<LED_RGB_ELEMENT;i++)
	{
		if(((color>>i)&0x01) == 1)
		{
			GPIO_PinOutClear(rgbArray[ledIndex][i].port,rgbArray[ledIndex][i].pin);
		}else
		{
			GPIO_PinOutSet(rgbArray[ledIndex][i].port,rgbArray[ledIndex][i].pin);
		}
	}
}
void offLed(LedNumber ledIndex)
{
	for(int i = 0; i<LED_RGB_ELEMENT;i++)
	{
		GPIO_PinOutSet(rgbArray[ledIndex][i].port,rgbArray[ledIndex][i].pin);
	}
}
void toggleLed(LedNumber ledIndex,LedColor color,uint8_t byToggleCount,uint32_t dwOnTimeMs,uint32_t dwOffTimeMs)
{

	ledAction[ledIndex].ledBlinkMode = LED_TOGGLE;
	ledAction[ledIndex].color = color;

	ledAction[ledIndex].ledAct.ledBlink.blinkTime = (byToggleCount * 2);
	ledAction[ledIndex].ledAct.ledBlink.onTime= dwOnTimeMs;
	ledAction[ledIndex].ledAct.ledBlink.offTime = dwOffTimeMs;


	emberAfCorePrintln("blinkTime = %d", ledAction[ledIndex].ledAct.ledBlink.blinkTime);
	emberAfCorePrintln("dwOnTimeMs = %d", ledAction[ledIndex].ledAct.ledBlink.onTime);
	emberAfCorePrintln("dwOnTimeMs = %d", ledAction[ledIndex].ledAct.ledBlink.offTime);


	emberEventControlSetActive(*ledEventControl[ledIndex]);
}
void led1ToggleEventControlHandler(void)
{
	emberEventControlSetInactive(led1ToggleEventControl);
	switch(ledAction[LED_1].ledBlinkMode)
	{
	case LED_TOGGLE:
		toggleLedHandle(LED_1);
		break;
	default:
		break;
	}

}
void led2ToggleEventControlHandler(void)
{
	emberEventControlSetInactive(led2ToggleEventControl);
	switch(ledAction[LED_2].ledBlinkMode)
	{
	case LED_TOGGLE:
		toggleLedHandle(LED_2);
		break;
	default:
		break;
	}

}
void toggleLedHandle(LedNumber ledIndex)
{
	if(ledAction[ledIndex].ledAct.ledBlink.blinkTime !=0)
	{
		if(ledAction[ledIndex].ledAct.ledBlink.blinkTime %2)
		{
			for(int i =0; i< LED_RGB_ELEMENT; i++)
			{
				if( ((ledAction[ledIndex].color>> i)&0x01) == 1)
				{
					GPIO_PinOutClear(rgbArray[ledIndex][i].port,rgbArray[ledIndex][i].pin);
				}
				else
				{
					GPIO_PinOutSet(rgbArray[ledIndex][i].port,rgbArray[ledIndex][i].pin);
				}
			}
			emberEventControlSetDelayMS(*ledEventControl[ledIndex],ledAction[ledIndex].ledAct.ledBlink.onTime);
		}
		else
		{
			for(int j = 0; j<LED_RGB_ELEMENT; j++)
			{
				GPIO_PinOutSet(rgbArray[ledIndex][j].port,rgbArray[ledIndex][j].pin);
			}
			emberEventControlSetDelayMS(*ledEventControl[ledIndex],ledAction[ledIndex].ledAct.ledBlink.offTime);

		}

		ledAction[ledIndex].ledAct.ledBlink.blinkTime --;
	}
	else
	{
		ledAction[ledIndex].ledBlinkMode = LED_FREE;
		for(int j = 0; j<LED_RGB_ELEMENT; j++)
		{
			GPIO_PinOutSet(rgbArray[ledIndex][j].port,rgbArray[ledIndex][j].pin);
		}
	}
}
