/************************************************************************
*                          Os Schedule Table                            *
*************************************************************************
* FileName:         os_sched_tbl.c                                      *
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

/************************************************************************
* Includes
************************************************************************/
#include "os_sched_tbl.h"

/************************************************************************
* Defines
************************************************************************/
/* Timing constants in uS */
#define TIMEOUT_100_US                                      ((UINT16)(1))

/* Task timing constants */
#define SCH_TBL_SNR_DRV_TO                                  SNR_DRV_PERIOD_MS*TICK_TO_MS
#define SCH_TBL_WDG_TASK_TO                                 WDG_TASK_PERIOD_MS*TICK_TO_MS
#define SCH_TBL_LED_TASK_TO                                 LED_TASK_PERIOD_MS*TICK_TO_MS
#define SCH_TBL_APP_TASK_TO                                 APP_TASK_PERIOD_MS*TICK_TO_MS
#define SCH_TBL_TEST_TASK_TO                                TEST_TASK_PERIOD_MS*TICK_TO_MS

/************************************************************************
* Global Variables
************************************************************************/
/* Schedule Table structure initialization */
SchedTblType TaskTable[] =
{
/* ---------------------------------------------------- Driver Task -------------------------------------------------- */ 
/* ------------------------------------------------------------------------------------------------------------------- */
/* Task                 State                Counter                    Timeout                             Options    */
/* ------------------------------------------------------------------------------------------------------------------- */
   {I2c_DataReceiveSm,  TaskIdle,            COUNTER_INIT,              TIMEOUT_100_US,                     NO_OPTIONS},
   #ifdef USE_SB310
   {SnrDrv_GetDistSm,   TaskIdle,            COUNTER_INIT,              SCH_TBL_SNR_DRV_TO,                 NO_OPTIONS},
   #endif   
/* ------------------------------------------------------------------------------------------------------------------- */  
   
/* ---------------------------------------------------- Stacks Task -------------------------------------------------- */ 
/* ------------------------------------------------------------------------------------------------------------------- */  
/* Task                 State                Counter                    Timeout                             Options    */
/* ------------------------------------------------------------------------------------------------------------------- */
   {I2cSlv_MainTask,    TaskIdle,            COUNTER_INIT,              TIMEOUT_100_US,                     NO_OPTIONS},
   {Wdg_MainTask,       TaskIdle,            COUNTER_INIT,              SCH_TBL_WDG_TASK_TO,                NO_OPTIONS},
   {Led_MainTask,       TaskIdle,            COUNTER_INIT,              SCH_TBL_LED_TASK_TO,                NO_OPTIONS},   
/* ------------------------------------------------------------------------------------------------------------------- */   
   
/* --------------------------------------------------- Application Task ---------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------- */   
/* Task                 State                Counter                    Timeout                             Options    */
/* ------------------------------------------------------------------------------------------------------------------- */
   {App_MainTask,       TaskIdle,            COUNTER_INIT,              SCH_TBL_APP_TASK_TO,                NO_OPTIONS},
   #if (TEST_TASK_STS == STD_ON)  
   {Test_MainTask,      TaskIdle,            COUNTER_INIT,              SCH_TBL_TEST_TASK_TO,               NO_OPTIONS},
   #endif
/* ------------------------------------------------------------------------------------------------------------------- */
};

/* Task number constant */
const UINT16 TaskNumber = (UINT16)(sizeof(TaskTable)/sizeof(SchedTblType));

