/************************************************************************
*                               Interrupts                              *
*************************************************************************
* FileName:         interrupts.c                                        *
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
#include "hal.h"
#include "os_sched.h"

/************************************************************************
* LOCAL Variables
************************************************************************/
/* Local counter */
static UINT16 InterruptCounter = 0;

/************************************************************************
* Interrupts Routines
************************************************************************/
#if (COMPILER_USED == XC8_COMPILER)
/************************************************************************
* Function:     MainInterrupt
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 Main interrupt handler.
* Date:         31/07/16
************************************************************************/
void interrupt MainInterrupt (void)
{
   /* Check if is a Timer 2 Interrupt */
   if (PIR1bits.TMR2IF == ON && PIE1bits.TMR2IE == SET)
   {       
      /* clear interrupt flag */
      PIR1bits.TMR2IF = CLEAR;
      /* Increment local counter */
      InterruptCounter++;
      /* If the scheduler period is elapsed */
      if (InterruptCounter >= SCHEDULER_DES_TICK_TIME)
      {
         /* Reset counter */
         InterruptCounter = 0;
         /* Main scheduler timebase flag */
         MainSystemTimebaseFlag = CallTaskPhase;
      }
   }
   
   /* Check if is a I2C Interrupt */
   if(PIE1bits.SSP1IE == ON && PIR1bits.SSP1IF == SET)
   {  
      /* Clear flag */
      PIR1bits.SSP1IF = CLEAR;
      
#ifdef STATE_MACHINE_IMPLEMENTATION      
      /* Notify data reception */
      GenerateEvt(&I2cNewTxNotification);
#endif      
      
#ifndef STATE_MACHINE_IMPLEMENTATION
      /* Receive I2C Data */
      I2c_DataReceive();
#endif
   }
   
   #ifdef USE_SB310
   /* Check if is a TMR0 Interrupt */
   if(PIR3bits.TMR4IF == ON && PIE3bits.TMR4IE == SET)
   {
      /* Clear Flag */
      PIR3bits.TMR4IF = CLEAR;  
      /* Measure echo pulse */
      SnrDrv_DetectEchoPulse();
   }   
   #endif

   #ifdef USE_SB130
   /* Check if is a TMR0 Interrupt */
   if(PIR3bits.TMR4IF == ON && PIE3bits.TMR4IE == SET)
   {
      /* Clear Flag */
      PIR3bits.TMR4IF = CLEAR;  
      /* Servo Drv periodic interrupt */
      SrvDrv_ServoPulse();
   }   
   #endif   
}
#endif