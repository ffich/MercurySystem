/************************************************************************
*                            Modem Bluetooth                            *
*************************************************************************
* FileName:         mdm_bt.h                                            *
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

#ifndef MDM_BT_H
#define	MDM_BT_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

#if (MODEM_USED == BT_MDM)
/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Task period in ms */
#define MDM_BT_TASK_PERIOD_MS                            ((UINT16)(1))
/* Rx buffer length */
#define BT_RX_BUFFER_LENGTH                              RX_BUFFER_LENGTH

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Bt Msg status */
typedef enum _BtRxMsgSts
{
   BtMsg_NotReceived,
   BtMsg_Received,
} BtRxMsgStsType;

/* BT Rx packet */
typedef struct _BtPacketType
{
   UINT8 RxBuffer[BT_RX_BUFFER_LENGTH];
   UINT8 RxByteNum;
   EventStructureType PacketRxEvt;
} BtPacketType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Bt modem Rx buffer */
extern UINT8 MdmBt_UartRxBuffer[BT_RX_BUFFER_LENGTH];
/* Rx counter */
extern UINT16 MdmBt_UartRxCounter;
/* BT Rx packet */
extern BtPacketType BtPacket;
/* Notify a new data received from uart driver to BT manager */
extern EventStructureType NewBtUartData;
/* Notify an new BT packet ready from BT manager to application */
extern EventStructureType NewBtPacket;

/************************************************************************
* EXPORTED Functions
************************************************************************/
void MdmBt_MainTask (UINT8 Options);
BtRxMsgStsType MdmBt_ReceiveBtMsg (UINT8* RxBuffer, UINT8* DataLenght);
BtRxMsgStsType MdmBt_ReceiveBtMsgTerminal (UINT8* RxBuffer, UINT8* DataLenght);
void MdmBt_SendBtMsgLen (UINT8* TxBuffer, UINT8 DataLenght);
void MdmBt_SendBtMsg (UINT8* TxBuffer);
void MdmBt_UartSetConfigAtMode (void);
void MdmBt_UartSetConfigComMode (void);

#endif /* MODEM_USED == BT_MDM */

#endif	/* MDM_BT_H */

