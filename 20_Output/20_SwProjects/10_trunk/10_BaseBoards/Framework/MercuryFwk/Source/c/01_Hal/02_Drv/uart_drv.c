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

/************************************************************************
* Includes
************************************************************************/
#include "uart_drv.h"
#include "mdm_bt.h"
#include "mdm_wifi.h"

/************************************************************************
* Defines
************************************************************************/


/************************************************************************
* Typedefs
************************************************************************/
/* Tx Buffer status */
typedef enum _TxBufferStatus
{
    TxBufferNotFull,
    TxBufferFull,
    TxBufferEmpty,
} TxBufferStatusType;

/* Rx Buffer status */
typedef enum _RxBufferStatus
{
    RxBufferNotFull,
    RxBufferIdle,
    RxBufferNoData,
} RxBufferStatusType;

typedef enum _RxState
{
   ReadData,
   CheckTermSeq1,
   CheckTermSeq2
} RxStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/


/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Global Rx buffer */
UINT8 RxBuffer[RX_BUFFER_LENGTH];
/* Received data length */
UINT16 RxLen = 0;


/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
void UartClearBuffer (UINT8 *Buffer, UINT16 Len);
void SetUartBaudRate (UINT8 Module, UartConfigType *UartConfig);
void UartWriteByte (UINT8 Module, UINT8 ByteToWrite);
UINT8 UartReadByte (UINT8 Module);
TxBufferStatusType UartTxBufferStatus (UINT8 Module);
RxBufferStatusType UartRxBufferStatus (UINT8 Module);

/************************************************************************
* LOCAL Functions Implementation
************************************************************************/

/************************************************************************
* Function:     UartClearBuffer
* Input:        UINT8 *Buffer
*               UINT16 Len 
* Output:       None
* Author:       F.Ficili
* Description:	 Clear the buffer content.
* Date:         11/09/16
************************************************************************/
void UartClearBuffer (UINT8 *Buffer, UINT16 Len)
{
   UINT8 Index = 0;
   
   /* Clear buffer */
   for (Index = 0; Index <= Len; Index++)
   {
      *(Buffer + Index) = 0;
   }
}

/************************************************************************
* Function:     SetUartBaudRate
* Input:        UINT8 Module
*               UartConfigType *UartConfig                
* Output:       None
* Author:       F.Ficili
* Description:  Set UART baud rate.
* Date:         11/09/16
************************************************************************/
void SetUartBaudRate (UINT8 Module, UartConfigType *UartConfig)
{
   UINT16 BaudRateRegister = 0;
   UINT8 DivisionFactor = 0;
   
   /* Check baudare mode (see component ds ch 20.1) */
   if ((UartConfig->BaudRateMode == BAUD_RATE_8_BIT) && (UartConfig->BaudRateMode == LOW_SPEED))
   {
      DivisionFactor = 64;
   }
   else if ((UartConfig->BaudRateMode == BAUD_RATE_16_BIT) && (UartConfig->BaudRateMode == LOW_SPEED))
   {
      DivisionFactor = 16;
   }
   else if ((UartConfig->BaudRateMode == BAUD_RATE_8_BIT) && (UartConfig->BaudRateMode == HIGH_SPEED))
   {
      DivisionFactor = 16;
   }
   else if ((UartConfig->BaudRateMode == BAUD_RATE_16_BIT) && (UartConfig->BaudRateMode == HIGH_SPEED))
   {
      DivisionFactor = 4;
   }
   else
   {
      /* Not allowed */
   }   

   /* Calculate baudare formula */   
   BaudRateRegister = ((UartConfig->McuBusClock/(DivisionFactor * UartConfig->DesiredBaudRate)) - 1);       
   
   if (Module == UART_MODULE_1)
   {
      /* BRG register update */
      UART_1_BRG_HIGH = (UINT8)((BaudRateRegister & BYTE_1_MASK) >> BIT_8_SHIFT);   
      UART_1_BRG_LOW = (UINT8)((BaudRateRegister & BYTE_0_MASK));    
   }
   else if (Module == UART_MODULE_2)
   {
      /* BRG register update */
      UART_2_BRG_HIGH = (UINT8)((BaudRateRegister & BYTE_1_MASK) >> BIT_8_SHIFT);   
      UART_2_BRG_LOW = (UINT8)((BaudRateRegister & BYTE_0_MASK));    
   }      
}

/************************************************************************
* Function:    UartWriteByte
* Input:       UINT8 ByteToWrite
* Output:      None
* Author:      F.Ficili
* Description: Write one byte.
* Date:        11/09/16
************************************************************************/
void UartWriteByte (UINT8 Module, UINT8 ByteToWrite)
{
   if (Module == UART_MODULE_1)
   {   
      /* Write byte to write on tx buffer 1 */
      UART_1_TX_BUFFER = ByteToWrite;
   }
   else if (Module == UART_MODULE_2)
   {   
      /* Write byte to write on tx buffer 2 */
      UART_2_TX_BUFFER = ByteToWrite;      
   }   
}

/************************************************************************
* Function:     UartReadByte
* Input:        None
* Output:       UINT8
* Author:       F.Ficili
* Description:  Read one byte.
* Date:         11/09/16
************************************************************************/
UINT8 UartReadByte (UINT8 Module)
{
   /* Service */
   UINT8 UartRxBuffer = 0;
   
   if (Module == UART_MODULE_1)
   {   
      /* Read content of rx buffer 1 */
      UartRxBuffer = UART_1_RX_BUFFER;
   }
   else if (Module == UART_MODULE_2)
   {   
      /* Read content of rx buffer 2 */
      UartRxBuffer = UART_2_RX_BUFFER;    
   }    
   
   /* Return content of rx buffer */
   return UartRxBuffer;
}

/************************************************************************
* Function:     UartTxBufferStatus
* Input:        UINT8 Module
* Output:       TxBufferStatusType
* Author:       F.Ficili
* Description:  Check the status of tx buffer (full, empty).
* Date:         11/09/16
************************************************************************/
TxBufferStatusType UartTxBufferStatus (UINT8 Module)
{
   TxBufferStatusType TxBufferStatus = TxBufferNotFull;
   
   if (Module == UART_MODULE_1)
   {
      /* Check TX buffer status */
      if (((UART_1_TX_STS_REG >> TX_SR_STS_SHIFT) & BIT_0_MASK) == TSR_FULL)
      {
          TxBufferStatus = TxBufferFull;
      }
      else if (((UART_1_TX_STS_REG >> TX_SR_STS_SHIFT) & BIT_0_MASK) == TSR_EMPTY)
      {
          TxBufferStatus = TxBufferEmpty;
      }
      else
      {
          /* Do nothing */
      }   
   }
   else if (Module == UART_MODULE_2)
   {
      /* Check TX buffer status */
      if (((UART_2_TX_STS_REG >> TX_SR_STS_SHIFT) & BIT_0_MASK) == TSR_FULL)
      {
          TxBufferStatus = TxBufferFull;
      }
      else if (((UART_2_TX_STS_REG >> TX_SR_STS_SHIFT) & BIT_0_MASK) == TSR_EMPTY)
      {
          TxBufferStatus = TxBufferEmpty;
      }
      else
      {
          /* Do nothing */
      }      
   }
   
   return TxBufferStatus;
}

/************************************************************************
* Function:     UartRxBufferStatus
* Input:        UINT8 Module
* Output:       RxBufferStatusType
* Author:       F.Ficili
* Description:  Check the status of rx buffer (full, empty).
* Date:         11/09/16
************************************************************************/
RxBufferStatusType UartRxBufferStatus (UINT8 Module)
{
   return 0;
}

/************************************************************************
* GLOBAL Functions Implementation
************************************************************************/
/************************************************************************
* Function:     Uart_InitUartModule
* Input:        UINT8 Module
*               UartConfigType *UartConfig
* Output:       None
* Author:       F.Ficili
* Description:  Initialize UART module.
* Date:         11/09/16
************************************************************************/
void Uart_InitUartModule (UINT8 Module, UartConfigType *UartConfig)
{
   if (Module == UART_MODULE_1)
   {
      /* Set the pin registers */
      BSP_UART_1_TX_TRIS = LINE_DIRECTION_OUTPUT;
      BSP_UART_1_RX_TRIS = LINE_DIRECTION_INPUT;      

      /* Baud rate settings */
      SetUartBaudRate(UART_MODULE_1, UartConfig);

      /* -- Control registers settings -- */

      /* TXSTA1 */
      UART_1_TX_STS_REG = 0x00;
      UART_1_TX_STS_REG |= (UartConfig->Tx9bitMode) << UART_9_BIT_TX_EN_SHIFT;
      UART_1_TX_STS_REG |= (UartConfig->TxEnable) << UART_TX_EN_SHIFT;
      UART_1_TX_STS_REG |= (UartConfig->UartMode) << UART_MODE_SHIFT;
      UART_1_TX_STS_REG |= (UartConfig->SendBreakEnable) << UART_SEND_BREAK_SHIFT;
      UART_1_TX_STS_REG |= (UartConfig->HighBaudMode) << UART_HIGH_BRG_SHIFT;

      /* RCSTA1 */
      UART_1_RX_STS_REG = 0x00;
      UART_1_RX_STS_REG |= (UartConfig->UartEnable) << UART_SER_PORT_EN_SHIFT;
      UART_1_RX_STS_REG |= (UartConfig->Rx9bitMode) << UART_9_BIT_RX_EN_SHIFT;
      UART_1_RX_STS_REG |= (UartConfig->RxEnable) << UART_RX_EN_SHIFT; 

      /* BAUDCON1 */
      UART_1_BRG_CON_REG = 0x00;
      UART_1_BRG_CON_REG |= (UartConfig->BaudRateMode) << UART_BAUD_RATE_MODE_SHIFT; 
      UART_1_BRG_CON_REG |= (UartConfig->WakeUpEnable) << UART_WAKE_UP_SHIFT;        
      UART_1_BRG_CON_REG |= (UartConfig->AutobaudEnable) << UART_AUTOBAUD_SHIFT;  
      
      /* Interrupts enable */
      PIE1bits.TX1IE = UartConfig->TxInterruptEnable;
      PIE1bits.RC1IE = UartConfig->RxInterruptEnable;
      
      /* TO_DO: interrupts priority */
   }
   else if (Module == UART_MODULE_2)
   {
      /* Set the pin registers */
      BSP_UART_2_TX_TRIS = LINE_DIRECTION_OUTPUT;
      BSP_UART_2_RX_TRIS = LINE_DIRECTION_INPUT;      

      /* Baud rate settings */
      SetUartBaudRate(UART_MODULE_2, UartConfig);

      /* -- Control registers settings -- */

      /* TXSTA2 */
      UART_2_TX_STS_REG = 0x00;
      UART_2_TX_STS_REG |= (UartConfig->Tx9bitMode) << UART_9_BIT_TX_EN_SHIFT;
      UART_2_TX_STS_REG |= (UartConfig->TxEnable) << UART_TX_EN_SHIFT;
      UART_2_TX_STS_REG |= (UartConfig->UartMode) << UART_MODE_SHIFT;
      UART_2_TX_STS_REG |= (UartConfig->SendBreakEnable) << UART_SEND_BREAK_SHIFT;
      UART_2_TX_STS_REG |= (UartConfig->HighBaudMode) << UART_HIGH_BRG_SHIFT;

      /* RCSTA2 */
      UART_2_RX_STS_REG = 0x00;
      UART_2_RX_STS_REG |= (UartConfig->UartEnable) << UART_SER_PORT_EN_SHIFT;
      UART_2_RX_STS_REG |= (UartConfig->Rx9bitMode) << UART_9_BIT_RX_EN_SHIFT;
      UART_2_RX_STS_REG |= (UartConfig->RxEnable) << UART_RX_EN_SHIFT; 

      /* BAUDCON2 */
      UART_2_BRG_CON_REG = 0x00;
      UART_2_BRG_CON_REG |= (UartConfig->BaudRateMode) << UART_BAUD_RATE_MODE_SHIFT; 
      UART_2_BRG_CON_REG |= (UartConfig->WakeUpEnable) << UART_WAKE_UP_SHIFT;        
      UART_2_BRG_CON_REG |= (UartConfig->AutobaudEnable) << UART_AUTOBAUD_SHIFT;    
      
      /* Interrupts */
      PIE3bits.TX2IE = UartConfig->TxInterruptEnable;
      PIE3bits.RC2IE = UartConfig->RxInterruptEnable;
      
      /* TO_DO: interrupts priority */      
   }
}

#if (MODEM_USED == GSM_GPRS_MDM)
/************************************************************************
* Function:     Uart_MdmGsmGprsReadService
* Input:        UINT8 Module
*               UINT8 UartData
* Output:       None
* Author:       F.Ficili
* Description:  Read service for GSM/GPRS modem
* Date:         11/09/16
************************************************************************/
void Uart_MdmGsmGprsReadService(UINT8 Module, UINT8 UartData)
{
   static UINT16 RxCounter = 0;
   static RxStateType RxState = ReadData;
   static UINT8 LocalRxBuffer[RX_BUFFER_LENGTH];
   UINT16 ServiceCounter = 0;
   
   switch (RxState)
   {         
      case ReadData:
         /* Check received byte */
         if (UartData == '\r')
         {
            /* Switch state */
            RxState = CheckTermSeq1;
         }
         else
         {
            if (RxCounter < RX_BUFFER_LENGTH)
            {
               /* Store byte */
               LocalRxBuffer[RxCounter] = UartData;
               /* Inc counter */
               RxCounter++;            
            }
         }         
         break;
                  
      case CheckTermSeq1:
         /* Check received byte */
         if (UartData == '\n')
         {       
            /* Filter empy packet */
            if (RxCounter > 0)
            {
               /* Rx completed - copy buffer, lenght and send event */
               for (ServiceCounter = 0; ServiceCounter < RxCounter; ServiceCounter++)
               {
                  RxBuffer[ServiceCounter] = LocalRxBuffer[ServiceCounter];
               }
               /* Update Rx counter */
               RxLen = RxCounter;
               /* Broadcast some events */
               GenerateEvt(&NewStringReceived); 
               GenerateEvt(&NewMdmEvtParserRequest); 
               GenerateEvt(&NewStringMdmTerminal);  
            }            
         }
         else
         {
            /* Wrong sequence - abort Rx */
         }
         /* Switch state */
         RxState = ReadData;
         /* Reset counter */         
         RxCounter = 0;
         break;
         
      default:
         break;
   }
}
#endif

#if (MODEM_USED == BT_MDM)
/************************************************************************
* Function:     Uart_MdmBtReadService
* Input:        UINT8 Module
*               UINT8 UartData
* Output:       None
* Author:       F.Ficili
* Description:  Read service for BT modem
* Date:         20/05/17
************************************************************************/
void Uart_MdmBtReadService (UINT8 Module, UINT8 UartData)
{   
   if (MdmBt_UartRxCounter < sizeof(MdmBt_UartRxBuffer))
   {   
      /* Store byte */
      MdmBt_UartRxBuffer[MdmBt_UartRxCounter] = UartData;
      /* Inc counter */
      MdmBt_UartRxCounter++;     
      /* Generate reception event */
      GenerateEvt(&NewBtUartData); 
   }
}
#endif

#if (MODEM_USED == WIFI_MDM)
/************************************************************************
* Function:     Uart_MdmWifiReadService
* Input:        UINT8 Module
*               UINT8 UartData
* Output:       None
* Author:       F.Ficili
* Description:  Read service for Wifi modem
* Date:         20/09/17
************************************************************************/
void Uart_MdmWifiReadService (UINT8 Module, UINT8 UartData)
{   
   if (MdmWifi_UartRxCounter < sizeof(WifiRxPacket.RxBuffer))
   {
      /* Store byte */
      WifiRxPacket.RxBuffer[MdmWifi_UartRxCounter] = UartData;
      /* Inc counter */
      MdmWifi_UartRxCounter++;
      /* Generate reception event */
      GenerateEvt(&NewWifiUartData);   
   }
}
#endif

/************************************************************************
* Function:     Uart_WriteConstString
* Input:        const UINT8 *ConstStringToWrite
* Output:       None
* Author:       F.Ficili
* Description:  Write a constant string of bytes.
* Date:         11/09/16
************************************************************************/
void Uart_WriteConstString (UINT8 Module, const UINT8 *ConstStringToWrite)
{
   /* If the buffer is not empty */
   while (*ConstStringToWrite)
   {
      /* Write current character */
      UartWriteByte(Module, *ConstStringToWrite);
      /* Increment pointer */
      ConstStringToWrite++;
      /* If tx buffer is full wait (blocking) */
      while (UartTxBufferStatus(Module) == TxBufferFull);
   }
}

/************************************************************************
* Function:     Uart_WriteString
* Input:        UINT8 *StringToWrite
* Output:       None
* Author:       F.Ficili
* Description:  Write a string of bytes.
* Date:         11/09/16
************************************************************************/
void Uart_WriteString (UINT8 Module, UINT8 *StringToWrite)
{
   /* If the buffer is not empty */
   while (*StringToWrite)
   {
      /* Write current character */
      UartWriteByte(Module, *StringToWrite);
      /* Increment pointer */
      StringToWrite++;
      /* If tx buffer is full wait (blocking) */
      while (UartTxBufferStatus(Module) == TxBufferFull);
   }
}

/************************************************************************
* Function:     Uart_WriteString
* Input:        UINT8 Module
*               UINT8 *StringToWrite
*               UINT8 Len 
* Output:       None
* Author:       F.Ficili
* Description:  Write a string of bytes with pre-determined length.
* Date:         11/09/16
************************************************************************/
void Uart_WriteStringLen (UINT8 Module, UINT8 *StringToWrite, UINT8 Len)
{
   UINT8 Index;
   
   for (Index = 0; Index < Len; Index++)
   {
      /* Write current character */
      UartWriteByte(Module, *StringToWrite);
      /* Increment pointer */
      StringToWrite++;
      /* If tx buffer is full wait (blocking) */
      while (UartTxBufferStatus(Module) == TxBufferFull);
   }
}

/************************************************************************
* Function:     UartClearRxBuffer
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Read one byte.
* Date:         17/09/16
************************************************************************/
void Uart_ClearRxBuffer (void)
{
   UINT16 Index = 0;
   
   /* Clear buffer */
   for (Index = 0; Index <= (RX_BUFFER_LENGTH - 1); Index++)
   {
      RxBuffer[Index] = 0;
   }   
}

/************************************************************************
* Function:     Uart_Uart1RxIsr
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  ISR for UART1 RX interrupt.
* Date:         17/04/17
************************************************************************/
void Uart_Uart1RxIsr (void)
{
   /* Local UART buffer */
   static UINT8 UartRxData = 0;   
   
   /* Clear interrupt flag by reading the buffer */
   UartRxData = RCREG1;
   
   #if (MODEM_USED == GSM_GPRS_MDM)
   /* Read Uart data GSM/GPRS modem case */
   Uart_MdmGsmGprsReadService(1,UartRxData);
   #endif

   #if (MODEM_USED == BT_MDM)
   /* Read Uart data BT modem case */
   Uart_MdmBtReadService(1,UartRxData);
   #endif

   #if (MODEM_USED == WIFI_MDM)
   /* Read Uart data WIFI modem case */
   Uart_MdmWifiReadService(1,UartRxData);
   #endif   
}