/************************************************************************
*                                Os Alarms                              *
*************************************************************************
* FileName:         os_alarms.c                                         *
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

/************************************************************************
* Includes
************************************************************************/
#include "os_alarms.h"

/************************************************************************
* Defines
************************************************************************/


/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/
/* Os alarm array */
OsAlarmType OsAlarms[OS_ALARM_NUMBER];

/************************************************************************
* GLOBAL Variables
************************************************************************/


/************************************************************************
* LOCAL Function Prototypes
************************************************************************/


/************************************************************************
* LOCAL Function Implementations
************************************************************************/


/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************   
* Function:     OsAlrm_SetOsAlarm
* Input:        UINT8 OsAlarmId
*               UINT32 OsAlarmTimeout
*               OsAlarmCallbackType AlarmCallback 
* Output:       None
* Author:       F.Ficili
* Description:  API to set an OS alarm. Once the alarm timeout expires the user callback
*               will be automatically executed. 
* Date:         24/04/17
************************************************************************/
void OsAlrm_SetOsAlarm (UINT16 OsAlarmId, UINT32 OsAlarmTimeout, OsAlarmCallbackType AlarmCallback)
{
   /* Enable alarm */
   OsAlarms[OsAlarmId].Enabled = STD_TRUE;
   /* Reset counter */
   OsAlarms[OsAlarmId].Counter = 0;
   /* Set alarm time */
   OsAlarms[OsAlarmId].AlarmTime = OsAlarmTimeout;
   /* Register callback */
   OsAlarms[OsAlarmId].AlrmCbk = AlarmCallback;
}

/************************************************************************   
* Function:     OsAlrm_ClearOsAlarm
* Input:        UINT8 OsAlarmId
* Output:       None
* Author:       F.Ficili
* Description:  API to clear an OS alarm.
* Date:         24/04/17
************************************************************************/
void OsAlrm_ClearOsAlarm (UINT8 OsAlarmId)
{
   /* Enable alarm */
   OsAlarms[OsAlarmId].Enabled = STD_FALSE;
   /* Reset counter */
   OsAlarms[OsAlarmId].Counter = 0;
   /* Reset alarm time */
   OsAlarms[OsAlarmId].AlarmTime = 0;
}

/************************************************************************   
* Function:     OsAlrm_ProcessOsAlarms
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Process alarms main function.
* Date:         24/04/17
************************************************************************/
void OsAlrm_ProcessOsAlarms (void)
{
   UINT16 AlarmIndex;
   
   /* Scroll alarm list */
   for (AlarmIndex = 0; AlarmIndex < OS_ALARM_NUMBER; AlarmIndex++)
   {
      /* If the alarm is enabled */
      if (OsAlarms[AlarmIndex].Enabled == STD_TRUE)
      {
         /* Increment counter */
         OsAlarms[AlarmIndex].Counter++;
         /* Check if the alarm is expired */
         if (OsAlarms[AlarmIndex].Counter >= OsAlarms[AlarmIndex].AlarmTime)
         { 
            /* Clear alarm... */
            OsAlrm_ClearOsAlarm(AlarmIndex);  
            /* ... check if the callback is valid... */
            if (OsAlarms[AlarmIndex].AlrmCbk != NULL)
            {
               /* ... and trigger callback */
               OsAlarms[AlarmIndex].AlrmCbk();            
            }            
         }  
      }
   }
}