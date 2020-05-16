/************************************************************************
*                             SBXXX APP                                 *
*************************************************************************
* FileName:         sbXXX_app.h                                         *
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
* F.Ficili     XX/XX/XX    1.0          First release                   *
************************************************************************/

#ifndef APP_H
#define APP_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Board Name */
#define BOARD_NAME                                          "SBXXX"
/* App version Major, Minor, Fix */
#define APP_VERSION                                         "01.00.00"
/* Task period in ms */
#define APP_TASK_PERIOD_MS                                  ((UINT16)(10))

/************************************************************************
* EXPORTED Typedef
************************************************************************/


/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Global command buffer */
extern UINT8 CmdBuffer[I2C_RX_BUFFER_DIM];
/* Global reply buffer */
extern UINT8 RepBuffer[I2C_TX_BUFFER_DIM];
/* Reply lenght */
extern UINT8 RepBufferLen;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* App task */
void App_MainTask (UINT8 Options);

#endif	/* APP_H */

