/************************************************************************
*                             SB110 APP                                 *
*************************************************************************
* FileName:         sb110_app.c                                         *
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
* F.Ficili     31/07/16    1.0          First release                   *
* F.Ficili     13/01/19    1.1          Updated dip-switch managment    *                   
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "sb110_app.h"

/************************************************************************
* Defines
************************************************************************/
/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                            ((UINT16)(100)/APP_TASK_PERIOD_MS)

#define OUT_CH_1                                      ((UINT8)(1))
#define OUT_CH_2                                      ((UINT8)(2))

/* Commands */
#define UPDATE_RELAY_STS                              ((UINT8)(0x50))
#define IMPULSE_RELAY_1_S                             ((UINT8)(0x51))
#define IMPULSE_RELAY_2_S                             ((UINT8)(0x52))
#define IMPULSE_BOTH_RELAYS_S                         ((UINT8)(0x53))

/* Queries */
#define REQUEST_RELAY_STS                             ((UINT8)(0x60))
#define REQUEST_RELAY_STS_LEN                         ((UINT8)(1))

/* UPDATE_RELAY_STS - Parameters */
#define RELAY_1_SHIFT                                 ((UINT8)(0))
#define RELAY_2_SHIFT                                 ((UINT8)(1))
#define RELAY_1_MASK                                  ((UINT8)(0b00000001))
#define RELAY_2_MASK                                  ((UINT8)(0b00000010))
#define RELAY_OFF                                     ((UINT8)(0))
#define RELAY_ON                                      ((UINT8)(1))

/************************************************************************
* Typedefs
************************************************************************/
/* Main app state */
typedef enum AppState
{
   WaitWpuSetState,
   SendI2cAddrState,
   ManageProtocolState,   
} AppStateType;

/* Impulse sm state type */
typedef enum _ImpulseStateType
{
   Imp_Idle,
   Imp_Delay,
} ImpulseStateType;

/* Impulse struct */
typedef struct _ImpulseType
{
   ImpulseStateType ImpulseState;
   UINT16 CounterMs;
   UINT16 CounterS;
   UINT16 ImpulseLen;
   UINT8 Channel;
   EventStructureType StartEvt;   
} ImpulseType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* Relay status */
UINT8 Relay1Sts = OFF;
UINT8 Relay2Sts = OFF;

/* Impulse set for relay 1 and 2 */
ImpulseType Relay1Set;
ImpulseType Relay2Set;

/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Global command buffer */
UINT8 CmdBuffer[I2C_RX_BUFFER_DIM];
/* Global reply buffer */
UINT8 RepBuffer[I2C_TX_BUFFER_DIM];
/* Reply lenght */
UINT8 RepBufferLen;

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
void UpdateRelaysSts (void);
void ManageProtocol (void);
void AppSm (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     SetOutChSts
* Input:        UINT8 Ch
*               UINT8 Sts
* Output:       None
* Author:       F.Ficili
* Description:  Set output channels status on request.
* Date:         05/01/18
************************************************************************/
void SetOutChSts (UINT8 Ch, UINT8 Sts)
{
   /* Check channel */
   switch (Ch)
   {
      case OUT_CH_1:
         /* Set ch1 status */
         BSP_RELAY_1 = Sts;
         break;
         
      case OUT_CH_2:
         /* Set ch1 status */         
         BSP_RELAY_2 = Sts;
         break;
         
      default:
         break;
   }
}

/************************************************************************
* Function:     UpdateRelaysSts
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Update relays status on request.
* Date:         06/08/16
************************************************************************/
void UpdateRelaysSts (void)
{
   /* Control Relay 1 */
   if (((CmdBuffer[PAR_1_INDEX] & RELAY_1_MASK) >> RELAY_1_SHIFT) == RELAY_ON)
   {
      Relay1Sts = RELAY_ON;
   }
   else
   {
      Relay1Sts = RELAY_OFF;
   }

   /* Control Relay 2 */
   if (((CmdBuffer[PAR_1_INDEX] & RELAY_2_MASK) >> RELAY_2_SHIFT) == RELAY_ON)
   {
      Relay2Sts = RELAY_ON;

   }
   else
   {
      Relay2Sts = RELAY_OFF;
   }
   
   /* Set status for both channels */
   SetOutChSts(OUT_CH_1,Relay1Sts);
   SetOutChSts(OUT_CH_2,Relay2Sts);
} 

/************************************************************************
* Function:     GenerateImpulse
* Input:        ImpulseType *Impulse
* Output:       None
* Author:       F.Ficili
* Description:  Generate a sw impulse (can be used to drive any type
*               of digital output (es: Relay1 = GenerateImpulse(&Rel1Set);).
* Date:         31/12/17
************************************************************************/
void GenerateImpulse (ImpulseType *Impulse)
{   
   switch (Impulse->ImpulseState)
   {
      case Imp_Idle:
         /* Check event */
         if (ReceiveEvt(&(Impulse->StartEvt)))
         {
            /* Set output status */
            SetOutChSts(Impulse->Channel,ON);
            Impulse->ImpulseState = Imp_Delay;         
         }
         break;
         
      case Imp_Delay:
         /* Increment ms counter */
         (Impulse->CounterMs)++;
         /* Check counter */        
         if (Impulse->CounterMs >= 1000/APP_TASK_PERIOD_MS)
         {
            /* Reset ms counter */
            Impulse->CounterMs = 0;
            /* Increment s counter */
            (Impulse->CounterS)++;
         }
         
         if (Impulse->CounterS >= Impulse->ImpulseLen)
         {
            /* Reset counter */
            Impulse->CounterS = 0;
            /* Clear output status */
            SetOutChSts(Impulse->Channel,OFF);
            /* Switch state */
            Impulse->ImpulseState = Imp_Idle;         
         }
         else
         {
            /* Do nothing */
         }
         break;

      default:
         break;         
   }
}

/************************************************************************
* Function:     ManageProtocol
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage generale and board-specific commands.
* Date:         22/06/17
************************************************************************/
void ManageProtocol (void)
{
   /* If a write evt is received */
   if (ReceiveEvt(&I2cWriteRequest))
   { 
      /* Check standard commands */
      StdS_ManageStdServices();              
      /* Check board-specific commands */
      switch (CmdBuffer[CMD_INDEX]) 
      {                               
         case UPDATE_RELAY_STS:
            /* Update relays status */
            UpdateRelaysSts();             
            break;
            
         case IMPULSE_RELAY_1_S:
            /* Get the impulse len */
            Relay1Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));             
            /* Send impulse event */
            GenerateEvt(&(Relay1Set.StartEvt));            
            break;        
            
         case IMPULSE_RELAY_2_S:
            /* Get the impulse len */
            Relay2Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));             
            /* Send impulse event */
            GenerateEvt(&(Relay2Set.StartEvt));            
            break;   
            
         case IMPULSE_BOTH_RELAYS_S:
            /* Get the impulse len */
            Relay1Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));  
            Relay2Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));             
            /* Send impulse event */
            GenerateEvt(&(Relay1Set.StartEvt)); 
            GenerateEvt(&(Relay2Set.StartEvt));            
            break;               
                                                
         case REQUEST_RELAY_STS:
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = (Relay1Sts | (Relay2Sts << RELAY_2_SHIFT));
            /* Set response lenght */
            RepBufferLen = REQUEST_RELAY_STS_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;

         default:
            break;
      }   
   }
}

/************************************************************************
* Function:     AppSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage App main state machine.
* Date:         31/07/16
************************************************************************/
void AppSm (void)
{
   static AppStateType AppState = WaitWpuSetState;
   static UINT8 WpuSetCounter = 0;
   
   switch (AppState)
   {
      case WaitWpuSetState:
         /* Inc counter */
         WpuSetCounter++;
         /* If timeout expired */
         if (WpuSetCounter >= WPU_SET_TIMEOUT_MS)
         {
            /* Get I2c address of the board */
            StdS_SetI2cAddress();            
            /* Switch state */
            AppState = SendI2cAddrState;         
         }         
         break;

      case SendI2cAddrState:
         /* Notify address could be set */
         GenerateEvt(&I2cSetAddress);
         /* Switch state */
         AppState = ManageProtocolState;            
         break;

      case ManageProtocolState:
         /* Resolve mercury protocol */
         ManageProtocol();  
         break;

      default:
         break;         
   }              
} 

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     App_MainTask
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Application main task.
* Date:         31/07/16
************************************************************************/
void App_MainTask (UINT8 Options)
{    
   WpuChType WpuCh = {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF};
   AnalogChSelType AnalogChSel = {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF};
   AdcConfigType AdcConfig;   
   
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* Initialize Relays DDRs and status  */
         BSP_RELAY_1_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_RELAY_2_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_RELAY_1 = LINE_STATE_LOW;
         BSP_RELAY_2 = LINE_STATE_LOW;
         
         /* Initialize dip-switch  DDRs */
         BSP_DIPSWITCH_1_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_2_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_3_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_4_TRIS = LINE_DIRECTION_INPUT;
         
         /* Set pull-ups status */         
         WpuCh.WpuA4 = CH_PU_ENABLED;
         WpuCh.WpuA5 = CH_PU_ENABLED;
         WpuCh.WpuC4 = CH_PU_ENABLED;
         WpuCh.WpuC5 = CH_PU_ENABLED;
         /* Set and activate pull-ups */
         Mcu_ConfigPullUps(&WpuCh);
         Mcu_SetPullUpsSts(Mcu_GlobalPuEnabled);
         
         /* Set ADC channels status */
         Adc_ConfigAnCh(&AnalogChSel);
         
         /* Configure die temp sensor */
         Mcu_SetDieTempSts(ON);
         Mcu_SetDieTempMode(DIE_TEMP_SENS_LOW_RANGE);
                  
         /* Adc initialization */
         AdcConfig.AdcConvClock = ADC_FRC;
         AdcConfig.AdcIntEnabled = OFF;
         AdcConfig.AdcNegRef = ADC_N_VREF_VSS;
         AdcConfig.AdcPosRef = ADC_P_VREF_VDD;
         AdcConfig.AdcResultFormat = ADC_RIGHT_JUST;
         AdcConfig.AdcEnable = ON;
         Adc_InitAdcModule(&AdcConfig);         
         
         /* Initialize relay impulse set */
         Relay1Set.ImpulseState = Imp_Idle;
         Relay1Set.CounterMs = 0;
         Relay1Set.CounterS = 0;
         Relay1Set.ImpulseLen = 0;
         Relay1Set.Channel = OUT_CH_1;
         Relay1Set.StartEvt.EventOptions = 0;
         Relay1Set.StartEvt.EventStatus = EventIdle;
         
         Relay2Set.ImpulseState = Imp_Idle;
         Relay2Set.CounterMs = 0;
         Relay2Set.CounterS = 0;
         Relay2Set.ImpulseLen = 0;
         Relay2Set.Channel = OUT_CH_2;
         Relay2Set.StartEvt.EventOptions = 0;
         Relay2Set.StartEvt.EventStatus = EventIdle;       
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Main app state machine */
         AppSm(); 
         /* Manage Relays impulses */
         GenerateImpulse(&Relay1Set);
         GenerateImpulse(&Relay2Set);
         break;

      /* Default */
      default:
         break;
   }
}
  