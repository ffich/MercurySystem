/************************************************************************
*                        	Servo Driver                                *
*************************************************************************
* FileName:         servo_drv.c                                         *
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
* F.Ficili     06/01/18    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "servo_drv.h"

#ifdef USE_SB130
/************************************************************************
* Defines
************************************************************************/
/* Periodic servo pulse timeout in us */
#define SERVO_DRV_PULSE_TIMEOUT_US                       ((UINT16)(20000))
/* Periodic servo pulse timeout */
#define SERVO_DRV_PULSE_TIMEOUT                          (SERVO_DRV_PULSE_TIMEOUT_US/SERVO_DRV_INT_PER_US)
/* On pulse timeout in us */
#define ON_PULSE_TIMEOUT_US                              ((UINT16)(1000))
/* On pulse timeout */
#define ON_PULSE_TIMEOUT                                 (ON_PULSE_TIMEOUT_US/SERVO_DRV_INT_PER_US)

/************************************************************************
* Typedefs
************************************************************************/
/* Servo Pulse State machine type */
typedef enum _ServoPulseStateType
{
   Sp_WaitTrigger,
   Sp_OnPulse,
   Sp_DutyPulse,           
} ServoPulseStateType;


/************************************************************************
* LOCAL Variables
************************************************************************/
/* Servo set array */
ServoCntType Servo[SERVO_SET_NUMB];

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
* Function:     SetOutChSts
* Input:        UINT8 Ch
*               UINT8 Sts
* Output:       None
* Author:       F.Ficili
* Description:  Set output channels status on request.
* Date:         05/01/18
************************************************************************/
void SetOutChSts (UINT8 Ch, UINT8 Sts)
{
   /* Check channel */
   switch (Ch)
   {
      case SERVO_CH_1:
         /* Set ch status */
         BSP_SERVO_1 = Sts;
         break;
         
      case SERVO_CH_2:
         /* Set ch status */         
         BSP_SERVO_2 = Sts;
         break;
         
      case SERVO_CH_3:
         /* Set ch status */
         BSP_SERVO_3 = Sts;
         break;
         
      case SERVO_CH_4:
         /* Set ch status */         
         BSP_SERVO_4 = Sts;
         break; 
         
      case SERVO_CH_5:
         /* Set ch status */
         BSP_SERVO_5 = Sts;
         break;
         
      case SERVO_CH_6:
         /* Set ch status */         
         BSP_SERVO_6 = Sts;
         break;           
         
      default:
         break;
   }
}


/************************************************************************
* GLOBAL Function Implementations
************************************************************************/
/************************************************************************
* Function:     SrvDrv_ServoInit
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Initialize the servo driver.
* Date:         21/04/19
************************************************************************/
void SrvDrv_ServoInit (void)
{
   UINT8 i;
   
   for (i = 0; i < SERVO_SET_NUMB; i++)
   {
      Servo[i].Duty = 0xFF;
      Servo[i].PulseCnt = 0x00;
      Servo[i].OutSts = OFF;
   }
}

/************************************************************************
* Function:     SrvDrv_ServoPulse
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  This function generates the servo driving pulse for
*               the enetire servo set. 
* Date:         06/01/18
************************************************************************/
void SrvDrv_ServoPulse (void)
{
   static ServoPulseStateType ServoPulseState = Sp_WaitTrigger;
   static UINT16 TriggerCounter = 0;
   static UINT16 OnPulseCounter = 0;
   UINT8 ServoIndex;
   UINT8 OffCounter;
      
   switch (ServoPulseState)
   {
      case Sp_WaitTrigger:
         /* Increment trigger counter */
         TriggerCounter++;
         /* Check trigger */
         if (TriggerCounter >= SERVO_DRV_PULSE_TIMEOUT)
         {
            /* Reset counter */
            TriggerCounter = 0;         
            /* Set status to on for all servos */
            for (ServoIndex = 0; ServoIndex < SERVO_SET_NUMB; ServoIndex++)
            {
               /* Reset pulse counter */
               Servo[ServoIndex].PulseCnt = 0;               
               if ((Servo[ServoIndex].Duty >= 0) && (Servo[ServoIndex].Duty <= 100))
               {
                  /* Drive servo channel */
                  SetOutChSts(ServoIndex,ON);
                  Servo[ServoIndex].OutSts = ON;               
               }               
            } 
            /* Switch state */
            ServoPulseState = Sp_OnPulse;               
         }         
         break;
         
      case Sp_OnPulse:
         /* Increment trigger counter */
         OnPulseCounter++;
         /* Check trigger */
         if (OnPulseCounter >= ON_PULSE_TIMEOUT)
         {
            /* Reset counter */
            OnPulseCounter = 0;
            /* Switch state */
            ServoPulseState = Sp_DutyPulse;
         }                        
         break;

      case Sp_DutyPulse:
         /* Init off counter */
         OffCounter = 0;
         /* Increment all servo counters */
         for (ServoIndex = 0; ServoIndex < SERVO_SET_NUMB; ServoIndex++)
         {
            /* Increment servo counter */
            Servo[ServoIndex].PulseCnt++;               
            /* If the servo signal is on */
            if (Servo[ServoIndex].OutSts == ON)
            {            
               /* Check servo counter */
               if (Servo[ServoIndex].PulseCnt >= (Servo[ServoIndex].Duty))
               {                  
                  /* Drive servo channel */
                  SetOutChSts(ServoIndex,OFF);
                  Servo[ServoIndex].OutSts = OFF;             
               }            
            }
            else
            {
               /* Increment number of ch in off */
               OffCounter++;
               /* If matches the number of channel */
               if (OffCounter >= SERVO_SET_NUMB)
               {
                  /* Switch state */
                  ServoPulseState = Sp_WaitTrigger;                  
               }
            }
         }
         break;   
         
      default:
         break;         
   }   
}
#endif
