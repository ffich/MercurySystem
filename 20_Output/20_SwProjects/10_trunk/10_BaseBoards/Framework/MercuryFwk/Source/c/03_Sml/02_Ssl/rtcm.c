/************************************************************************
*                             RTCC Manager                              *
*************************************************************************
* FileName:         rtcm.c                                              *
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

#include "rtcm.h"


/************************************************************************
* Includes
************************************************************************/


/************************************************************************
* Defines
************************************************************************/
//#define ALARM_EXAMPLE

/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/
/* Rtcc config */
RtccConfigType RtcConfig;
/* Alarm config */
RtccAlarmConfigType AlCfg;


/************************************************************************
* GLOBAL Variables
************************************************************************/
/* System date */
RtccDateType Rtcm_SystemDate = {0, 0, 0, 0, 0, 0, 0};
/* Alarm variable */
RtccAlarmType Rtcm_RtccAlarm = {0, 0, 0, 0, 0, 0};
/* Events */
EventStructureType AlarmEvent = {EventIdle};

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/


/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     Alarm
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  Alarm example.
* Date:         23/04/17
************************************************************************/
void Alarm (void)
{
   /* Set LED and send Alarm event */
   BSP_LED_1 = !BSP_LED_1;
   /* Send alarm event */
   GenerateEvt(&AlarmEvent);
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/
/************************************************************************
* Function:     Rtcm_MainTask
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Manage RTCC maintask.
* Date:         23/04/17
************************************************************************/
void Rtcm_MainTask (UINT8 Options)
{    
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:         
         /* Power the LP oscillator */
         T1CONbits.T1OSCEN = STD_ON;                  
         /* RTCC */
         RtcConfig.RtcOutPinEnable = STD_OFF;
         RtcConfig.RtcOutputSelectBit = RtccAlarmPulseOnRtccPin;
         RtcConfig.IntEnable = EnableInt;
         RtcConfig.IntPriority = RTCC_ISR_PRIORITY;

         /* Alarm */
         AlCfg.RtcAlarmEnable = STD_ON;
         AlCfg.RtcChimeEnable = STD_ON;
         AlCfg.AlarmMask = EverySecond;
         AlCfg.AlarmRepeat = 0;  
                  
         /* Unlock */
         Rtcc_UnlockRtc();  
         /* Init RTCC and Alarm */
         Rtcc_RtcConfig(&RtcConfig);
         Rtcc_AlarmConfig(&AlCfg);
         /* Start rtcc */
         Rtcc_SetRtcStatus(STD_ON);
         /* Lock */
         Rtcc_LockRtc();
         /* Dummy date */
         Rtcm_SetRtccDate(Rtcm_SystemDate);      
         
#ifdef ALARM_EXAMPLE
         /* Set date */
         Rtcm_SystemDate.Year = 2017;
         Rtcm_SystemDate.Month = 4;
         Rtcm_SystemDate.Day = 23;
         Rtcm_SystemDate.Weekday = WEEKDAY_SUNDAY;
         Rtcm_SystemDate.Hour = 0;
         Rtcm_SystemDate.Minute = 0;
         Rtcm_SystemDate.Second = 0;         
         Rtcm_SetRtccDate(Rtcm_SystemDate);
         
         /* Set alarm - 10 seconds */
         Rtcm_RtccAlarm.AlrmMonth = 4;
         Rtcm_RtccAlarm.AlrmDay = 23;
         Rtcm_RtccAlarm.AlrmWeekday = WEEKDAY_SUNDAY;
         Rtcm_RtccAlarm.AlrmHour = 0;
         Rtcm_RtccAlarm.AlrmMinute = 1;
         Rtcm_RtccAlarm.AlrmSecond = 0;         
         Rtcm_SetRtccAlarm(Rtcm_RtccAlarm);                  
         
         /* Finally register alarm action */
         Rtcm_SetAlarmAction(&Alarm);
#endif
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Update system date every second */
         Rtcm_SystemDate = Rtcm_GetRtccDate();
         break;
         
      /* Default */
      default:
         break;
   }
}    

/************************************************************************
* Function:     Rtcm_SetRtccDate
* Input:        RtccDateType Date                     
* Output:       None
* Author:       F.Ficili
* Description:  API to set the RTCC date.
* Date:         23/04/17
************************************************************************/
void Rtcm_SetRtccDate (RtccDateType Date)
{
   /* Unlock */
   Rtcc_UnlockRtc();  
   
   /* Set date */
   Rtcc_SetRtccValue(ValPtrYear,(Date.Year - 2000));
   Rtcc_SetRtccValue(ValPtrMonth,Date.Month);
   Rtcc_SetRtccValue(ValPtrDay,Date.Day);   
   Rtcc_SetRtccValue(ValPtrWeekday,Date.Weekday);
   Rtcc_SetRtccValue(ValPtrHours,Date.Hour);
   Rtcc_SetRtccValue(ValPtrMinutes,Date.Minute);   
   Rtcc_SetRtccValue(ValPtrSeconds,Date.Second);     
   
   /* Lock */
   Rtcc_LockRtc();      
}

/************************************************************************
* Function:     Rtcm_GetRtccDate
* Input:        None                     
* Output:       RtccDateType
* Author:       F.Ficili
* Description:  API to get the current RTCC date.
* Date:         23/04/17
************************************************************************/
RtccDateType Rtcm_GetRtccDate (void)
{
   RtccDateType LocalDate;
   
   /* Get date */
   LocalDate.Year = 2000 + Rtcc_GetRtccValue(ValPtrYear);
   LocalDate.Month = Rtcc_GetRtccValue(ValPtrMonth);
   LocalDate.Day = Rtcc_GetRtccValue(ValPtrDay);
   LocalDate.Weekday = Rtcc_GetRtccValue(ValPtrWeekday);  
   LocalDate.Hour = Rtcc_GetRtccValue(ValPtrHours);
   LocalDate.Minute = Rtcc_GetRtccValue(ValPtrMinutes);  
   LocalDate.Second = Rtcc_GetRtccValue(ValPtrSeconds);
     
   /* Return date */
   return LocalDate;
}

/************************************************************************
* Function:     Rtcm_SetRtccAlarm
* Input:        RtccAlarmType Alarm                   
* Output:       None
* Author:       F.Ficili
* Description:  API to set the RTCC Alarm.
* Date:         23/04/17
************************************************************************/
void Rtcm_SetRtccAlarm (RtccAlarmType Alarm)
{
   /* Unlock */
   Rtcc_UnlockRtc();  
   
   /* Set Alarm */
   Rtcc_SetRtccAlarm(AlrmPtrMonth,Alarm.AlrmMonth);
   Rtcc_SetRtccAlarm(AlrmPtrDay,Alarm.AlrmDay);   
   Rtcc_SetRtccAlarm(AlrmPtrWeekday,Alarm.AlrmWeekday);
   Rtcc_SetRtccAlarm(AlrmPtrHours,Alarm.AlrmHour);
   Rtcc_SetRtccAlarm(AlrmPtrMinutes,Alarm.AlrmMinute);   
   Rtcc_SetRtccAlarm(AlrmPtrSeconds,Alarm.AlrmSecond);     
   
   /* Lock */
   Rtcc_LockRtc();      
}

/************************************************************************
* Function:     Rtcm_GetRtccAlarm
* Input:        None                 
* Output:       RtccAlarmType
* Author:       F.Ficili
* Description:  API to get the RTCC Alarm.
* Date:         23/04/17
************************************************************************/
RtccAlarmType Rtcm_GetRtccAlarm (void)
{
   RtccAlarmType LocalAlarm;
   
   /* Get Alarm */
   LocalAlarm.AlrmMonth = Rtcc_GetRtccAlarm(AlrmPtrMonth);
   LocalAlarm.AlrmDay = Rtcc_GetRtccAlarm(AlrmPtrDay);   
   LocalAlarm.AlrmWeekday = Rtcc_GetRtccAlarm(AlrmPtrWeekday);
   LocalAlarm.AlrmHour = Rtcc_GetRtccAlarm(AlrmPtrHours);
   LocalAlarm.AlrmMinute = Rtcc_GetRtccAlarm(AlrmPtrMinutes);   
   LocalAlarm.AlrmSecond = Rtcc_GetRtccAlarm(AlrmPtrSeconds);     
   
   /* Return date */
   return LocalAlarm;   
}

/************************************************************************
* Function:     Rtcm_SetAlarmAction
* Input:        None                 
* Output:       RtccAlarmType
* Author:       F.Ficili
* Description:  API to set the action to be performed when the alarm fires.
* Date:         23/04/17
************************************************************************/
void Rtcm_SetAlarmAction (Rtcc_CallbackType Action)
{
   /* Register alarm action */
   Rtcc_RegisterRtcIsrCallback(Action);
}
