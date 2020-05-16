/************************************************************************
*                        	ADC Driver                                  *
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

/************************************************************************
* Includes
************************************************************************/
#include "adc_drv.h"

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
* Function:     Adc_ConfigAnCh
* Input:        AnalogChSelType* AnChSel
* Output:       None
* Author:       F.Ficili
* Description:	Configure the analog channels.
* Date:         23/05/17
************************************************************************/
void Adc_ConfigAnCh (AnalogChSelType* AnChSel)
{
   /* ANSELA */
   ANSELA = 0x00;
   ANSELAbits.ANSA0 = AnChSel->AnSelA0An0;
   ANSELAbits.ANSA1 = AnChSel->AnSelA1An1;
   ANSELAbits.ANSA2 = AnChSel->AnSelA2An2;
   ANSELAbits.ANSA4 = AnChSel->AnSelA4An3;
   /* ANSELB */
#if defined(__16F1829)
   ANSELB = 0x00;
   ANSELBbits.ANSB4 = AnChSel->AnSelB4An10;
   ANSELBbits.ANSB5 = AnChSel->AnSelB5An11;
#endif
   /* ANSELC */
   ANSELC = 0x00;
   ANSELCbits.ANSC0 = AnChSel->AnSelC0An4; 
   ANSELCbits.ANSC1 = AnChSel->AnSelC1An5;   
   ANSELCbits.ANSC2 = AnChSel->AnSelC2An6;   
   ANSELCbits.ANSC3 = AnChSel->AnSelC3An7; 
#if defined(__16F1829)
   ANSELCbits.ANSC6 = AnChSel->AnSelC6An8;
   ANSELCbits.ANSC7 = AnChSel->AnSelC7An9;     
#endif
}

/************************************************************************
* Function:     Adc_InitAdcModule
* Input:        AdcConfigType* AdcConfig
* Output:       None
* Author:       F.Ficili
* Description:	Initialize ADC module.
* Date:         23/05/17
************************************************************************/
void Adc_InitAdcModule (AdcConfigType* AdcConfig)
{
   /* Enable ADC */
   ADCON0bits.ADON = AdcConfig->AdcEnable;
   /* Result format */
   ADCON1bits.ADFM = AdcConfig->AdcResultFormat;
   /* Conversion clock */
   ADCON1bits.ADCS0 = AdcConfig->AdcConvClock;
   /* Negative and positive reference */
   ADCON1bits.ADNREF = AdcConfig->AdcNegRef;
   ADCON1bits.ADPREF = AdcConfig->AdcPosRef;
   /* Interrupt enable */
   PIE1bits.ADIE = AdcConfig->AdcIntEnabled;   
}

/************************************************************************
* Function:     Adc_SelectChannel
* Input:        UINT8 SelCh
* Output:       None
* Author:       F.Ficili
* Description:	Select channle to convert.
* Date:         23/05/17
************************************************************************/
void Adc_SelectChannel (UINT8 SelCh)
{
   /* Select channel to convert */
   ADCON0bits.CHS = SelCh;
}

/************************************************************************
* Function:     Adc_StartConversion
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	Start ADC conversion.
* Date:         23/05/17
************************************************************************/
void Adc_StartConversion (void)
{
   /* Start adc conversion */
   ADCON0bits.GO_nDONE = ADC_START_CONV;
}  

/************************************************************************
* Function:     Adc_WaitConvComplete
* Input:        None
* Output:       AdcConvStsType
* Author:       F.Ficili
* Description:	Wait until conversion is completed.
* Date:         23/05/17
************************************************************************/
AdcConvStsType Adc_GetConvSts (void)
{
   /* Return conv status */
   return ((AdcConvStsType)(ADCON0bits.GO_nDONE));
}

/************************************************************************
* Function:     Adc_StartConversion
* Input:        None
* Output:       UINT16
* Author:       F.Ficili
* Description:	Get result of ADC conversion
* Date:         23/05/17
************************************************************************/
UINT16 Adc_GetConvResult (void)
{
   UINT16 AdcResult;
   
   /* Build result in a 16-bit wide variable...  */
   AdcResult = (((ADRESH & 0x03) << 8) | ADRESL);   
   /* ... and return */
   return AdcResult;
}