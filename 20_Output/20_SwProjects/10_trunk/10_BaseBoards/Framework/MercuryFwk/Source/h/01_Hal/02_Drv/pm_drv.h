/************************************************************************
*                          Power Modes Driver                           *
*************************************************************************
* FileName:         pm_drv.c                                            *
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
* F.Ficili     23/04/17    1.0          First release                   *
************************************************************************/

#ifndef PWR_MODES_DRV_H
#define	PWR_MODES_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/


/************************************************************************
* EXPORTED Typedef
************************************************************************/

/* -- Power modes -- */

/************************************************************************
* Power Mode Summary table (DS table 4-1)
* -----------------------------------------------------------------------
* - Mode       - Cpu Clk - Per. Clk - Volt. Reg - Tmr1 Run - RTCC Run   
* -----------------------------------------------------------------------
* - Run        - Clocked - Clocked  - On        - Run      - Run    
* - Idle       - Off     - Clocked  - On        - Run      - Run
* - Sleep      - Off     - Off      - On        - Run      - Run
* - Deep Sleep - Off     - Off      - Off       - Not Run  - Run     
* -----------------------------------------------------------------------
************************************************************************/

typedef enum _PowerModesType
{
   RunMode = 0,
   IdleMode,
   SleepMode,
   DeepSleepMode,
   UltraLowPowerWakeUpMode
} PowerModesType;

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Change current power mode to target mode */
void Pm_ChangePwrMode (PowerModesType TargetMode);
/* Save data which needs to persist after a DS cyclce */
void Pm_SaveDsData(UINT8 Data0, UINT8 Data1);
/* Restore data which needs to persist after a DS cyclce */
void Pm_RestoreDsData(UINT8 *Data0, UINT8 *Data1);
/* Release pin control after Deep Sleep */
void Pm_DsReleasePinControl(void);

#endif	/* PWR_MODES_DRV_H */

