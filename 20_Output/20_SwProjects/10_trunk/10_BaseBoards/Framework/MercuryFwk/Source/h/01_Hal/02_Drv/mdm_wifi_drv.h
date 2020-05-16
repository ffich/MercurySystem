/************************************************************************
*                       Modem WiFi Low Level Driver                     *
*************************************************************************
* FileName:         mdm_wifi_drv.h                                      *
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
* F.Ficili     23/09/17    1.0          First release                   *
************************************************************************/

#ifndef MDM_WIFI_DRV_H
#define	MDM_WIFI_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

#if (MODEM_USED == WIFI_MDM)
/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Driver main function period in ms */
#define MDM_WIFI_DRIVER_PERIOD_MS                           ((UINT16)(10))

/************************************************************************
* EXPORTED Typedef
************************************************************************/

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Modem Status */
extern UINT8 WifiMdmStatus;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Wifi modem main function */
void MdmWifi_DrvMainFn (UINT8 Options);
/* API to restart modem */
void MdmWifi_RestartModem (void);

#endif /* MODEM_USED == BT_MDM */

#endif	/* MDM_BT_DRV_H */

