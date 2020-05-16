/************************************************************************
*                             RTCC Manager                              *
*************************************************************************
* FileName:         rtcm.h                                              *
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
* F.Ficili     21/04/17    1.0          First release                   *
************************************************************************/

#ifndef RTCC_MGR_H
#define	RTCC_MGR_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Task period in ms */
#define RTCM_TASK_PERIOD_MS                               ((UINT16)(100))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Rtcc Date Struct */
typedef struct _RtccDateType
{
   UINT16 Year;
   UINT8 Month;
   UINT8 Day;
   UINT8 Weekday;
   UINT8 Hour;
   UINT8 Minute;
   UINT8 Second;   
} RtccDateType;

/* Rtcc Alarm Struct */
typedef struct _RtccAlarmType
{
   UINT8 AlrmMonth;
   UINT8 AlrmDay;
   UINT8 AlrmWeekday;
   UINT8 AlrmHour;
   UINT8 AlrmMinute;
   UINT8 AlrmSecond;   
} RtccAlarmType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* System Date variable */
extern RtccDateType Rtcm_SystemDate;
/* Alarm variable */
extern RtccAlarmType Rtcm_RtccAlarm;
/* Events */
extern EventStructureType AlarmEvent;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* RTCM main task */
void Rtcm_MainTask (UINT8 Options);
/* API to set the RTCC date */
void Rtcm_SetRtccDate (RtccDateType Date);
/* API to get the current RTCC date */
RtccDateType Rtcm_GetRtccDate (void);
/* API to set the RTCC Alarm */
void Rtcm_SetRtccAlarm (RtccAlarmType Alarm);
/* API to get the RTCC Alarm */
RtccAlarmType Rtcm_GetRtccAlarm (void);
/* API to set the action to be performed when the alarm fires */
void Rtcm_SetAlarmAction (Rtcc_CallbackType Action);

#endif	/* RTCC_MGR_H */

