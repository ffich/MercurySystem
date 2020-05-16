/************************************************************************
*                           I2C Slave Manager                           *
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

/************************************************************************
* EXPORTED Typedef
************************************************************************/


/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Slave address variable */
extern UINT8 I2cSlaveAddress;
/* Set address event */
extern EventStructureType I2cSetAddress;
/* Write request event */
extern EventStructureType I2cWriteRequest;
/* Reply ready event */
extern EventStructureType I2cReplyPacketReady;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* I2c Slave task */
void I2cSlv_MainTask (UINT8 Options);
/* API used to send and I2c message to a specific slave device */
void I2cSlv_SendI2cMsg (UINT8* TxBuffer, UINT8 SlaveAddr, UINT8 DataLenght);

#endif	/* I2C_SLV_H */

