/************************************************************************
*                             SB310 APP                                 *
*************************************************************************
* FileName:         sb310_app.c                                         *
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
* F.Ficili     13/01/19    1.1          Updated dip-switch managment    *      
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "sb310_app.h"

/************************************************************************
* Defines
************************************************************************/
/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                            ((UINT16)(100)/APP_TASK_PERIOD_MS)
/* New measure timeout */
#define NEW_MEAS_TIMEOUT_MS                           ((UINT16)(100)/APP_TASK_PERIOD_MS)
/* Measure led blink timeout */
#define MEAS_LED_TIMEOUT_MS                           ((UINT16)(20)/APP_TASK_PERIOD_MS)

/* Commands */
#define TRIGGER_DISTANCE_MEASURE                      ((UINT8)(0x50))
#define AUTO_MEASURE_OPTION                           ((UINT8)(0x51))

/* Queries */
#define REQUEST_DISTANCE_CM                           ((UINT8)(0x60))
#define REQUEST_DISTANCE_CM_LEN                       ((UINT8)(2))

#define REQUEST_DISTANCE_CM_ASCII                     ((UINT8)(0x61))
#define REQUEST_DISTANCE_CM_ASCII_LEN                 ((UINT8)(3))

/* Automeasure values */
#define AUTO_MEASURE_OFF                              ((UINT8)(0))
#define AUTO_MEASURE_ON                               ((UINT8)(1))

/************************************************************************
* Typedefs
************************************************************************/
/* RB01 main app state */
typedef enum _AppState
{
   WaitWpuSetState,
   SendI2cAddrState,
   ManageProtocolState,   
} AppStateType;

/* Sonar measure state type */
typedef enum _SonarMeasState
{
   TriggerMeasure,
   WaitComp,
   BlinkLed
} SonarMeasStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/


/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Global command buffer */
UINT8 CmdBuffer[I2C_RX_BUFFER_DIM];
/* Global reply buffer */
UINT8 RepBuffer[I2C_TX_BUFFER_DIM];
/* Reply lenght */
UINT8 RepBufferLen;
/* Auto-measure */
UINT8 AutomeasureOption = AUTO_MEASURE_OFF;
/* Start Measure event */
EventStructureType StartMeasure = {0, EventIdle};

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
void ManageProtocol (void);
void AppSm (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     ManageProtocol
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage generale and board-specific commands.
* Date:         02/01/18
************************************************************************/
void ManageProtocol (void)
{
   UINT8 DistanceCmStr[REQUEST_DISTANCE_CM_ASCII_LEN];
   
   /* If a write evt is received */
   if (ReceiveEvt(&I2cWriteRequest))
   {
      /* Check standard commands */
      StdS_ManageStdServices();              
      /* Check board-specific commands */
      switch (CmdBuffer[CMD_INDEX]) 
      {    
         case TRIGGER_DISTANCE_MEASURE:
            /* Trigger a new measure */
            GenerateEvt(&StartMeasure);
            break;         
         
         case REQUEST_DISTANCE_CM:
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = (UINT8)((DistanceCm & 0xFF00) >> 8);
            RepBuffer[REP_PAR_2_INDEX] = (UINT8)(DistanceCm & 0x00FF);
            /* Set response lenght */
            RepBufferLen = REQUEST_DISTANCE_CM_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;

         case REQUEST_DISTANCE_CM_ASCII:
            /* Convert distance */
            utoa(DistanceCmStr,DistanceCm,10);
            /* Prepare response (Shift buffer if needed) */
            StringCopy(DistanceCmStr,RepBuffer,REQUEST_DISTANCE_CM_ASCII_LEN);            
            if (DistanceCm < 10)
            {
               RepBuffer[2] = RepBuffer[0];
               RepBuffer[1] = '0';
               RepBuffer[0] = '0';
            }            
            else if (DistanceCm < 100)
            {
               RepBuffer[2] = RepBuffer[1];
               RepBuffer[1] = RepBuffer[0];
               RepBuffer[0] = '0';
            }
            /* Set response lenght */
            RepBufferLen = REQUEST_DISTANCE_CM_ASCII_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;
            
         case AUTO_MEASURE_OPTION:
            /* Check option */
            if (CmdBuffer[PAR_1_INDEX] == AUTO_MEASURE_ON)
            {
               AutomeasureOption = AUTO_MEASURE_ON;
               /* Trigger a new measure */
               GenerateEvt(&StartMeasure);               
            }
            else if (CmdBuffer[PAR_1_INDEX] == AUTO_MEASURE_OFF)
            {
               AutomeasureOption = AUTO_MEASURE_OFF;
            }
            else
            {
               /* Invalid, do nothing */
            }
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
* Function:     SonarMeasure
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Continously trigger sonar measure.
* Date:         29/09/16
************************************************************************/
void SonarMeasure (void)
{
   static SonarMeasStateType SonarMeasState = TriggerMeasure;
   static UINT16 Counter = 0;
   
   switch (SonarMeasState)
   {
      case TriggerMeasure:
         if (ReceiveEvt(&StartMeasure))
         {
            /* Trigger a new measure */
            SnrDrv_StartMeasure();
            /* LED on */
            BSP_LED_2 = ON;            
            /* Switch state */
            SonarMeasState = BlinkLed;         
         }
         break;
         
      case BlinkLed:
         /* Increment counter */
         Counter++;
         /* Check */
         if (Counter >= MEAS_LED_TIMEOUT_MS)
         {         
            /* LED off */
            BSP_LED_2 = OFF;
            /* Reset */
            Counter = 0;            
            /* Check if automerasure is enabled */
            if (AutomeasureOption == AUTO_MEASURE_ON)
            {
               /* Re-arm event and switch state*/
               GenerateEvt(&StartMeasure);
               /* Switch state */
               SonarMeasState = WaitComp;                 
            }   
            else
            {
               /* Bacxk to trigger */
               SonarMeasState = TriggerMeasure;                 
            }
         }
         break;           
         
      case WaitComp:
         /* Increment counter */
         Counter++;
         /* Check */
         if (Counter >= NEW_MEAS_TIMEOUT_MS)
         {
            /* Reset */
            Counter = 0;
            /* Switch state */
            SonarMeasState = TriggerMeasure;         
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
* Function:     App_MainTask
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Application main task.
* Date:         02/01/18
************************************************************************/
void App_MainTask (UINT8 Options)
{       
   WpuChType WpuCh = {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF};
   AnalogChSelType AnalogChSel = {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF};
   AdcConfigType AdcConfig;  
   
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:         
         /* Initialize dip-switch  DDRs */
         BSP_DIPSWITCH_1_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_2_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_3_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_4_TRIS = LINE_DIRECTION_INPUT;
         
         /* Initialize LED2 */
         BSP_LED_2_TRIS = LINE_DIRECTION_OUTPUT;
         
         /* Set charge pump shutdown control pin as output */
         BSP_CP_SHDN_TRIS = LINE_DIRECTION_OUTPUT;
         /* Enable charge pump */
         BSP_CP_SHDN = LINE_STATE_HIGH;
                 
         /* Set pull-ups status */ 
         WpuCh.WpuC6 = CH_PU_ENABLED;
         WpuCh.WpuC3 = CH_PU_ENABLED;
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
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Trigger sonar measure */
         SonarMeasure();         
         /* Main app state machine */
         AppSm();         
         break;

      /* Default */
      default:
         break;
   }
}
  