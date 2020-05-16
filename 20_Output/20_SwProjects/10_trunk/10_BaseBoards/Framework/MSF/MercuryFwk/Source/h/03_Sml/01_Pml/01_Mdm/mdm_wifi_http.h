/************************************************************************
*                        	Modem Wifi HTTP                             *
*************************************************************************
* FileName:         mdm_wifi_http.h                                     *
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
* F.Ficili     06/11/17    1.0          First release                   *
************************************************************************/

#ifndef MDM_WIFI_HTTP_H
#define	MDM_WIFI_HTTP_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Task period in ms */
#define MDM_WIFI_HTTP_TASK_PERIOD_MS                         ((UINT16)(10))

/************************************************************************
* EXPORTED Typedef
************************************************************************/


/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Manage wifi HTTP main function task */
void MdmWifiHttp_MainTask (UINT8 Options);
/* API to initialize the wifi modem in Join Network modee */
void MdmWifiHttp_MdmJoinNetwork (const UINT8 *AccessPoint, const UINT8 *Password);
/* API to get if MdmWifiHttp_MdmJoinNetwork is completed */
UINT8 MdmWifiHttp_IsMdmJoinNetworkCompleted (void);
/* API to send a GET request to a specified Host */
void MdmWifiHttp_GetMethod (const UINT8 *GetString, const UINT8 *Host);
/* API to get if MdmWifiHttp_GetMethod is completed */
UINT8 MdmWifiHttp_IsGetMethodCompleted (void);
/* API to send a POST request to a specified Host */
void MdmWifiHttp_PostMethod (const UINT8 *PostString, const UINT8 *Host);
/* API to get if MdmWifiHttp_PostMethod is completed */
UINT8 MdmWifiHttp_IsPostMethodCompleted (void);

#endif	/* MDM_WIFI_HTTP_H */

