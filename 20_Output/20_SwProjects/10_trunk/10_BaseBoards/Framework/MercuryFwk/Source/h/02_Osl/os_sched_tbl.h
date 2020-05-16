/************************************************************************
*                      Scheduler Table                                  *
*************************************************************************
* FileName:         os_sched_tbl.h                                      *
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
* F.Ficili     24/07/16    1.0          First release                   *
************************************************************************/

#ifndef OS_SCHED_TBL_H
#define OS_SCHED_TBL_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* Defines
************************************************************************/
/* Counter init value */
#define COUNTER_INIT                                     ((UINT8)(0))
/* Options Constants */
#define NO_OPTIONS                                       ((UINT8)(0))

/************************************************************************
* Typedef
************************************************************************/
/* Task State Type */
typedef enum _TaskStateType
{
   TaskIdle,
   TaskRunning,
   TaskStopped,
   TaskBlocked,
} TaskStateType;

/* Schedule Table Type */
typedef struct _SchedTblType
{
   void (*Task) (UINT8);
   TaskStateType State;
   UINT16 Counter;
   const UINT16 TimeoutMs;
   UINT8 Options;
} SchedTblType;

/************************************************************************
* Global Variables
************************************************************************/
/* Task Table */
extern SchedTblType TaskTable[];
/* Task Number Constant */
extern const UINT16 TaskNumber;

#endif /* OS_SCHED_TBL_H */