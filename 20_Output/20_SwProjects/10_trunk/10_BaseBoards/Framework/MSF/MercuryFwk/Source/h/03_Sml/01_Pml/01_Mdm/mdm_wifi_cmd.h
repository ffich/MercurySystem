/************************************************************************
*                        	Modem Wifi Commands                         *
*************************************************************************
* FileName:         mdm_wifi_cmd.h                                      *
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

#ifndef MDM_WIFI_CMD_H
#define	MDM_WIFI_CMD_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Task period in ms */
#define MDM_WIFI_CMD_TASK_PERIOD_MS                         ((UINT16)(10))

/* Transmission mode options */
#define TX_MODE_NORMAL                                      ((UINT8)(0))
#define TX_MODE_UNVARNISHED                                 ((UINT8)(1))

/* Wifi mode options */
#define STATION_MODE                                        ((UINT8)(1))
#define SOFT_AP_MODE                                        ((UINT8)(2))
#define SOFT_AP_AND_STATION_MODE                            ((UINT8)(3))

/* Connection mode options */
#define CONN_MODE_SINGLE                                    ((UINT8)(0))
#define CONN_MODE_MULTIPLE                                  ((UINT8)(1))

/* Protocol options */
#define PROTOCOL_UDP                                        ((UINT8)(0))
#define PROTOCOL_TCP                                        ((UINT8)(1))

/* Connection status */
#define CONN_REFUSED                                        ((UINT8)(0))
#define CONN_ACCEPTED                                       ((UINT8)(1))

/* Server Mode */
#define DELETE_SERVER                                       ((UINT8)(0))
#define CREATE_SERVER                                       ((UINT8)(1))

/* Soft AP encoding */
#define ENC_OPEN                                            ((UINT8)(0))  
#define WPA_PSK                                             ((UINT8)(2))  
#define WPA2_PSK                                            ((UINT8)(3))  
#define WPA_WPA2_PSK                                        ((UINT8)(4))  

/************************************************************************
* EXPORTED Typedef
************************************************************************/
typedef enum _WifiDataRecType
{
   WiFiRcv_DataNotReceived,
   WiFiRcv_DataReceived,
} WifiDataRecType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
extern EventStructureType WifiSendError;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Manage wifi cmd main function task */
void MdmWifiCmd_MainTask (UINT8 Options);
/* API used to restart Wifi Modem */
void MdmWifiCmd_RestartModem (void);
/* API used to set the modem wifi mode */
UINT8 MdmWifiCmd_SetWifiMode (UINT8 WifiMode);
/* API used to join an existing access point with password */
void MdmWifiCmd_JoinAccessPoint (const UINT8 *AccessPoint, const UINT8 *Password);
/* API used to quit a previously joined access point */
void MdmWifiCmd_QuitAccessPoint (void);
/* API used to set the transmission mode (normal or unvarnished) */
UINT8 MdmWifiCmd_SetTransferMode (UINT8 Mode);
/* API used to set the connection mode (single or multiple) */
UINT8 MdmWifiCmd_SetConnectionMode (UINT8 Mode);
/* API used to start a TCP or UDP connection */
void MdmWifiCmd_StartConnection (UINT8 Mode, UINT8 Id, UINT8 Type, UINT8 *Address, UINT8 Port);
/* API used to send a TCP or UDP packet */
void MdmWifiCmd_SendData (UINT8 Mode, UINT8 Id, UINT8 Len, UINT8 *Buffer);
/* API used to check if a bunch of data is received and get the data */
WifiDataRecType MdmWifiCmd_ReceiveWifiMsg (UINT8* RxBuffer, UINT8* DataLenght);
/* API used to close a connection */
void MdmWifiCmd_CloseConnection (UINT8 Mode, UINT8 Id);
/* API used to configure the softAP */
UINT8 MdmWifiCmd_ConfigureSoftAPMode(void* ssid, void* pwd, UINT8 chid, UINT8 enc);
/* API used to configure the softAP IP Address */
UINT8 MdmWifiCmd_ConfigureSoftAPIpAddress(void* ip);
/* API used to configure server mode and port */
UINT8 MdmWifiCmd_ConfigureServer (UINT8 Mode, UINT16 Port);

#endif	/* MDM_WIFI_CMD_H */

