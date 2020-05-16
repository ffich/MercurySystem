/************************************************************************
*                             System Manager                            *
*************************************************************************
* FileName:         sysm.c                                              *
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
* F.Ficili     21/04/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "sysm.h"

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


/************************************************************************
* LOCAL Function Implementations
************************************************************************/


/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     Sysm_IdleMode
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  API to trigger the system idle mode (CPU  off, peripherals on). 
*               Wakeup by:
*               - Any enabled interrupt
*               - Wdg
*               - Reset    
* Date:         24/04/17
************************************************************************/
void Sysm_IdleMode (void)
{
   /* Switch to idle mode */
   Pm_ChangePwrMode(IdleMode);
}

/************************************************************************
* Function:     Sysm_SleepMode
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  API to trigger sleep mode (CPU and peripherals off). 
*               Wakeup by:
*               - Rtcc alarm
*               - Timer 1 interrupt
*               - INTx interrupt
*               - Wdg
*               - Reset    
* Date:         24/04/17
************************************************************************/
void Sysm_SleepMode (void)
{
   /* Switch to sleep mode */
   Pm_ChangePwrMode(SleepMode);
}

/************************************************************************
* Function:     Sysm_DeepSleepMode
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  API to trigger deep sleep mode (CPU and peripherals off). 
*               Wakeup by:
*               - Rtcc alarm
*               - INT0 interrupt
*               - DsWdg
*               - MCLR    
* Date:         24/04/17
************************************************************************/
void Sysm_DeepSleepMode (void)
{
   /* Switch to deep sleep mode */
   Pm_ChangePwrMode(DeepSleepMode);
}