/************************************************************************
*                        	MCU Driver                              *
*************************************************************************
* FileName:         mcu_drv.h                                           *
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

#ifndef MCU_DRV_H
#define	MCU_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/


/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Timer B Config type */
typedef struct _McuConfigType
{
   IntPriorityEnType IntPriorityEn;
   UINT8 GieGiehEnable;
   UINT8 PeieGielEnable;
} McuConfigType;

/* Callback typedef */
typedef void (*ExtInt_CallbackType) (void);

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* ExtInt ISR callback pointer */
extern ExtInt_CallbackType ExtInt0IsrCallback;
extern ExtInt_CallbackType ExtInt1IsrCallback;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Initialize system PLL */
void Mcu_StartSystemPll (void);
/* External interrutps */
void Mcu_ConfigExtInt(void);
/* Enable/disable internal watchdog */
void Mcu_SetWdgSts (UINT8 WdgSts);
/* Clear internal watchdog */
void Mcu_ClearWdg (void);

/* -- ISRs ----------------------------------------------------------- */
/* Interrutp Service Routine for ExtInt0 */
void Mcu_Int0Isr (void);
void Mcu_Int1Isr (void);

/* -- Callbacks Registrators ------------------------------------------ */
/* Callback registratio for ISR */
void Mcu_RegisterInt0IsrCallback (ExtInt_CallbackType IsrCallback);
void Mcu_RegisterInt1IsrCallback (ExtInt_CallbackType IsrCallback);
#endif	/* MCU_DRV_H */

