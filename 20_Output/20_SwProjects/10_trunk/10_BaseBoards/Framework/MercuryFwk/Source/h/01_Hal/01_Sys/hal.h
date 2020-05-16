/************************************************************************
*                       Hardware Abstraction Layer                      *
*************************************************************************
* FileName:         hal.h                                               *
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
* F.Ficili     23/07/16    1.0          First release                   *
************************************************************************/

#ifndef HAL_H
#define	HAL_H

/* Disable useless warning 1510 */
#pragma warning disable 1510

/************************************************************************
* Common Inclusions
************************************************************************/
#include "xc.h"
#include "sys_cfg.h"
#include "typedefs.h"
#include "sys_init.h"
#include "hw_profile.h"
#include "os_ser.h"
#include "irq.h"
#include "fwk_version.h"

/************************************************************************
* Common Constants
************************************************************************/
/* Standard ON and OFF constants */
#define STD_ON                                                  1u
#define STD_OFF                                                 0u

/* Standard TRUE and FALSE constants */
#define STD_TRUE                                                1u
#define STD_FALSE                                               0u

/* Standard OK and NOT_OK constants */
#define STD_OK                                                  1u
#define STD_NOT_OK                                              0u

/* Interrupt priority */
#define HIGH_PRIORITY                                           1u
#define LOW_PRIORITY                                            0u

/************************************************************************
* Common Typedef
************************************************************************/
/* Configuration status typedef */
typedef enum _CfgStsType
{
   CfgOk = 0,
   CfgError = 1           
} CfgStsType;

#endif	/* HAL_H */

