/************************************************************************
*                          Interrupts                              	*
*************************************************************************
* FileName:         irq.c                                               *
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
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"
#include "os_sched.h"

/************************************************************************
* Defines
************************************************************************/
#define INT_NOT_PENDING                               ((UINT8)(0))
#define INT_PENDING                                   ((UINT8)(1))

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
* Function:     Irq_InitIntController
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Configure interrupt controller priority mode and global
*               interrupt enable/disable.
* Date:         17/04/17
************************************************************************/
void Irq_InitIntController (IntPriorityEnType IntPriorityEn, UINT8 GlobalInt_HighPInt, UINT8 PerInt_LowPInt)
{
   /* Set IPEN: legacy or priority mode */
   RCONbits.IPEN = IntPriorityEn;
   /* GIE_GIEH: global interrupts or high priority interrupts enable/disable */
   INTCONbits.GIE_GIEH = GlobalInt_HighPInt;
   /* PEIE_GIEL: peripheral interrupts or low priority interrupts enable/disable */
   INTCONbits.PEIE_GIEL = PerInt_LowPInt;   
}

/************************************************************************
* Function:     Irq_SetGieGieh
* Input:        None                   
* Output:       None
* Author:       F.Ficili
* Description:  Enable all/high priority (GIE/GIEH bits).
* Date:         23/04/17
************************************************************************/
void Irq_SetGieGieh(void)
{
   /* Enable all interrupts */
   INTCONbits.GIE_GIEH = STD_ON;
}

/************************************************************************
* Function:     Irq_ClearGieGieh
* Input:        None                   
* Output:       None
* Author:       F.Ficili
* Description:  Disable all/high priority interrupts (GIE/GIEH bits).
* Date:         23/04/17
************************************************************************/
void Irq_ClearGieGieh (void)
{
   /* Disable all interrupts */
   INTCONbits.GIE_GIEH = STD_OFF;   
}

/************************************************************************
* Function:     Irq_SetPeieGiel
* Input:        None                   
* Output:       None
* Author:       F.Ficili
* Description:  Enable peripheral/low priority interrupts (PEIE/GIEL bits).
* Date:         23/04/17
************************************************************************/
void Irq_SetPeieGiel (void)
{
   /* Enable all interrupts */
   INTCONbits.PEIE_GIEL = STD_ON;
}

/************************************************************************
* Function:     Irq_SetPeieGiel
* Input:        None                   
* Output:       None
* Author:       F.Ficili
* Description:  Disable peripheral/low priority interrupts (PEIE/GIEL bits).
* Date:         23/04/17
************************************************************************/
void Irq_ClearPeieGiel (void)
{
   /* Disable all interrupts */
   INTCONbits.PEIE_GIEL = STD_OFF;   
}

/************************************************************************
* Interrupts Routines
************************************************************************/
#if (COMPILER_USED == XC8_COMPILER)
/************************************************************************
* Function:     HighPriorityIsr
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  High priority ISR handler.
* Date:         17/04/17
************************************************************************/
void high_priority interrupt HighPriorityIsr (void)
{
#if (USE_INT_0_ISR == STD_ON)
   /* INT0 Interrupt */
   if (INTCONbits.INT0IF == INT_PENDING)
   {       
      /* Call INT0 ISR */
      Mcu_Int0Isr();
   }
#endif  
   
#if ((USE_INT_1_ISR == STD_ON) && (INT_1_ISR_PRIORITY == HIGH_PRIORITY))
   /* INT1 Interrupt */
   if (INTCON3bits.INT1IF == INT_PENDING)
   {       
      /* Call INT1 ISR */
      Mcu_Int1Isr();
   }
#endif        
   
#if ((USE_RB_CHANGE_ISR == STD_ON) && (RB_CHANGE_ISR_PRIORITY == HIGH_PRIORITY))
   /* RB Change Interrupt */
   if (INTCONbits.RBIF == INT_PENDING)
   {       
      /* Call RB change ISR */
      Irq_RbChangeIsr();
   }
#endif   
   
#if ((USE_TMR2_ISR == STD_ON) && (TMR2_ISR_PRIORITY == HIGH_PRIORITY))
   /* TMR2 Interrupt */
   if (PIR1bits.TMR2IF == INT_PENDING)
   {       
      /* Call Tmr2 ISR */
      Tmr_Tmr2Isr();
   }
#endif
   
#if ((USE_TMR4_ISR == STD_ON) && (TMR4_ISR_PRIORITY == HIGH_PRIORITY))
   /* TMR4 Interrupt */
   if (PIR3bits.TMR4IF == INT_PENDING)
   {       
      /* Call Tmr4 ISR */
      Tmr_Tmr4Isr();
   }
#endif

#if ((USE_RTCC_ISR == STD_ON) && (RTCC_ISR_PRIORITY == HIGH_PRIORITY))
   /* RTCC Interrupt */
   if (PIR3bits.RTCCIF == INT_PENDING)
   {       
      /* Call RTCC ISR */
      Rtcc_RtcIsr();
   }
#endif       
    
#if ((USE_UART_1_RX_ISR == STD_ON) && (UART_1_RX_ISR_PRIORITY == HIGH_PRIORITY))   
   /* UART1 RX Interrupt */
   if (PIR1bits.RC1IF == INT_PENDING)
   {
      /* Call UART1 Rx ISR */
      Uart_Uart1RxIsr();
   }
#endif   
   
#if ((USE_UART_2_RX_ISR == STD_ON) && (UART_2_RX_ISR_PRIORITY == HIGH_PRIORITY))   
   /* UART1 RX Interrupt */
   if (PIR3bits.RC2IF == INT_PENDING)
   {
      /* Call UART2 Rx ISR */
      Uart_Uart2RxIsr();
   }
#endif      
}

/************************************************************************
* Function:     LowPriorityIsr
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Low priority ISR.
* Date:         13/09/16
************************************************************************/
void low_priority interrupt LowPriorityIsr (void)
{
#if ((USE_INT_1_ISR == STD_ON) && (INT_1_ISR_PRIORITY == LOW_PRIORITY))
   /* INT1 Interrupt */
   if (INTCON3bits.INT1IF == INT_PENDING)
   {       
      /* Call INT1 ISR */
      Irq_Int1Isr();
   }
#endif      
   
#if ((USE_RB_CHANGE_ISR == STD_ON) && (RB_CHANGE_ISR_PRIORITY == LOW_PRIORITY))
   /* RB Change Interrupt */
   if (INTCONbits.RBIF == INT_PENDING)
   {       
      /* Call RB change ISR */
      Irq_RbChangeIsr();
   }
#endif  
   
#if ((USE_TMR2_ISR == STD_ON) && (TMR2_ISR_PRIORITY == LOW_PRIORITY))
   /* TMR2 Interrupt */
   if (PIR1bits.TMR2IF == INT_PENDING)
   {       
      /* Call Tmr2 ISR */
      Tmr_Tmr2Isr();
   }
#endif
   
#if ((USE_TMR4_ISR == STD_ON) && (TMR4_ISR_PRIORITY == LOW_PRIORITY))
   /* TMR4 Interrupt */
   if (PIR3bits.TMR4IF == INT_PENDING)
   {       
      /* Call Tmr4 ISR */
      Tmr_Tmr4Isr();
   }
#endif
   
#if ((USE_RTCC_ISR == STD_ON) && (RTCC_ISR_PRIORITY == LOW_PRIORITY))
   /* RTCC Interrupt */
   if (PIR3bits.RTCCIF == INT_PENDING)
   {       
      /* Call RTCC ISR */
      Rtcc_RtcIsr();
   }
#endif        
   
#if ((USE_UART_1_RX_ISR == STD_ON) && (UART_1_RX_ISR_PRIORITY == LOW_PRIORITY))   
   /* UART1 RX Interrupt */
   if (PIR1bits.RC1IF == INT_PENDING)
   {
      /* Call UART1 Rx ISR */
      Uart_Uart1RxIsr();
   }
#endif   

#if ((USE_UART_2_RX_ISR == STD_ON) && (UART_2_RX_ISR_PRIORITY == LOW_PRIORITY))   
   /* UART1 RX Interrupt */
   if (PIR3bits.RC2IF == INT_PENDING)
   {
      /* Call UART2 Rx ISR */
      Uart_Uart2RxIsr();
   }
#endif   
   
}
#endif