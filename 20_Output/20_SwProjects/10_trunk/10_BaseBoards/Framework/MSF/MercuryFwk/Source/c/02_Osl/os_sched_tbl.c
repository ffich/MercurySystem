/************************************************************************
*                          Scheduler Table                              *
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
#include "mod_imp.h"

/************************************************************************
* Defines
************************************************************************/
/* Constants period in ms */
#define PERIOD_1_MS                                      ((UINT16)(1))
#define PERIOD_10_MS                                     ((UINT16)(10))
#define PERIOD_100_MS                                    ((UINT16)(100))
#define PERIOD_1000_MS                                   ((UINT16)(1000))

/************************************************************************
* Global Variables
************************************************************************/
/* Schedule Table structure initialization */
SchedTblType TaskTable[] =
{
/*  --------------------------------------- HAL Tasks ----------------------------------------------------------  */ 
/*  ------------------------------------------------------------------------------------------------------------  */  
/*  Task                  State                Counter                 Timeout                        Options     */
/*  ------------------------------------------------------------------------------------------------------------  */
   #if (MODEM_USED == GSM_GPRS_MDM)
   {Mdm_DrvMainFn,        TaskIdle,            COUNTER_INIT,           MDM_DRIVER_PERIOD_MS,          NO_OPTIONS},
   #endif
   #if (MODEM_USED == BT_MDM)
   {MdmBt_DrvMainFn,      TaskIdle,            COUNTER_INIT,           MDM_BT_DRIVER_PERIOD_MS,       NO_OPTIONS},
   #endif
   #if (MODEM_USED == WIFI_MDM)
   {MdmWifi_DrvMainFn,    TaskIdle,            COUNTER_INIT,           MDM_WIFI_DRIVER_PERIOD_MS,     NO_OPTIONS},
   #endif
/*  ------------------------------------------------------------------------------------------------------------- */ 
        
/*  ---------------------------------------- SML Tasks ---------------------------------------------------------- */ 
/*  ------------------------------------------------------------------------------------------------------------  */  
/*  Task                   State                Counter                Timeout                        Options     */
/*  ------------------------------------------------------------------------------------------------------------  */
   {I2cSlv_MainTask,       TaskIdle,            COUNTER_INIT,          I2C_SLV_TASK_PERIOD_MS,        NO_OPTIONS},  
   #if (MODEM_USED == GSM_GPRS_MDM)
   {Mdm_MainTask,          TaskIdle,            COUNTER_INIT,          MDM_TASK_PERIOD_MS,            NO_OPTIONS},
   {MdmHttp_MainTask,      TaskIdle,            COUNTER_INIT,          MDM_HTTP_TASK_PERIOD_MS,       NO_OPTIONS},   
   #endif   
   #if (MODEM_USED == BT_MDM)
   {MdmBt_MainTask,        TaskIdle,            COUNTER_INIT,          MDM_BT_TASK_PERIOD_MS,         NO_OPTIONS},
   #endif   
   #if (MODEM_USED == WIFI_MDM)
   {MdmWifi_MainTask,      TaskIdle,            COUNTER_INIT,          MDM_WIFI_TASK_PERIOD_MS,       NO_OPTIONS},
   {MdmWifiCmd_MainTask,   TaskIdle,            COUNTER_INIT,          MDM_WIFI_CMD_TASK_PERIOD_MS,   NO_OPTIONS},
   {MdmWifiHttp_MainTask,  TaskIdle,            COUNTER_INIT,          MDM_WIFI_HTTP_TASK_PERIOD_MS,  NO_OPTIONS},    
   #endif   
   #if (USB_STS == STD_ON)
   {UsbDevice_MainTask,    TaskIdle,            COUNTER_INIT,          USB_DEV_TASK_PERIOD_MS,        NO_OPTIONS},
   #endif    
   #if (WDG_STS == STD_ON)
   {Wdg_MainTask,          TaskIdle,            COUNTER_INIT,          WDG_TASK_PERIOD_MS,            NO_OPTIONS},
   #endif       
   {Led_MainTask,          TaskIdle,            COUNTER_INIT,          LED_TASK_PERIOD_MS,            NO_OPTIONS},
   {Rtcm_MainTask,         TaskIdle,            COUNTER_INIT,          RTCM_TASK_PERIOD_MS,           NO_OPTIONS},
   #if (TERM_TASK_STS == STD_ON)   
   {Term_MainTask,         TaskIdle,            COUNTER_INIT,          TERM_TASK_PERIOD_MS,           NO_OPTIONS}, 
   #endif         
/*  ------------------------------------------------------------------------------------------------------------- */   
   
/*  ------------------------------------- Application Tasks ----------------------------------------------------  */
/*  ------------------------------------------------------------------------------------------------------------  */   
/*  Task                   State                Counter                Timeout                        Options     */
/*  ------------------------------------------------------------------------------------------------------------  */
   #if (APP_TASK_STS == STD_ON)      
   {APP_TASK_SYMB,         TaskIdle,            COUNTER_INIT,          APP_TASK_PERIOD_MS,            NO_OPTIONS},
   #endif      
/*  ------------------------------------------------------------------------------------------------------------  */
};

/* Task number constant */
const UINT16 TaskNumber = (UINT16)(sizeof(TaskTable)/sizeof(SchedTblType));

