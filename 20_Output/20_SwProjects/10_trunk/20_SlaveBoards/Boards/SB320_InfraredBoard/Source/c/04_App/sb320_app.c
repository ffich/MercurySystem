/************************************************************************
*                             SB320 APP                                 *
*************************************************************************
* FileName:         sb320_app.c                                         *
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
#include "sb320_app.h"

/************************************************************************
* Defines
************************************************************************/
/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                            ((UINT16)(100)/APP_TASK_PERIOD_MS)

/* Queries */
#define REQUEST_IR_SENS_STS_RAW                       ((UINT8)(0x60))
#define REQUEST_IR_SENS_STS_RAW_LEN                   ((UINT8)(1))

#define REQUEST_IR_SENS_STS_ASCII                     ((UINT8)(0x61))
#define REQUEST_IR_SENS_STS_ASCII_LEN                 ((UINT8)(2))

/* IR_SENS_STATUS - Parameters */
#define IR_SENS_1_SHIFT                               ((UINT8)(0))
#define IR_SENS_2_SHIFT                               ((UINT8)(1))
#define IR_SENS_1_MASK                                ((UINT8)(0b00000001))
#define IR_SENS_2_MASK                                ((UINT8)(0b00000010))
#define IR_SESN_LOW                                   ((UINT8)(0))
#define IR_SENS_HIGH                                  ((UINT8)(1))

#define OBJ_PRESENT                                   ((UINT8)(0))
#define OBJ_NOT_PRESENT                               ((UINT8)(1))

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

/************************************************************************
* LOCAL Variables
************************************************************************/
/* IR Sensor variables */
UINT8 IrSens1 = IR_SESN_LOW;
UINT8 IrSens2 = IR_SESN_LOW;

UINT8 IrSens1Str = '0';
UINT8 IrSens2Str = '0';

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
void ManageProtocol (void);
void AppSm (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     GetIrSensSts
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Get status of infra-red sensors.
* Date:         05/01/18
************************************************************************/
void GetIrSensSts (void)
{
   /* Get sensor status */
   IrSens1 = BSP_IR_SENS_1;
   IrSens2 = BSP_IR_SENS_2;
}

/************************************************************************
* Function:     ManageProtocol
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage generale and board-specific commands.
* Date:         06/08/16
************************************************************************/
void ManageProtocol (void)
{
   /* If a write evt is received */
   if (ReceiveEvt(&I2cWriteRequest))
   {
      /* Check standard commands */
      StdS_ManageStdServices();         
      /* Check command */
      switch (CmdBuffer[CMD_INDEX]) 
      {
         case REQUEST_IR_SENS_STS_RAW:
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = (IrSens1 | (IrSens2 << IR_SENS_2_SHIFT));
            /* Set response lenght */
            RepBufferLen = REQUEST_IR_SENS_STS_RAW_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;
            
         case REQUEST_IR_SENS_STS_ASCII:
            /* Convert both sensor to ascii */
            utoa(&IrSens1Str,IrSens1,10);
            utoa(&IrSens2Str,IrSens2,10);            
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = IrSens2Str;
            RepBuffer[REP_PAR_2_INDEX] = IrSens1Str;
            /* Set response lenght */
            RepBufferLen = REQUEST_IR_SENS_STS_ASCII_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;            

         default:
            break;
      }   
   }
}

/************************************************************************
* Function:     Int0Trigger
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage INT0 line trigger logic.
* Date:         26/05/17
************************************************************************/
void Int0Trigger (void)
{
   /* If both sensors detect an object */
   if ((IrSens1 == OBJ_PRESENT) && (IrSens2 == OBJ_PRESENT))
   {
      /* Set line low */
      BSP_INT_0 = LINE_STATE_LOW;
   }
   else
   {
      /* Set line low */
      BSP_INT_0 = LINE_STATE_HIGH;   
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
         /* Initialize IR Sensors DDRs */
         BSP_IR_SENS_1_TRIS = LINE_DIRECTION_INPUT;
         BSP_IR_SENS_2_TRIS = LINE_DIRECTION_INPUT;
         
         /* Set int0 line DDR */
         BSP_INT_0_TRIS = LINE_DIRECTION_OUTPUT;

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
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Get sensor status */
         GetIrSensSts();  
         /* Manage INT0 line */
         Int0Trigger();
         /* Main app state machine */
         AppSm();
         break;

      /* Default */
      default:
         break;
   }
}
  