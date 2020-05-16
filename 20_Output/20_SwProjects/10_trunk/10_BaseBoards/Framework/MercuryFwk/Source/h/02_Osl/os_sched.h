/************************************************************************
*                            Scheduler                                  *
*************************************************************************
* FileName:         os_sched.h                                          *
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

#ifndef OS_SCHED_H
#define OS_SCHED_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"
#include "os_sched_tbl.h"
#include "os_ser.h"
#include "os_timers.h"
#include "os_alarms.h"
#include "mod_imp.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/

/************************************************************************
* Scheduler timing configuration
* ------------------------------
* Set: 
* - SCHED_TIMER_FREQ_HZ          --> In Hertz!!!
* - DESIRED_SCHED_PERIOD_MS      --> In ms!!!
* To obtain the desired scheduler period with the desired timer freq.
************************************************************************/
/* Frequency of scheduler timer */
#define SCHED_TIMER_FREQ_HZ                     ((UINT16)(1000))
/* Desired scheduler period */
#define DESIRED_SCHED_PERIOD_MS                 ((UINT16)(1))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Main system timebase flag type */
typedef enum MainSystemTimebaseEnum
{
   CallTaskPhase,
   WaitTriggerPhase,
} MainSystemTimebaseType;

/* Main system state type */
typedef enum SystemStateEnum
{
   InitializationState,
   RunningState
} SystemStateType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Main system timebase flag */
extern MainSystemTimebaseType MainSystemTimebaseFlag;
/* System state */
extern SystemStateType SystemState;


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* System main scheduler */
void Sch_MainScheduler (void);
/* Manage the various Scheduler interrupt services */
void Sch_SchmIntService (void);

#endif /* OS_SCHED_H */