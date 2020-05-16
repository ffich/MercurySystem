/************************************************************************
*                             Os Scheduler                              *
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

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Define scheduler tick */
#define TICK_100_US                                      ((UINT16)(1))

/* Ticks to 1 ms */
#define TICK_TO_MS                                       ((UINT16)(10))

/* Scheduler period */
#define SCHEDULER_DES_TICK_TIME                          TICK_100_US

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
void MainScheduler (void);

#endif /* OS_SCHED_H */