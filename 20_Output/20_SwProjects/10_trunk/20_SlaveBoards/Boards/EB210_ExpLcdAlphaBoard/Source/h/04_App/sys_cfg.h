/************************************************************************
*                          System Config                                *
*************************************************************************
* FileName:         sys_cfg.h                                           *
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

#ifndef SYS_CFG_H
#define	SYS_CFG_H

/************************************************************************
* Includes
************************************************************************/
#include "xc.h"

/************************************************************************
* Derivative
************************************************************************/
#include "pic16f1829.h"

/************************************************************************
* STD_ON/STD_OFF constants
************************************************************************/
#define STD_ON                                          1u
#define STD_OFF                                         0u

/************************************************************************
* Board Definition
************************************************************************/
/* SB Board Definition */
#define USE_EB210

/************************************************************************
* Oscillator Configuiration
************************************************************************/
#define INTERNAL_OSCILLATOR                             0u
#define EXT_HS_OSCILLATOR                               1u

#define OSCILLATOR_CFG                                  INTERNAL_OSCILLATOR

/************************************************************************
* System Timing
************************************************************************/
/* CPU clock frequency (Hz) */
#define SYSTEM_FREQUENCY_VALUE_HZ                       ((UINT32)(16000000))
/* Peripheral clock frequency (Hz) */
#define PER_FREQUENCY_VALUE_HZ                          (SYSTEM_FREQUENCY_VALUE_HZ/4)

/************************************************************************
* I2C Addressing
************************************************************************/
/* Address mode options */
#define ADDR_MODE_DIP_SWITCH                            0u
#define ADDR_MODE_HARDCODED                             1u    
#define ADDRESS_MODE                                    ADDR_MODE_DIP_SWITCH

/* I2c Hardcoded address */
#define I2C_HARDCODED_SLAVE_ADDRESS                     ((UINT8)(0x01))

/************************************************************************
* Test Task Config
************************************************************************/
#define TEST_TASK_STS                                   STD_OFF 

#endif	/* SYS_CFG_H */