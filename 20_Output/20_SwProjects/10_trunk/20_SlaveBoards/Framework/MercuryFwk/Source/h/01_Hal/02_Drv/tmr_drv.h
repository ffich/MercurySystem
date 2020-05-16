/************************************************************************
*                        	Tmr Driver                                  *
*************************************************************************
* FileName:         tmr_drv.h                                           *
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

#ifndef TMR_DRV_H
#define	TMR_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
#define TIMER_0                                             ((UINT8)(0))
#define TIMER_1                                             ((UINT8)(1))
#define TIMER_2                                             ((UINT8)(2))
#define TIMER_4                                             ((UINT8)(4))
#define TIMER_6                                             ((UINT8)(6))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Start timer typedef */
typedef enum _StartTmrType
{
   StartTmrNotReq = 0,
   StartTmrReq = 1           
} StartTmrType;

/* Interrupt enable typedef */
typedef enum _IntEnableType
{
   DisableInt = 0,
   EnableInt = 1           
} IntEnableType;

/* Timer B Config type */
typedef struct _TmrBConfigType
{
   UINT8 PreScaler;
   UINT8 PostScaler;
   IntEnableType IntEnable;
   UINT16 IntFreqHz;
   StartTmrType StartTmrRequest;
} TmrBConfigType;

/* Callback typedef */
typedef void (*Tmr_CallbackType) (void);

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Tmr2 ISR callback pointer */
Tmr_CallbackType Tmr2IsrCallback = NULL;
/* Tmr4 ISR callback pointer */
Tmr_CallbackType Tmr4IsrCallback = NULL;
/* Tmr6 ISR callback pointer */
Tmr_CallbackType Tmr6IsrCallback = NULL;

/************************************************************************
* EXPORTED Functions
************************************************************************/

/* -- APIs ----------------------------------------------------------- */
/* Set Tmr interrupt */
void Tmr_TmrBIrqSet (UINT8 TmrNumb, UINT8 TmrIrqSts);
/* Set timer period match register */
CfgStsType Tmr_TmrBPrSet (UINT8 TmrNumb, UINT16 IntFreqHz, UINT8 Prescaler, UINT8 Postscaler);
/* Start timer B type API */
void Tmr_StartTmr (UINT8 TmrNumb);
/* Configure timer B type API */
void Tmr_TmrBConfig (UINT8 TmrNumb, TmrBConfigType *TmrBConfig);

#ifdef USE_TMR_CALLBACK_FOR_ISR
/* -- ISRs ----------------------------------------------------------- */
/* Tmr2 Interrupt Service Routine */
void Tmr_Tmr2Isr (void);
/* Tmr4 Interrupt Service Routine */
void Tmr_Tmr4Isr (void);
/* Tmr6 Interrupt Service Routine */
void Tmr_Tmr6Isr (void);

/* -- Callbacks Registrators ------------------------------------------ */
/* Tmr2 ISR callback registration */
void Tmr_RegisterTmr2IsrCallback (Tmr_CallbackType IsrCallback);
/* Tmr4 ISR callback registration */
void Tmr_RegisterTmr4IsrCallback (Tmr_CallbackType IsrCallback);
/* Tmr6 ISR callback registration */
void Tmr_RegisterTmr6IsrCallback (Tmr_CallbackType IsrCallback);
#endif

#endif	/* TMR_DRV_H */

