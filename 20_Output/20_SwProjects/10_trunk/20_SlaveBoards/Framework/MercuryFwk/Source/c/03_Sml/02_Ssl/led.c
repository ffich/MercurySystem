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
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "led.h"
#include "led_drv.h"

/************************************************************************
* Defines
************************************************************************/
#define INIT_TMOUT_MS                              (50u)/(LED_TASK_PERIOD_MS)
#define BLINK_COUNT                                5u

/************************************************************************
* Typedefs
************************************************************************/
/* Init status */
typedef enum
{
   Startup,
   Initializing,
   Done,         
} InitSeqStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* Led 1 sts, pulse & blink variables */
static LedStsType Led1Sts = LED_STS_OFF;
static LedPulseType Led1Pulse = {LedPulseOn, OffState, 0, 0};
static LedBlinkType Led1Blink = {LedBlinkInit, OffState, 0, 0, 0, LED_OFF};

/************************************************************************
* GLOBAL Variables
************************************************************************/


/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
UINT8 LedStateMachine (UINT8 Led, LedStsType *LedSts, LedBlinkType *LedBlink, LedPulseType *LedPulse);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

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
UINT8 LedStateMachine (UINT8 Led, LedStsType *LedSts, LedBlinkType *LedBlink, LedPulseType *LedPulse)
{        
   /* LED out sts */
   UINT8 LedOutSts = STD_OFF;
   
   /* Get the requested led state */
   switch (*LedSts)
   {
      case LED_STS_OFF:
         /* Turn led off */
         LedOutSts = LED_OFF;
         break;
         
      case LED_STS_ON:
         /* Turn led om */
         LedOutSts = LED_ON;
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
* Function:     Led_InitSeq
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Internal init sequence.
* Date:         20/05/19
************************************************************************/
void Led_InitSeq (void)
{
   static UINT16 DelayCounter = 0;
   static UINT8 Counter = 0;   
   static UINT8 LedInitSts = LED_STS_ON; 
   static InitSeqStateType InitSeqState = Startup;
   
   switch (InitSeqState)
   {
      case Startup:
         /* Set led status on */
         Led_SetLedStatus(LED_1,LedInitSts);
         /* Swithc state */
         InitSeqState = Initializing;
         break;
         
      case Initializing:
         /* Inc counter */
         DelayCounter++;
         /* Check counter */
         if (DelayCounter > INIT_TMOUT_MS)
         {
            /* Invert status */
            LedInitSts = !LedInitSts;
            /* Set led status on */
            Led_SetLedStatus(LED_1,LedInitSts);            
            /* Reset counter */
            DelayCounter = 0;
            /* Increment count*/
            Counter++;
            /* Check blink count */
            if (Counter > BLINK_COUNT)
            {
               /* Set led status on */
               Led_SetLedStatus(LED_1,LED_STS_OFF);
               /* Swithc state */
               InitSeqState = Done;                  
            }            
         }         
         break;
         
      case Done:
         break;
         
      default:
         break;
   }
}

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
         BSP_LED_1 = LINE_STATE_LOW;
         /* Set LED default behaviour */
         SetLedBehavSts(LED_DEFAULT_BEHAV);         
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Led init seq */
         Led_InitSeq();
         /* LED1 State machine */
         BSP_LED_1 = LedStateMachine(LED_1,&Led1Sts,&Led1Blink,&Led1Pulse);        
         break;

      /* Default */
      default:
         break;
   }
}

/************************************************************************
* Function:     LedStateMachine
* Input:        UINT8 Led
*               UINT8 LedSts
* Output:       None
* Author:       F.Ficili
* Description:  API to set the LED behavour.
* Date:         27/11/18
************************************************************************/
void Led_SetLedStatus (UINT8 Led, UINT8 LedSts)
{
   /* Set the led status */
   switch (Led)
   {
      case LED_1:
         Led1Sts = LedSts;
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
* Description:  API to set the LED blink timing.
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

      default:
         break;
   }   
}