/************************************************************************
*                        	ADC Driver                              *
*************************************************************************
* FileName:         adc_drv.h                                           *
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
* F.Ficili     23/05/17    1.0          First release                   *
************************************************************************/

#ifndef ADC_DRV_H
#define	ADC_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Ch mode */
#define CH_SEL_DIGITAL                                   ((UINT8)(0)) 
#define CH_SEL_ANALOG                                    ((UINT8)(1))

/* Ch id */
#define CH_AN0                                           ((UINT8)(0)) 
#define CH_AN1                                           ((UINT8)(1)) 
#define CH_AN2                                           ((UINT8)(2)) 
#define CH_AN3                                           ((UINT8)(3)) 
#define CH_AN4                                           ((UINT8)(4)) 
#define CH_AN5                                           ((UINT8)(5)) 
#define CH_AN6                                           ((UINT8)(6)) 
#define CH_AN7                                           ((UINT8)(7)) 
#define CH_AN8                                           ((UINT8)(8)) 
#define CH_AN9                                           ((UINT8)(9)) 
#define CH_AN10                                          ((UINT8)(10)) 
#define CH_AN11                                          ((UINT8)(11))

#define TEMP_SENS                                        ((UINT8)(29))

/* Justification */
#define ADC_LEFT_JUST                                    ((UINT8)(0)) 
#define ADC_RIGHT_JUST                                   ((UINT8)(1))

/* Adc conversion clock */
#define ADC_FOSC_2                                       ((UINT8)(0)) 
#define ADC_FOSC_8                                       ((UINT8)(1)) 
#define ADC_FOSC_32                                      ((UINT8)(2)) 
#define ADC_FRC                                          ((UINT8)(3)) 
#define ADC_FOSC_4                                       ((UINT8)(4)) 
#define ADC_FOSC_16                                      ((UINT8)(5)) 
#define ADC_FOSC_64                                      ((UINT8)(6)) 

/* Adc negative reference voltage  */
#define ADC_N_VREF_VSS                                   ((UINT8)(0)) 
#define ADC_N_VREF_EXT_PIN                               ((UINT8)(1))

/* Adc positive reference voltage  */
#define ADC_P_VREF_VDD                                   ((UINT8)(0)) 
#define ADC_P_VREF_EXT_PIN                               ((UINT8)(2))
#define ADC_P_VREF_FVR                                   ((UINT8)(3))

/* Conv status */
#define ADC_CONV_IN_PROGRESS                             ((UINT8)(0)) 
#define ADC_START_CONV                                   ((UINT8)(1))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Conversion status */
typedef enum _AdcConvStsType
{
   AdcConvInProgress,
   AdcConvCompleted
} AdcConvStsType;

/* Analog channle selection type */
typedef struct _AnalogChSelType
{
   UINT8 AnSelA0An0;
   UINT8 AnSelA1An1;
   UINT8 AnSelA2An2;
   UINT8 AnSelA4An3;
#if defined(__16F1829)   
   UINT8 AnSelB4An10;
   UINT8 AnSelB5An11;
#endif
   UINT8 AnSelC0An4;
   UINT8 AnSelC1An5;
   UINT8 AnSelC2An6;
   UINT8 AnSelC3An7;
#if defined(__16F1829)   
   UINT8 AnSelC6An8;
   UINT8 AnSelC7An9;
#endif   
} AnalogChSelType;

/* Adc config type */
typedef struct _AdcCfgType
{
   UINT8 AdcEnable;
   UINT8 AdcResultFormat;
   UINT8 AdcConvClock;
   UINT8 AdcNegRef;
   UINT8 AdcPosRef;
   UINT8 AdcIntEnabled;
} AdcConfigType;

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
void Adc_ConfigAnCh (AnalogChSelType* AnChSel);
void Adc_InitAdcModule (AdcConfigType* AdcConfig);
void Adc_SelectChannel (UINT8 SelCh);
void Adc_StartConversion (void);
AdcConvStsType Adc_GetConvSts (void);
UINT16 Adc_GetConvResult (void);

#endif	/* ADC_DRV_H */

