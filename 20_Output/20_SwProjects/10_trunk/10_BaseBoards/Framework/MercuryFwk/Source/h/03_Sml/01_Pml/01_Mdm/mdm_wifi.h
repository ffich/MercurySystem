/************************************************************************
*                             Modem Wifi                                *
*************************************************************************
* FileName:         mdm_wifi.h                                          *
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
* F.Ficili     20/05/17    1.0          First release                   *
************************************************************************/

#ifndef MDM_WIFI_H
#define	MDM_WIFI_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"
#include "mdm_wifi_cmd.h"
#include "mdm_wifi_http.h"

#if (MODEM_USED == WIFI_MDM)
/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Wifi TX Mode */
#define WIFI_TX_SYNC                                     0u
#define WIFI_TX_ASYNC                                    1u
#define WIFI_TX_MODE                                     WIFI_TX_SYNC

/* Task period in ms */
#define MDM_WIFI_TASK_PERIOD_MS                          ((UINT16)(1))
/* Rx buffer length */
#define WIFI_RX_BUFFER_LENGTH                            RX_BUFFER_LENGTH
/* Tx buffer length */
#define WIFI_TX_BUFFER_LENGTH                            ((UINT8)(100))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Wifi Msg status */
typedef enum _WifiRxMsgSts
{
   WifiMsg_NotReceived,
   WifiMsg_Received,
} WifiRxMsgStsType;

/* Wifi Rx packet */
typedef struct _WifiRxPacketType
{
   UINT8 RxBuffer[WIFI_RX_BUFFER_LENGTH];
   UINT16 RxByteNum;
   EventStructureType PacketRxNotificationEvt;
} WifiRxPacketType;

/* Wifi Tx packet */
typedef struct _WifiTxPacketType
{
   UINT8 TxBuffer[WIFI_TX_BUFFER_LENGTH];
   UINT16 TxByteNum;
   EventStructureType RequestPacketTxEvt;
} WifiTxPacketType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Rx counter */
extern UINT16 MdmWifi_UartRxCounter;
/* Wifi Rx packet */
extern WifiRxPacketType WifiRxPacket;
#if (WIFI_TX_MODE == WIFI_TX_ASYNC)
/* Wifi Tx packet */
extern WifiTxPacketType WifiTxPacket;
#endif
/* Notify a new data received from uart driver to Wifi manager */
extern EventStructureType NewWifiUartData;
/* Notify an new Wifi packet ready from Wifi manager to application */
extern EventStructureType NewWifiPacket;
/* Rx event for parser */
extern EventStructureType NewWifiPacketParser;
/* Generic Error event */
extern EventStructureType WifiGenericError;
/* Network data receive event */
extern EventStructureType WifiReceiveNetData;

/************************************************************************
* EXPORTED Functions
************************************************************************/
void MdmWifi_MainTask (UINT8 Options);
void MdmWifi_SendWifiMsgLen (UINT8* TxBuffer, UINT8 DataLenght);
void MdmWifi_SendWifiMsg (UINT8* TxBuffer);
WifiRxMsgStsType MdmWifi_ReceiveWifiMsg (UINT8* RxBuffer, UINT8* DataLenght);
WifiRxMsgStsType MdmWifi_ReceiveWifiMsgTerminal (UINT8* RxBuffer, UINT8* DataLenght);

#endif /* MODEM_USED == WIFI_MDM */

#endif	/* MDM_WIFI_H */

