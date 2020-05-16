/************************************************************************
*                        	Tmr Driver                                   *
*************************************************************************
* FileName:         tmr_drv.c                                           *
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
#include "tmr_drv.h"

/************************************************************************
* Defines
************************************************************************/
#define TMR_B_PRESCALER_1                                   ((UINT8)(1))
#define TMR_B_PRESCALER_4                                   ((UINT8)(4))
#define TMR_B_PRESCALER_16                                  ((UINT8)(16))

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
void TmrBInit (UINT8 TmrNumb, UINT8 Prescaler, UINT8 Postscaler);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     TmrBInit
* Input:        UINT8 TmrNumb
*               UINT8 Prescaler
*               UINT8 Postscaler                              
* Output:       None
* Author:       F.Ficili
* Description:  Initialize timer B type setting pre and post-scaler.
* Date:         17/04/17
************************************************************************/
void TmrBInit (UINT8 TmrNumb, UINT8 Prescaler, UINT8 Postscaler)
{
   /* Check timer number */
   switch (TmrNumb)
   {
      case TIMER_2:
         /* Prescaler */
         switch (Prescaler)
         {
            case TMR_B_PRESCALER_1:
               /* Set prescaler bits */
               T2CONbits.T2CKPS0 = STD_OFF;
               T2CONbits.T2CKPS1 = STD_OFF;   
               break;
               
            case TMR_B_PRESCALER_4:
               /* Set prescaler bits */
               T2CONbits.T2CKPS0 = STD_ON;
               T2CONbits.T2CKPS1 = STD_OFF;                  
               break;    
               
            case TMR_B_PRESCALER_16:
               T2CONbits.T2CKPS0 = STD_ON;
               T2CONbits.T2CKPS1 = STD_ON;                 
               break;    
               
            default:
               break;
         }
         
         /* Postccaler */
         T2CONbits.T2OUTPS = (Postscaler - 1);         
         break;
         
      case TIMER_4:
         /* Prescaler */
         switch (Prescaler)
         {
            case TMR_B_PRESCALER_1:
               /* Set prescaler bits */
               T4CONbits.T4CKPS0 = STD_OFF;
               T4CONbits.T4CKPS1 = STD_OFF;   
               break;
               
            case TMR_B_PRESCALER_4:
               /* Set prescaler bits */
               T4CONbits.T4CKPS0 = STD_ON;
               T4CONbits.T4CKPS1 = STD_OFF;                  
               break;    
               
            case TMR_B_PRESCALER_16:
               T4CONbits.T4CKPS0 = STD_ON;
               T4CONbits.T4CKPS1 = STD_ON;                 
               break;    
               
            default:
               break;
         }
         
         /* Postccaler */
         T4CONbits.T4OUTPS = (Postscaler - 1);         
         break;           
         
      default:
         break;
   }
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     Tmr_TmrBIrqSet
* Input:        UINT8 TmrNumb
*               UINT8 TmrIrqSts (ON or OFF)                       
* Output:       None
* Author:       F.Ficili
* Description:  Initialize timer B type Interrupts.
* Date:         17/04/17
************************************************************************/
void Tmr_TmrBIrqSet (UINT8 TmrNumb, IntPriorityType Priority, UINT8 TmrIrqSts)
{
   /* Check timer number */
   switch (TmrNumb)
   {
      case TIMER_2:
         /* Clear timer flag */
         PIR1bits.TMR2IF = STD_OFF;
         /* Set priority */
         IPR1bits.TMR2IP = Priority;
         /* Enable peripheral interrupt */
         PIE1bits.TMR2IE = TmrIrqSts;
         break;
         
      case TIMER_4:
         /* Clear timer flag */
         PIR3bits.TMR4IF = STD_OFF;
         /* Set priority */
         IPR3bits.TMR4IP = Priority;
         /* Enable peripheral interrupt */
         PIE3bits.TMR4IE = TmrIrqSts;         
         break;            
         
      default:
         break;
   }   
}

/************************************************************************
* Function:     Tmr_TmrBPrSet
* Input:        UINT8 TmrNumb
*               UINT8 IntFreqHz
*               UINT8 Prescaler
*               UINT8 Postscaler                                
* Output:       CfgStsType
* Author:       F.Ficili
* Description:  Initialize timer B type Period Register with desired
*               interrupt frequency.
* Date:         17/04/17
************************************************************************/
CfgStsType Tmr_TmrBPrSet (UINT8 TmrNumb, UINT16 IntFreqHz, UINT8 Prescaler, UINT8 Postscaler)
{
   CfgStsType TmrBPrCfgSts;
   UINT32 Divisor = 0;

   /* Calculate the divisor */
   Divisor = (UINT32)Prescaler*(UINT32)Postscaler*(UINT32)IntFreqHz;
   
   /* Check saturation */
   if ((PER_FREQUENCY_VALUE_HZ/Divisor) <= 255)
   {
      /* Check timer number */
      switch (TmrNumb)
      {
         case TIMER_2:
            /* Set period register for TMR2 */       
            PR2 = PER_FREQUENCY_VALUE_HZ/Divisor;            
            break;

         case TIMER_4:
             /* Set period register for TMR4 */
            PR4 = PER_FREQUENCY_VALUE_HZ/Divisor;
            break;   

         default:
            break;
      }
      
      /* Config status OK */
      TmrBPrCfgSts = CfgOk;
   }
   else
   {
      /* Config status Error */
      TmrBPrCfgSts = CfgError;   
   }
   
   /* Return config status */
   return TmrBPrCfgSts;
}

/************************************************************************
* Function:     Tmr_StartTmr
* Input:        UINT8 TmrNumb                      
* Output:       None
* Author:       F.Ficili
* Description:  Start Timer B type (local).
* Date:         17/04/17
************************************************************************/
void Tmr_StartTmr (UINT8 TmrNumb)
{   
   /* Check timer number */
   switch (TmrNumb)
   {
      case TIMER_2:
         /* Start selected timer */
         T2CONbits.TMR2ON = STD_ON;
         break;
         
      case TIMER_4:
         /* Start selected timer */
         T4CONbits.TMR4ON = STD_ON;     
         break;         
         
      default:
         break;
   }
}

/************************************************************************
* Function:     Tmr_TmrBConfig
* Input:        UINT8 TmrNumb
*               TmrBConfigType *TmrBConfig                       
* Output:       None
* Author:       F.Ficili
* Description:  Configure timer B type (tmr2/tmr4).
* Date:         17/04/17
************************************************************************/
void Tmr_TmrBConfig (UINT8 TmrNumb, TmrBConfigType *TmrBConfig)
{
   /* Set prescaler and postscaler */
   TmrBInit(TmrNumb,TmrBConfig->PreScaler,TmrBConfig->PostScaler);
   /* Configure interrupts */
   if (TmrBConfig->IntEnable == EnableInt)
   {
      /* Enable interrupt */
      Tmr_TmrBIrqSet(TmrNumb,TmrBConfig->IntPriority,STD_ON);
   }
   else
   {
      /* Disnable interrupt */
      Tmr_TmrBIrqSet(TmrNumb,TmrBConfig->IntPriority,STD_OFF);
   }
   /* Set PR register */
   Tmr_TmrBPrSet(TmrNumb,TmrBConfig->IntFreqHz,TmrBConfig->PreScaler,TmrBConfig->PostScaler);
   /* Check start timer request */
   if (TmrBConfig->StartTmrRequest == StartTmrReq)
   {
      /* If requested start timer */
      Tmr_StartTmr(TmrNumb);
   }
   else
   {
      /* Timer still needs to be activated */
   }
}

/************************************************************************
* Function:     Tmr_Tmr2Isr
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  Interrutp Service Routine for TMR2.
* Date:         17/04/17
************************************************************************/
void Tmr_Tmr2Isr (void)
{
   /* Clear timer flag */
   PIR1bits.TMR2IF = STD_OFF;
   
   /* Check if the callback function has been correctly registered */
   if (Tmr2IsrCallback != NULL)
   {
      /* Call callback funtion */
      Tmr2IsrCallback();   
   }
}

/************************************************************************
* Function:     Tmr_RegisterTmr2IsrCallback
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  Register callback function to service the ISR.
* Date:         17/04/17
************************************************************************/
void Tmr_RegisterTmr2IsrCallback (Tmr_CallbackType IsrCallback)
{
   /* Callback registration */
   Tmr2IsrCallback = IsrCallback;
}

/************************************************************************
* Function:     Tmr_Tmr4Isr
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  Interrutp Service Routine for TMR4.
* Date:         17/04/17
************************************************************************/
void Tmr_Tmr4Isr (void)
{
   /* Clear timer flag */
   PIR3bits.TMR4IF = STD_OFF;
   
   /* Check if the callback function has been correctly registered */
   if (Tmr4IsrCallback != NULL)
   {
      /* Call callback funtion */
      Tmr4IsrCallback();   
   }
}

/************************************************************************
* Function:     Tmr_RegisterTmr4IsrCallback
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  Register callback function to service the ISR.
* Date:         17/04/17
************************************************************************/
void Tmr_RegisterTmr4IsrCallback (Tmr_CallbackType IsrCallback)
{
   /* Callback registration */
   Tmr4IsrCallback = IsrCallback;
}

