/************************************************************************
*                        	Interrupts                              *
*************************************************************************
* FileName:         irq.h                                               *
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

#ifndef IRQ_H
#define	IRQ_H

/************************************************************************
* Includes
************************************************************************/


/************************************************************************
* EXPORTED Defines
************************************************************************/
/* INT0 ISR compile time configuration (only high priority interrupt) */
#define USE_INT_0_ISR                                 STD_ON
/* INT1 ISR compile time configuration */
#define USE_INT_1_ISR                                 STD_ON
#define INT_1_ISR_PRIORITY                            HIGH_PRIORITY
/* RB Change ISR compile time configuration */
#define USE_RB_CHANGE_ISR                             STD_OFF
#define RB_CHANGE_ISR_PRIORITY                        HIGH_PRIORITY
/* TMR2 ISR compile time configuration */
#define USE_TMR2_ISR                                  STD_ON
#define TMR2_ISR_PRIORITY                             LOW_PRIORITY
/* TMR4 ISR compile time configuration */
#define USE_TMR4_ISR                                  STD_OFF
#define TMR4_ISR_PRIORITY                             HIGH_PRIORITY
/* RTCC ISR compile time configuration */
#define USE_RTCC_ISR                                  STD_ON
#define RTCC_ISR_PRIORITY                             HIGH_PRIORITY
/* UART1 RX ISR compile time configuration */
#define USE_UART_1_RX_ISR                             STD_ON
#define UART_1_RX_ISR_PRIORITY                        HIGH_PRIORITY
/* UART2 RX ISR compile time configuration */
#define USE_UART_2_RX_ISR                             STD_OFF
#define UART_2_RX_ISR_PRIORITY                        HIGH_PRIORITY

/************************************************************************
* EXPORTED Macros
************************************************************************/
/* Macros for General/High Priority Int Enable/Disable */
#define ENABLE_ALL_INT()                              Irq_SetGieGieh();
#define ENABLE_HIGH_PRIORITY_INT()                    Irq_SetGieGieh();
#define DISABLE_ALL_INT()                             Irq_ClearGieGieh();
#define DISABLE_HIGH_PRIORITY_INT()                   Irq_ClearGieGieh();
/* Macros for Peripheral/Low Priority Int Enable/Disable */
#define ENABLE_PERIPHERAL_INT()                       Irq_SetPeieGiel();
#define ENABLE_LOW_PRIORITY_INT()                     Irq_SetPeieGiel();
#define DISABLE_PERIPHERAL_INT()                      Irq_ClearPeieGiel();
#define DISABLE_LOW_PRIORITY_INT()                    Irq_ClearPeieGiel();

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Interrupt enable typedef */
typedef enum _IntEnableType
{
   DisableInt = 0,
   EnableInt = 1           
} IntEnableType;

/* Interrupt priority typedef */
typedef enum _IntPriorityType
{
   IntLowPriority = 0,
   IntHighPriority = 1           
} IntPriorityType;

/* Int controller priority */
typedef enum _IntPriorityEnType
{
   IntLegacyMode = 0,
   IntPriorityMode = 1   
} IntPriorityEnType;

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Initialize interrupt controller */
void Irq_InitIntController (IntPriorityEnType IntPriorityEn, UINT8 GlobalInt_HighPInt, UINT8 PerInt_LowPInt);
/* Enable all/high priority interrupts (GIE/GIEH bits) */
void Irq_SetGieGieh(void);
/* Disable all/high priority interrupts (GIE/GIEH bits) */
void Irq_ClearGieGieh (void);
/* Enable peripheral/low priority interrupts (PEIE/GIEL bits) */
void Irq_SetPeieGiel (void);
/* Disable peripheral/low priority interrupts (PEIE/GIEL bits) */
void Irq_ClearPeieGiel (void);

#endif	/* IRQ_H */

