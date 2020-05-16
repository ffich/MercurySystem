/************************************************************************
*                               Led                                     *
*************************************************************************
* FileName:         led.c                                               *
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
* F.Ficili     17/03/19    1.1          Added LED_STS_TOGGLE option.    *  
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "led.h"

/************************************************************************
* Defines
************************************************************************/


/************************************************************************
* Typedefs
************************************************************************/
/* LED blink state */
typedef enum _LedBlinkStateType
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
* LOCAL Variables
************************************************************************/
/* Led 1 sts, pulse & blink variables */
static LedStsType Led1Sts = LED_STS_OFF;
static UINT8 Led1OutSts = LED_OFF;
static LedPulseType Led1Pulse = {LedPulseOn, OffState, 0, 0};
static LedBlinkType Led1Blink = {LedBlinkInit, OffState, 0, 0, 0, LED_OFF};
/* Led 2 sts, pulse & blink variables */
static LedStsType Led2Sts = LED_STS_OFF;
static UINT8 Led2OutSts = LED_OFF;
static LedPulseType Led2Pulse = {LedPulseOn, OffState, 0, 0};
static LedBlinkType Led2Blink = {LedBlinkInit, OffState, 0, 0, 0, LED_OFF};
/* Led 3 sts, pulse & blink variables */
static LedStsType Led3Sts = LED_STS_OFF;
static UINT8 Led3OutSts = LED_OFF;
static LedPulseType Led3Pulse = {LedPulseOn, OffState, 0, 0};
static LedBlinkType Led3Blink = {LedBlinkInit, OffState, 0, 0, 0, LED_OFF};

/************************************************************************
* GLOBAL Variables
************************************************************************/


/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
UINT8 LedBlinkStateMachine (LedBlinkType *LedBlink);
UINT8 LedPulseStateMachine (LedPulseType *LedPulse, UINT8 Led);
UINT8 LedStateMachine (UINT8 Led, LedStsType *LedSts, UINT8 *LedOut, LedBlinkType *LedBlink, LedPulseType *LedPulse);

/************************************************************************
* LOCAL Function Implementations
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
         break;

      case LedPulseDelay:
         /* Increment counter */
         LedPulse->LedPulseCounter++;
         /* If off time is expired */
         if (LedPulse->LedPulseCounter >= (LedPulse->LedPulseTimeMs/LED_TASK_PERIOD_MS))
         {
            /* Reset counter */
            LedPulse->LedPulseCounter = 0;
            /* Go to LedPulseOff State */
            LedPulse->LedPulseState = LedPulseOff;
         }
         break;

      case LedPulseOff:
         /* Initialize Led Out State */
         LedPulse->LedOutState = LED_OFF;
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

/************************************************************************
* Function:     LedStateMachine
* Input:        UINT8 Led
*               LedStsType *LedSts
*               LedBlinkType *LedBlink
*               LedPulseType *LedPulse 
* Output:       UINT8
* Author:       F.Ficili
* Description:  State machine to manage the whole LED behaviour.
* Date:         27/11/18
************************************************************************/
UINT8 LedStateMachine (UINT8 Led, LedStsType *LedSts, UINT8 *LedOut, LedBlinkType *LedBlink, LedPulseType *LedPulse)
{        
   /* LED out sts */
   UINT8 LedOutSts = STD_OFF;
   
   /* Get the requested led state */
   switch (*LedSts)
   {
      case LED_STS_OFF:
         /* Turn led off */
         LedOutSts = LED_OFF;
         *LedOut = LED_OFF;
         break;
         
      case LED_STS_ON:
         /* Turn led on */
         LedOutSts = LED_ON;
         *LedOut = LED_ON;
         break;
         
      case LED_STS_TOGGLE:
         /* Toggle LED */
         if (*LedOut == 1)
         {
            /* Set led status off */
            Led_SetLedStatus(Led,LED_STS_OFF);
         }
         else
         {
            /* Set led status on */
            Led_SetLedStatus(Led,LED_STS_ON);
         }
         break;         

      case LED_STS_BLINK:
         /* Blink led */
         LedOutSts = LedBlinkStateMachine(LedBlink);
         break; 
         
      case LED_STS_PULSE:
         /* Pulse led */
         LedOutSts = LedPulseStateMachine(LedPulse, Led);
         break;            
         
      default:
         break;
   }
   
   return LedOutSts;
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     Led_MainTask
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Manage LED task
* Date:         28/02/15
************************************************************************/
void Led_MainTask (UINT8 Options)
{       
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* Initialize LEDs pins */
         BSP_LED_1_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_LED_2_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_LED_3_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_LED_1 = LINE_STATE_LOW;
         BSP_LED_2 = LINE_STATE_LOW;      
         BSP_LED_3 = LINE_STATE_LOW; 
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* LED1 State machine */
         BSP_LED_1 = LedStateMachine(LED_1,&Led1Sts,&Led1OutSts,&Led1Blink,&Led1Pulse);
         /* LED2 State machine */
         BSP_LED_2 = LedStateMachine(LED_2,&Led2Sts,&Led2OutSts,&Led2Blink,&Led2Pulse);
         /* LED3 State machine */
         BSP_LED_3 = LedStateMachine(LED_3,&Led3Sts,&Led3OutSts,&Led3Blink,&Led3Pulse);                
         break;

      /* Default */
      default:
         break;
   }
}
 
/************************************************************************
* Function:     LedStateMachine
* Input:        UINT8 Led
*               LedStsType LedSts
* Output:       None
* Author:       F.Ficili
* Description:  API to set the LED behaviour.
* Date:         27/11/18
************************************************************************/
void Led_SetLedStatus (UINT8 Led, LedStsType LedSts)
{
   /* Set the led status */
   switch (Led)
   {
      case LED_1:
         Led1Sts = LedSts;
         break;
         
      case LED_2:
         Led2Sts = LedSts;
         break;

      case LED_3:
         Led3Sts = LedSts;
         break;     
         
      default:
         break;
   }
}  

/************************************************************************
* Function:     Led_SetLedBlinkTime
* Input:        UINT8 Led
*               UINT16 OnTimeMs
*               UINT16 OffTimeMs  
* Output:       None
* Author:       F.Ficili
* Description:  API to set the LED blink timing. This blink timing will be 
*               applied if the LED status is set to LED_STS_BLINK using the API Led_SetLedStatus.
* Date:         27/11/18
************************************************************************/
void Led_SetLedBlinkTime (UINT8 Led, UINT16 OnTimeMs, UINT16 OffTimeMs)
{
   /* Set the led blink time */
   switch (Led)
   {
      case LED_1:
         Led1Blink.LedOnTimeMs = OnTimeMs;
         Led1Blink.LedOffTimeMs = OffTimeMs;
         break;
         
      case LED_2:
         Led2Blink.LedOnTimeMs = OnTimeMs;
         Led2Blink.LedOffTimeMs = OffTimeMs;
         break;

      case LED_3:
         Led2Blink.LedOnTimeMs = OnTimeMs;
         Led2Blink.LedOffTimeMs = OffTimeMs;
         break;     
         
      default:
         break;
   }   
}

/************************************************************************
* Function:     Led_SetLedPulseTime
* Input:        UINT8 Led
*               UINT16 PulseTimeMs
* Output:       None
* Author:       F.Ficili
* Description:  API to set the LED pulse timing.
* Date:         27/11/18
************************************************************************/
void Led_SetLedPulseTime (UINT8 Led, UINT16 PulseTimeMs)
{
   /* Set the led blink time */
   switch (Led)
   {
      case LED_1:
         Led1Pulse.LedPulseTimeMs = PulseTimeMs;
         break;
         
      case LED_2:
         Led2Pulse.LedPulseTimeMs = PulseTimeMs;
         break;

      case LED_3:
         Led3Pulse.LedPulseTimeMs = PulseTimeMs;
         break;     
         
      default:
         break;
   }   
}