/************************************************************************
*                             RTCC Driver                              	*
*************************************************************************
* FileName:         rtcc_drv.h                                          *
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
* F.Ficili     19/04/17    1.0          First release                   *
************************************************************************/

#ifndef RTCC_DRV_H
#define	RTCC_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Weekdays defines */
#define WEEKDAY_SUNDAY                                      ((UINT8)(0))
#define WEEKDAY_MONDAY                                      ((UINT8)(1))
#define WEEKDAY_TUESDAY                                     ((UINT8)(2))
#define WEEKDAY_WEDNESDAY                                   ((UINT8)(3))
#define WEEKDAY_THURSDAY                                    ((UINT8)(4))
#define WEEKDAY_FRIDAY                                      ((UINT8)(5))
#define WEEKDAY_SATURDAY                                    ((UINT8)(6))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Rtcc out sel bit typedef */
typedef enum _RtcOutSelBitType
{
   RtccAlarmPulseOnRtccPin = 0,
   RtccSecClkOnRtccPin,
   RtccSourceClkOnRtccPin,
} RtcOutSelBitType;

/* Rtcc Alarma mask typedef */
typedef enum _RtcAlarmMaskType
{
   EveryHalfSecond = 0,
   EverySecond,
   Every10Seconds,
   EveryMinute,
   Every10Minutes,
   EveryHour,
   OnceADay,
   OnceAWeek,
   OnceAMonth,
   OnceAYear           
} RtcAlarmMaskType;

/* Value pointer enum typedef */
typedef enum _RtccValPointerType
{
   ValPtrYear = 0,
   ValPtrMonth,
   ValPtrDay,
   ValPtrWeekday,
   ValPtrHours,
   ValPtrMinutes,
   ValPtrSeconds,            
} RtccValPointerType;

/* Alarm pointer enum typedef */
typedef enum _RtccAlrmPointerType
{
   AlrmPtrMonth = 0,
   AlrmPtrDay,
   AlrmPtrWeekday,
   AlrmPtrHours,
   AlrmPtrMinutes,
   AlrmPtrSeconds,            
} RtccAlrmPointerType;

/* Rtcc configuration typedef */
typedef struct _RtccConfigType
{
   UINT8 RtcOutPinEnable;
   RtcOutSelBitType RtcOutputSelectBit;
   IntEnableType IntEnable;
   IntPriorityType IntPriority;   
} RtccConfigType;

/* Alarm configuration typedef */
typedef struct _RtccAlarmConfigType
{
   UINT8 RtcAlarmEnable;
   UINT8 RtcChimeEnable;
   RtcAlarmMaskType AlarmMask;
   UINT8 AlarmRepeat;
} RtccAlarmConfigType;

/* Callback typedef */
typedef void (*Rtcc_CallbackType) (void);

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* RTCC ISR callback pointer */
Rtcc_CallbackType RtccIsrCallback = NULL;

/************************************************************************
* EXPORTED Functions
************************************************************************/

/* -- APIs ----------------------------------------------------------- */
/* Configure the RTCC (Real Time Clock Calendar) module */
void Rtcc_RtcConfig(RtccConfigType *RtcConfig);
/* Configure the RTC alarm */
void Rtcc_AlarmConfig (RtccAlarmConfigType *AlarmConfig);
/* Set the RTC status */
void Rtcc_SetRtcStatus (UINT8 Status);
/* Unlock RTC */
void Rtcc_UnlockRtc (void);
/* Lock RTC */
void Rtcc_LockRtc (void);
/* Get the requested RTCC value */
UINT8 Rtcc_GetRtccValue (RtccValPointerType RtccPtrVal);
/* Get the requested RTCC alarm */
UINT8 Rtcc_GetRtccAlarm (RtccAlrmPointerType RtccPtrAlrm);
/* Set the pointed RTCC value */
void Rtcc_SetRtccValue (RtccValPointerType RtccPtrVal, UINT8 Value);
/* Set the pointed RTCC Alarm */
void Rtcc_SetRtccAlarm (RtccValPointerType RtccPtrVal, UINT8 Value);
/* Calibrate the RTCC module */
void Rtcc_RtcCalibrate (INT8 Cal);

/* -- ISRs ----------------------------------------------------------- */
/* Interrutp Service Routine for RTCC */
void Rtcc_RtcIsr (void);

/* -- Callbacks Registrators ------------------------------------------ */
/* Register callback function to service the ISR */
void Rtcc_RegisterRtcIsrCallback (Rtcc_CallbackType IsrCallback);

#endif	/* RTCC_DRV_H */

