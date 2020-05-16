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
* F.Ficili     23/07/16    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "sys_init.h"
#include "os_sched.h"

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
void InitSchedTimer (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     InitSchedTimer
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Initialize timer 2 as scheduler timebase.
* Date:         23/07/16
************************************************************************/
void InitSchedTimer (void)
{   
   TmrBConfigType TmrBConfig;
   
   /* Configure the scehduler timer */
   TmrBConfig.PreScaler = 4;
   TmrBConfig.PostScaler = 12;
   TmrBConfig.IntPriority = TMR2_ISR_PRIORITY;
   TmrBConfig.IntEnable = STD_ON;
   TmrBConfig.IntFreqHz = SCHED_TIMER_FREQ_HZ;
   TmrBConfig.StartTmrRequest = StartTmrReq;
   
   /* Initialize scheduler timer */
   Tmr_TmrBConfig(TIMER_2,&TmrBConfig);
}

/************************************************************************
* Function:     InitAuxTimer
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Initialize timer 4 as auxiliary timer.
* Date:         18/04/17
************************************************************************/
void InitAuxTimer (void)
{
   TmrBConfigType TmrBConfig;
   
   /* Configure the scehduler timer */
   TmrBConfig.PreScaler = 4;
   TmrBConfig.PostScaler = 12;
   TmrBConfig.IntPriority = TMR4_ISR_PRIORITY;
   TmrBConfig.IntEnable = STD_ON;
   TmrBConfig.IntFreqHz = 100;
   TmrBConfig.StartTmrRequest = StartTmrReq;
   
   /* Initialize scheduler timer */
   Tmr_TmrBConfig(TIMER_4,&TmrBConfig);   
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
   /* Release pin control after a DS */
   Pm_DsReleasePinControl();
   /* Start PLL */
   Mcu_StartSystemPll();
   /* Configure interrupts */
   Irq_InitIntController(IntPriorityMode,STD_ON,STD_ON);
   /* Register Tmr2 ISR callback: Scheduler Services */
   Tmr_RegisterTmr2IsrCallback(&Sch_SchmIntService);
   /* Init and start scheduler timer */
   InitSchedTimer();
   /* Configure external interrupts */
   Mcu_ConfigExtInt();
}