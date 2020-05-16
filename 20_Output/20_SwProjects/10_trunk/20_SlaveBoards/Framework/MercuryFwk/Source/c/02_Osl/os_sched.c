/************************************************************************
*                               Os Sched                                *
*************************************************************************
* FileName:         os_sched.c                                          *
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
#include "os_sched.h"

/************************************************************************
* Defines
************************************************************************/
#define FOREVER                                         ((UINT8)(1))

/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/


/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Main system timebase */
MainSystemTimebaseType MainSystemTimebaseFlag = WaitTriggerPhase;
/* Main system state */
SystemStateType SystemState = InitializationState;

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
/* Service function for task initialization */
void InitializeTasks (void);
/* Service function for task scheduling */
void ScheduleTasks (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/


/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     void MainScheduler (void)
* Input:        None
* Output:       None
* Author:       F.Ficili	
* Description:	Main system scheduler.	       
* Date:         24/07/16
************************************************************************/
void MainScheduler (void)
{
   /* Initialize all Tasks */
   InitializeTasks();
   /* Switch to running state */
   SystemState = RunningState;
   
   #ifdef USE_EXEC_TIME_MEAS_PIN
      BSP_IO_1_TRIS = LINE_DIRECTION_OUTPUT;
   #endif

   /* Infinite loop */
   while(FOREVER)
   {
      /* If the scheduler timer has expired */
      if (MainSystemTimebaseFlag == CallTaskPhase)
      {
         /* Start scheduling cycle */
         ScheduleTasks();
         /* Reset flag */
         MainSystemTimebaseFlag = WaitTriggerPhase;
      }
   }
}
/************************************************************************
* Function:     void InitializeTasks (void)
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	Function used to initialize all tasks.
* Date:         24/07/16
************************************************************************/
void InitializeTasks (void)
{
   /* Index of active task */
   UINT8 ActiveTaskIndex = 0u;
    
   /* Initialize all the tasks */ 
   for (ActiveTaskIndex = 0u; ActiveTaskIndex < TaskNumber; ActiveTaskIndex++)
   {
      TaskTable[ActiveTaskIndex].Task(TaskTable[ActiveTaskIndex].Options);
   }    
}
/************************************************************************
* Function:     void ScheduleTasks (void)
* Input:        None
* Output:       None
* Author:       F.Ficili	
* Description:	Scheduling algorithm.       
* Date:         24/07/16
************************************************************************/
void ScheduleTasks (void)
{
   /* Index of active task */
   UINT8 ActiveTaskIndex = 0u;

   /* Schedule Task Execution */  
   for (ActiveTaskIndex = 0u; ActiveTaskIndex < TaskNumber; ActiveTaskIndex++)
   {
       if (TaskTable[ActiveTaskIndex].Counter >= (TaskTable[ActiveTaskIndex].Timeout))
       {
           TaskTable[ActiveTaskIndex].Counter = COUNTER_INIT;
           TaskTable[ActiveTaskIndex].State = TaskRunning;
           TaskTable[ActiveTaskIndex].Task(TaskTable[ActiveTaskIndex].Options);
       }
       else
       {
           TaskTable[ActiveTaskIndex].Counter++;
           TaskTable[ActiveTaskIndex].State = TaskIdle;
       }
   }    
}