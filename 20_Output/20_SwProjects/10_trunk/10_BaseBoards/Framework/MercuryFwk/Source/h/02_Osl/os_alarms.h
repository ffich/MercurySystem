/************************************************************************
*                                Os Alarms                              *
*************************************************************************
* FileName:         os_alarms.h                                         *
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

#ifndef OS_ALARMS_H
#define	OS_ALARMS_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Number of Os alarms */
#define OS_ALARM_NUMBER                                 ((UINT8)(20))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Alarm callback */
typedef void (*OsAlarmCallbackType) (void);

/* OsAlarm type */
typedef struct _OsAlarmType
{
   UINT8 Enabled;
   UINT32 Counter;
   UINT32 AlarmTime;
   OsAlarmCallbackType AlrmCbk; 
} OsAlarmType;

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* API to set an OS alarm */
void OsAlrm_SetOsAlarm (UINT16 OsAlarmId, UINT32 OsAlarmTimeout, OsAlarmCallbackType AlarmCallback);
/* API to clear an OS alarm */
void OsAlrm_ClearOsAlarm (UINT8 OsAlarmId);
/* Process alarms main function */
void OsAlrm_ProcessOsAlarms (void);

#endif	/* OS_ALARMS_H */

