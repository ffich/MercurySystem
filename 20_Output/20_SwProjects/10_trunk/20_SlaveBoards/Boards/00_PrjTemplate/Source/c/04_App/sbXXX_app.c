/************************************************************************
*                             SBXXX APP                                 *
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
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "sbXXX_app.h"

/************************************************************************
* Defines
************************************************************************/
/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                            ((UINT16)(100)/APP_TASK_PERIOD_MS)

/* Queries */

/* Parameters */


/************************************************************************
* Typedefs
************************************************************************/
/* Main app state */
typedef enum AppState
{
   GetI2cAddressState,
   WaitWpuSetState,
   SendI2cAddrState,
   ManageProtocolState,   
} AppStateType;

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
* Description:  Manage SB320 main application.
* Date:         31/07/16
************************************************************************/
void AppSm (void)
{
   static AppStateType AppState = GetI2cAddressState;
   static UINT8 WpuSetCounter = 0;
   
   switch (AppState)
   {
      case GetI2cAddressState:
      #if (ADDRESS_MODE == ADDR_MODE_DIP_SWITCH)
         /* Get addr from dip-switch */
         I2cSlaveAddress |= (BSP_DIPSWITCH_4 << 3);
         I2cSlaveAddress |= (BSP_DIPSWITCH_3 << 2);
         I2cSlaveAddress |= (BSP_DIPSWITCH_2 << 1);
         I2cSlaveAddress |= (BSP_DIPSWITCH_1 << 0); 
      #endif
         
      #if (ADDRESS_MODE == ADDR_MODE_HARDCODED)
         /* Get Hard-coded address */
         I2cSlaveAddress = I2C_HARDCODED_SLAVE_ADDRESS;
      #endif
         
         /* Switch state */
         AppState = WaitWpuSetState;
         break;

      case WaitWpuSetState:
         /* Inc counter */
         WpuSetCounter++;
         /* If timeout expired */
         if (WpuSetCounter >= WPU_SET_TIMEOUT_MS)
         {
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
			/* Add app-specific functions here */
			
         /* Main app state machine */
         AppSm();
         break;

      /* Default */
      default:
         break;
   }
}
  