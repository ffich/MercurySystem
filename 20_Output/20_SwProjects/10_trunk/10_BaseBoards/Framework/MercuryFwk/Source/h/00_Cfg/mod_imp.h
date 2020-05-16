/************************************************************************
*                             Module Importer                           *
*************************************************************************
* FileName:         mod_imp.h                                           *
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
* F.Ficili     14/04/17    1.0          First release                   *
************************************************************************/

#ifndef MOD_IMP_H
#define	MOD_IMP_H

/************************************************************************
* Imported Modules
************************************************************************/
/* HAL - System */
#include "hal.h"

/* HAL - Drivers */
#include "i2c_drv.h"
#if (MODEM_USED == GSM_GPRS_MDM)
#include "mdm_drv.h"
#endif
#if (MODEM_USED == BT_MDM)
#include "mdm_bt_drv.h"
#endif
#if (MODEM_USED == WIFI_MDM)
#include "mdm_wifi_drv.h"
#endif
#include "uart_drv.h"
#include "mcu_drv.h"
#include "tmr_drv.h"
#include "mcu_drv.h"
#include "tmr_drv.h"
#include "rtcc_drv.h"
#include "pm_drv.h"

/* SML - Peripheral Management */
#include "i2c_slv.h"
#if (MODEM_USED == GSM_GPRS_MDM)
#include "mdm_gsm_gprs.h"
#endif
#if (MODEM_USED == BT_MDM)
#include "mdm_bt.h"
#endif
#if (MODEM_USED == WIFI_MDM)
#include "mdm_wifi.h"
#endif
#include "usb_device_mgr.h"

/* SML - System Services */
#include "led.h"
#include "rtcm.h"
#include "sysm.h"
#include "sf.h"
#include "term.h"
#include "test.h"

/* APP */

/* Import here you app modules */

#endif	/* MOD_IMP_H */

