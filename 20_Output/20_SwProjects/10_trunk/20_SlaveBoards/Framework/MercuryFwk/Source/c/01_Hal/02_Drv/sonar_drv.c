/************************************************************************
*                          Sonar Driver                                 *
*************************************************************************
* FileName:         sonar_drv.c                                         *
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
* F.Ficili     29/09/16    1.0          First release                   *
************************************************************************/


/************************************************************************
* Includes
************************************************************************/
#include "sonar_drv.h"

#ifdef USE_SB310

/************************************************************************
* Defines
************************************************************************/
/* Sonar measure sm timeout */
#define TIMEOUT_MS                            ((UINT16)(1000)/SNR_DRV_PERIOD_MS)

#define SCALE_FACTOR                          1u  

/************************************************************************
* Typedefs
************************************************************************/
/* Detect echo state type */
typedef enum _DetectEchoState
{
   WaitEchoRisEdge,
   MeasureEchoPulse
} DetectEchoStateType;

/* Sonar measure state type */
typedef enum _SonarMeasState
{
   Snr_InitSonar,
   Snr_WaitRequest,
   Snr_SetTrigger,
   Snr_ClearTrigger,           
   Snr_GetResult,
} SonarMeasStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* Echo pulse count value */
static UINT16 EchoPulseCount = 0;
/* Start Measure event */
EventStructureType SonarStartMeasure = {0, EventIdle};
/* Echo count ready event */
EventStructureType SonarEchoCountReady = {0, EventIdle};
/* Measure complete event */
EventStructureType SonarMeasureComplete = {0, EventIdle};

/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Measured distance */
UINT16 DistanceCm = 0;

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
void SnrInitSonar (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/
/************************************************************************
* Function:     SnrInitSonar
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Initialize sonar interface.
* Date:         29/09/16
************************************************************************/
void SnrInitSonar (void)
{
   /* Set trigger as output and echo as input */
   BSP_TRIGGER_TRIS = LINE_DIRECTION_OUTPUT;
   BSP_ECHO_TRIS = LINE_DIRECTION_INPUT;
   
   BSP_TRIGGER = LINE_STATE_LOW;
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/
/************************************************************************
* Function:     SnrDrv_GetDistSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Trigger a distance measure and get result.
* Date:         29/09/16
************************************************************************/
void SnrDrv_GetDistSm(UINT8 Options)
{
   static SonarMeasStateType SonarMeasState = Snr_InitSonar;
   static UINT16 Counter = 0;
   
   switch (SonarMeasState)
   {
      case Snr_InitSonar:
         /* Init sonmar interface */
         SnrInitSonar();
         /* Switch state */
         SonarMeasState = Snr_WaitRequest;                  
         break;
         
      case Snr_WaitRequest:
         /* If start measure evt is received */
         if (ReceiveEvt(&SonarStartMeasure))
         {
            /* Pre-reset counter */
            Counter = 0;
            /* Switch state */
            SonarMeasState = Snr_SetTrigger;
         }
         break;
         
      case Snr_SetTrigger:
         /* Set trigger */
         BSP_TRIGGER = LINE_STATE_HIGH;           
         /* Switch state */
         SonarMeasState = Snr_ClearTrigger;       
         break;
         
      case Snr_ClearTrigger:
         /* Set trigger */
         BSP_TRIGGER = LINE_STATE_LOW;        
         /* Switch state */
         SonarMeasState = Snr_GetResult;
         break;

      case Snr_GetResult:
         /* Increment Timeout counter */
         Counter++;
         /* If result is ready */
         if (ReceiveEvt(&SonarEchoCountReady))
         {
            /* Set distance value */
            DistanceCm = (SCALE_FACTOR * EchoPulseCount);
            /* Send measure complete event */
            GenerateEvt(&SonarMeasureComplete);
            /* Switch state */
            SonarMeasState = Snr_WaitRequest;                   
         }
         else if (Counter >= TIMEOUT_MS)
         {          
            /* Switch state */
            SonarMeasState = Snr_WaitRequest;               
         }
         break;

      default:
         break;         
   }
}

/************************************************************************
* Function:     SnrDrv_DetectEchoPulse
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Count the return echo pulse.
* Date:         29/09/16
************************************************************************/
void SnrDrv_DetectEchoPulse(void)
{
   static DetectEchoStateType DetectEchoState = WaitEchoRisEdge;
   static UINT16 EchoCounter = 0;
   
   switch (DetectEchoState)
   {
      case WaitEchoRisEdge:
         /* Wait for edge */
         if (BSP_ECHO == LINE_STATE_HIGH)
         {
            /* Increment counter */
            EchoCounter++; 
            /* Switch state */
            DetectEchoState = MeasureEchoPulse;
         }
         break;
         
      case MeasureEchoPulse:
         /* If an echo pulse is detected */
         if (BSP_ECHO == LINE_STATE_HIGH)
         {
            /* Increment counter */
            EchoCounter++;
         }
         else
         {
            /* Copy result value */
            EchoPulseCount = EchoCounter;
            /* Reset service counter */
            EchoCounter = 0;
            /* Send event */
            GenerateEvt(&SonarEchoCountReady);
            /* Switch state */
            DetectEchoState = WaitEchoRisEdge;            
         }         
         break;
         
      default:
         break;
   }
}

/************************************************************************
* Function:     SnrDrv_StartMeasure
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Trigger a new measure.
* Date:         29/09/16
************************************************************************/
void SnrDrv_StartMeasure(void)
{
   /* Send new measure event */
   GenerateEvt(&SonarStartMeasure);    
}
#endif