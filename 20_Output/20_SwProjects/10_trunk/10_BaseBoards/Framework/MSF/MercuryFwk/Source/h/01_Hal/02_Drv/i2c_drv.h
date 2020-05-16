/************************************************************************
*                            I2C Driver                                 *
*************************************************************************
* FileName:         i2c_drv.h                                           *
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

#ifndef I2C_DRV_H
#define	I2C_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"


/************************************************************************
* EXPORTED Defines
************************************************************************/
/* I2C Module registers */
#define I2C_CONTROL_REG_1                                   SSP2CON1
#define I2C_CONTROL_REG_2                                   SSP2CON2
#define I2C_STATUS_REG                                      SSP2STAT
#define I2C_TX_RX_BUFFER                                    SSP2BUF
#define I2C_BRG_REG                                         SSP2ADD

/* Baud Rate generator parameters */
#define DESIRED_I2C_BUS_CLOCK_HZ                            ((UINT32)(400000))

/* -- Communication control parameters -- */

/* CONTROL 1 Register */
#define ENABLE_MODULE_SHIFT                                 ((UINT8)(5))
#define I2C_MODULE_OFF                                      ((UINT8)(0))
#define I2C_MODULE_ON                                       ((UINT8)(1))

#define MODE_SHIFT                                          ((UINT8)(0))
#define I2C_SLAVE_10BIT_ADDR_WITH_INT                       ((UINT8)(0b1111))
#define I2C_SLAVE_7BIT_ADDR_WITH_INT                        ((UINT8)(0b1110))
#define I2C_MASTER_FW_CONTROLLED                            ((UINT8)(0b1011))
#define I2C_MASTER_HW_CLOCK                                 ((UINT8)(0b1000))
#define I2C_SLAVE_10BIT_ADDR                                ((UINT8)(0b0111))
#define I2C_SLAVE_7BIT_ADDR                                 ((UINT8)(0b0110))

/* CONTROL 2 Register */

#define ACK_STATUS_MASK                                     ((UINT8)(0x40))
#define ACK_STATUS_SHIFT                                    ((UINT8)(6))
#define ACK_RECEIVED_FROM_SLAVE                             ((UINT8)(0))
#define ACK_NOT_RECEIVED_FROM_SLAVE                         ((UINT8)(1))

#define DATA_ACK_STATUS_MASK                                ((UINT8)(0x20))
#define DATA_ACK_STATUS_SHIFT                               ((UINT8)(5))
#define DATA_ACK_RECEIVED_FROM_SLAVE                        ((UINT8)(0))
#define DATA_ACK_NOT_RECEIVED_FROM_SLAVE                    ((UINT8)(1))

#define ACK_COND_MASK                                       ((UINT8)(0x10))
#define ACK_COND_SHIFT                                      ((UINT8)(4))
#define INITIATE_ACK_COND                                   ((UINT8)(1))
#define DO_NOT_INITIATE_ACK_COND                            ((UINT8)(0))

#define ENABLE_RECEIVE_MASK                                 ((UINT8)(0x08))
#define ENABLE_RECEIVE_SHIFT                                ((UINT8)(3))
#define ENABLE_I2C_RECEIVE                                  ((UINT8)(1))
#define I2C_RECEIVE_IDLE                                    ((UINT8)(0))

#define STOP_COND_SHIFT                                     ((UINT8)(2))
#define INITIATE_STOP_COND                                  ((UINT8)(1))
#define STOP_COND_IDLE                                      ((UINT8)(0))

#define REP_START_COND_SHIFT                                ((UINT8)(1))
#define INITIATE_REP_START_COND                             ((UINT8)(1))
#define REP_START_COND_IDLE                                 ((UINT8)(0))

#define START_COND_SHIFT                                    ((UINT8)(0))
#define INITIATE_START_COND                                 ((UINT8)(1))
#define START_COND_IDLE                                     ((UINT8)(0))

/* STATUS Register */
#define STOP_BIT_DETECTED_LAST_MASK                         ((UINT8)(0x10))
#define STOP_BIT_DETECTED_LAST_SHIFT                        ((UINT8)(4))
#define STOP_BIT_NOT_DET_LAST                               ((UINT8)(0))
#define STOP_BIT_DET_LAST                                   ((UINT8)(1))

#define START_BIT_DETECTED_LAST_MASK                        ((UINT8)(0x08))
#define START_BIT_DETECTED_LAST_SHIFT                       ((UINT8)(3))
#define START_BIT_NOT_DET_LAST                              ((UINT8)(0))
#define START_BIT_DET_LAST                                  ((UINT8)(1))

#define TX_STATUS_MASK                                      ((UINT8)(0x04))
#define TX_STATUS_SHIFT                                     ((UINT8)(2))
#define TX_COMPLETED                                        ((UINT8)(0))
#define TX_NOT_COMPLETED                                    ((UINT8)(1))

#define BUFFER_FULL_MASK                                    ((UINT8)(0x01))
#define BUFFER_FULL_SHIFT                                   ((UINT8)(0))
#define SSP_BUFFER_FULL                                     ((UINT8)(1))
#define SSP_BUFFER_EMPTY                                    ((UINT8)(0))

/* General I2C defines */
#define WRITE_OP                                            ((UINT8)(0))
#define READ_OP                                             ((UINT8)(1))

#define ACKNOWLEDGE_DATA                                    ((UINT8)(0))
#define DO_NOT_ACKNOWLEDGE_DATA                             ((UINT8)(1))

#define MAX_NUMBER_OF_TENTATIVE                             ((UINT8)(3))

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
   TxAssertStart,
   TxSendSlaveAddress,
   TxCheckTxAddr,
   TxSendDataByte,
   TxCheckTxData,
   TxCheckLenght,
   TxAssertStop,
   TxCompleted,
   TxCheckAck,
   TxErrorStatus
} I2cWriteStateType;

/* I2C configuration */
typedef struct _I2cConfig
{
   UINT32 I2cBaudRateHz;
   UINT32 McuBusClockHz;
   UINT8 I2cModuleMode;
   UINT8 EnableI2cModule;
} I2cConfigType;

/* I2c write data packet */
typedef struct _I2cWritePacket
{
   UINT8 SlaveAddress;
   UINT8 ReadWrite;
   UINT8 *Buffer;
   UINT8 Lenght;
} I2cWritePacketType;

/* I2c read data packet */
typedef struct _I2cReadPacket
{
   UINT8 SlaveAddress;
   UINT8 ReadWrite;
   UINT8 *Buffer;
   UINT8 Lenght;
   EventStructureType ReadCompleteEvt;
} I2cReadPacketType;

/* I2c read state */
typedef enum _I2cReadState
{
   RxAssertStart,
   RxSendSlaveAddr,
   RxCheckTxAddr,
   RxCheckAck,
   RxGetData,           
   RxReadData,
   RxCheckLen,
   RxAssertStop,
   RxErrorStatus,
} I2cReadStateType;

typedef enum _I2cReadSts
{
   I2cReadInProgress,
   I2cReadNotOk,
   I2cReadOk
} I2cReadStsType;

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
void I2c_InitI2cModule (I2cConfigType* I2cConfig);
I2cWriteStsType I2c_WriteData (I2cWritePacketType *WritePacket);
I2cReadStsType I2c_ReadData (I2cReadPacketType *ReadPacket);

#endif	/* I2C_DRV_H */
