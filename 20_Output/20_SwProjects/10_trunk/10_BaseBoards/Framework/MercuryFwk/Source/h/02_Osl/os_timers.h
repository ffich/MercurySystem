/************************************************************************
*                          Os Software Timers                           *
*************************************************************************
* FileName:         os_timers.h                                         *
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
************************************************************************/

#ifndef OS_SW_TIMERS_H
#define	OS_SW_TIMERS_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Max count define */
#define MAX_SW_TIMER_COUNT                                  0xFFFFFFFF
/* Init dis macro */
#define SW_TIMER_INIT_DIS     \
{                             \
   STD_FALSE,                 \
   SwTimerExpired,            \
   0,                         \
   0,                         \
   DelayNotExpired,           \
}

/* Init en macro */
#define SW_TIMER_INIT_EN      \
{                             \
   STD_TRUE,                  \
   SwTimerExpired,            \
   0,                         \
   0,                         \
   DelayNotExpired,           \
}

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Sw timer Status type */
typedef enum _SwTimerStatusType
{
   SwTmrNotExpired = 0,
   SwTimerExpired,
   SwTimerDisabled,           
} SwTimerStatusType;

/* Wait state type */
typedef enum _WaitStateType
{
   WtStartSwTimer = 0,
   WtWait,
} WaitStateType;

/* Delay Status type */
typedef enum _DelayStatusType
{
   DelayNotExpired = 0,   
   DelayExpired,        
} DelayStatusType;

/* Sw timer type */
typedef struct _SwTimerType
{
   UINT8 Enabled;
   SwTimerStatusType Status;
   UINT32 StartTime;
   UINT32 TargetTime;
   WaitStateType WaitState;
} SwTimerType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Os tick global counter */
extern UINT32 OsTmr_OsTickCounter;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* API to start a software timer */
void OsTmr_StartTimer (SwTimerType *Timer, UINT32 Timeout);
/* API to stop a software timer */
void OsTmr_StopTimer (SwTimerType *Timer);
/* API that implement a non-blocking delay funtion */
DelayStatusType OsTmr_Wait (SwTimerType *Timer, UINT32 DelayMs);
/* API that checks the software timer status */
SwTimerStatusType OsTmr_GetTimerStatus (SwTimerType *Timer);
/* API that gets the elapsed time since the sw timer started */
UINT32 OsTmr_GetElapsedTime (SwTimerType *Timer);
/* API that gets the remaining time before a sw timer expires */
UINT32 OsTmr_GetRemainingTime (SwTimerType *Timer);

#endif	/* OS_SW_TIMERS_H */

