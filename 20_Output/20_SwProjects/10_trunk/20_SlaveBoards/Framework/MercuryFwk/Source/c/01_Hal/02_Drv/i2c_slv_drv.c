/************************************************************************
*                            I2C Driver                                 *
*************************************************************************
* FileName:         i2c_slv_drv.h                                       *
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
* F.Ficili     29/07/16    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "i2c_slv_drv.h"
#include "mcu_drv.h"

/************************************************************************
* Defines
************************************************************************/
#define RX_ERROR_TH                                         10u

/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/
/* Rx buffer */
UINT8 I2cRxBuffer[I2C_RX_BUFFER_DIM];
/* Tx buffer */
UINT8 I2cTxBuffer[I2C_TX_BUFFER_DIM];

/************************************************************************
* GLOBAL Variables
************************************************************************/
/* I2c RxPacket */
I2cReadPacketType I2cPacket;
/* New packet event */
EventStructureType I2cNewPacketNotification = {0, EventIdle};
/* New Transmission event */
EventStructureType I2cNewTxNotification = {0, EventIdle}; 

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
UINT8 CheckAckStatus (void);
void ClearRxBuffer (void);
UINT8 CheckOpStatus (void);
UINT8 CheckByteStatus (void);
UINT8 CheckStopBitStatus (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     RestartCondition
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Initiate a re-start condition.
* Date:         29/07/16
************************************************************************/
void RestartCondition (void)
{
   /* Initiate a re-start condition on bus */
   I2C_CONTROL_REG_2 |= (INITIATE_REP_START_COND << REP_START_COND_SHIFT);
}

/************************************************************************
* Function:     StopCondition
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Initiate a stop condition.
* Date:         29/07/16
************************************************************************/
void StopCondition (void)
{
   /* Initiate a stop condition on bus */
   I2C_CONTROL_REG_2 |= (INITIATE_STOP_COND << STOP_COND_SHIFT);
}

/************************************************************************
* Function:     CheckAckStatus
* Input:        None
* Output:       UINT8
* Author:       F.Ficili
* Description:  Return the ack status of the last byte tx.
* Date:         29/07/16
************************************************************************/
UINT8 CheckAckStatus (void)
{
   /* Return ack status  */
   return ((I2C_CONTROL_REG_2 & ACK_STATUS_MASK) >> ACK_STATUS_SHIFT);
}

/************************************************************************
* Function:     CheckOpStatus
* Input:        None
* Output:       WRITE_OP or READ_OP
* Author:       F.Ficili
* Description:  Return the op status (read or write).
* Date:         30/07/16
************************************************************************/
UINT8 CheckOpStatus (void)
{
   /* Return tx status  */
   return ((I2C_STATUS_REG & RW_STATUS_MASK) >> RW_STATUS_SHIFT);
}

/************************************************************************
* Function:     CheckByteStatus
* Input:        None
* Output:       RX_BYTE_IS_ADDR or RX_BYTE_IS_DATA
* Author:       F.Ficili
* Description:  Return the byte status (address or data).
* Date:         31/07/16
************************************************************************/
UINT8 CheckByteStatus (void)
{
   /* Return tx status  */
   return ((I2C_STATUS_REG & DA_STATUS_MASK) >> DA_STATUS_SHIFT);
}

/************************************************************************
* Function:     CheckStopBitStatus
* Input:        None
* Output:       STOP_BIT_DET_LAST or STOP_BIT_NOT_DET_LAST
* Author:       F.Ficili
* Description:  Return the stop bit status (detected or not detected).
* Date:         31/07/16
************************************************************************/
UINT8 CheckStopBitStatus (void)
{
   /* Return tx status  */
   return ((I2C_STATUS_REG & STOP_BIT_DETECTED_LAST_MASK) >> STOP_BIT_DETECTED_LAST_SHIFT);
}

/************************************************************************
* Function:     ClearRxBuffer
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Clear local RX buffer.
* Date:         29/07/16
************************************************************************/
void ClearRxBuffer (void)
{
   UINT8 Index;
   
   /* Initialize local buffer */
   for (Index = 0; Index < I2C_RX_BUFFER_DIM; Index++)
   {
      I2cRxBuffer[Index] = 0x00;
   }
}

/************************************************************************
* Function:     ClearTxBuffer
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Clear local TX buffer.
* Date:         06/08/16
************************************************************************/
void ClearTxBuffer (void)
{
   UINT8 Index;
   
   /* Initialize local buffer */
   for (Index = 0; Index < I2C_TX_BUFFER_DIM; Index++)
   {
      I2cTxBuffer[Index] = 0x00;
   }
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     I2c_InitI2cModule
* Input:        I2cConfigType* I2cConfig
* Output:       None
* Author:       F.Ficili
* Description:  Initialize I2C module.
* Date:         29/07/16
************************************************************************/
void I2c_InitI2cModule (I2cConfigType* I2cConfig)
{
   /* Set the pin registers */
   BSP_I2C_1_SDA_TRIS = LINE_DIRECTION_INPUT;
   BSP_I2C_1_SCL_TRIS = LINE_DIRECTION_INPUT;      
   
   /* -- Control registers settings -- */   
   /* Control Register 1 */    
   I2C_CONTROL_REG_1 |= (I2cConfig->I2cModuleMode) << MODE_SHIFT;
   
   /* Control Register 2 */    
   I2C_CONTROL_REG_2 |= (I2cConfig->I2cReceiverEnable) << ENABLE_RECEIVE_SHIFT;
   I2C_CONTROL_REG_2 |= (I2cConfig->ClockStretchEnable) << CLOCK_STRETCH_EN_SHIFT; 
   I2C_CONTROL_REG_2 |= (I2cConfig->GeneralCallEnable) << GENERAL_CALL_ENABLE_SHIFT; 

   /* Control Register 3 */  
   I2C_CONTROL_REG_3 |= (I2cConfig->StopCondIntEnable) << STOP_COND_INT_SHIFT;
   I2C_CONTROL_REG_3 |= (I2cConfig->StartCondIntEnable) << START_COND_INT_SHIFT;   
   I2C_CONTROL_REG_3 |= (I2cConfig->BufferOverwriteEnable) << BUFF_OVERWRITE_EN_SHIFT;  
   I2C_CONTROL_REG_3 |= (I2cConfig->BusCollisionDetectEnable) << BUS_COLLISION_DET_SHIFT;   
   I2C_CONTROL_REG_3 |= (I2cConfig->AddrHoldEnable) << ADDR_HOLD_SHIFT;    
   I2C_CONTROL_REG_3 |= (I2cConfig->DataHoldEnable) << DATA_HOLD_SHIFT; 
   
   /* Enable Module */
   I2C_CONTROL_REG_1 |= (I2cConfig->EnableI2cModule) << ENABLE_MODULE_SHIFT;
   
   /* Enable peripheral interrupt */   
   PIR1bits.SSP1IF = OFF;
   PIE1bits.SSP1IE = ON;   
   
   /* Init Rx and Tx buffer */
   ClearRxBuffer();
   ClearTxBuffer();
}

/************************************************************************
* Function:     SetI2cAddress
* Input:        UINT8 Byte
* Output:       None
* Author:       F.Ficili
* Description:  Set the I2C module address.
* Date:         30/07/16
************************************************************************/
void I2c_SetI2cAddress (UINT8 SlaveAddress)
{
   /* Set address */
   SSP1ADD = (SlaveAddress << SLAVE_ADDRESS_SHIFT); 
}

/************************************************************************
* Function:     SetI2cAddressMask
* Input:        UINT8 Byte
* Output:       None
* Author:       F.Ficili
* Description:  Set the I2C module address mask.
* Date:         30/07/16
************************************************************************/
void I2c_SetI2cAddressMask (UINT8 SlaveAddressMask)
{
   /* Set address mask */
   SSP1MSK = (SlaveAddressMask << SLAVE_ADDRESS_MASK_SHIFT);    
}

/************************************************************************
* Function:     I2c_UpdateTxBuffer
* Input:        UINT8 *DataBuffer
*               UINT8 Length 
* Output:       None
* Author:       F.Ficili
* Description:  Update the driver Tx buffer with data to be sent.
* Date:         06/08/16
************************************************************************/
void I2c_UpdateTxBuffer(UINT8 *DataBuffer, UINT8 Length)
{
   UINT8 Index = 0;
   
   /* Update Tx buffer */
   for (Index = 0; Index < Length; Index++)
   {
      I2cTxBuffer[Index] = *(DataBuffer + Index);
   }
}

/************************************************************************
* Function:     I2c_DataReceiveSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  receive data from I2C bus.
* Date:         31/07/16
************************************************************************/
void I2c_DataReceiveSm (UINT8 Options)
{
#ifdef STATE_MACHINE_IMPLEMENTATION
   static I2cRxStateType I2cRxState = CheckRxByte;
   static UINT8 I2cRxAddress;
   static UINT8 I2cRxFlag = OFF;
   static UINT8 OpType;
   static UINT8 RxCounter = 0;
   static UINT8 TxCounter = 0;
   static UINT8 AddressStored = FALSE;
   static UINT8 ErrorCounter = 0;
   UINT8 Index;
      
   switch (I2cRxState)
   {         
      case CheckRxByte:
         /* Check for data reception */
         if (ReceiveEvt(&I2cNewTxNotification))
         {         
            /* Check if address or data */
            if (CheckByteStatus() == RX_BYTE_IS_ADDR)
            {
               if (AddressStored == TRUE)
               {
                  /* Error condition */
                  ErrorCounter++;
                  /* Check error occurrence and eventually apply recovery */
                  if (ErrorCounter > RX_ERROR_TH)
                  {
                     Mcu_Reset();
                  }                  
               }
               else
               {
                  /* Reset error counter */
                  ErrorCounter = 0;
               }
               /* Indicates that the address has been stored */
               AddressStored = TRUE;               
               /* Set Rx flag to ON to indicate a valid address */
               I2cRxFlag = ON;               
               /* Switch state */
               I2cRxState = StoreAddrByte;               
            }
            else if ((CheckByteStatus() == RX_BYTE_IS_DATA) && (I2cRxFlag == ON))
            {
               /* Reset addres strored flag */
               AddressStored = FALSE;
               if (OpType == WRITE_OP)
               {
                  /* Write Operation */
                  I2cRxState = HandleWrite;                
               }
               else
               {
                  /* Read Operation */
                  I2cRxState = HandleRead;                
               }                                    
            }
            else
            {
               /* Do nothing */
            }
         }   
         break;

      case StoreAddrByte:
         /* Get address */
         I2cRxAddress = I2C_TX_RX_BUFFER;
         /* Shift Address */
         I2cRxAddress = (I2cRxAddress >> SLAVE_ADDRESS_SHIFT);
         /* Get operation type */
         OpType = (I2C_TX_RX_BUFFER & OP_TYPE_MASK);
         /* If is a read operation load the first byte */
         if (OpType == READ_OP)
         {
            /* Load buffer */
            I2C_TX_RX_BUFFER = I2cTxBuffer[TxCounter];
            /* Increment counter */
            TxCounter++;             
         }         
         else
         {
            /* Otherwise do nothing */
         }
         /* Switch state */
         I2cRxState = StopClkStretch;  
         break;

      case HandleWrite:  
         /* Check if transmission is completed */
         if (CheckStopBitStatus() == STOP_BIT_DET_LAST)
         {
            /* Reset flag */
            I2cRxFlag = OFF;
            /* Update packet data */
            I2cPacket.Lenght = RxCounter;
            I2cPacket.ReadWrite = WRITE_OP;
            I2cPacket.SlaveAddress = I2cRxAddress;
            for (Index = 0; Index < I2C_RX_BUFFER_DIM; Index++)
            {
               I2cPacket.Buffer[Index] = I2cRxBuffer[Index];
            }

            /* Notify message reception */
            GenerateEvt(&I2cNewPacketNotification);

            /* Reset counter */
            RxCounter = 0;
            /* Clear buffer after reception */
            ClearRxBuffer();               
         }
         else
         {
            /* Get Rx data */
            I2cRxBuffer[RxCounter] = I2C_TX_RX_BUFFER;
            /* Inc rx counter */
            RxCounter++;            
         }
         /* Switch state */
         I2cRxState = StopClkStretch;           
         break;
         
      case HandleRead:
         /* Check ack status */
         if (CheckAckStatus() == ACK_RECEIVED)
         {
            /* Load buffer */
            I2C_TX_RX_BUFFER = I2cTxBuffer[TxCounter];
            /* Increment counter */
            TxCounter++;                        
         }
         else
         {
            /* Transmission completed */
            TxCounter = 0;
            /* Reset flag */
            I2cRxFlag = OFF;            
            #ifndef HOLD_DATA_IN_TX_BUFFER
               ClearTxBuffer();
            #endif
         }                           
         /* Switch state */
         I2cRxState = StopClkStretch;             
         break;

      case StopClkStretch:                         
         /* Stop Clock Stretching */
         SSP1CON1bits.CKP = SET; 
         /* Switch state */
         I2cRxState = CheckRxByte;           
         break;

      default:
         break;
   }
#endif
}

/************************************************************************
* Function:     I2c_DataReceiveSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Receive data from I2C bus.
* Date:		    31/07/16
************************************************************************/
void I2c_DataReceive (void)
{
#ifndef STATE_MACHINE_IMPLEMENTATION
   static UINT8 I2cRxAddress;
   static UINT8 OpType;
   static UINT8 RxCounter = 0;
   UINT8 Index;   
   
   /* Clear int flag */
   PIR1bits.SSP1IF = 0;       
   /* Check if address or data */
   if (CheckByteStatus() == RX_BYTE_IS_ADDR)
   {
      /* Get address */
      I2cRxAddress = I2C_TX_RX_BUFFER;          
   }
   else if (CheckByteStatus() == RX_BYTE_IS_DATA)
   {
      
      /* Check if transmission is completed */
      if (CheckStopBitStatus() == STOP_BIT_DET_LAST)
      {
         /* Update packet data */
         I2cPacket.Lenght = RxCounter;
         I2cPacket.ReadWrite = WRITE_OP;
         I2cPacket.SlaveAddress = (I2cRxAddress >> 1);
         for (Index = 0; Index < I2C_RX_BUFFER_DIM; Index++)
         {
            I2cPacket.Buffer[Index] = I2cRxBuffer[Index];
         }
         
         /* Notify message reception */
         GenerateEvt(&I2cNewPacketNotification);
         
         /* Reset counter */
         RxCounter = 0;
         /* Clear buffer after reception */
         ClearRxBuffer();
      }
      else
      {
         /* Get Rx data */
         I2cRxBuffer[RxCounter] = I2C_TX_RX_BUFFER;
         /* Inc rx counter */
         RxCounter++;           
      }
   }
   else
   {
   /* Do nothing */
   }  
   
   /* Stop Clock Stretching */
   SSP1CON1bits.CKP = SET; 
#endif
}