/************************************************************************
*                             SB140 APP                                 *
*************************************************************************
* FileName:         sb140_app.c                                         *
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
#include "sb140_app.h"

/************************************************************************
* Defines
************************************************************************/
/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                               ((UINT16)(100)/APP_TASK_PERIOD_MS)

#define OUT_CH_1                                         ((UINT8)(1))
#define OUT_CH_2                                         ((UINT8)(2))
#define OUT_CH_3                                         ((UINT8)(3))
#define OUT_CH_4                                         ((UINT8)(4))

/* Commands */
#define REQUEST_HSD_STS_CMD_CMD                          ((UINT8)(0x50))
#define IMPULSE_CH_1_S                                   ((UINT8)(0x51))
#define IMPULSE_CH_2_S                                   ((UINT8)(0x52))
#define IMPULSE_CH_3_S                                   ((UINT8)(0x53))
#define IMPULSE_CH_4_S                                   ((UINT8)(0x54))
#define IMPULSE_ALL_CH_S                                 ((UINT8)(0x55))

/* Queries */
#define REQUEST_HSD_STS                                  ((UINT8)(0x60))
#define REQUEST_HSD_STS_LEN                              ((UINT8)(1))

/* REQUEST_HSD_STS_CMD_CMD - Parameters */
#define HSD_1_SHIFT                                      ((UINT8)(0))
#define HSD_2_SHIFT                                      ((UINT8)(1))
#define HSD_3_SHIFT                                      ((UINT8)(2))
#define HSD_4_SHIFT                                      ((UINT8)(3))
#define HSD_1_MASK                                       ((UINT8)(0b00000001))
#define HSD_2_MASK                                       ((UINT8)(0b00000010))
#define HSD_3_MASK                                       ((UINT8)(0b00000100))
#define HSD_4_MASK                                       ((UINT8)(0b00001000))
#define HSD_OFF                                          ((UINT8)(0))
#define HSD_ON                                           ((UINT8)(1))

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
/* HSDs status */
UINT8 Hsd1Sts = OFF;
UINT8 Hsd2Sts = OFF;
UINT8 Hsd3Sts = OFF;
UINT8 Hsd4Sts = OFF;

/* Impulse set for HSDs*/
ImpulseType Hsd1Set;
ImpulseType Hsd2Set;
ImpulseType Hsd3Set;
ImpulseType Hsd4Set;

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
void UpdateHsdSts (void);
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
         BSP_HSD_1 = Sts;
         break;
         
      case OUT_CH_2:
         /* Set ch1 status */         
         BSP_HSD_2 = Sts;
         break;
         
      case OUT_CH_3:
         /* Set ch1 status */
         BSP_HSD_3 = Sts;
         break;
         
      case OUT_CH_4:
         /* Set ch1 status */         
         BSP_HSD_4 = Sts;
         break;         
         
      default:
         break;
   }
}

/************************************************************************
* Function:     UpdateHsdSts
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Update relays status on request.
* Date:         06/08/16
************************************************************************/
void UpdateHsdSts (void)
{
   /* Control Relay 1 */
   if (((CmdBuffer[PAR_1_INDEX] & HSD_1_MASK) >> HSD_1_SHIFT) == HSD_ON)
   {
      Hsd1Sts = HSD_ON;
   }
   else
   {
      Hsd1Sts = HSD_OFF;
   }

   /* Control Relay 2 */
   if (((CmdBuffer[PAR_1_INDEX] & HSD_2_MASK) >> HSD_2_SHIFT) == HSD_ON)
   {
      Hsd2Sts = HSD_ON;

   }
   else
   {
      Hsd2Sts = HSD_OFF;
   }
   
   /* Control Relay 3 */
   if (((CmdBuffer[PAR_1_INDEX] & HSD_3_MASK) >> HSD_3_SHIFT) == HSD_ON)
   {
      Hsd3Sts = HSD_ON;
   }
   else
   {
      Hsd3Sts = HSD_OFF;
   }

   /* Control Relay 4 */
   if (((CmdBuffer[PAR_1_INDEX] & HSD_4_MASK) >> HSD_4_SHIFT) == HSD_ON)
   {
      Hsd4Sts = HSD_ON;
   }
   else
   {
      Hsd4Sts = HSD_OFF;
   }   
   
   /* Update relay status */
   SetOutChSts(OUT_CH_1,Hsd1Sts);
   SetOutChSts(OUT_CH_2,Hsd2Sts);
   SetOutChSts(OUT_CH_3,Hsd3Sts);
   SetOutChSts(OUT_CH_4,Hsd4Sts);   
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
         case REQUEST_HSD_STS_CMD_CMD:
            /* Update relays status */
            UpdateHsdSts();          
            break;
            
         case IMPULSE_CH_1_S:
            /* Get the impulse len */
            Hsd1Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));             
            /* Send impulse event */
            GenerateEvt(&(Hsd1Set.StartEvt));            
            break;        
            
         case IMPULSE_CH_2_S:
            /* Get the impulse len */
            Hsd2Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));             
            /* Send impulse event */
            GenerateEvt(&(Hsd2Set.StartEvt));            
            break;  
            
         case IMPULSE_CH_3_S:
            /* Get the impulse len */
            Hsd3Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));             
            /* Send impulse event */
            GenerateEvt(&(Hsd3Set.StartEvt));            
            break;        
            
         case IMPULSE_CH_4_S:
            /* Get the impulse len */
            Hsd4Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));             
            /* Send impulse event */
            GenerateEvt(&(Hsd4Set.StartEvt));            
            break;             
            
         case IMPULSE_ALL_CH_S:
            /* Get the impulse len */
            Hsd1Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));  
            Hsd2Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));
            Hsd3Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));  
            Hsd4Set.ImpulseLen = CmdBuffer[PAR_2_INDEX] | (((UINT16)(CmdBuffer[PAR_1_INDEX]) << 8));             
            /* Send impulse event */
            GenerateEvt(&(Hsd1Set.StartEvt)); 
            GenerateEvt(&(Hsd2Set.StartEvt));    
            GenerateEvt(&(Hsd3Set.StartEvt)); 
            GenerateEvt(&(Hsd4Set.StartEvt));               
            break;              
            
         case REQUEST_HSD_STS:
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] =  (Hsd1Sts                 |   \
                                          (Hsd2Sts << HSD_2_SHIFT) |   \
                                          (Hsd3Sts << HSD_3_SHIFT) |   \
                                          (Hsd4Sts << HSD_4_SHIFT));
            /* Set response lenght */
            RepBufferLen = REQUEST_HSD_STS_LEN;
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
* Author:	F.Ficili
* Description:	Application main task.
* Date:		31/07/16
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
         BSP_HSD_1_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_HSD_2_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_HSD_3_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_HSD_4_TRIS = LINE_DIRECTION_OUTPUT;         
         BSP_HSD_1 = LINE_STATE_LOW;
         BSP_HSD_2 = LINE_STATE_LOW;
         BSP_HSD_3 = LINE_STATE_LOW;
         BSP_HSD_4 = LINE_STATE_LOW;        
         
         /* Initialize dip-switch  DDRs */
         BSP_DIPSWITCH_1_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_2_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_3_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_4_TRIS = LINE_DIRECTION_INPUT;
                
         /* Set pull-ups status */         
         WpuCh.WpuA4 = CH_PU_ENABLED;
         WpuCh.WpuC4 = CH_PU_ENABLED;
         WpuCh.WpuC3 = CH_PU_ENABLED;
         WpuCh.WpuC7 = CH_PU_ENABLED;
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
         
         Hsd1Set.ImpulseState = Imp_Idle;
         Hsd1Set.CounterMs = 0;
         Hsd1Set.CounterS = 0;
         Hsd1Set.ImpulseLen = 0;
         Hsd1Set.Channel = OUT_CH_1;
         Hsd1Set.StartEvt.EventOptions = 0;
         Hsd1Set.StartEvt.EventStatus = EventIdle;
         
         Hsd2Set.ImpulseState = Imp_Idle;
         Hsd2Set.CounterMs = 0;
         Hsd2Set.CounterS = 0;
         Hsd2Set.ImpulseLen = 0;
         Hsd2Set.Channel = OUT_CH_2;
         Hsd2Set.StartEvt.EventOptions = 0;
         Hsd2Set.StartEvt.EventStatus = EventIdle;  
         
         Hsd3Set.ImpulseState = Imp_Idle;
         Hsd3Set.CounterMs = 0;
         Hsd3Set.CounterS = 0;
         Hsd3Set.ImpulseLen = 0;
         Hsd3Set.Channel = OUT_CH_3;
         Hsd3Set.StartEvt.EventOptions = 0;
         Hsd3Set.StartEvt.EventStatus = EventIdle;
         
         Hsd4Set.ImpulseState = Imp_Idle;
         Hsd4Set.CounterMs = 0;
         Hsd4Set.CounterS = 0;
         Hsd4Set.ImpulseLen = 0;
         Hsd4Set.Channel = OUT_CH_4;
         Hsd4Set.StartEvt.EventOptions = 0;
         Hsd4Set.StartEvt.EventStatus = EventIdle;           
         
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Main app state machine */
         AppSm();   
         /* Manage HSD impulses */
         GenerateImpulse(&Hsd1Set);
         GenerateImpulse(&Hsd2Set);  
         GenerateImpulse(&Hsd3Set);
         GenerateImpulse(&Hsd4Set);            
         break;

      /* Default */
      default:
         break;
   }
}
  