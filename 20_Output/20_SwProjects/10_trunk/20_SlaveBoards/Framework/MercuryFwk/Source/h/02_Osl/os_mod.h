/************************************************************************
*                          	Os Modules                            		*
*************************************************************************
* FileName:         os_mod.h                                      		*
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
* F.Ficili     01/01/18    1.0          First release                   *
************************************************************************/

#ifndef OS_MOD_H
#define OS_MOD_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* Defines
************************************************************************/


/************************************************************************
* Typedef
************************************************************************/


/************************************************************************
* Global Variables
************************************************************************/


/************************************************************************
* General Modules
************************************************************************/
/* Hal - System */
#include "hal.h"

/* Hal - Drivers */
#include "led_drv.h"
#include "i2c_slv_drv.h"
#include "sonar_drv.h"
#include "servo_drv.h"
#include "neopixel_drv.h"
#include "adc_drv.h"
#include "lcd_drv.h"
#include "mcu_drv.h"
#include "tmr_drv.h"
#include "dht11_drv.h"

/* Com Stacks */
#include "i2c_slv.h"

/* Tasks */
#include "led.h"
#include "std_ser.h"
#include "test.h"
#include "sf.h"
#include "wdg.h"

/************************************************************************
* Board App Specific
************************************************************************/

#ifdef USE_SB110
#include "sb110_app.h"
#endif

#ifdef USE_SB111
#include "sb111_app.h"
#endif

#ifdef USE_SB120
#include "sb120_app.h"
#endif

#ifdef USE_SB130
#include "sb130_app.h"
#endif

#ifdef USE_SB140
#include "sb140_app.h"
#endif

#ifdef USE_SB310
#include "sb310_app.h"
#endif

#ifdef USE_SB320
#include "sb320_app.h"
#endif

#ifdef USE_SB330
#include "sb330_app.h"
#endif

#ifdef USE_SB810
#include "sb810_app.h"
#endif

#ifdef USE_EB210
#include "eb210_app.h"
#endif

#ifdef USE_SBXXX
#include "sbXXX_app.h"
#endif

#endif /* OS_SCHED_TBL_H */