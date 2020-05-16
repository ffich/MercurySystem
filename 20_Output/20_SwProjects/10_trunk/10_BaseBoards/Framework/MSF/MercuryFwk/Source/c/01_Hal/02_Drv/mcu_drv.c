/************************************************************************
*                        	MCU Driver                              *
*************************************************************************
* FileName:         mcu_drv.c                                           *
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
* F.Ficili     17/04/17    1.0          First release                   *
* F.Ficili     17/03/19    1.1          Added wdg support.              *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "mcu_drv.h"


/************************************************************************
* Defines
************************************************************************/
/* System PLL constants */
#define DISABLE_PLL                                      ((UINT8)(0))
#define ENABLE_PLL                                       ((UINT8)(1))

/* PLL startup counter constant */
#define PLL_STARTUP_CNT_INIT                             ((UINT16)(1000))

/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/


/************************************************************************
* GLOBAL Variables
************************************************************************/
/* ExtInt ISR callback pointer */
ExtInt_CallbackType ExtInt0IsrCallback = NULL;
ExtInt_CallbackType ExtInt1IsrCallback = NULL;

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
* Function:     Mcu_StartSystemPll
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Start the system PLL.
* Date:         17/04/17
************************************************************************/
void Mcu_StartSystemPll (void)
{
   volatile UINT16 PllStartupCounter = PLL_STARTUP_CNT_INIT;      
   
   /* Enable PLL */
   OSCTUNEbits.PLLEN = ENABLE_PLL;
   /* Wait for PLL to stabilize */
   while(PllStartupCounter--);
}

/************************************************************************
* Function:     Mcu_InitExtInt
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Initialize external interrupts.
* Date:         26/05/17
************************************************************************/
void Mcu_ConfigExtInt(void)
{
   /* Initialize TRIS registers */
   BSP_INT_0_TRIS = LINE_DIRECTION_INPUT;
   BSP_INT_1_TRIS = LINE_DIRECTION_INPUT;
   
   /* Set RB0 as digital */
   ANCON1bits.PCFG12 = STD_ON;   
   /* Remap INT1 function to RP20 (RD3) */
   RPINR1 = 20;
   
   /* Enable interrupts */
   INTCONbits.INT0IE = STD_ON;
   INTCON3bits.INT1IE = STD_ON;
   INTCON3bits.INT1IP = INT_1_ISR_PRIORITY;
   INTCON2bits.INTEDG0 = STD_OFF;
   INTCON2bits.INTEDG1 = STD_OFF;
}

/************************************************************************
* Function:     Mcu_Int0Isr
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  Interrupt Service Routine for ExtInt.
* Date:         26/05/17
************************************************************************/
void Mcu_Int0Isr (void)
{
   /* Clear interrupt flag */
   INTCONbits.INT0IF = STD_OFF;
   
   /* Check if the callback function has been correctly registered */
   if (ExtInt0IsrCallback != NULL)
   {
      /* Call callback funtion */
      ExtInt0IsrCallback();   
   }   
}

/************************************************************************
* Function:     Mcu_RegisterInt0IsrCallback
* Input:        ExtInt_CallbackType IsrCallback                     
* Output:       None
* Author:       F.Ficili
* Description:  Register callback function to service the ISR.
* Date:         26/05/17
************************************************************************/
void Mcu_RegisterInt0IsrCallback (ExtInt_CallbackType IsrCallback)
{
   /* Callback registration */
   ExtInt0IsrCallback = IsrCallback;
}

/************************************************************************
* Function:     Mcu_Int0Isr
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  Interrupt Service Routine for ExtInt.
* Date:         26/05/17
************************************************************************/
void Mcu_Int1Isr (void)
{
   /* Clear interrupt flag */
   INTCON3bits.INT1IF = STD_OFF;
   
   /* Check if the callback function has been correctly registered */
   if (ExtInt1IsrCallback != NULL)
   {
      /* Call callback funtion */
      ExtInt1IsrCallback();   
   }   
}

/************************************************************************
* Function:     Mcu_RegisterInt1IsrCallback
* Input:        ExtInt_CallbackType IsrCallback                     
* Output:       None
* Author:       F.Ficili
* Description:  Register callback function to service the ISR.
* Date:         26/05/17
************************************************************************/
void Mcu_RegisterInt1IsrCallback (ExtInt_CallbackType IsrCallback)
{
   /* Callback registration */
   ExtInt1IsrCallback = IsrCallback;
}

/************************************************************************
* Function:     Mcu_SetWdgSts
* Input:        UINT8 WdgSts (ON or OFF)
* Output:       None
* Author:       F.Ficili
* Description:  Enable/disable internal watchdog.
* Date:         17/03/19
************************************************************************/
void Mcu_SetWdgSts (UINT8 WdgSts)
{
   /* Set Wdg Status */
   WDTCONbits.SWDTEN = WdgSts;   
}

/************************************************************************
* Function:     Mcu_ClearWdg
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Clear internal watchdog.
* Date:         17/03/19
************************************************************************/
void Mcu_ClearWdg (void)
{
   /* Clear Wdg */
   asm ("CLRWDT");   
}
