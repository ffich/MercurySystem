/************************************************************************
*                              	APP                              		*
*************************************************************************
* FileName:         app.h                                      			*
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
* F.Ficili     09/07/17    1.0          First release                   *
************************************************************************/
#ifndef APP_H
#define	APP_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Task period in ms */
#define MY_APP_TASK_PERIOD_MS                         ((UINT16)(50))

/************************************************************************
* EXPORTED Typedef
************************************************************************/


/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Application main task */
void MyApp_Task (UINT8 Options);

#endif	/* APP_H */

