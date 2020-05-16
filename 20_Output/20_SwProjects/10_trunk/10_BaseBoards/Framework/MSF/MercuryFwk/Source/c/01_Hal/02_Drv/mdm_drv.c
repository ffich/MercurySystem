/************************************************************************
*                          Modem GSM GPRS Driver                        *
*************************************************************************
* FileName:         mdm_drv.c                                           *
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
* F.Ficili     11/09/16    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "mdm_drv.h"

#if (MODEM_USED == GSM_GPRS_MDM)
/************************************************************************
* Defines
************************************************************************/
/* SMS RI pulse ms */
#define SMS_DELAY_MS                                     (((UINT16)(1200))/MDM_DRIVER_PERIOD_MS)
/* SMS RI pulse tolerance in ms */
#define SMS_DELAY_TOLL_MS                                (((UINT16)(100))/MDM_DRIVER_PERIOD_MS)
/* ON/OFF pulse ms */
#define ON_OFF_DELAY_MS                                  (((UINT16)(1500))/MDM_DRIVER_PERIOD_MS)
/* RESET pulse ms */
#define RESET_DELAY_MS                                   (((UINT16)(100))/MDM_DRIVER_PERIOD_MS)

/************************************************************************
* Typedefs
************************************************************************/
typedef enum _MdmDrvState
{
   WaitActivation,
   CheckRi,
   SmsReceptioDelay,
   CheckEventType,
   WaitCallEnd,           
} MdmDrvStateType;

typedef enum _MdmCntState
{
   InitModemInt,
   CheckRequest,
   MdmOnOffPulse,
   MdmResetPulse,
} MdmCntStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/


/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Global RI status */
MdmRiType Mmd_RingIndicator = NoEvents;
/* Events */
EventStructureType ActivateRiDetection = {EventIdle};
EventStructureType NewSmsEvent = {EventIdle};
EventStructureType IncomingCallEvent = {EventIdle};
EventStructureType CallClosedEvent = {EventIdle};
EventStructureType MdmOnOffPulseRequest = {EventIdle};
EventStructureType MdmResetRequest = {EventIdle};

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
void InitModemInterface (void);
void RiSm (void);
void MdmCntSm (void);


/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     InitModemInterface
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Initialize modem inetrface.
* Date:         11/09/16
************************************************************************/
void InitModemInterface (void)
{
   /* Init Reset, On and RI pins */
   BSP_MDM_RESET_TRIS = LINE_DIRECTION_OUTPUT;
   BSP_MDM_ON_OFF_TRIS = LINE_DIRECTION_OUTPUT;
   BSP_MDM_RI_TRIS = LINE_DIRECTION_INPUT;
}

/************************************************************************
* Function:     RiSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Process the ring indicator signal.
* Date:         16/09/16
************************************************************************/
void RiSm (void)
{
   static UINT16 Counter = 0;
   static MdmDrvStateType MdmDrvState = WaitActivation;
   
   switch (MdmDrvState)
   {
      case WaitActivation:
         /* Check activation */
         if (ReceiveEvt(&ActivateRiDetection))
         {
            /* Switch state */
            MdmDrvState = CheckRi;            
         }
         break;
      case CheckRi:
         /* Check Line status */
         if (BSP_MDM_RI == LINE_STATE_LOW)
         {
            /* Switch state */
            MdmDrvState = SmsReceptioDelay;
         }                  
         break;
         
      case SmsReceptioDelay:
         /* Increment counter */
         Counter++;
         /* Wait delay for SMS */
         if (Counter >= (SMS_DELAY_MS + SMS_DELAY_TOLL_MS))
         {
            /* Switch state */
            MdmDrvState = CheckEventType;            
         }
         break;
         
      case CheckEventType:
         /* Check if is a call or an SMS */
         if (BSP_MDM_RI == LINE_STATE_HIGH)
         {
            /* SMS */
            Mmd_RingIndicator = IncomingSms;
            /* Send SMS event */
            GenerateEvt(&NewSmsEvent);            
            /* Go back to CheckRi state */
            MdmDrvState = CheckRi;                    
         }
         else
         {
            /* Call */
            Mmd_RingIndicator = IncomingCall;
            /* Send Call event */
            GenerateEvt(&IncomingCallEvent);            
            /* Wait for end of the call in WaitCallEnd state */
            MdmDrvState = WaitCallEnd;                 
         }         
         break;
         
      case WaitCallEnd:
         /* Wait call end */
         if (BSP_MDM_RI == LINE_STATE_HIGH)
         {
            /* Send Call event */
            GenerateEvt(&CallClosedEvent);               
            /* Go back to CheckRi state */
            MdmDrvState = CheckRi;              
         }
         break;
         
      default:
         break;
   } 
}

/************************************************************************
* Function:     MdmOnSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Turn on and off the module.
* Date:         17/09/16
************************************************************************/
void MdmCntSm (void)
{
   static MdmCntStateType MdmCntState = InitModemInt;
   static UINT16 Counter = 0;
   
   switch (MdmCntState)
   {
      case InitModemInt:
         /* Modem HW initialization */
         InitModemInterface();     
         /* Switch state */
         MdmCntState = CheckRequest;
         break;
         
      case CheckRequest:
         /* Check for incoming reuest */
         if (ReceiveEvt(&MdmOnOffPulseRequest))
         {
            /* Switch state */
            MdmCntState = MdmOnOffPulse;
            /* Set OnOff line status high */
            BSP_MDM_ON_OFF = LINE_STATE_HIGH;
            /* Reset counter */
            Counter = 0;
         }
         else if (ReceiveEvt(&MdmResetRequest))
         {
            /* Switch state */
            MdmCntState = MdmResetPulse;
            /* Set OnOff line status high */
            BSP_MDM_RESET = LINE_STATE_HIGH;
            /* Reset counter */
            Counter = 0;            
         }
         else
         {
            /* Do nothing */
         }
         break;

      case MdmOnOffPulse:
         /* Increment counter */
         Counter++;
         /* Check counter */        
         if (Counter >= ON_OFF_DELAY_MS)
         {  
            /* Set OnOff line status low */
            BSP_MDM_ON_OFF = LINE_STATE_LOW;
            /* Switch state */
            MdmCntState = CheckRequest;
         }
         break;

      case MdmResetPulse:
         /* Increment counter */
         Counter++;
         /* Check counter */        
         if (Counter >= RESET_DELAY_MS)
         {  
            /* Set OnOff line status low */
            BSP_MDM_RESET = LINE_STATE_LOW;
            /* Switch state */
            MdmCntState = CheckRequest;
         }         
         break;

      default:
         break;         
   }
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     Mdm_GetModemRiSts
* Input:        None
* Output:       MdmRiType
* Author:       F.Ficili
* Description:  Get the Ring Indicator (RI) line status.
* Date:         11/09/16
************************************************************************/
MdmRiType Mdm_GetModemRiSts (void)
{
   /* Service */
   MdmRiType ReturnState;
   
   /* Get return state */
   ReturnState = Mmd_RingIndicator;   
   /* Clear global variable */
   Mmd_RingIndicator = NoEvents;    
   
   /* Return ring indicator status */
   return ReturnState;
}

/************************************************************************
* Function:     Mdm_DrvMainFn
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Modem driver periodic function.
* Date:         11/09/16
************************************************************************/
void Mdm_DrvMainFn (UINT8 Options)
{
   /* Call Modem On/Off State Machine */
   MdmCntSm();
}

#endif /* MODEM_USED == GSM_GPRS_MDM */