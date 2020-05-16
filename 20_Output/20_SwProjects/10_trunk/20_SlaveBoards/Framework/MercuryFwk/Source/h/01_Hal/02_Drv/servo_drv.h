/************************************************************************
*                        	Servo Driver                                *
*************************************************************************
* FileName:         servo_drv.h                                         *
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
* F.Ficili     06/01/18    1.0          First release                   *
************************************************************************/

#ifndef SERVO_DRV_H
#define	SERVO_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Servo driver interrupt timebase */
#define SERVO_DRV_INT_PER_US                             ((UINT16)(20))
/* Number of servos in the servo set */
#define SERVO_SET_NUMB                                   ((UINT8)(6))

#define SERVO_CH_1                                       ((UINT8)(0))
#define SERVO_CH_2                                       ((UINT8)(1))
#define SERVO_CH_3                                       ((UINT8)(2))
#define SERVO_CH_4                                       ((UINT8)(3))
#define SERVO_CH_5                                       ((UINT8)(4))
#define SERVO_CH_6                                       ((UINT8)(5))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Single servo control type*/
typedef struct _ServoCntType
{
   UINT16 PulseCnt;
   UINT8 OutSts;
   UINT8 Duty;      
} ServoCntType;


/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Servo set array */
extern ServoCntType Servo[SERVO_SET_NUMB];

/************************************************************************
* EXPORTED Functions
************************************************************************/
void SrvDrv_ServoPulse (void);
void SrvDrv_ServoInit (void);

#endif	/* SERVO_DRV_H */

