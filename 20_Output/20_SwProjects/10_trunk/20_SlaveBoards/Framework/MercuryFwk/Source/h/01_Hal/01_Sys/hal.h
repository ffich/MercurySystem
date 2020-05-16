/************************************************************************
*                               HAL                                     *
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

/************************************************************************
* Common Inclusions
************************************************************************/
#include "xc.h"
#include "stdint.h"
#include "stdbool.h" 
#include "stdio.h"
#include "stdlib.h"
#include "typedefs.h"
#include "sys_cfg.h"
#include "sys_init.h"
#include "hw_profile.h"
#include "config_bits.h"
/* To allow drivers use events */
#include "os_ser.h"

/************************************************************************
* Compiler Switches
************************************************************************/


/************************************************************************
* Common Constants
************************************************************************/
#define ON                                               ((UINT8)(1))
#define OFF                                              ((UINT8)(0))

#define SET                                              ((UINT8)(1))
#define CLEAR                                            ((UINT8)(0))

/************************************************************************
* Compiler
************************************************************************/
#define XC8_COMPILER                                     1
#define XC16_COMPILER                                    2
#define XC32_COMPILER                                    3

#define COMPILER_USED                                    XC8_COMPILER     

/************************************************************************
* Typedef
************************************************************************/
/* Configuration status typedef */
typedef enum _CfgStsType
{
   CfgOk = 0,
   CfgError = 1           
} CfgStsType;

#endif	/* HAL_H */

