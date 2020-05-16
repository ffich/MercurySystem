/************************************************************************
*                                Test                                   *
*************************************************************************
* FileName:         test.h                                              *
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

#ifndef TEST_H
#define	TEST_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

#if (TEST_TASK_STS == STD_ON)   
/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Task period in ms */
#define TEST_TASK_PERIOD_MS                               ((UINT16)(1000))

/************************************************************************
* EXPORTED Typedef
************************************************************************/


/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Test task */
void Test_MainTask (UINT8 Options);

#endif /* (TEST_TASK_STS == STD_ON) */

#endif	/* TEST_H */

