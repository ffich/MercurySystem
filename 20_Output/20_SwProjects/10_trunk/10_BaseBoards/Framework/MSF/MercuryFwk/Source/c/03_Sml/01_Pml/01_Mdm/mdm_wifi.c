/************************************************************************
*                             Modem Wifi                                *
*************************************************************************
* FileName:         mdm_wifi.c                                          *
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

/************************************************************************
* Includes
************************************************************************/
#include "mdm_wifi.h"

#if (MODEM_USED == WIFI_MDM)
/************************************************************************
* Defines
************************************************************************/
/* Wifi module baud rate */
/* IMPORTANT NOTE: must be checked that the baud rate could be reached with 
   the UART module configuration (see config UartConfig.BaudRateMode and
   UartConfig.HighBaudMode) */

#if (WIFI_MODULE == STD_ESP8266_MODULE)
   #define WIFI_MDM_BAUD_RATE                               115200
#endif

#if (WIFI_MODULE == UPANEL_MODULE)
   #define WIFI_MDM_BAUD_RATE                               57600
#endif

/* Rx stop delay */
#define WIFI_COMM_TIMEOUT_MS                             (((UINT16)(10))/MDM_WIFI_TASK_PERIOD_MS)

/************************************************************************
* Typedefs
************************************************************************/
/* WifiUartGetDataStateType*/
typedef enum _WifiUartGetDataStateType
{
   WifiMdmUart_RxIdle,
   WifiMdmUart_Receiving
} WifiUartGetDataStateType;

/* WifiUartSendDataStateType*/
typedef enum _WifiUartSendDataStateType
{
   WifiMdmUart_TxIdle,
   WifiMdmUart_Transmitting
} WifiUartSendDataStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* UART configuration variable */
static UartConfigType UartConfig;

/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Rx counter */
UINT16 MdmWifi_UartRxCounter;
/* Wifi Rx packet */
WifiRxPacketType WifiRxPacket;
#if (WIFI_TX_MODE == WIFI_TX_ASYNC)
/* Wifi Tx packet */
WifiTxPacketType WifiTxPacket;
#endif
/* Notify a new data received from uart driver to Wifi manager */
EventStructureType NewWifiUartData;
/* Notify an new Wifi packet ready from Wifi manager to application */
EventStructureType NewWifiPacket;
/* Rx event for parser */
EventStructureType NewWifiPacketParser;
/* Generic Error event */
EventStructureType WifiGenericError;
/* Network data receive event */
EventStructureType WifiReceiveNetData;

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/


/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     UartSetConfig
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Set the Uart module configuration.
* Date:         15/09/17
************************************************************************/
void UartSetConfig (void)
{
   UartConfig.UartEnable = SERIAL_PORT_ENABLED;
   UartConfig.UartMode = ASYNC_MODE;
   UartConfig.BaudRateMode = BAUD_RATE_16_BIT;
   UartConfig.HighBaudMode = HIGH_SPEED;
   UartConfig.Tx9bitMode = TX_9_BIT_DISABLED;
   UartConfig.Rx9bitMode = RX_9_BIT_DISABLED;
   UartConfig.TxEnable = TX_ENABLED;
   UartConfig.RxEnable = RX_ENABLED;
   UartConfig.SendBreakEnable = UART_NOT_SEND_BREAK;
   UartConfig.AutobaudEnable = AUTOBAUD_DIS;
   UartConfig.WakeUpEnable = UART_WAKE_UP_DIS;
   UartConfig.TxInterruptEnable = TX_INTERRUPT_DISABLED;
   UartConfig.RxInterruptEnable = RX_INTERRUPT_ENABLED;
   UartConfig.McuBusClock = SYSTEM_FREQUENCY_VALUE_HZ;
   UartConfig.DesiredBaudRate = WIFI_MDM_BAUD_RATE;
}

/************************************************************************
* Function:     MdmWifiGetUartData
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Get data from UART and pass it to upper layer.
* Date:         15/09/17
************************************************************************/
void MdmWifiGetUartData (void)
{
   static WifiUartGetDataStateType WifiUartGetDataState = WifiMdmUart_RxIdle;
   static UINT8 Counter = 0;
   
   switch (WifiUartGetDataState)
   {
      case WifiMdmUart_RxIdle:
         /* If new data is received */
         if (ReceiveEvt(&NewWifiUartData))
         {
            /* Switch state */
            WifiUartGetDataState = WifiMdmUart_Receiving;
         }         
         break;
         
      case WifiMdmUart_Receiving:
         /* If new data is received */
         if (ReceiveEvt(&NewWifiUartData))
         {
            /* Reset counter */
            Counter = 0;
         }
         else
         {
            /* Increment counter */
            Counter++;
            /* If timeout is expired */
            if (Counter >= WIFI_COMM_TIMEOUT_MS)
            {
               /* Timeout expired */
               Counter = 0;               
               /* Align packet counter */
               WifiRxPacket.RxByteNum = MdmWifi_UartRxCounter;
               /* Reset UART Rx counter */
               MdmWifi_UartRxCounter = 0;
               /* Notify new string reception */
               GenerateEvt(&(WifiRxPacket.PacketRxNotificationEvt));
               GenerateEvt(&NewWifiPacket);
               GenerateEvt(&NewWifiPacketParser);
               /* Switch state */
               WifiUartGetDataState = WifiMdmUart_RxIdle;
            }
         }         
         break;

      default:
         break;         
   }
}

/************************************************************************
* Function:     MdmWifiEvtParser
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Parser for Wifi Events.
* Date:         02/12/17
************************************************************************/
void MdmWifiEvtParser (void)
{
   /* If a new parser event is received */
   if (ReceiveEvt(&NewWifiPacketParser))
   {
      /* "ERROR" Message */
      if (strstr(WifiRxPacket.RxBuffer,(void*)"ERROR") != NULL)  
      {
         /* Notify generic error */
         GenerateEvt(&WifiGenericError);
      }
      /* "+IPD" Message */
      else if (strstr(WifiRxPacket.RxBuffer,(void*)"+IPD") != NULL) 
      {
         /* Notify network data receive */
         GenerateEvt(&WifiReceiveNetData);      
      }
      else
      {
         /* Do nothing */
      }
   }
}

#if (WIFI_TX_MODE == WIFI_TX_ASYNC)
/************************************************************************
* Function:     MdmWifiSendUartData
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Send data received from upper layer to UART.
* Date:         20/09/17
************************************************************************/
void MdmWifiSendUartData (void)
{
   static WifiUartSendDataStateType WifiUartSendDataState = WifiMdmUart_TxIdle;
   
   switch (WifiUartSendDataState)
   {
      case WifiMdmUart_TxIdle:
         /* If a Tx event is received */
         if (ReceiveEvt(&(WifiTxPacket.RequestPacketTxEvt)))
         {
            /* Go to Tx State */
            WifiUartSendDataState = WifiMdmUart_Transmitting;
         }
         break;
         
      case WifiMdmUart_Transmitting:         
         /* Write data to UART */
         Uart_WriteStringLen(UART_MODULE_1, WifiTxPacket.TxBuffer, WifiTxPacket.TxByteNum);
         /* Clear Packet buffer and reset counter  */
         ClearBuffer(WifiTxPacket.TxBuffer,WifiTxPacket.TxByteNum);
         WifiTxPacket.TxByteNum = 0;         
         /* Go back to Idle State */
         WifiUartSendDataState = WifiMdmUart_TxIdle;         
         break;
         
      default:
         break;
   }           
}
#endif

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     MdmWifi_MainTask
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Manage Wifi Modem communication.
* Date:         15/09/17
************************************************************************/
void MdmWifi_MainTask (UINT8 Options)
{
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* Set UART configuration */
         UartSetConfig();
         /* Init UART Interface */
         Uart_InitUartModule(UART_MODULE_1,&UartConfig);
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Get uart data service */
         MdmWifiGetUartData();         
         /* Wifi Event Parser */
         MdmWifiEvtParser();
#if (WIFI_TX_MODE == WIFI_TX_ASYNC)         
         /* Send uart data service */
         MdmWifiSendUartData();
#endif         
         break;

      /* Default */
      default:
         break;
   }
}

#if (WIFI_TX_MODE == WIFI_TX_ASYNC)
/************************************************************************
* Function:     MdmWifi_SendWifiMsgLen
* Input:        UINT8* TxBuffer
*               UINT8 DataLenght
* Output:       None
* Author:       F.Ficili
* Description:  API used to send an UART message to WiFi Modem.
* Date:         20/09/17
************************************************************************/
void MdmWifi_SendWifiMsgLen (UINT8* TxBuffer, UINT8 DataLenght)
{
   /* Copy data into WiFi Tx buffer */
   StringCopy(TxBuffer, WifiTxPacket.TxBuffer, DataLenght);
   /* Set lenght */
   WifiTxPacket.TxByteNum = DataLenght;
   /* Trigger send event */
   GenerateEvt(&(WifiTxPacket.RequestPacketTxEvt));
}
#endif

/************************************************************************
* Function:     MdmWifi_ReceiveWifiMsg
* Input:        UINT8* TxBuffer
*               UINT8* DataLenght
* Output:       WifiRxMsgStsType
* Author:       F.Ficili
* Description:  API used to receive an UART message from WiFi Modem.
* Date:         20/09/17
************************************************************************/
WifiRxMsgStsType MdmWifi_ReceiveWifiMsg (UINT8* RxBuffer, UINT8* DataLenght)
{
   WifiRxMsgStsType Ret;
   
   /* If an Rx event is received */
   if (ReceiveEvt(&WifiRxPacket.PacketRxNotificationEvt))
   {
      /* Copy data in Rx buffer */
      StringCopy(WifiRxPacket.RxBuffer, RxBuffer, WifiRxPacket.RxByteNum);
      /* Update data len */
      *DataLenght = WifiRxPacket.RxByteNum;
      /* Clean rx buffer and counter */
      ClearBuffer(WifiRxPacket.RxBuffer,sizeof(WifiRxPacket.RxBuffer));
      WifiRxPacket.RxByteNum = 0;
      /* Message received */
      Ret = WifiMsg_Received;
   }
   else
   {
      /* Message not received */
      Ret = WifiMsg_NotReceived;   
   }
   
   return Ret;
}

/************************************************************************
* Function:     MdmWifi_ReceiveWifiMsgTerminal
* Input:        UINT8* TxBuffer
*               UINT8* DataLenght
* Output:       WifiRxMsgStsType
* Author:       F.Ficili
* Description:  API reserved for the terminal to receive an UART message from WiFi Modem.
* Date:         23/09/17
************************************************************************/
WifiRxMsgStsType MdmWifi_ReceiveWifiMsgTerminal (UINT8* RxBuffer, UINT8* DataLenght)
{
   WifiRxMsgStsType Ret;
   
   /* If an Rx event is received */
   if (ReceiveEvt(&NewWifiPacket))
   {
      /* Copy data in Rx buffer */
      StringCopy(WifiRxPacket.RxBuffer, RxBuffer, WifiRxPacket.RxByteNum);
      /* Update data len */
      *DataLenght = WifiRxPacket.RxByteNum;    
      /* Message received */
      Ret = WifiMsg_Received;
   }
   else
   {
      /* Message not received */
      Ret = WifiMsg_NotReceived;   
   }
   
   return Ret;
}

#if (WIFI_TX_MODE == WIFI_TX_SYNC)
/************************************************************************
* Function:     MdmWifi_SendWifiMsgLen
* Input:        UINT8* TxBuffer
*               UINT8 DataLenght
* Output:       None
* Author:       F.Ficili
* Description:  API used to send an UART message to Wifi Modem, providing
*               message lenght.
* Date:         23/09/17
************************************************************************/
void MdmWifi_SendWifiMsgLen (UINT8* TxBuffer, UINT8 DataLenght)
{
   /* Send data to Modem UART */
   Uart_WriteStringLen(UART_MODULE_1, TxBuffer, DataLenght);
}

/************************************************************************
* Function:     MdmWifi_SendWifiMsg
* Input:        UINT8* TxBuffer
* Output:       None
* Author:       F.Ficili
* Description:  API used to send an UART message to Wifi Modem, without 
*               providing message lenght.
* Date:         06/11/17
************************************************************************/
void MdmWifi_SendWifiMsg (UINT8* TxBuffer)
{
   /* Send data to Modem UART */
   Uart_WriteString(UART_MODULE_1, TxBuffer);
}
#endif

#endif /* MODEM_USED == WIFI_MDM */