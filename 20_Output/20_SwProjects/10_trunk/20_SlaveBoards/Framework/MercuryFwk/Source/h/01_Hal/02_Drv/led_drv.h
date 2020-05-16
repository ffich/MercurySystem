/************************************************************************
*                            Led Driver                                 *
*************************************************************************
* FileName:         led_drv.h                                           *
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
* F.Ficili     23/07/16    1.0          First release                   *
************************************************************************/

#ifndef LED_DRV_H
#define	LED_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/


/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* LED blink state */
typedef enum LedBlinkSmEnum
{
    LedBlinkInit,
    LedBlinkRunning
} LedBlinkStateType;

/* LED blink state */
typedef enum _LedPulseStateType
{
   LedPulseOn,
   LedPulseDelay,
   LedPulseOff
} LedPulseStateType;

/* LED output state */
typedef enum _LedOutStateType
{
   OffState,
   OnState
} LedOutStateType;

/* Led blink structure */
typedef struct _LedBlinkType
{
   LedBlinkStateType LedBlinkState;
   LedOutStateType LedOutState;
   UINT16 LedBlinkCounter;
   UINT16 LedOnTimeMs;
   UINT16 LedOffTimeMs;
   LedOutStateType LedInitStatus;
} LedBlinkType;

/* Led blink structure */
typedef struct _LedPulseType
{
   LedPulseStateType LedPulseState;
   LedOutStateType LedOutState;
   UINT16 LedPulseCounter;
   UINT16 LedPulseTimeMs;
} LedPulseType;
/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
UINT8 LedBlinkStateMachine (LedBlinkType *LedBlink);
UINT8 LedPulseStateMachine (LedPulseType *LedPulse, UINT8 Led);

#endif	/* LED_DRV_H */

