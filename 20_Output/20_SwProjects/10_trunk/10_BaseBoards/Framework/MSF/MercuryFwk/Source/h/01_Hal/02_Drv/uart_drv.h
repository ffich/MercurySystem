/************************************************************************
*                           UART Driver                                 *
*************************************************************************
* FileName:         uart_drv.h                                          *
* HW:               Mercury System                                      *
* Author:           F.Ficili                                            *
*                                                                       *
* Software License Agreement                                            *
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
* Author       Date         Version      Comment                        *
* ---------------------------------------------------------------------	*
* F.Ficili     11/09/16    1.0          First release                   *
************************************************************************/

#ifndef UART_DRV_H
#define	UART_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Module selection constants */
#define UART_MODULE_1                                          ((UINT8)(1))
#define UART_MODULE_2                                          ((UINT8)(2))

/* Rx buffer length */
#define RX_BUFFER_LENGTH                                       ((UINT16)(400))

/* UART 1 register set*/
#define UART_1_TX_STS_REG                                      TXSTA1
#define UART_1_RX_STS_REG                                      RCSTA1
#define UART_1_BRG_CON_REG                                     BAUDCON1
#define UART_1_TX_BUFFER                                       TXREG1
#define UART_1_RX_BUFFER                                       RCREG1
#define UART_1_BRG_HIGH                                        SPBRGH1
#define UART_1_BRG_LOW                                         SPBRG1

/* UART 2 register set*/
#define UART_2_TX_STS_REG                                      TXSTA2
#define UART_2_RX_STS_REG                                      RCSTA2
#define UART_2_BRG_CON_REG                                     BAUDCON2
#define UART_2_TX_BUFFER                                       TXREG2
#define UART_2_RX_BUFFER                                       RCREG2
#define UART_2_BRG_HIGH                                        SPBRGH2
#define UART_2_BRG_LOW                                         SPBRG2

/* Baud Rate generator parameters */
#define DESIRED_UART_BAUD_RATE                                 ((UINT32)(9600))

/* TXSTAx */
#define UART_9_BIT_TX_EN_SHIFT                                 ((UINT8)(6))
#define TX_9_BIT_DISABLED                                      ((UINT8)(0))
#define TX_9_BIT_ENABLED                                       ((UINT8)(1))

#define UART_TX_EN_SHIFT                                       ((UINT8)(5))
#define TX_DISABLED                                            ((UINT8)(0))
#define TX_ENABLED                                             ((UINT8)(1))

#define UART_MODE_SHIFT                                        ((UINT8)(4))
#define ASYNC_MODE                                             ((UINT8)(0))
#define SYNC_MODE                                              ((UINT8)(1))

#define UART_SEND_BREAK_SHIFT                                  ((UINT8)(3))
#define UART_NOT_SEND_BREAK                                    ((UINT8)(0))
#define UART_SEND_BREAK                                        ((UINT8)(1))

#define UART_HIGH_BRG_SHIFT                                    ((UINT8)(2))
#define LOW_SPEED                                              ((UINT8)(0))
#define HIGH_SPEED                                             ((UINT8)(1))

#define TX_SR_STS_MASK                                         ((UINT8)(0b00000010))
#define TX_SR_STS_SHIFT                                        ((UINT8)(1))
#define TSR_EMPTY                                              ((UINT8)(1))
#define TSR_FULL                                               ((UINT8)(0))

/* RCSTAx */
#define UART_SER_PORT_EN_SHIFT                                 ((UINT8)(7))
#define SERIAL_PORT_DISABLED                                   ((UINT8)(0))
#define SERIAL_PORT_ENABLED                                    ((UINT8)(1))

#define UART_9_BIT_RX_EN_SHIFT                                 ((UINT8)(6))
#define RX_9_BIT_DISABLED                                      ((UINT8)(0))
#define RX_9_BIT_ENABLED                                       ((UINT8)(1))

#define UART_RX_EN_SHIFT                                       ((UINT8)(4))
#define RX_DISABLED                                            ((UINT8)(0))
#define RX_ENABLED                                             ((UINT8)(1))

#define UART_FRAMING_ERR_SHIFT                                 ((UINT8)(2))
#define NO_FRAMING_ERROR                                       ((UINT8)(0))
#define FRAMING_ERROR                                          ((UINT8)(1))

#define UART_OVERRUN_ERR_SHIFT                                 ((UINT8)(1))
#define NO_OVERRUN_ERROR                                       ((UINT8)(0))
#define OVERRUN_ERROR                                          ((UINT8)(1))

/* BAUDCONx */
#define UART_RX_OP_SHIFT                                       ((UINT8)(6))
#define RX_ACTIVE                                              ((UINT8)(0))
#define RX_IDLE                                                ((UINT8)(1))

#define UART_DATA_RX_POL_SHIFT                                 ((UINT8)(5))
#define RX_POL_NOT_INV                                         ((UINT8)(0))
#define RX_POL_INV                                             ((UINT8)(1))

#define UART_IDLE_STATE_POL_SHIFT                              ((UINT8)(4))
#define IDLE_STATE_HIGH                                        ((UINT8)(0))
#define IDLE_STATE_LOW                                         ((UINT8)(1))

#define UART_BAUD_RATE_MODE_SHIFT                              ((UINT8)(3))
#define BAUD_RATE_8_BIT                                        ((UINT8)(0))
#define BAUD_RATE_16_BIT                                       ((UINT8)(1))

#define UART_WAKE_UP_SHIFT                                     ((UINT8)(1))
#define UART_WAKE_UP_DIS                                       ((UINT8)(0))
#define UART_WAKE_UP_EN                                        ((UINT8)(1))

#define UART_AUTOBAUD_SHIFT                                    ((UINT8)(0))
#define AUTOBAUD_DIS                                           ((UINT8)(0))
#define AUTOBAUD_EN                                            ((UINT8)(1))

/* Interrupts */
#define TX_INTERRUPT_ENABLED                                   ((UINT8)(1))
#define TX_INTERRUPT_DISABLED                                  ((UINT8)(0))
#define RX_INTERRUPT_ENABLED                                   ((UINT8)(1))
#define RX_INTERRUPT_DISABLED                                  ((UINT8)(0))

/* General defines */
#define BIT_0_MASK                                             ((UINT8)(0x01))
#define BYTE_0_MASK                                            ((UINT8)(0x00FF))
#define BYTE_1_MASK                                            ((UINT8)(0xFF00))

#define BIT_8_SHIFT                                            ((UINT8)(8))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Uart config type*/
typedef struct _UartConfig
{
   UINT8 UartEnable;
   UINT8 UartMode;
   UINT8 BaudRateMode;
   UINT8 HighBaudMode;
   UINT8 Tx9bitMode;
   UINT8 Rx9bitMode;
   UINT8 TxEnable;
   UINT8 RxEnable;
   UINT8 SendBreakEnable;
   UINT8 AutobaudEnable;
   UINT8 WakeUpEnable;
   UINT8 TxInterruptEnable;
   UINT8 RxInterruptEnable;
   UINT32 McuBusClock;
   UINT32 DesiredBaudRate;
} UartConfigType;

/* UART write data packet */
typedef struct _UartWritePacket
{
   UINT8 UartModule;
   UINT8 *Buffer;
   UINT8 Lenght;
} UartWritePacketType;

/* UART read data packet */
typedef struct _UartReadPacket
{
   UINT8 UartModule;
   UINT8 *Buffer;
   UINT8 Lenght;
   EventStructureType UartReadCompleteEvt;
} UartReadPacketType;

typedef enum _UartWriteSts
{
   UartWriteInProgress,
   UartWriteNotOk,
   UartWriteOk
} UartWriteStsType;

typedef enum _UartReadSts
{
   UartReadInProgress,
   UartReadNotOk,
   UartReadOk
} UartReadStsType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Global Rx buffer */
extern UINT8 RxBuffer[RX_BUFFER_LENGTH];
/* Received data length */
extern UINT16 RxLen;

/************************************************************************
* EXPORTED Functions
************************************************************************/
void Uart_InitUartModule (UINT8 Module, UartConfigType *UartConfig);
void Uart_WriteConstString (UINT8 Module, const UINT8 *ConstStringToWrite);
void Uart_WriteString (UINT8 Module, UINT8 *StringToWrite);
void Uart_WriteStringLen (UINT8 Module, UINT8 *StringToWrite, UINT8 Len);
#if (MODEM_USED == GSM_GPRS_MDM)
void Uart_MdmGsmGprsReadService(UINT8 Module, UINT8 UartData);
#endif
#if (MODEM_USED == BT_MDM)
void Uart_MdmBtReadService (UINT8 Module, UINT8 UartData);
#endif
#if (MODEM_USED == WIFI_MDM)
void Uart_MdmWifiReadService (UINT8 Module, UINT8 UartData);
#endif
void Uart_ClearRxBuffer (void);

/* -- ISRs -- */
void Uart_Uart1RxIsr (void);

#endif	/* UART_DRV_H */

