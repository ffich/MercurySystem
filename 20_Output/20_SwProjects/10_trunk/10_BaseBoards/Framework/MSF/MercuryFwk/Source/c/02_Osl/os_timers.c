/************************************************************************
*                          Os Software Timers                           *
*************************************************************************
* FileName:         os_timers.c                                         *
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
* F.Ficili     24/04/17    1.0          First release                   *
* F.Ficili     25/12/18    1.1          Added OsTmr_Wait API.           *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "os_timers.h"

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
/* Os tick global counter */
UINT32 OsTmr_OsTickCounter = 0;

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/


/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************   
* Function:     GetOsTickCounter
* Input:        None
* Output:       UINT32
* Author:       F.Ficili
* Description:  Return the Os tick counter.
* Date:         24/04/17
************************************************************************/
UINT32 GetOsTickCounter (void)
{
   /* Return tick counter */
   return OsTmr_OsTickCounter;
}

/************************************************************************
* Function:     GetElapsedTime
* Input:        UINT32 StartTime
* Output:       UINT32
* Author:       F.Ficili
* Description:  Get elapsed time from a given start time.
* Date:         24/04/17
************************************************************************/
UINT32 GetElapsedTime (UINT32 StartTime)
{
   UINT32 CurrentTimestamp;
   UINT32 TimeValue;
   
   /* Get timestamp */
   CurrentTimestamp = GetOsTickCounter();
   
   /* Wraparound control */
   if (CurrentTimestamp >= StartTime)
   {
      TimeValue = (CurrentTimestamp - StartTime);
   }
   else
   {
      TimeValue = (CurrentTimestamp + MAX_SW_TIMER_COUNT) - StartTime;
   }
   
   return TimeValue;
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     OsTmr_StartTimer
* Input:        SwTimerType *Timer
*               UINT32 Timeout 
* Output:       None
* Author:       F.Ficili
* Description:  API to start a software timer.
* Date:         24/04/17
************************************************************************/
void OsTmr_StartTimer (SwTimerType *Timer, UINT32 Timeout)
{
   /* Mark as not expired */
   Timer->Status = SwTmrNotExpired;
   /* Get os tick counter value */
   Timer->StartTime = GetOsTickCounter();
   /* Set target time value */
   Timer->TargetTime = Timeout;
   /* Start the timer */
   Timer->Enabled = STD_TRUE;   
}    

/************************************************************************
* Function:     OsTmr_StopTimer
* Input:        SwTimerType *Timer
* Output:       None
* Author:       F.Ficili
* Description:  API to stop a software timer.
* Date:         24/04/17
************************************************************************/
void OsTmr_StopTimer (SwTimerType *Timer)
{
   /* Stop the timer */
   Timer->Enabled = STD_FALSE;      
}  

/************************************************************************
* Function:     OsTmr_Wait
* Input:        SwTimerType *WaitTimer
*               UINT32 DelayMs
* Output:       None
* Author:       F.Ficili
* Description:  API that implement a non-blocking delay function. 
*               It waits for the defined amount of time (in ms) passed as parameter.
* Date:         02/12/18
************************************************************************/
DelayStatusType OsTmr_Wait (SwTimerType *WaitTimer, UINT32 DelayMs)
{
   DelayStatusType Ret = DelayNotExpired;
   
   switch (WaitTimer->WaitState)
   {
      case WtStartSwTimer:
         /* Start Sw trimer */ 
         OsTmr_StartTimer(WaitTimer,DelayMs);
         /* Switch state */
         WaitTimer->WaitState = WtWait;
         break;
         
      case WtWait:         
         /* Check if the timer is expired */
         if (OsTmr_GetTimerStatus(WaitTimer) == SwTimerExpired)
         {
            /* Switch state */
            WaitTimer->WaitState = WtStartSwTimer;
            /* Delay expired */
            Ret = DelayExpired;
         }
         break;
         
      default:
         break;
   }
   
   return Ret;
}

/************************************************************************
* Function:     OsTmr_GetTimerStatus
* Input:        SwTimerType *Timer
* Output:       SwTimerStatusType
* Author:       F.Ficili
* Description:  API that checks the software timer status.
* Date:         24/04/17
************************************************************************/
SwTimerStatusType OsTmr_GetTimerStatus (SwTimerType *Timer)
{
   /* Check if the timer is enabled */
   if (Timer->Enabled == STD_TRUE)
   {
      /* Check if expired */
      if (GetElapsedTime(Timer->StartTime) >= Timer->TargetTime)
      {
         /* Disable timer */
         Timer->Enabled = STD_FALSE;
         /* Mark as expired */
         Timer->Status = SwTimerExpired;
      }
      else
      {
         /* Do nothing */
      }
   }
   else
   {
      /* Mark as disabled on second request after expiration */
      Timer->Status = SwTimerDisabled;
   }
   
   /* Return timer status */
   return Timer->Status;
}  

/************************************************************************
* Function:     OsTmr_GetElapsedTime
* Input:        SwTimerType *Timer
* Output:       UINT32
* Author:       F.Ficili
* Description:  API that gets the elapsed time since the sw timer started.
* Date:         24/04/17
************************************************************************/
UINT32 OsTmr_GetElapsedTime (SwTimerType *Timer)
{
   UINT32 CurrentTimestamp;
   UINT32 TimeValue;
   
   /* Get timestamp */
   CurrentTimestamp = GetOsTickCounter();
   
   /* Wraparound control */
   if (CurrentTimestamp >= Timer->StartTime)
   {
      TimeValue = (CurrentTimestamp - Timer->StartTime);
   }
   else
   {
      TimeValue = (CurrentTimestamp + MAX_SW_TIMER_COUNT) - Timer->StartTime;
   }
   
   return TimeValue;   
}  

/************************************************************************
* Function:     OsTmr_GetRemainingTime
* Input:        SwTimerType *Timer
* Output:       UINT32
* Author:       F.Ficili
* Description:  API that gets the remaining time before a sw timer expires.
* Date:         24/04/17
************************************************************************/
UINT32 OsTmr_GetRemainingTime (SwTimerType *Timer)
{
   UINT32 TimeValue;
   
   /* Check if already reached the target time */
   if (GetElapsedTime(Timer->StartTime) >= Timer->TargetTime)
   {
      /* If yes return 0 */
      TimeValue = 0;
   }
   else
   {
      /* Otherwise calculate the correct value */
      TimeValue = GetElapsedTime(Timer->StartTime) - Timer->TargetTime;
   }
   
   return TimeValue;
}  