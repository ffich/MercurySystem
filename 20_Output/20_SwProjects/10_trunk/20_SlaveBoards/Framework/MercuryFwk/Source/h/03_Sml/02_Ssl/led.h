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
#define LED_TASK_PERIOD_MS                                  ((UINT16)(10)) 

/* LED status constants */
#define LED_ON                                              LINE_STATE_HIGH
#define LED_OFF                                             LINE_STATE_LOW

#define LED_1                                               1u
#define LED_2                                               2u
#define LED_3                                               3u

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Led status type */
typedef enum _LedStsType
{
   LED_STS_OFF,
   LED_STS_ON,
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
/* API to set the LED behavour */
void Led_SetLedStatus (UINT8 Led, UINT8 LedSts);
/* API to set the LED blink timing */
void Led_SetLedBlinkTime (UINT8 Led, UINT16 OnTimeMs, UINT16 OffTimeMs);
/* API to set the LED pulse timing */
void Led_SetLedPulseTime (UINT8 Led, UINT16 PulseTimeMs);

#endif	/* LED_H */
