/************************************************************************
*                           I2C Slave                                   *
*************************************************************************
* FileName:         i2c_slv.h                                           *
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
************************************************************************/

#ifndef I2C_SLV_H
#define	I2C_SLV_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Task period in ms */
#define I2C_SLV_TASK_PERIOD_MS                           ((UINT16)(1))

/* -- Legacy MACRO support -- */

/* MACRO to ensure compatibility with some old demos */
#define QueryI2cData                                     I2cSlv_QueryI2cData

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* I2c Msg status */
typedef enum _I2cReadMsgSts
{
   MessageNotReceived,
   MessageReceived,
} I2cReadMsgStsType;

/* I2c Bus status type */
typedef enum _I2cBusStatusType
{
   I2cBusNotReady,
   I2cBusReady,
} I2cBusStatusType;

/* I2c Comm status type */
typedef enum _I2cCommSts
{
   I2cTxRxInProgress,
   I2cTxRxComplete,
} I2cCommStsType;

typedef enum
{
   OP_IN_PROGRESS = 0,
   OP_COMPLETE,
   OP_TIMEOUT
} I2cOpStsType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
extern I2cBusStatusType I2cBusStatus;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* I2c Slave task */
void I2cSlv_MainTask (UINT8 Options);
/* API used to send and I2c message to a specific slave device */
UINT8 I2cSlv_SendI2cMsg (UINT8* TxBuffer, UINT8 SlaveAddr, UINT8 DataLenght);
/* API used to receive and I2c message from a specific slave device */
UINT8 I2cSlv_ReceiveI2cMsg (UINT8* RxBuffer, UINT8 SlaveAddr, UINT8 DataLenght);
/* API used to check if a message has been received from the slave */
I2cReadMsgStsType I2cSlv_I2cReadMsgSts (void);
/* API used to get the global I2C status (TxRxbusy or Read/Write complete).*/
I2cCommStsType I2cSlv_GetI2cSts(void);
/* API used to query an I2c Data to a slave */
I2cOpStsType I2cSlv_QueryI2cData (UINT8* TxBuffer, UINT8 TxLen, UINT8* RxBuffer, UINT8 RxLen, UINT8 SlaveAddr);

#endif	/* I2C_SLV_H */

