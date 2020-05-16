/************************************************************************
*                               Watchdog                                *
*************************************************************************
* FileName:         wdg.c                                               *
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
* F.Ficili     17/03/19    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "wdg.h"

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
* Function:     Wdg_MainTask
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Internal wdg main task.
* Date:         01/01/18
************************************************************************/
void Wdg_MainTask (UINT8 Options)
{    
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* Enable Wdg */
         Mcu_SetWdgSts(STD_ON);
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Clear Wdg */
         Mcu_ClearWdg();
         break;

      /* Default */
      default:
         break;
   }
}