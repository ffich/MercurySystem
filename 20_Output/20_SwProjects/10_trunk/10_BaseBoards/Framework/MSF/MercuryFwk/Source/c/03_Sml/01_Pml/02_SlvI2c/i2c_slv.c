/************************************************************************
*                           I2C Slave                                   *
*************************************************************************
* FileName:         i2c_slv_task.c                                      *
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
* F.Ficili     10/05/19    1.1          Added I2cSlv_QueryI2cData API.  *
* F.Ficili     01/11/19    1.2          Fixed a bug on                  *
*                                       I2cSlv_ReceiveI2cMsg function.  *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "i2c_slv.h"

/************************************************************************
* Defines
************************************************************************/
#define QUERY_REQ_DATA_DELAY_MS                          500u
#define QUERY_TIMEOUT_MS                                 QUERY_REQ_DATA_DELAY_MS * 5u

/************************************************************************
* Typedefs
************************************************************************/
/* I2c Send Message type */
typedef enum _I2cComState
{
   I2cComIdle,
   I2cComWriteData,
   I2cComReadData,
   I2cComWriteComplete,
   I2cComReadComplete,
} I2cComStateType;

/* I2c Tx Status */
typedef enum _I2cComSts
{
   TxRxIdle,
   TxInProgress,
   RxInProgress,
} I2cComStsType;

/* App state type */
typedef enum 
{       
   ASK_DATA = 0,
   REQ_DATA,
   GET_DATA,              
} I2cOpStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* I2c configuration variable */
static I2cConfigType I2cConfig;
static I2cWritePacketType I2cTxPacket;
static I2cReadPacketType I2cRxPacket;
static I2cComStsType I2cComSts = TxRxIdle;

/************************************************************************
* GLOBAL Variables
************************************************************************/
I2cBusStatusType I2cBusStatus = I2cBusNotReady;
EventStructureType I2cWriteDataEvt;
EventStructureType I2cReadDataEvt;

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
void I2cSendMsgSm (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/
/************************************************************************
* Function:     I2cSetConfig
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Set the I2c module configuration.
* Date:         29/07/16
************************************************************************/
void I2cSetConfig (void)
{
    I2cConfig.I2cBaudRateHz = DESIRED_I2C_BUS_CLOCK_HZ;
    I2cConfig.McuBusClockHz = SYSTEM_FREQUENCY_VALUE_HZ;
    I2cConfig.I2cModuleMode = I2C_MASTER_HW_CLOCK;
    I2cConfig.EnableI2cModule = I2C_MODULE_ON;
}

/************************************************************************
* Function:     I2cSendMsgSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  I2c Tx state machine.
* Date:         29/07/16
************************************************************************/
void I2cSendMsgSm (void)
{
   /* State variable */
   static I2cComStateType I2cComState = I2cComIdle;
   
   switch(I2cComState)
   {
      case I2cComIdle:
         /* Check for tx request */
         if (ReceiveEvt(&I2cWriteDataEvt))
         {                        
            /* Switch to I2cComWriteData state */
            I2cComState = I2cComWriteData;
            /* Tx in progress */
            I2cComSts = TxInProgress;
         }
         else if (ReceiveEvt(&I2cReadDataEvt))
         {
            /* Switch to I2cComReadData state */
            I2cComState = I2cComReadData;
            /* Tx in progress */
            I2cComSts = RxInProgress;            
         }
         else
         {
            /* Tx/Rx idle */
            I2cComSts = TxRxIdle;         
         }
         break;
         
      case I2cComWriteData:
         /* Send data */
         if (I2c_WriteData(&I2cTxPacket) != I2cWriteInProgress)
         {                                               
            /* Switch to I2cComWriteCompletestate */
            I2cComState = I2cComWriteComplete;            
         }         
         break;
         
      case I2cComReadData:         
         /* Receive data */
         if (I2c_ReadData(&I2cRxPacket) != I2cReadInProgress)
         {            
            /* Switch to I2cComReadComplete */
            I2cComState = I2cComReadComplete;            
         }      
         
         break;
         
      case I2cComWriteComplete:
         /* Switch to I2cComIdle state */
         I2cComState = I2cComIdle;
         /* Tx/Rx idle */
         I2cComSts = TxRxIdle;         
         break;
         
      case I2cComReadComplete:
         /* Switch to I2cComIdle state */
         I2cComState = I2cComIdle;
         /* Tx/Rx idle */
         I2cComSts = TxRxIdle;         
         break;         
         
      default:
         break;         
   }
}   

/************************************************************************
* Function:     I2cDummyTx
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Dummy transfer to activate the i2c bus.
* Date:         01/01/18
************************************************************************/
void I2cDummyTx (void)
{
   static UINT8 DummyBuffer = 0x00;
   
   /* Dummy Tx*/
   I2cSlv_SendI2cMsg(&DummyBuffer,0x00,1);
   /* Update bus status */
   I2cBusStatus = I2cBusReady;
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     I2cSlv_MainTask
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage I2c communication with slave devices.
* Date:         29/07/16
************************************************************************/
void I2cSlv_MainTask (UINT8 Options)
{       
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* Set module config */
         I2cSetConfig();
         /* Initialize I2C Module */
         I2c_InitI2cModule(&I2cConfig); 
         /* Dummy tx to activate the i2c bus */
         OsAlrm_SetOsAlarm(0x00,50,&I2cDummyTx);
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Send I2c Messages to slaves */
         I2cSendMsgSm();
         break;

      /* Default */
      default:
         break;
   }
}

/************************************************************************
* Function:     I2cSlv_SendI2cMsg
* Input:        UINT8* TxBuffer
*               UINT8 SlaveAddr
*               UINT8 DataLenght
* Output:       UINT8 - STD_OK (Tx OK), STD_NOT_OK (Tx failed)
* Author:       F.Ficili
* Description:  API used to send and I2c message to a specific slave device.
*               The API returns the if the requested Tx operation was ok or failed. 
* Date:         29/07/16
************************************************************************/
UINT8 I2cSlv_SendI2cMsg (UINT8* TxBuffer, UINT8 SlaveAddr, UINT8 DataLenght)
{
   UINT8 TxSts = STD_NOT_OK;
   
   if (I2cComSts != TxInProgress)
   {
      /* Prepare Tx packet */
      I2cTxPacket.SlaveAddress = SlaveAddr;            
      I2cTxPacket.ReadWrite = WRITE_OP;
      I2cTxPacket.Lenght = DataLenght;
      I2cTxPacket.Buffer = TxBuffer;
      
      /* Send Tx event */
      GenerateEvt(&I2cWriteDataEvt);
      /* Update status */
      TxSts = STD_OK;
   }
   else
   {
      /* I2C State machine busy - Update status */
      TxSts = STD_NOT_OK;      
   }
   
   return TxSts;
}

/************************************************************************
* Function:     I2cSlv_ReceiveI2cMsg
* Input:        UINT8* RxBuffer
*               UINT8 SlaveAddr
*               UINT8 DataLenght
* Output:       UINT8 - STD_OK (Rx OK), STD_NOT_OK (Rx failed)
* Author:       F.Ficili
* Description:  API used to receive and I2c message from a specific slave device.
*               The API returns the if the requested Rx operation was ok or failed. 
* Date:         21/08/16
************************************************************************/
UINT8 I2cSlv_ReceiveI2cMsg (UINT8* RxBuffer, UINT8 SlaveAddr, UINT8 DataLenght)
{
   UINT8 RxSts = STD_NOT_OK;
   
   if (I2cComSts != RxInProgress)
   {
      /* Prepare Rx packet */
      I2cRxPacket.SlaveAddress = SlaveAddr;         
      I2cRxPacket.ReadWrite = READ_OP;
      I2cRxPacket.Lenght = DataLenght;
      I2cRxPacket.Buffer = RxBuffer;

      /* Send Rx event */
      GenerateEvt(&I2cReadDataEvt);
      /* Update status */
      RxSts = STD_OK;
   }
   else
   {
      /* I2C State machine busy - Update status */
      RxSts = STD_NOT_OK;      
   }
   
   return RxSts;
}

/************************************************************************
* Function:     I2cSlv_I2cReadMsgSts
* Input:        None
* Output:       I2cReadMsgStsType
* Author:       F.Ficili
* Description:  API used to check if a message has been received from 
*               the slave device.
* Date:         24/08/16
************************************************************************/
I2cReadMsgStsType I2cSlv_I2cReadMsgSts (void)
{
   /* Service */
   I2cReadMsgStsType Result;
   
   /* Check if the message has been received or not */
   if (ReceiveEvt(&(I2cRxPacket.ReadCompleteEvt)))
   {
      Result = MessageReceived;
   }
   else
   {
      Result = MessageNotReceived;
   }
   
   return Result;
}

/************************************************************************
* Function:     I2cSlv_SendI2cMsg
* Input:        None
* Output:       I2cCommStsType
* Author:       F.Ficili
* Description:  API used to get the global I2C status (TxRxbusy or Read/Write
*               complete). 
* Date:         10/12/18
************************************************************************/
I2cCommStsType I2cSlv_GetI2cSts(void)
{
   I2cCommStsType Ret;
   
   /* Check the com variable */
   if (I2cComSts == TxRxIdle)
   {
      Ret = I2cTxRxComplete;
   }
   else
   {
      Ret = I2cTxRxInProgress;
   }
   
   return Ret;
}

/************************************************************************
* Function:     I2cSlv_QueryI2cData
* Input:        UINT8* TxBuffer
*               UINT8 TxLen
*               UINT8* RxBuffer
*               UINT8 RxLen
*               UINT8 SlaveAddr
* Output:       I2cOpStsType
* Author:       F.Ficili
* Description:  API used to query an I2c Data to a slave. Note: this API works in state-machine fashion,
*               the user has to keep calling the API until either OP_COMPLETE or OP_TIMEOUT is returned.
* Date:         17/01/19
************************************************************************/
I2cOpStsType I2cSlv_QueryI2cData (UINT8* TxBuffer, UINT8 TxLen, UINT8* RxBuffer, UINT8 RxLen, UINT8 SlaveAddr)
{
   static I2cOpStateType I2cOpState = ASK_DATA;
   static SwTimerType QueryTimer;
   static SwTimerType TimeoutTimer;
   static I2cOpStsType Ret;
   
   switch(I2cOpState)
   {
      case ASK_DATA:
         /* Load tx buffer and transmit */
         I2cSlv_SendI2cMsg(TxBuffer, SlaveAddr, TxLen);
         /* Switch state */
         I2cOpState = REQ_DATA; 
         /* Update output */
         Ret = OP_IN_PROGRESS;
         break;
         
      case REQ_DATA:
         /* Request read op  */
         if (OsTmr_Wait(&QueryTimer,QUERY_REQ_DATA_DELAY_MS) == DelayExpired)
         {
            OsTmr_StartTimer(&TimeoutTimer,QUERY_TIMEOUT_MS);
            I2cSlv_ReceiveI2cMsg(RxBuffer, SlaveAddr, RxLen);
            I2cOpState = GET_DATA;         
         }     
         /* Update output */
         Ret = OP_IN_PROGRESS;         
         break;
         
      case GET_DATA:
         /* If the message is arrived */
         if (I2cSlv_I2cReadMsgSts() == MessageReceived)
         {            
            /* Switch state */
            I2cOpState = ASK_DATA; 
            /* Update output */
            Ret = OP_COMPLETE;             
         }    
         else
         {
            if (OsTmr_GetTimerStatus(&TimeoutTimer) == SwTimerExpired)
            {
               /* Switch state */
               I2cOpState = ASK_DATA;                
               /* Update output */
               Ret = OP_TIMEOUT;                
            }
            else
            {
               /* Update output */
               Ret = OP_IN_PROGRESS;                 
            }            
         }        
         break;
         
      default:
         break;
   }
   
   return Ret;
}