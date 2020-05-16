/************************************************************************
*                            Modem Bluetooth                            *
*************************************************************************
* FileName:         mdm_bt.c                                            *
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
#include "mdm_bt.h"

#if (MODEM_USED == BT_MDM)
/************************************************************************
* Defines
************************************************************************/
/* BT module AT mode baud rate */
/* IMPORTANT NOTE: must be checked that the baud rate could be reached with 
   the UART module configuration (see config UartConfig.BaudRateMode and
   UartConfig.HighBaudMode) */

#define BT_MDM_AT_MODE_BAUD_RATE                         ((UINT32)(38400))
#define BT_MDM_COMM_MODE_BAUD_RATE                       ((UINT32)(9600))
#define BT_MDM_BAUD_RATE                                 BT_MDM_COMM_MODE_BAUD_RATE

/* Initial delay */
#define BT_COMM_TIMEOUT_MS                               (((UINT16)(50))/MDM_BT_TASK_PERIOD_MS)

/************************************************************************
* Typedefs
************************************************************************/
/* BtUartGetDataStateType*/
typedef enum _BtUartGetDataStateType
{
   BtMdmUart_Idle,
   BtMdmUart_Receiving
} BtUartGetDataStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* UART configuration variable */
static UartConfigType UartConfig;

/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Bt modem Rx buffer */
UINT8 MdmBt_UartRxBuffer[BT_RX_BUFFER_LENGTH];
/* Rx counter */
UINT16 MdmBt_UartRxCounter = 0;
/* BT Rx packet */
BtPacketType BtPacket;
/* Notify a new data received from uart driver to BT manager */
EventStructureType NewBtUartData = {EventIdle};
/* Notify an new BT packet ready from BT manager to application */
EventStructureType NewBtPacket = {EventIdle};

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
* Date:         11/09/16
************************************************************************/
void UartSetConfig (void)
{
   UartConfig.UartEnable = SERIAL_PORT_ENABLED;
   UartConfig.UartMode = ASYNC_MODE;
   UartConfig.BaudRateMode = BAUD_RATE_8_BIT;
   UartConfig.HighBaudMode = LOW_SPEED;
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
   UartConfig.DesiredBaudRate = BT_MDM_BAUD_RATE;
}

/************************************************************************
* Function:     MdmBtGetUartData
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Get data from UART and pass it to upper layer.
* Date:         20/05/16
************************************************************************/
void MdmBtGetUartData (void)
{
   static BtUartGetDataStateType BtUartGetDataState = BtMdmUart_Idle;
   static UINT8 Counter = 0;
   
   switch (BtUartGetDataState)
   {
      case BtMdmUart_Idle:
         /* If new data is received */
         if (ReceiveEvt(&NewBtUartData))
         {
            /* Switch state */
            BtUartGetDataState = BtMdmUart_Receiving;
         }         
         break;
         
      case BtMdmUart_Receiving:
         /* If new data is received */
         if (ReceiveEvt(&NewBtUartData))
         {
            /* Reset counter */
            Counter = 0;
         }
         else
         {
            /* Increment counter */
            Counter++;
            /* If timeout is expired or the buffer is nearly full */
            if ((Counter >= BT_COMM_TIMEOUT_MS) || (MdmBt_UartRxCounter > RX_BUFFER_LENGTH - 10))
            {
               /* Timeout expired */
               Counter = 0;               
               /* Manage data reception */
               StringCopy(MdmBt_UartRxBuffer,BtPacket.RxBuffer,MdmBt_UartRxCounter);
               BtPacket.RxByteNum = MdmBt_UartRxCounter;
               /* Clear UART buffer and reset counter  */
               ClearBuffer(MdmBt_UartRxBuffer,MdmBt_UartRxCounter);
               MdmBt_UartRxCounter = 0;
               /* Notify new string reception */
               GenerateEvt(&(BtPacket.PacketRxEvt));
               GenerateEvt(&NewBtPacket);
               /* Switch state */
               BtUartGetDataState = BtMdmUart_Idle;
            }
         }         
         break;

      default:
         break;         
   }
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     MdmBt_MainTask
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Manage BT Modem communication.
* Date:         20/05/16
************************************************************************/
void MdmBt_MainTask (UINT8 Options)
{
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* Set UART configuration */
         UartSetConfig();
         /* Init UART Interface */
         Uart_InitUartModule(1,&UartConfig);
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Get uart data service */
         MdmBtGetUartData();
         break;

      /* Default */
      default:
         break;
   }
}

/************************************************************************
* Function:     MdmBt_ReceiveBtMsg
* Input:        UINT8* RxBuffer
*               UINT8* DataLenght
* Output:       BtRxMsgStsType
* Author:       F.Ficili
* Description:  API used to receive an UART message from BT Modem.
* Date:         20/09/17
************************************************************************/
BtRxMsgStsType MdmBt_ReceiveBtMsg (UINT8* RxBuffer, UINT8* DataLenght)
{
   BtRxMsgStsType Ret;
   
   /* If an Rx event is received */
   if (ReceiveEvt(&BtPacket.PacketRxEvt))
   {
      /* Copy data in Rx buffer */
      StringCopy(BtPacket.RxBuffer, RxBuffer, BtPacket.RxByteNum);
      /* Update data len */
      *DataLenght = BtPacket.RxByteNum;
      /* Message received */
      Ret = BtMsg_Received;
   }
   else
   {
      /* Message not received */
      Ret = BtMsg_NotReceived;   
   }
   
   return Ret;
}

/************************************************************************
* Function:     MdmBt_ReceiveBtMsgTerminal
* Input:        UINT8* RxBuffer
*               UINT8* DataLenght
* Output:       BtRxMsgStsType
* Author:       F.Ficili
* Description:  API reserved for the Terminal to receive an UART message from BT Modem.
* Date:         20/09/17
************************************************************************/
BtRxMsgStsType MdmBt_ReceiveBtMsgTerminal (UINT8* RxBuffer, UINT8* DataLenght)
{
   BtRxMsgStsType Ret;
   
   /* If an Rx event is received */
   if (ReceiveEvt(&NewBtPacket))
   {
      /* Copy data in Rx buffer */
      StringCopy(BtPacket.RxBuffer, RxBuffer, BtPacket.RxByteNum);
      /* Update data len */
      *DataLenght = BtPacket.RxByteNum;
      /* Message received */
      Ret = BtMsg_Received;
   }
   else
   {
      /* Message not received */
      Ret = BtMsg_NotReceived;   
   }
   
   return Ret;
}

/************************************************************************
* Function:     MdmBt_SendBtMsgLen
* Input:        UINT8* TxBuffer
*               UINT8 DataLenght
* Output:       None
* Author:       F.Ficili
* Description:  API used to send an UART message to BT Modem. If len is 0
*               the Uart_WriteString without len parameter is used.
* Date:         20/09/17
************************************************************************/
void MdmBt_SendBtMsgLen (UINT8* TxBuffer, UINT8 DataLenght)
{
   if (DataLenght > 0)
   {
      /* Send data to Modem UART with len  */
      Uart_WriteStringLen(UART_MODULE_1, TxBuffer, DataLenght);      
   }
   else
   {
      /* Send data to Modem UART without len  */
      Uart_WriteString(UART_MODULE_1, TxBuffer);
   }
}

/************************************************************************
* Function:     MdmBt_SendBtMsg
* Input:        UINT8* TxBuffer
* Output:       None
* Author:       F.Ficili
* Description:  API used to send an UART message to BT Modem without
*               lenght parameter. 
* Date:         27/04/19
************************************************************************/
void MdmBt_SendBtMsg (UINT8* TxBuffer)
{
   /* Send data to Modem UART without len  */
   Uart_WriteString(UART_MODULE_1, TxBuffer);   
}

/************************************************************************
* Function:     MdmBt_UartSetConfigAtMode
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Set the Uart module configuration for AT Mode.
* Date:         24/04/19
************************************************************************/
void MdmBt_UartSetConfigAtMode (void)
{
   UartConfig.UartEnable = SERIAL_PORT_ENABLED;
   UartConfig.UartMode = ASYNC_MODE;
   UartConfig.BaudRateMode = BAUD_RATE_8_BIT;
   UartConfig.HighBaudMode = LOW_SPEED;
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
   UartConfig.DesiredBaudRate = BT_MDM_AT_MODE_BAUD_RATE;   
   
   Uart_InitUartModule(1,&UartConfig);
}

/************************************************************************
* Function:     MdmBt_UartSetConfigComMode
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Set the Uart module configuration for COM Mode.
* Date:         24/04/19
************************************************************************/
void MdmBt_UartSetConfigComMode (void)
{
   UartConfig.UartEnable = SERIAL_PORT_ENABLED;
   UartConfig.UartMode = ASYNC_MODE;
   UartConfig.BaudRateMode = BAUD_RATE_8_BIT;
   UartConfig.HighBaudMode = LOW_SPEED;
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
   UartConfig.DesiredBaudRate = BT_MDM_COMM_MODE_BAUD_RATE;

   Uart_InitUartModule(1,&UartConfig);   
}
#endif /* MODEM_USED == BT_MDM */