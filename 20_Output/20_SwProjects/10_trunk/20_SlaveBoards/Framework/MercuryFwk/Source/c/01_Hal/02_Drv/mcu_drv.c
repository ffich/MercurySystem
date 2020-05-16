/************************************************************************
*                               Mcu Driver                              *
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
* F.Ficili     01/01/18    1.0          First release                   *
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

/* Interrupts constants */
#define DISABLE_INTERRUPTS                               ((UINT8)(0))
#define ENABLE_INTERRUPTS                                ((UINT8)(1))

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
* Function:     Mcu_InitIntOsc
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Internal clock initialization.
* Date:         30/07/16
************************************************************************/
void Mcu_InitIntOsc (void)
{
   /* Enable internal oscillator */
   OSCCONbits.SCS1 = ON;   
   /* Set INTOSC */
   OSCCONbits.IRCF = 0b1111;     
   /* Wait until the internal oscillator is accurate enough */
   while (OSCSTATbits.HFIOFS == 0);   
}

/************************************************************************
* Function:     Mcu_EnableInterrupts
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Enable all interrupts.
* Date:         23/07/16
************************************************************************/
void Mcu_EnableInterrupts (void)
{
   /* Global interrupts enable */
   INTCONbits.GIE = ENABLE_INTERRUPTS;
   INTCONbits.PEIE = ENABLE_INTERRUPTS;
}

/************************************************************************
* Function:     Mcu_EnableSchedulerInt
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Enable scheduler interrupt.
* Date:         24/04/19
************************************************************************/
void Mcu_EnableSchedulerInt (void)
{
   PIE1bits.TMR2IE = ON;
}

/************************************************************************
* Function:     Mcu_DisableInterrupts
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Disable all interrupts.
* Date:         23/07/16
************************************************************************/
void Mcu_DisableInterrupts (void)
{
   /* Global interrupts enable */
   INTCONbits.GIE = DISABLE_INTERRUPTS;
   INTCONbits.PEIE = DISABLE_INTERRUPTS;
}

/************************************************************************
* Function:     Mcu_DisableSchedulerInt
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Disable scheduler interrupt.
* Date:         24/04/19
************************************************************************/
void Mcu_DisableSchedulerInt (void)
{
   PIE1bits.TMR2IE = OFF;   
}

/************************************************************************
* Function:     Mcu_StartSystemPll
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Start the system PLL.
* Date:         23/07/16
************************************************************************/
void Mcu_StartSystemPll (void)
{
   /* Enable PLL */
   OSCCONbits.SPLLEN = ENABLE_PLL;
}

/************************************************************************
* Function:     Mcu_SetPullUpsSts
* Input:        GlobalPuStsType PuSts
* Output:       None
* Author:       F.Ficili
* Description:  Activate/deactivate pull-ups network.
* Date:         01/01/18
************************************************************************/
void Mcu_SetPullUpsSts (GlobalPuStsType PuSts)
{
   /* Set global PU status */
   OPTION_REGbits.nWPUEN = PuSts;
} 

/************************************************************************
* Function:     Mcu_ConfigPullUps
* Input:        WpuChType *WpuCh
* Output:       None
* Author:       F.Ficili
* Description:  Configure internal pull-ups.
* Date:         01/01/18
************************************************************************/
void Mcu_ConfigPullUps (WpuChType *WpuCh)
{   
   /* PORT A */   
   WPUA = 0x00;
   WPUAbits.WPUA0 = WpuCh->WpuA0;
   WPUAbits.WPUA1 = WpuCh->WpuA1;
   WPUAbits.WPUA2 = WpuCh->WpuA2;
   WPUAbits.WPUA3 = WpuCh->WpuA3;
   WPUAbits.WPUA4 = WpuCh->WpuA4;
   WPUAbits.WPUA5 = WpuCh->WpuA5; 
   
   /* PORT B */
#if defined(__16F1829)
   WPUB = 0x00;
   WPUBbits.WPUB4 = WpuCh->WpuB4;
   WPUBbits.WPUB5 = WpuCh->WpuB5;
   WPUBbits.WPUB6 = WpuCh->WpuB6;
   WPUBbits.WPUB7 = WpuCh->WpuB7;   
#endif
   
   /* PORT C */
   WPUC = 0x00;
   WPUCbits.WPUC0 = WpuCh->WpuC0;
   WPUCbits.WPUC1 = WpuCh->WpuC1;
   WPUCbits.WPUC2 = WpuCh->WpuC2;
   WPUCbits.WPUC3 = WpuCh->WpuC3;
   WPUCbits.WPUC4 = WpuCh->WpuC4;
   WPUCbits.WPUC5 = WpuCh->WpuC5;  
#if defined(__16F1829)   
   WPUCbits.WPUC6 = WpuCh->WpuC6; 
   WPUCbits.WPUC7 = WpuCh->WpuC7; 
#endif
}    

/************************************************************************
* Function:     Mcu_SetWdgPeriod
* Input:        WdgPeriodType WdgPeriod (look up table)
* Output:       None
* Author:       F.Ficili
* Description:  Setp expire periodo of internal wdg.
* Date:         01/01/18
************************************************************************/
void Mcu_SetWdgPeriod (WdgPeriodType WdgPeriod)
{
   /* Set wdg period */
   WDTCONbits.WDTPS = WdgPeriod;
}

/************************************************************************
* Function:     Mcu_SetWdgSts
* Input:        UINT8 WdgSts (ON or OFF)
* Output:       None
* Author:       F.Ficili
* Description:  Enable/disable internal watchdog.
* Date:         01/01/18
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
* Date:         01/01/18
************************************************************************/
void Mcu_ClearWdg (void)
{
   /* Clear Wdg */
   asm ("CLRWDT");   
}

/************************************************************************
* Function:     Mcu_SetDieTempSts
* Input:        UINT8 DieTempSts
* Output:       None
* Author:       F.Ficili
* Description:  Enable/disable die temprature sensor.
* Date:         01/01/18
************************************************************************/
void Mcu_SetDieTempSts (UINT8 DieTempSts)
{
   /* Enable or disable die temp sensor */
   FVRCONbits.TSEN = DieTempSts;
}

/************************************************************************
* Function:     Mcu_SetDieTempMode
* Input:        UINT8 DieTempMode
* Output:       None
* Author:       F.Ficili
* Description:  Set Die temp mode (High Range or Low Range).
* Date:         01/01/18
************************************************************************/
void Mcu_SetDieTempMode (UINT8 DieTempMode)
{
   /* Set die temp mode */
   FVRCONbits.TSRNG = DieTempMode;
}

/************************************************************************
* Function:     Mcu_Reset
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Reset the MCU.
* Date:         24/04/19
************************************************************************/
void Mcu_Reset (void)
{
   /* Reset with assembly command */
   asm ("RESET");   
}
