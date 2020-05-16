/************************************************************************
*                               Mcu Driver                              *
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
* F.Ficili     01/01/18    1.0          First release                   *
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
/* Indivisual channels PU enable/disable */
#define CH_PU_DISABLED                                   ((UINT8)(0)) 
#define CH_PU_ENABLED                                    ((UINT8)(1))

#define DIE_TEMP_SENS_LOW_RANGE                          ((UINT8)(0)) 
#define DIE_TEMP_SENS_HIGH_RANGE                         ((UINT8)(1)) 

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Pu sts type */
typedef enum _GlobalPuStsType
{
    Mcu_GlobalPuEnabled = 0,
    Mcu_GlobalPuDisabled = 1,            
} GlobalPuStsType;

typedef struct _WpuChType
{
    /* Port A */
    UINT8 WpuA0;
    UINT8 WpuA1;
    UINT8 WpuA2;
    UINT8 WpuA3;
    UINT8 WpuA4;
    UINT8 WpuA5;
    /* Port B */
#if defined(__16F1829)
    UINT8 WpuB4;
    UINT8 WpuB5;
    UINT8 WpuB6;
    UINT8 WpuB7;   
#endif
    /* Port C */
    UINT8 WpuC0;
    UINT8 WpuC1;
    UINT8 WpuC2;
    UINT8 WpuC3;
    UINT8 WpuC4;
    UINT8 WpuC5;
#if defined(__16F1829)    
    UINT8 WpuC6;
    UINT8 WpuC7;    
#endif        
} WpuChType;


/* Wdg period type */
typedef enum _WdgPeriodType
{
   WdgPer_1ms = 0,
   WdgPer_2ms,
   WdgPer_4ms,
   WdgPer_8ms,
   WdgPer_16ms,
   WdgPer_32ms,
   WdgPer_64ms,
   WdgPer_128ms,
   WdgPer_256ms,
   WdgPer_512ms,
   WdgPer_1s,
   WdgPer_2s,
   WdgPer_4s,
   WdgPer_8s,
   WdgPer_16s,
   WdgPer_32s,
   WdgPer_64s,
   WdgPer_128s,
   WdgPer_256s,                      
} WdgPeriodType;

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
void Mcu_InitIntOsc (void);
void Mcu_EnableInterrupts (void);
void Mcu_EnableSchedulerInt (void);
void Mcu_DisableInterrupts (void);
void Mcu_DisableSchedulerInt (void);
void Mcu_StartSystemPll (void);
void Mcu_SetPullUpsSts (GlobalPuStsType PuSts);
void Mcu_ConfigPullUps (WpuChType *WpuCh);
void Mcu_SetWdgPeriod (WdgPeriodType WdgPeriod);
void Mcu_SetWdgSts (UINT8 WdgSts);
void Mcu_ClearWdg (void);
void Mcu_SetDieTempSts (UINT8 DieTempSts);
void Mcu_SetDieTempMode (UINT8 DieTempMode);
void Mcu_Reset (void);

#endif	/* MCU_DRV_H */

