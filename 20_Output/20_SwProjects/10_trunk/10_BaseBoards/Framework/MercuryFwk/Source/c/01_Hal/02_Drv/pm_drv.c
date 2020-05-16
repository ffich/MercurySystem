/************************************************************************
*                          Power Modes Driver                           *
*************************************************************************
* FileName:         pm_drv.c                                            *
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
* F.Ficili     23/04/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "pm_drv.h"

/************************************************************************
* Defines
************************************************************************/
/* Go to sleep function-like macro */
#define GO_TO_SLEEP()                                       asm ("SLEEP")

/************************************************************************
* Macros
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
* Function:     Pm_ChangePwrMode
* Input:        PowerModesType TargetMode                   
* Output:       None
* Author:       F.Ficili
* Description:  Change current power mode to target mode.
*               Allowed transition:
*               Run --> Idle
*               Run --> Sleep
*               Run --> DeepSleep
* Date:         23/04/17
************************************************************************/
void Pm_ChangePwrMode (PowerModesType TargetMode)
{
   switch (TargetMode)
   {
      case RunMode:
         /* Do nothing */
         break;
         
      case IdleMode:
         /* Set IDLEN bit */
         OSCCONbits.IDLEN = STD_ON;
         /* Enter sleep mode */
         GO_TO_SLEEP();         
         break;
         
      case SleepMode:
         /* Clear IDLEN bit */
         OSCCONbits.IDLEN = STD_OFF;
         /* Enter sleep mode */
         GO_TO_SLEEP();
         break;
         
      case DeepSleepMode:
         /* Set REGSLP bit */
         WDTCONbits.REGSLP = STD_ON;
         /* Clear IDLEN bit */
         OSCCONbits.IDLEN = STD_OFF;
         /* Clear GIE/GIEH */
         Irq_ClearGieGieh();
         /* Set DSEN bit */
         DSCONHbits.DSEN = STD_ON;
         /* Enter deep sleep mode */
         GO_TO_SLEEP();                  
         break;
         
      case UltraLowPowerWakeUpMode:
         /* Not implemented at the moment */
         break;
         
      default:
         break;
   }
}

/************************************************************************
* Function:     Pm_SaveDsData
* Input:        UINT8 Data0
*               UINT8 Data1                   
* Output:       None
* Author:       F.Ficili
* Description:  Save data which needs to persist after a DS cyclce.
* Date:         23/04/17
************************************************************************/
void Pm_SaveDsData(UINT8 Data0, UINT8 Data1)
{
   /* Save data */
   DSGPR0 = Data0;
   DSGPR1 = Data1;
}

/************************************************************************
* Function:     Pm_RestoreDsData
* Input:        None              
* Output:       UINT8 *Data0
*               UINT8 *Data1     
* Author:       F.Ficili
* Description:  Restore data which needs to persist after a DS cyclce.
* Date:         23/04/17
************************************************************************/
void Pm_RestoreDsData(UINT8 *Data0, UINT8 *Data1)
{
   /* Restore data */
   *Data0 = DSGPR0;
   *Data1 = DSGPR0;
}

/************************************************************************
* Function:     Pm_DsReleasePinControl
* Input:        None              
* Output:       None  
* Author:       F.Ficili
* Description:  Release pin control after Deep Sleep. This is needed
*               to allow control TRIS and LAT registers after a deep sleep. 
* Date:         23/04/17
************************************************************************/
void Pm_DsReleasePinControl(void)
{
   /* Clear RELEASE bit */
   DSCONLbits.RELEASE = STD_OFF;   
}