/************************************************************************
*                            System Init                                *
*************************************************************************
* FileName:         sys_init.c                                          *
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
* F.Ficili     02/01/18    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"
#include "sys_init.h"
#include "tmr_drv.h"
#include "mcu_drv.h"

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
void InitSchTimer (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     InitSchTimer
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Initialize the scheduler timebase.
* Date:         23/07/16
************************************************************************/
void InitSchTimer (void)
{
   TmrBConfigType TmrBConfig;
   
   /* Configure the scehduler timer */
   TmrBConfig.PreScaler = 1;
   TmrBConfig.PostScaler = 8;
   TmrBConfig.IntEnable = STD_ON;
   TmrBConfig.IntFreqHz = 10000; /* 100 us */
   TmrBConfig.StartTmrRequest = StartTmrReq;
   
   /* Initialize scheduler timer */
   Tmr_TmrBConfig(TIMER_2,&TmrBConfig);   
} 

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     SystemInit
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Global system inizialization
* Date:         23/07/16
************************************************************************/
void SystemInit (void)
{
   /* Start Sys Clock */
   Mcu_StartSystemPll();      
   /* Initialize scheduler timer */
   InitSchTimer();
   /* Enable Interrupts */
   Mcu_EnableInterrupts();
}