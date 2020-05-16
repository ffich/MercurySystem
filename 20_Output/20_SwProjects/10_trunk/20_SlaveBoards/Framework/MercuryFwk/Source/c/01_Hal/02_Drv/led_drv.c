/************************************************************************
*                               Led Driver                              *
*************************************************************************
* FileName:         led_drv.c                                           *
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

/************************************************************************
* Includes
************************************************************************/
#include "led_drv.h"
#include "led.h"

/************************************************************************
* Defines
************************************************************************/


/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/


/************************************************************************
* GLOBAL Variables
************************************************************************/


/************************************************************************
* LOCAL Function Prototypes
************************************************************************/


/************************************************************************
* LOCAL Function Implementations
************************************************************************/


/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     LedBlinkStateMachine
* Input:        LedBlinkType *LedBlink
* Output:       UINT8
* Author:       F.Ficili
* Description:  State machine to manage the LED blink behavour.
* Date:         23/09/16
************************************************************************/
UINT8 LedBlinkStateMachine (LedBlinkType *LedBlink)
{
   /* State Machine */
   switch (LedBlink->LedBlinkState)
   {
      case LedBlinkInit:
         /* Initialize Led Out State */
         LedBlink->LedOutState = LedBlink->LedInitStatus;
         /* Go to LedBlinkRunning State */
         LedBlink->LedBlinkState = LedBlinkRunning;
         break;

      case LedBlinkRunning:
         /* Increment counter */
         LedBlink->LedBlinkCounter++;
         /* If Led is in off state */
         if (LedBlink->LedOutState == OffState)
         {
            /* If off time is expired */
            if (LedBlink->LedBlinkCounter >= (LedBlink->LedOffTimeMs/LED_TASK_PERIOD_MS))
            {
               /* Reset counter */
               LedBlink->LedBlinkCounter = 0;
               /* Switch status */
               LedBlink->LedOutState = OnState;
            }
         }
         else if (LedBlink->LedOutState == OnState)
         {
            /* If on time is expired */
            if (LedBlink->LedBlinkCounter >= (LedBlink->LedOnTimeMs/LED_TASK_PERIOD_MS))
            {
               /* Reset counter */
               LedBlink->LedBlinkCounter = 0;
               /* Switch status */
               LedBlink->LedOutState = OffState;
            }
         }
         break;

      default:
         break;
   }
   
   /* Return Status */
   return (LedBlink->LedOutState);
}

/************************************************************************
* Function:     LedPulseStateMachine
* Input:        LedPulseType *LedPulse
*               UINT8 Led  
* Output:       UINT8
* Author:       F.Ficili
* Description:  State machine to manage the LED pulse behavour.
* Date:         27/11/18
************************************************************************/
UINT8 LedPulseStateMachine (LedPulseType *LedPulse, UINT8 Led)
{
   /* State Machine */
   switch (LedPulse->LedPulseState)
   {
      case LedPulseOn:
         /* Initialize Led Out State */
         LedPulse->LedOutState = LED_ON;
         /* Go to LedPulseDelay State */
         LedPulse->LedPulseState = LedPulseDelay;
         /* Intentionally fall-through */

      case LedPulseDelay:
         /* Increment counter */
         LedPulse->LedPulseCounter++;
         /* If off time is expired */
         if (LedPulse->LedPulseCounter >= (LedPulse->LedPulseTimeMs/LED_TASK_PERIOD_MS))
         {
            /* Reset counter */
            LedPulse->LedPulseCounter = 0;
            /* Set Led Out State */
            LedPulse->LedOutState = LED_OFF;            
            /* Go to LedPulseOff State */
            LedPulse->LedPulseState = LedPulseOff;
         }
         break;

      case LedPulseOff:
         /* Go to LedPulseDelay State */
         LedPulse->LedPulseState = LedPulseOn;
         /* Set led status off */
         Led_SetLedStatus(Led,LED_STS_OFF);         
         break;

      default:
         break;
   }   
   
   /* Return Status */
   return (LedPulse->LedOutState);   
}

