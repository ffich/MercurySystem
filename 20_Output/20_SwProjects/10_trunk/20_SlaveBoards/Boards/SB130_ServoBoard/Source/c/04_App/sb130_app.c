/************************************************************************
*                             SB130 APP                                 *
*************************************************************************
* FileName:         sb130_app.c                                         *
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
* F.Ficili     31/07/16    1.0.0        First release                   *
* F.Ficili     13/01/19    1.1.0        Updated dip-switch managment    *
* F.Ficili     24/03/19    1.2.0        Added commands 0x71, 0x72       *
*                                       and 0x73. Fixed some issues.    * 
* F.Ficili     21/04/19    1.2.1        Fixed servo driver init. issue. *                
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "sb130_app.h"

/************************************************************************
* Defines
************************************************************************/
/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                            ((UINT16)(100)/APP_TASK_PERIOD_MS)

/* Commands */
#define SET_SERVO_1_POSITION                          ((UINT8)(0x51))
#define SET_SERVO_2_POSITION                          ((UINT8)(0x52))
#define SET_SERVO_3_POSITION                          ((UINT8)(0x53))
#define SET_SERVO_4_POSITION                          ((UINT8)(0x54))
#define SET_SERVO_5_POSITION                          ((UINT8)(0x55))
#define SET_SERVO_6_POSITION                          ((UINT8)(0x56))

#define SET_SERVO_POS_CHX_PRM                         ((UINT8)(1))

/* Queries */
#define REQ_SERVO_1_POSITION                          ((UINT8)(0x61))
#define REQ_SERVO_2_POSITION                          ((UINT8)(0x62))
#define REQ_SERVO_3_POSITION                          ((UINT8)(0x63))
#define REQ_SERVO_4_POSITION                          ((UINT8)(0x64))
#define REQ_SERVO_5_POSITION                          ((UINT8)(0x65))
#define REQ_SERVO_6_POSITION                          ((UINT8)(0x66))

#define REQ_SERVO_POS_LEN                             ((UINT8)(1))

/* Special commands */
#define SET_ALL_SERVO_POSITION                        ((UINT8)(0x71))
#define SET_BITMASK_SERVO_POSITION_1                  ((UINT8)(0x72))
#define SET_BITMASK_SERVO_POSITION_2                  ((UINT8)(0x73))

#define SET_ALL_SERVO_POS_CH1_PRM                     ((UINT8)(1))  
#define SET_ALL_SERVO_POS_CH2_PRM                     ((UINT8)(2)) 
#define SET_ALL_SERVO_POS_CH3_PRM                     ((UINT8)(3)) 
#define SET_ALL_SERVO_POS_CH4_PRM                     ((UINT8)(4)) 
#define SET_ALL_SERVO_POS_CH5_PRM                     ((UINT8)(5)) 
#define SET_ALL_SERVO_POS_CH6_PRM                     ((UINT8)(6)) 

#define SET_BMSK_SERVO_MASK_PRM                       ((UINT8)(1)) 
#define SET_BMSK_SERVO_GLOB_POS_PRM                   ((UINT8)(2)) 

#define SET_BITMASK_SERVO_CH1_MASK                    0b00000001
#define SET_BITMASK_SERVO_CH2_MASK                    0b00000010
#define SET_BITMASK_SERVO_CH3_MASK                    0b00000100
#define SET_BITMASK_SERVO_CH4_MASK                    0b00001000
#define SET_BITMASK_SERVO_CH5_MASK                    0b00010000
#define SET_BITMASK_SERVO_CH6_MASK                    0b00100000

#define MAX_SERVO_NUMB                                ((UINT8)(6)) 

#define SERVO_DRIVE_PERCENT_K                         16u
#define SERVO_DRIVE_DEGREES_K                         10u
#define SERVO_DRIVE_K                                 SERVO_DRIVE_PERCENT_K

#define CMD_SATURATION

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
UINT8 ServoTargetPos = 0xff;
#define INC_STEP  1u

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
   UINT8 ServoCmdNumber = 0;
   UINT8 i;
   
   /* If a write evt is received */
   if (ReceiveEvt(&I2cWriteRequest))
   {
      /* Check standard commands */
      StdS_ManageStdServices();
      
#ifdef CMD_SATURATION
      /* Command saturation */
      if (CmdBuffer[SET_SERVO_POS_CHX_PRM] >= 100)
      {
         CmdBuffer[SET_SERVO_POS_CHX_PRM] = 100;
      }
#endif
      
      /* Check command */
      switch (CmdBuffer[CMD_INDEX]) 
      {
         case SET_SERVO_1_POSITION:            
            /* Set servo 1 position */
            Servo[SERVO_CH_1].Duty = ((CmdBuffer[SET_SERVO_POS_CHX_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US);               
            break;
            
         case SET_SERVO_2_POSITION:
            /* Set servo 2 position */
            Servo[SERVO_CH_2].Duty = ((CmdBuffer[SET_SERVO_POS_CHX_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US);           
            break;
            
         case SET_SERVO_3_POSITION:
            /* Set servo 3 position */
            Servo[SERVO_CH_3].Duty = ((CmdBuffer[SET_SERVO_POS_CHX_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US);
            //ServoTargetPos = ((CmdBuffer[SET_SERVO_POS_CHX_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US); 
            break;

         case SET_SERVO_4_POSITION:
            /* Set servo 4 position */
            Servo[SERVO_CH_4].Duty = ((CmdBuffer[SET_SERVO_POS_CHX_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US);           
            break;

         case SET_SERVO_5_POSITION:
            /* Set servo 5 position */
            Servo[SERVO_CH_5].Duty = ((CmdBuffer[SET_SERVO_POS_CHX_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US);           
            break;

         case SET_SERVO_6_POSITION:
            /* Set servo 6 position */
            Servo[SERVO_CH_6].Duty = ((CmdBuffer[SET_SERVO_POS_CHX_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US);           
            break; 
            
         case REQ_SERVO_1_POSITION:            
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = Servo[SERVO_CH_1].Duty;
            /* Set response lenght */
            RepBufferLen = REQ_SERVO_POS_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;     
            
         case REQ_SERVO_2_POSITION:            
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = Servo[SERVO_CH_2].Duty;
            /* Set response lenght */
            RepBufferLen = REQ_SERVO_POS_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;  
            
         case REQ_SERVO_3_POSITION:            
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = Servo[SERVO_CH_3].Duty;
            /* Set response lenght */
            RepBufferLen = REQ_SERVO_POS_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;      
            
         case REQ_SERVO_4_POSITION:            
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = Servo[SERVO_CH_4].Duty;
            /* Set response lenght */
            RepBufferLen = REQ_SERVO_POS_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;                
            
         case REQ_SERVO_5_POSITION:            
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = Servo[SERVO_CH_5].Duty;
            /* Set response lenght */
            RepBufferLen = REQ_SERVO_POS_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;     
            
         case REQ_SERVO_6_POSITION:            
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = Servo[SERVO_CH_6].Duty;
            /* Set response lenght */
            RepBufferLen = REQ_SERVO_POS_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);
            break;       
            
         case SET_ALL_SERVO_POSITION:
            /* Set all servos position */
            Servo[SERVO_CH_1].Duty = ((CmdBuffer[SET_ALL_SERVO_POS_CH1_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US);  
            Servo[SERVO_CH_2].Duty = ((CmdBuffer[SET_ALL_SERVO_POS_CH2_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US); 
            Servo[SERVO_CH_3].Duty = ((CmdBuffer[SET_ALL_SERVO_POS_CH3_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US); 
            Servo[SERVO_CH_4].Duty = ((CmdBuffer[SET_ALL_SERVO_POS_CH4_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US); 
            Servo[SERVO_CH_5].Duty = ((CmdBuffer[SET_ALL_SERVO_POS_CH5_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US); 
            Servo[SERVO_CH_6].Duty = ((CmdBuffer[SET_ALL_SERVO_POS_CH6_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US);         
            break;    
            
         case SET_BITMASK_SERVO_POSITION_1:
            /* Set initial value for ServoCmdNumber */
            ServoCmdNumber = 1;
            /* Actuate requested servos */
            for (i = 0; i < MAX_SERVO_NUMB; i++)
            {
               /* Check command bitmask */
               if ((CmdBuffer[SET_BMSK_SERVO_MASK_PRM]) & (SET_BITMASK_SERVO_CH1_MASK << i))
               {
                  /* Set servo position */
                  Servo[i].Duty = ((CmdBuffer[ServoCmdNumber + 1])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US);
                  ServoCmdNumber++;               
               }            
            }                                 
            break;     
            
         case SET_BITMASK_SERVO_POSITION_2:
            /* Actuate requested servos */
            for (i = 0; i < MAX_SERVO_NUMB; i++)
            {
               /* Check command bitmask */
               if ((CmdBuffer[SET_BMSK_SERVO_MASK_PRM]) & (SET_BITMASK_SERVO_CH1_MASK << i))
               {
                  /* Set servo position */
                  Servo[i].Duty = ((CmdBuffer[SET_BMSK_SERVO_GLOB_POS_PRM])*SERVO_DRIVE_K/SERVO_DRV_INT_PER_US);              
               }            
            }                                 
            break;              
            
         default:
            break;
      }     
   }
}

/************************************************************************
* Function:     ServoControl
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage control of servo signal.
* Date:         22/05/19
************************************************************************/
void ServoControl (void)
{   
   static UINT16 Counter = 0;
   
   Counter++;
   
   if (Counter >= 8)
   {
      Counter = 0;
      
      if (ServoTargetPos > Servo[SERVO_CH_3].Duty)
      {
         /* Decrement servo signal */
         Servo[SERVO_CH_3].Duty += INC_STEP;     
      }
      else if (ServoTargetPos < Servo[SERVO_CH_3].Duty)
      {
         /* Increment servo signal */
         Servo[SERVO_CH_3].Duty -= INC_STEP;         
      }
      else
      {
         /* Do nothing */
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
         
         /* Set pull-ups status */         
         WpuCh.WpuC5 = CH_PU_ENABLED;
         WpuCh.WpuC4 = CH_PU_ENABLED;
         WpuCh.WpuC3 = CH_PU_ENABLED;
         WpuCh.WpuC6 = CH_PU_ENABLED;
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
         
         /* Init the servo driver */
         SrvDrv_ServoInit();

         BSP_SERVO_1_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_SERVO_2_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_SERVO_3_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_SERVO_4_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_SERVO_5_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_SERVO_6_TRIS = LINE_DIRECTION_OUTPUT;         
         
         break;

      /* System Normal operation Phase */
      case RunningState:
			/* Add app-specific functions here */
			
         /* Main app state machine */
         AppSm();
         
         //ServoControl();
         break;

      /* Default */
      default:
         break;
   }
}
  