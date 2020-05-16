/************************************************************************
*                            I2C Driver                                 *
*************************************************************************
* FileName:         i2c_drv.c                                           *
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
#include "i2c_drv.h"

/************************************************************************
* Defines
************************************************************************/


/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/


/************************************************************************
* GLOBAL Variables
************************************************************************/


/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
void SetBaudRate (UINT32 DesiredI2cBusClockHz, UINT32 McuClockHz);
void StartCondition (void);
void RestartCondition (void);
void StopCondition (void);
UINT8 CheckAckStatus (void);
UINT8 CheckTxStatus (void);
void WriteByte (UINT8 Byte);
UINT8 ReadByte (void);
void SetAckStatus (UINT8 AckSts);
void EnableReceiver (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     SetBaudRate
* Input:        UINT32 DesiredI2cBusClockHz
*               UINT32 McuClockHz
* Output:       None
* Author:       F.Ficili
* Description:  Set the I2c module baud rate.
* Date:         29/07/16
************************************************************************/
void SetBaudRate (UINT32 DesiredI2cBusClockHz, UINT32 McuClockHz)
{
   /* Calculate baud rate */
   I2C_BRG_REG = ((McuClockHz/(4*DesiredI2cBusClockHz)) - 1);
}

/************************************************************************
* Function:     StartCondition
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Initiate a start condition.
* Date:         29/07/16
************************************************************************/
void StartCondition (void)
{
   /* Initiate a start condition on bus */
   I2C_CONTROL_REG_2 |= (INITIATE_START_COND << START_COND_SHIFT); 
}

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
* Function:     CheckTxStatus
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Return the tx status (in progress or completed).
* Date:         29/07/16
************************************************************************/
UINT8 CheckTxStatus (void)
{
   /* Return tx status  */
   return ((I2C_STATUS_REG & TX_STATUS_MASK) >> TX_STATUS_SHIFT);
}

/************************************************************************
* Function:     SetAckStatus
* Input:        UINT8 AckSts
* Output:       None
* Author:       F.Ficili
* Description:  Set the ack status.
* Date:         21/08/16
************************************************************************/
void SetAckStatus (UINT8 AckSts)
{
   //I2C_CONTROL_REG_2 |= (AckSts << ACK_COND_SHIFT);
   SSP2CON2bits.ACKDT = AckSts;
   SSP2CON2bits.ACKEN = INITIATE_ACK_COND;
}

/************************************************************************
* Function:     EnableReceiver
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Set receive enable status.
* Date:         21/08/16
************************************************************************/
void EnableReceiver (void)
{
   I2C_CONTROL_REG_2 |= ((ENABLE_I2C_RECEIVE << ENABLE_RECEIVE_SHIFT) & ENABLE_RECEIVE_MASK); 
}

/************************************************************************
* Function:     WriteByte
* Input:        UINT8 Byte
* Output:       None
* Author:       F.Ficili
* Description:  Write a byte on tx register.
* Date:         29/07/16
************************************************************************/
void WriteByte (UINT8 Byte)
{
   /* Write byte */
   I2C_TX_RX_BUFFER = Byte;
}

/************************************************************************
* Function:     ReadByte
* Input:        None
* Output:       UINT8
* Author:       F.Ficili
* Description:  Read a byte from rx register.
* Date:         21/08/16
************************************************************************/
UINT8 ReadByte (void)
{   
   /* Service */
   UINT8 Byte;
   
   /* Read byte */
   Byte = I2C_TX_RX_BUFFER;
   
   /* Return data */
   return Byte;
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
   BSP_I2C_2_SDA_TRIS = LINE_DIRECTION_INPUT;
   BSP_I2C_2_SCL_TRIS = LINE_DIRECTION_INPUT;      
   
   /* Baud rate settings */
   SetBaudRate(I2cConfig->I2cBaudRateHz, I2cConfig->McuBusClockHz);

   /* -- Control registers settings -- */
   
   /* Control Register 1 */    
   I2C_CONTROL_REG_1 |= (I2cConfig->I2cModuleMode) << MODE_SHIFT;
   I2C_CONTROL_REG_1 |= (I2cConfig->EnableI2cModule) << ENABLE_MODULE_SHIFT;
}

/************************************************************************
* Function:     I2c_WriteData
* Input:        I2cWritePacketType *WritePacket
* Output:       I2cWriteStsType
* Author:       F.Ficili
* Description:  Write data to I2C slave.
* Date:         29/07/16
************************************************************************/
I2cWriteStsType I2c_WriteData (I2cWritePacketType *WritePacket)
{
   /* Write state */
   static I2cWriteStateType I2cWriteState = TxAssertStart;
   static I2cWriteStateType I2cNextState = TxAssertStart;
   static UINT8 TentativeCounter = 0;
   static I2cWriteStsType WriteStatus;

   switch (I2cWriteState)
   {
      case TxAssertStart:
         /* Assert start condition on I2c bus */
         StartCondition();
         /* Update status */
         WriteStatus = I2cWriteInProgress;
         /* Jump to TxSendSlaveAddress */
         I2cWriteState = TxSendSlaveAddress;
         break;

      case TxSendSlaveAddress:
         /* Add R/W bit */
         WritePacket->SlaveAddress = ((WritePacket->SlaveAddress) << 1) | WritePacket->ReadWrite;
         /* Write slave address */
         WriteByte(WritePacket->SlaveAddress);
         /* Jump to TxCheckTxAddr */
         I2cWriteState = TxCheckTxAddr;
         
      case TxCheckTxAddr:
         /* Check transmission status */
         if (CheckTxStatus() == TX_COMPLETED)
         {
            /* Store next state */
            I2cNextState = TxSendDataByte;
            /* Jump to CheckAckStatus */
            I2cWriteState = TxCheckAck;
         }
         break;

      case TxSendDataByte:
         /* Write data */
         WriteByte(*(WritePacket->Buffer));
         /* Jump to TxCheckTxData */
         I2cWriteState = TxCheckTxData;
         break;
          
      case TxCheckTxData:
          /* Check transmission status */
          if (CheckTxStatus() == TX_COMPLETED)
          {
            /* Store next TxCheckLenght */
            I2cNextState = TxCheckLenght;
            /* Jump to CheckAckStatus */
            I2cWriteState = TxCheckAck;
          }
          break;

      case TxCheckLenght:
         if (WritePacket->Lenght > 0)
         {
            /* Decrement lenght */
            (WritePacket->Lenght)--;
            /* Increment buffer pointer */
            WritePacket->Buffer++;         
         }

         /* Check lenght */
         if (WritePacket->Lenght == 0)
         {
            /* Jump to TxAssertStop */
            I2cWriteState = TxAssertStop;
         }
         else
         {
            /* Jump to TxSendDataByte */
            I2cWriteState = TxSendDataByte;
         }
         break;

      case TxAssertStop:
         /* Assert stop condition */
         StopCondition();
         /* Jump to TxCompleted */
         I2cWriteState = TxCompleted;
         break;
         
      case TxCompleted:
         /* Tx completed, re-init states */
         I2cWriteState = TxAssertStart;
         I2cNextState = TxAssertStart;
         /* Update status */
         WriteStatus = I2cWriteOk;
         break;         

      case TxCheckAck:
         /* Increment tentatives */
         TentativeCounter++;
         /* Check ack status */
         if (CheckAckStatus() == ACK_RECEIVED_FROM_SLAVE)
         {
            /* Reset counter */
            TentativeCounter = 0;
            /* Jump to next state */
            I2cWriteState = I2cNextState;
         }
         else
         {
            /* Check number of tentatives */
            if (TentativeCounter >= MAX_NUMBER_OF_TENTATIVE)
            {
                /* Jump to TxErrorStatus */
                I2cWriteState = TxErrorStatus;
            }
         }
         break;

      case TxErrorStatus:
         /* Error, re-init states */
         I2cWriteState = TxAssertStart;
         I2cNextState = TxAssertStart;
         /* Reset counter */
         TentativeCounter = 0;         
         /* Update status */
         WriteStatus = I2cWriteNotOk;
         break;

      default:
         break;
   }

   /* Return status */
   return WriteStatus;
}

/************************************************************************
* Function:     I2c_ReadData
* Input:        I2cReadPacketType *ReadPacket
* Output:       I2cReadStsType
* Author:       F.Ficili
* Description:  Read data from I2C slave.
* Date:         21/08/16
************************************************************************/
I2cReadStsType I2c_ReadData (I2cReadPacketType *ReadPacket)
{
   /* Write state */
   static I2cReadStateType I2cReadState = RxAssertStart;
   static UINT8 DataCounter = 0;
   static UINT8 TentativeCounter = 0;   
   
   static I2cReadStsType ReadStatus;
   
   switch (I2cReadState)
   {
      case RxAssertStart:
         /* Assert start condition on I2c bus */
         StartCondition();
         /* Update status */
         ReadStatus = I2cReadInProgress;
         /* Switch state */
         I2cReadState = RxSendSlaveAddr;         
         break;
         
      case RxSendSlaveAddr:
         /* Add R/W bit */
         ReadPacket->SlaveAddress = ((ReadPacket->SlaveAddress) << 1) | ReadPacket->ReadWrite;
         /* Write slave address */
         WriteByte(ReadPacket->SlaveAddress);
         /* Switch state */
         I2cReadState = RxCheckTxAddr;
         break;

      case RxCheckTxAddr:
         /* Check transmission status */
         if (CheckTxStatus() == TX_COMPLETED)
         {
            /* Jump to CheckAckStatus */
            I2cReadState = RxCheckAck;
         }
         break;

      case RxCheckAck:
         /* Increment tentatives */
         TentativeCounter++;
         /* Check ack status */
         if (CheckAckStatus() == ACK_RECEIVED_FROM_SLAVE)
         {
            /* Reset counter */
            TentativeCounter = 0;
            /* Jump to next state */
            I2cReadState = RxGetData;
         }
         else
         {
            /* Check number of tentatives */
            if (TentativeCounter >= MAX_NUMBER_OF_TENTATIVE)
            {
                /* Jump to RxErrorStatus */
                I2cReadState = RxErrorStatus;
            }
         }
         break; 
         
      case RxGetData:
         /* Enable Rx */
         EnableReceiver();         
         /* Jump to RxReadData */
         I2cReadState = RxReadData;                         
         break;
                  
      case RxReadData:
         /* Check transmission status */
         if (CheckTxStatus() == TX_COMPLETED)
         {     
            /* Read Data */
            *(ReadPacket->Buffer) = ReadByte();       
            /* Increment buffer pointer */
            ReadPacket->Buffer++;                       
            /* Inc data counter */
            DataCounter++;
            /* Switch state */
            I2cReadState = RxCheckLen;
         }
         break;
         
      case RxCheckLen:
         /* Check data lenght */
         if (DataCounter >= ReadPacket->Lenght)
         {
            /* Not Ack Data */
            SetAckStatus(DO_NOT_ACKNOWLEDGE_DATA);
            /* Switch state */
            I2cReadState = RxAssertStop;
            /* Reset counter */
            DataCounter = 0;                
         }
         else
         {
            /* Ack data */
            SetAckStatus(ACKNOWLEDGE_DATA);
            /* Jump back to RxGetData */
            I2cReadState = RxGetData;            
         }
         break;

      case RxAssertStop:
         /* Assert stop condition on I2c bus */
         StopCondition();  
         /* Update status */
         ReadStatus = I2cReadOk;
         /* Send Event */
         GenerateEvt(&(ReadPacket->ReadCompleteEvt));
         /* Switch state */
         I2cReadState = RxAssertStart;         
         break;
         
      case RxErrorStatus:
         /* Error, re-init states */
         I2cReadState = RxAssertStart;
         /* Update status */
         ReadStatus = I2cReadNotOk;
         break;         
         
      default:
         break;
   }
   
   /* Return status */
   return ReadStatus;   
}