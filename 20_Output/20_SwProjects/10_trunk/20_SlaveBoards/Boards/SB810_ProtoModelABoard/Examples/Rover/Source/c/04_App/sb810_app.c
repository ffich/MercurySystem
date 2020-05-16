/************************************************************************
*                             SB810 APP                                 *
*************************************************************************
* FileName:         sb810_app.c                                         *
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
* F.Ficili     02/07/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "sb810_app.h"

/************************************************************************
* Defines
************************************************************************/
/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                            ((UINT16)(100)/APP_TASK_PERIOD_MS)

#define ROVER_STOP                                    0x50
#define ROVER_FORWARD                                 0x51
#define ROVER_BACKWARD                                0x52
#define ROVER_RIGHT                                   0x53
#define ROVER_LEFT                                    0x54

#define M1_A                                          BSP_RC7_LAT
#define M1_B                                          BSP_RB5_LAT
#define M2_A                                          BSP_RC1_LAT
#define M2_B                                          BSP_RC2_LAT

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
* Author:	F.Ficili
* Description:	Manage the mercuiry slave protocol for SB01.
* Date:		02/07/17
************************************************************************/
void ManageProtocol (void)
{
   UINT8 Index;
   
   /* If a write evt is received */
   if (ReceiveEvt(&I2cWriteRequest))
   {
      /* Check standard commands */
      StdS_ManageStdServices();               
      /* Check command */
      switch (CmdBuffer[CMD_INDEX]) 
      {
         /* ---------------------------------- */
         /* - Add your commands handles here - */
         /* ---------------------------------- */
         
         case ROVER_FORWARD:
            M1_A = LINE_STATE_HIGH;
            M2_A = LINE_STATE_HIGH;
            M1_B = LINE_STATE_LOW;
            M2_B = LINE_STATE_LOW;            
            break;
            
         case ROVER_BACKWARD:
            M1_A = LINE_STATE_LOW;
            M2_A = LINE_STATE_LOW;
            M1_B = LINE_STATE_HIGH;
            M2_B = LINE_STATE_HIGH;             
            break;
            
         case ROVER_RIGHT:
            M1_A = LINE_STATE_LOW;
            M2_A = LINE_STATE_HIGH;
            M1_B = LINE_STATE_LOW;
            M2_B = LINE_STATE_LOW;              
            break;
            
         case ROVER_LEFT:
            M1_A = LINE_STATE_HIGH;
            M2_A = LINE_STATE_LOW;
            M1_B = LINE_STATE_LOW;
            M2_B = LINE_STATE_LOW;              
            break;      
            
         case ROVER_STOP:
            M1_A = LINE_STATE_LOW;
            M2_A = LINE_STATE_LOW;
            M1_B = LINE_STATE_LOW;
            M2_B = LINE_STATE_LOW;              
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
* Author:	F.Ficili
* Description:	Manage SB01 main application.
* Date:		02/07/17
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
* Author:	F.Ficili
* Description:	Main app task.
* Date:		02/07/17
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
         
         BSP_RC7_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_RC1_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_RB5_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_RC2_TRIS = LINE_DIRECTION_OUTPUT;
         
         M1_A = LINE_STATE_LOW;
         M2_A = LINE_STATE_LOW;
         M1_B = LINE_STATE_LOW;
         M2_B = LINE_STATE_LOW;         
         
         /* Set pull-ups status */         
         WpuCh.WpuC6 = CH_PU_ENABLED;
         WpuCh.WpuC3 = CH_PU_ENABLED;
         WpuCh.WpuC4 = CH_PU_ENABLED;
         WpuCh.WpuC5 = CH_PU_ENABLED;
         /* Set and activate pull-ups */
         Mcu_ConfigPullUps(&WpuCh);
         Mcu_SetPullUpsSts(Mcu_GlobalPuEnabled);
         
         /* Analog Channels configurations */
         AnalogChSel.AnSelA0An0 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelA1An1 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelA2An2 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelA4An3 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelC0An4 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelC1An5 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelC2An6 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelC3An7 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelC6An8 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelC7An9 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelB4An10 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelB5An11 = CH_SEL_DIGITAL;         
         Adc_ConfigAnCh(&AnalogChSel);
         
         /* Configure die temp sensor */
         Mcu_SetDieTempSts(ON);
         Mcu_SetDieTempMode(DIE_TEMP_SENS_LOW_RANGE);
         break;

      /* System Normal operation Phase */
      case RunningState:         
         
         /* Main app state machine */
         AppSm();         
         break;

      /* Default */
      default:
         break;
   }
}
  