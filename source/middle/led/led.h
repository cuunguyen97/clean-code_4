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
 * File name: led.h
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
#ifndef SOURCE_MIDDLE_LED_LED_H_
#define SOURCE_MIDDLE_LED_LED_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "app/framework/include/af.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define LED_RGB_COUNT		2
#define LED_RGB_ELEMENT		3

// LED 1
#define LED_PORT_1			gpioPortA
#define LED_RED_PIN_1		(4U)
#define LED_GREEN_PIN_1		(3U)
#define LED_BLUE_PIN_1		(0U)

// LED 2
#define LED_PORT_2			gpioPortD
#define LED_RED_PIN_2		(0U)
#define LED_GREEN_PIN_2		(1U)
#define LED_BLUE_PIN_2		(2U)

#define LED_RGB_1			{{LED_PORT_1,LED_RED_PIN_1}, {LED_PORT_1,LED_GREEN_PIN_1}, {LED_PORT_1,LED_BLUE_PIN_1}}
#define LED_RGB_2			{{LED_PORT_2,LED_RED_PIN_2}, {LED_PORT_2,LED_GREEN_PIN_2}, {LED_PORT_2,LED_BLUE_PIN_2}}
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

typedef enum
{
	LED_OFF = 0X000,
	LED_RED = BIT(0),
	LED_GREEN = BIT(1),
	LED_BLUE = BIT(2),
	LED_PINK = BIT(0)|BIT(1),
	LED_YELLOW = BIT(1)|BIT(2)
}LedColor;

typedef enum
{
	RED,
	GREEN,
	BLUE,
	OFF
}RbgState;

typedef enum
{
	LED_1,
	LED_2
}LedNumber;

typedef enum
{
	LED_TOGGLE,
	LED_FREE
}BlinkMode;

typedef struct{
	uint8_t blinkTime;
	uint32_t onTime;
	uint32_t offTime;
}TimeOfAct_t;

typedef struct{
	TimeOfAct_t ledBlink;
}ModeLedAct_t;

typedef struct
{
	BlinkMode ledBlinkMode;
	LedColor  color;
	ModeLedAct_t ledAct;
}LedAction_t;
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

#endif


