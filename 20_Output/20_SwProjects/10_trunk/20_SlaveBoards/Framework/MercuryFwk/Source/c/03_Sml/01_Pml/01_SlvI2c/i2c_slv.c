/************************************************************************
*                             I2C Slave                                 *
*************************************************************************
* FileName:         i2c_slv.c                                           *
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
* F.Ficili     30/07/16    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "i2c_slv.h"

/************************************************************************
* Defines
************************************************************************/
#define SLAVE_ADDRESS_MASK                               ((UINT8)(0x7F))
#define GEN_CALL_ADDR                                    ((UINT8)(0x00))

/************************************************************************
* Typedefs
************************************************************************/
/* I2c Receive data type */
typedef enum _I2cReceiveMsgState
{
   I2cSetSlaveAddres,
   I2cReceiveData,
} I2cReceiveMsgStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* I2c configuration variable */
static I2cConfigType I2cConfig;

/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Slave address variable */
UINT8 I2cSlaveAddress;
/* Set address event */
EventStructureType I2cSetAddress = {0, EventIdle};
/* Write request event */
EventStructureType I2cWriteRequest = {0, EventIdle};
/* Reply ready event */
EventStructureType I2cReplyPacketReady = {0, EventIdle};

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
void I2c_SetConfig (void);
void I2cSlv_I2cDrvHandle(void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/
/************************************************************************
* Function:     I2c_SetConfig
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Set the I2c module configuration.
* Date:         29/07/16
************************************************************************/
void I2c_SetConfig (void)
{
    I2cConfig.I2cModuleMode = I2C_SLAVE_7BIT_ADDR;
    I2cConfig.I2cReceiverEnable = I2C_RECEIVE_IDLE;
    I2cConfig.EnableI2cModule = I2C_MODULE_ON;
    I2cConfig.ClockStretchEnable = CLOCK_STRETCH_ENABLED;
    I2cConfig.StopCondIntEnable = STOP_COND_INT_ENABLED;
    I2cConfig.StartCondIntEnable = START_COND_INT_ENABLED;
    I2cConfig.BufferOverwriteEnable = BUFF_OVERWRITE_DISABLED;
    I2cConfig.BusCollisionDetectEnable = BUS_COLLISION_DET_ENABLED;
    I2cConfig.AddrHoldEnable = ADDR_HOLD_DISABLED;
    I2cConfig.DataHoldEnable = DATA_HOLD_DISABLED; 
    I2cConfig.GeneralCallEnable = GC_ENABLED;
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     I2cSlv_MainTask
* Input:        UINT8 Options
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
         I2c_SetConfig();
         /* Initialize I2C Module */
         I2c_InitI2cModule(&I2cConfig);  
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Handle driver communication */
         I2cSlv_I2cDrvHandle();
         break;

      /* Default */
      default:
         break;
   }
}

/************************************************************************
* Function:     I2cSlv_I2cDrvHandle
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Handle I2c slave driver.
* Date:         31/07/16
************************************************************************/
void I2cSlv_I2cDrvHandle(void)
{
   static I2cReceiveMsgStateType I2cReceiveMsgState = I2cSetSlaveAddres;
   UINT8 Index = 0;
   
   switch (I2cReceiveMsgState)
   {
      case I2cSetSlaveAddres:
         /* Set address event */
         if (ReceiveEvt(&I2cSetAddress))
         {
            /* Slave address and address mask set */
            I2c_SetI2cAddress(I2cSlaveAddress);         
            I2c_SetI2cAddressMask(SLAVE_ADDRESS_MASK);
            /* Slave ready for reception, switch to I2cReceiveData */
            I2cReceiveMsgState = I2cReceiveData;
         }                 
         break;
         
      case I2cReceiveData:
         /* Driver Evt - New packet  */      
         if (ReceiveEvt(&I2cNewPacketNotification))
         {
            /* Double check Address */
            if ((I2cPacket.SlaveAddress == I2cSlaveAddress) || (I2cPacket.SlaveAddress == GEN_CALL_ADDR))
            {
               if (I2cPacket.ReadWrite == WRITE_OP)
               {
                  /* Fill command buffer */
                  for (Index = 0; Index < I2C_RX_BUFFER_DIM;Index++)
                  {
                     CmdBuffer[Index] = I2cPacket.Buffer[Index];
                  }                  
                  /* Notify write request */
                  GenerateEvt(&I2cWriteRequest);
                  /* Pulse LED if configured to do so */
                  if (LedBehaviourSts == LED_I2C_PULSE)
                  {      
                     Led_SetLedStatus(LED_1, LED_STS_PULSE);       
                  }                     
               }
            }            
         }
         /* APP Evt - Reply Ready */
         else if (ReceiveEvt(&I2cReplyPacketReady))
         {
            /* Update Driver Tx buffer */
            I2c_UpdateTxBuffer(RepBuffer,RepBufferLen);
            /* Pulse LED if configured to do so */
            if (LedBehaviourSts == LED_I2C_PULSE)
            {      
               Led_SetLedStatus(LED_1, LED_STS_PULSE);       
            }             
         }         
         break;
         
      default:
         break;
   }
}  
