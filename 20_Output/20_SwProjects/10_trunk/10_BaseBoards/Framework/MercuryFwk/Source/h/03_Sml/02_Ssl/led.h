/************************************************************************
*                                Led                                    *
*************************************************************************
* FileName:         led.h                                               *
* HW:               Mercury System                                      *
* Author:           F.Ficili                                            *
*                                                                       *
* Software License Agreement:                                           *
*                                                                       *
* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,     *
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED     *
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE AUTHOR SHALL NOT,      *
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR            *
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.                     *
*                                                                       *  
* --------------------------------------------------------------------- * 
* Responsible For This File: Francesco Ficili                           *
*                                                                       *
* --------------------------------------------------------------------- *
* Author       Date        Version      Comment                         *
* ---------------------------------------------------------------------	*
* F.Ficili     29/07/16    1.0          First release                   *
* F.Ficili     17/03/19    1.1          Added LED_STS_TOGGLE option.    * 
* F.Ficili     10/05/19    1.2          Added Macros for LED handling.  * 
************************************************************************/

#ifndef LED_H
#define	LED_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Task period in ms */
#define LED_TASK_PERIOD_MS                                  ((UINT16)(50))

/* LED status constants */
#define LED_ON                                              LINE_STATE_HIGH
#define LED_OFF                                             LINE_STATE_LOW

#define LED_1                                               1u
#define LED_2                                               2u
#define LED_3                                               3u

/* Macros */
#define SET_LED_1_BLINKING(On,Off)                          Led_SetLedBlinkTime(LED_1,On,Off);  \
                                                            Led_SetLedStatus(LED_1,LED_STS_BLINK);
                                                            
#define SET_LED_2_BLINKING(On,Off)                          Led_SetLedBlinkTime(LED_2,On,Off);  \
                                                            Led_SetLedStatus(LED_2,LED_STS_BLINK);

#define SET_LED_3_BLINKING(On,Off)                          Led_SetLedBlinkTime(LED_3,On,Off);  \
                                                            Led_SetLedStatus(LED_3,LED_STS_BLINK);

#define SET_LED_1_PULSE(x)                                  Led_SetLedPulseTime(LED_1,x);  \
                                                            Led_SetLedStatus(LED_1,LED_STS_PULSE);

#define SET_LED_2_PULSE(x)                                  Led_SetLedPulseTime(LED_2,x);  \
                                                            Led_SetLedStatus(LED_2,LED_STS_PULSE);

#define SET_LED_3_PULSE(x)                                  Led_SetLedPulseTime(LED_3,x);  \
                                                            Led_SetLedStatus(LED_3,LED_STS_PULSE);

#define SET_LED_1_STS(x)                                    Led_SetLedStatus(LED_1,x);
#define SET_LED_1_TOGGLE()                                  Led_SetLedStatus(LED_1,LED_STS_TOGGLE);

#define SET_LED_2_STS(x)                                    Led_SetLedStatus(LED_2,x);
#define SET_LED_2_TOGGLE()                                  Led_SetLedStatus(LED_2,LED_STS_TOGGLE);

#define SET_LED_3_STS(x)                                    Led_SetLedStatus(LED_3,x);
#define SET_LED_3_TOGGLE()                                  Led_SetLedStatus(LED_3,LED_STS_TOGGLE);

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Led status type */
typedef enum _LedStsType
{
   LED_STS_OFF = 0,
   LED_STS_ON,
   LED_STS_TOGGLE,
   LED_STS_BLINK,
   LED_STS_PULSE
} LedStsType;

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Led task */
void Led_MainTask (UINT8 Options);
/* API to set the LED behaviour */
void Led_SetLedStatus (UINT8 Led, LedStsType LedSts);
/* API to set the LED blink timing */
void Led_SetLedBlinkTime (UINT8 Led, UINT16 OnTimeMs, UINT16 OffTimeMs);
/* API to set the LED pulse timing */
void Led_SetLedPulseTime (UINT8 Led, UINT16 PulseTimeMs);

#endif	/* LED_H */

