/************************************************************************
*                               Scheduler                               *
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
/* Forever define for main while loop */
#define FOREVER                                          ((UINT8)(1))
/* Scheduler counter treshold define */
#define SCHED_COUNTER_TH                                 ((SCHED_TIMER_FREQ_HZ*DESIRED_SCHED_PERIOD_MS)/1000)

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
/* Manage the schedule flags with the right timing */
void UpdateSchFlag (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/
/************************************************************************   
* Function:     InitializeTasks
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Function used to initialize all tasks.
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
* Function:     ScheduleTasks
* Input:        None
* Output:       None
* Author:       F.Ficili	
* Description:  Scheduling algorithm.       
* Date:         24/07/16
************************************************************************/
void ScheduleTasks (void)
{
   /* Index of active task */
   UINT8 ActiveTaskIndex = 0u;

   /* Schedule Task Execution */  
   for (ActiveTaskIndex = 0u; ActiveTaskIndex < TaskNumber; ActiveTaskIndex++)
   {
      /* Increment task counter */
      TaskTable[ActiveTaskIndex].Counter++;
      /* Check for task activation */
      if (TaskTable[ActiveTaskIndex].Counter >= (TaskTable[ActiveTaskIndex].TimeoutMs/SCHED_COUNTER_TH))
      {
         TaskTable[ActiveTaskIndex].Counter = COUNTER_INIT;
         TaskTable[ActiveTaskIndex].State = TaskRunning;
         TaskTable[ActiveTaskIndex].Task(TaskTable[ActiveTaskIndex].Options);
         TaskTable[ActiveTaskIndex].State = TaskIdle;
      }
      else
      {
         /* IDLE, do nothing */
      }
   }    
}

/************************************************************************   
* Function:     UpdateSchFlag
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage the schedule flags with the right timing.
* Date:         17/04/17
************************************************************************/
void UpdateSchFlag (void)
{
   /* Local counter */
   static UINT16 InterruptCounter = 0;
   
   /* Increment local counter */
   InterruptCounter++;
   /* If the scheduler period is elapsed */
   if (InterruptCounter >= SCHED_COUNTER_TH)
   {
      /* Reset counter */
      InterruptCounter = 0;
      /* Main scheduler timebase flag */
      MainSystemTimebaseFlag = CallTaskPhase;
   }   
}

/************************************************************************   
* Function:     UpdateOsCounters
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Function used to increment Os-related counters.
* Date:         24/07/16
************************************************************************/
void UpdateOsCounters (void)
{
   /* Increment Os tick counter, this counter wraps after 4.294.967.296 counts */
   OsTmr_OsTickCounter++;
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     Sch_MainScheduler
* Input:        None
* Output:       None
* Author:       F.Ficili	
* Description:  Main system scheduler.	       
* Date:         24/07/16
************************************************************************/
void Sch_MainScheduler (void)
{
   /* Initialize all Tasks */
   InitializeTasks();
   /* Switch to running state */
   SystemState = RunningState;
      
   #if (USE_EXEC_TIME_MEAS_PIN == STD_ON)
      EXEC_TIME_MEAS_PIN_TRIS = LINE_DIRECTION_OUTPUT;
   #endif   
   
   /* Infinite loop */
   while(FOREVER)
   {
      /* If the scheduler timer has expired */
      if (MainSystemTimebaseFlag == CallTaskPhase)
      {            
         #if (USE_EXEC_TIME_MEAS_PIN == STD_ON)
            EXEC_TIME_MEAS_PIN = STD_ON;
         #endif         
         /* Start scheduling cycle */
         ScheduleTasks();
         #if (USE_EXEC_TIME_MEAS_PIN == STD_ON)
            EXEC_TIME_MEAS_PIN = STD_OFF;
         #endif           
         /* Reset flag */
         MainSystemTimebaseFlag = WaitTriggerPhase;
      }
   }
}

/************************************************************************   
* Function:     Sch_SchmIntService
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage the various Scheduler interrupt services.
* Date:         24/04/17
************************************************************************/
void Sch_SchmIntService (void)
{
   /* Update scheduler flag */
   UpdateSchFlag();
   /* Update Os-related counters */
   UpdateOsCounters();
   /* Process Os alarms */
   OsAlrm_ProcessOsAlarms();
}