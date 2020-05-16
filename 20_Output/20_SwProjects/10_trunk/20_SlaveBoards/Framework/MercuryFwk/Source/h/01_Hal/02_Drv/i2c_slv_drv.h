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

#ifndef I2C_SLV_DRV_H
#define	I2C_SLV_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Implementation type */
#define STATE_MACHINE_IMPLEMENTATION

/* Task period in ms */
#define I2C_SLV_DRIVER_PERIOD_MS                            ((UINT16)(1))

/* I2C Module registers */
#define I2C_CONTROL_REG_1                                   SSP1CON1
#define I2C_CONTROL_REG_2                                   SSP1CON2
#define I2C_CONTROL_REG_3                                   SSP1CON3
#define I2C_STATUS_REG                                      SSP1STAT
#define I2C_TX_RX_BUFFER                                    SSP1BUF
#define I2C_BRG_REG                                         SSP1ADD

/* -- Communication control parameters -- */

/* CONTROL 1 Register */
#define RECEIVE_OVERFLOW_SHIFT                              ((UINT8)(6))
#define NO_RX_OVERFLOW                                      ((UINT8)(0))
#define RX_OVERFLOW                                         ((UINT8)(1))

#define ENABLE_MODULE_SHIFT                                 ((UINT8)(5))
#define I2C_MODULE_OFF                                      ((UINT8)(0))
#define I2C_MODULE_ON                                       ((UINT8)(1))

#define CLOCK_STRETCH_SHIFT                                 ((UINT8)(5))
#define STRETCH_CLOCK                                       ((UINT8)(0))
#define ALLOW_CLOCK                                         ((UINT8)(1))

#define MODE_SHIFT                                          ((UINT8)(0))
#define I2C_SLAVE_10BIT_ADDR_WITH_INT                       ((UINT8)(0b1111))
#define I2C_SLAVE_7BIT_ADDR_WITH_INT                        ((UINT8)(0b1110))
#define I2C_MASTER_FW_CONTROLLED                            ((UINT8)(0b1011))
#define I2C_MASTER_HW_CLOCK                                 ((UINT8)(0b1000))
#define I2C_SLAVE_10BIT_ADDR                                ((UINT8)(0b0111))
#define I2C_SLAVE_7BIT_ADDR                                 ((UINT8)(0b0110))

/* CONTROL 2 Register */
#define GENERAL_CALL_ENABLE_SHIFT                           ((UINT8)(7))
#define GC_ENABLED                                          ((UINT8)(1))
#define GC_DISABLED                                         ((UINT8)(0))

#define ACK_STATUS_MASK                                     ((UINT8)(0x40))
#define ACK_STATUS_SHIFT                                    ((UINT8)(6))
#define ACK_RECEIVED                                        ((UINT8)(0))
#define ACK_NOT_RECEIVED                                    ((UINT8)(1))

#define DATA_ACK_STATUS_SHIFT                               ((UINT8)(5))
#define DATA_ACK_RECEIVED_FROM_SLAVE                        ((UINT8)(0))
#define DATA_ACK_NOT_RECEIVED_FROM_SLAVE                    ((UINT8)(1))

#define ACK_COND_SHIFT                                      ((UINT8)(4))
#define INITIATE_ACK_COND                                   ((UINT8)(1))
#define ACQ_COND_IDLE                                       ((UINT8)(0))

#define ENABLE_RECEIVE_SHIFT                                ((UINT8)(3))
#define ENABLE_I2C_RECEIVE                                  ((UINT8)(1))
#define I2C_RECEIVE_IDLE                                    ((UINT8)(0))

#define STOP_COND_SHIFT                                     ((UINT8)(2))
#define INITIATE_STOP_COND                                  ((UINT8)(1))
#define STOP_COND_IDLE                                      ((UINT8)(0))

#define REP_START_COND_SHIFT                                ((UINT8)(1))
#define INITIATE_REP_START_COND                             ((UINT8)(1))
#define REP_START_COND_IDLE                                 ((UINT8)(0))

#define CLOCK_STRETCH_EN_SHIFT                              ((UINT8)(0))
#define CLOCK_STRETCH_ENABLED                               ((UINT8)(1))
#define CLOCK_STRETCH_DISABLED                              ((UINT8)(0))

/* CONTROL 3 Register */
#define ACK_SEQ_STS_SHIFT                                   ((UINT8)(7))
#define ACK_SEQ_IN_PROGRESS                                 ((UINT8)(1))
#define NO_ACK_SEQ                                          ((UINT8)(0))

#define STOP_COND_INT_SHIFT                                 ((UINT8)(6))
#define STOP_COND_INT_ENABLED                               ((UINT8)(1))
#define STOP_COND_INT_DISABLED                              ((UINT8)(0))

#define START_COND_INT_SHIFT                                ((UINT8)(5))
#define START_COND_INT_ENABLED                              ((UINT8)(1))
#define START_COND_INT_DISABLED                             ((UINT8)(0))

#define BUFF_OVERWRITE_EN_SHIFT                             ((UINT8)(4))
#define BUFF_OVERWRITE_ENABLED                              ((UINT8)(1))
#define BUFF_OVERWRITE_DISABLED                             ((UINT8)(0))

#define BUS_COLLISION_DET_SHIFT                             ((UINT8)(2))
#define BUS_COLLISION_DET_ENABLED                           ((UINT8)(1))
#define BUS_COLLISION_DET_DISABLED                          ((UINT8)(0))

#define ADDR_HOLD_SHIFT                                     ((UINT8)(1))
#define ADDR_HOLD_ENABLED                                   ((UINT8)(1))
#define ADDR_HOLD_DISABLED                                  ((UINT8)(0))

#define DATA_HOLD_SHIFT                                     ((UINT8)(0))
#define DATA_HOLD_ENABLED                                   ((UINT8)(1))
#define DATA_HOLD_DISABLED                                  ((UINT8)(0))

/* STATUS Register */
#define STOP_BIT_DETECTED_LAST_MASK                         ((UINT8)(0x10))
#define STOP_BIT_DETECTED_LAST_SHIFT                        ((UINT8)(4))
#define STOP_BIT_NOT_DET_LAST                               ((UINT8)(0))
#define STOP_BIT_DET_LAST                                   ((UINT8)(1))

#define START_BIT_DETECTED_LAST_MASK                        ((UINT8)(0x08))
#define START_BIT_DETECTED_LAST_SHIFT                       ((UINT8)(3))
#define START_BIT_NOT_DET_LAST                              ((UINT8)(0))
#define START_BIT_DET_LAST                                  ((UINT8)(1))

#define DA_STATUS_MASK                                      ((UINT8)(0x20))
#define DA_STATUS_SHIFT                                     ((UINT8)(5))
#define LAST_BYTE_ADDR                                      ((UINT8)(0))
#define LAST_BYTE_DATA                                      ((UINT8)(1))

#define RW_STATUS_MASK                                      ((UINT8)(0x04))
#define RW_STATUS_SHIFT                                     ((UINT8)(2))
#define MASTER_WRITE_REQUEST                                ((UINT8)(0))
#define MASTER_READ_REQUEST                                 ((UINT8)(1))

#define BUFFER_FULL_MASK                                    ((UINT8)(0x01))
#define BUFFER_FULL_SHIFT                                   ((UINT8)(0))
#define SSP_BUFFER_FULL                                     ((UINT8)(1))
#define SSP_BUFFER_EMPTY                                    ((UINT8)(0))

/* General I2C defines */
#define WRITE_OP                                            ((UINT8)(0))
#define READ_OP                                             ((UINT8)(1))

#define RX_BYTE_IS_ADDR                                     ((UINT8)(0))
#define RX_BYTE_IS_DATA                                     ((UINT8)(1))

#define SLAVE_ADDRESS_SHIFT                                 ((UINT8)(1))
#define SLAVE_ADDRESS_MASK_SHIFT                            ((UINT8)(1))

#define OP_TYPE_MASK                                        ((UINT8)(0x01))

/* Buffers lenght */
#define I2C_RX_BUFFER_DIM                                   ((UINT8)(80))
#define I2C_TX_BUFFER_DIM                                   ((UINT8)(80))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
typedef enum _I2cWriteSts
{
    I2cWriteInProgress,
    I2cWriteNotOk,
    I2cWriteOk
} I2cWriteStsType;

/* I2c write state */
typedef enum _I2cWriteState
{
    AssertStart,
    SendSlaveAddress,
    CheckTxAddr,
    SendDataByte,
    CheckTxData,
    CheckLenght,
    AssertStop,
    TxCompleted,
    CheckAck,
    ErrorStatus
} I2cWriteStateType;

/* I2C configuration */
typedef struct _I2cConfig
{
   UINT8 I2cModuleMode;
   UINT8 I2cReceiverEnable;
   UINT8 EnableI2cModule;
   UINT8 ClockStretchEnable;
   UINT8 StopCondIntEnable;
   UINT8 StartCondIntEnable;
   UINT8 BufferOverwriteEnable;
   UINT8 BusCollisionDetectEnable;
   UINT8 AddrHoldEnable;
   UINT8 DataHoldEnable;
   UINT8 GeneralCallEnable;
} I2cConfigType;

/* I2c data packet */
typedef struct _I2cReadPacket
{
   UINT8 SlaveAddress;
   UINT8 ReadWrite;
   UINT8 Buffer[I2C_RX_BUFFER_DIM];
   UINT8 Lenght;
} I2cReadPacketType;

/* Rx state machine state */
typedef enum _I2cRxState
{
   CheckRxByte,
   StoreAddrByte,
   HandleWrite,
   HandleRead,        
   StopClkStretch,     
} I2cRxStateType;


/************************************************************************
* EXPORTED Variables
************************************************************************/
/* I2c rx packet */
extern I2cReadPacketType I2cPacket;
/* New packet event */
extern EventStructureType I2cNewPacketNotification;
/* New Transmission event */
extern EventStructureType I2cNewTxNotification;

/************************************************************************
* EXPORTED Functions
************************************************************************/
void I2c_InitI2cModule (I2cConfigType* I2cConfig);
void I2c_SetI2cAddress (UINT8 SlaveAddress);
void I2c_SetI2cAddressMask (UINT8 SlaveAddressMask);
void I2c_DataReceiveSm (UINT8 Options);
void I2c_UpdateTxBuffer(UINT8 *DataBuffer, UINT8 Length);
/* Only used in Non-StateMachine implementation */
void I2c_DataReceive (void);

#endif	/* I2C_SLV_DRV_H */
