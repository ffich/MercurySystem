/************************************************************************
*                          Sonar Driver                                 *
*************************************************************************
* FileName:         sonar_drv.h                                         *
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
* F.Ficili     29/09/16    1.0          First release                   *
************************************************************************/

#ifndef SONAR_DRV_H
#define	SONAR_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
#define SNR_DRV_PERIOD_MS                             ((UINT16)(1)) 

/************************************************************************
* EXPORTED Typedef
************************************************************************/


/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Measured distance */
extern UINT16 DistanceCm;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Sonar driver main state machine */
void SnrDrv_GetDistSm(UINT8 Options);
/* Measure echo pulse */
void SnrDrv_DetectEchoPulse(void);
/* API to start a new sonar measure */
void SnrDrv_StartMeasure(void);

#endif	/* SONAR_DRV_H */

