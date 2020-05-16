/************************************************************************
*                             EB210 APP                                 *
*************************************************************************
* FileName:         eb210_app.c                                         *
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
* F.Ficili     16/01/19    1.1          Added line-oriented write cmd   *
*                                       0x5A (line1) and 0x5B (line2)   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "eb210_app.h"

/************************************************************************
* Defines
************************************************************************/
/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                                  ((UINT16)(100)/APP_TASK_PERIOD_MS)

/* LCD Display Send command */
#define LCD_DISPLAY_SEND_CMD                                ((UINT8)(0x50))
/* LCD Display Write String */
#define LCD_DISPLAY_WRITE_STRING                            ((UINT8)(0x51))
/* LCD display backlight control */
#define LCD_DISPLAY_BL_CONTROL                              ((UINT8)(0x52))
/* Specific commands */
#define LCD_DISPLAY_CLEAR_SCR                               ((UINT8)(0x53))
#define LCD_DISPLAY_LINE_1                                  ((UINT8)(0x54))
#define LCD_DISPLAY_LINE_2                                  ((UINT8)(0x55))
/* Line-write commands */
#define LCD_WRITE_LINE_1                                    ((UINT8)(0x5A))
#define LCD_WRITE_LINE_2                                    ((UINT8)(0x5B))

/* LCD Backlight constant */
#define LCD_BL_OFF                                          ((UINT8)(0))
#define LCD_BL_ON                                           ((UINT8)(1))


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
* Description:  Manage the mercuiry slave protocol for SB01.
* Date:         02/07/17
************************************************************************/
void ManageProtocol (void)
{
   UINT8 Index;
   UINT8 LcdWriteBuffer[40];
   
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
         
         case LCD_DISPLAY_SEND_CMD:
            /* Send command */
            Lcd_WriteCommand(CmdBuffer[PAR_1_INDEX]);
            break;
            
         case LCD_DISPLAY_WRITE_STRING:
            /* Clear buffer */
            ClearBuffer(LcdWriteBuffer,20);
            /* Fill buffer */
            for (Index = 0; Index < CmdBuffer[PAR_1_INDEX]; Index++)
            {
               LcdWriteBuffer[Index] = CmdBuffer[Index + 2];
            }
            /* Add termination */
            LcdWriteBuffer[Index + 3] = NULL;            
            /* Write LCD */
            Lcd_WriteString(LcdWriteBuffer);
            break;
            
         case LCD_DISPLAY_BL_CONTROL:
            /* Control backlight */
            if ((CmdBuffer[PAR_1_INDEX]) == LCD_BL_ON)
            {
               /* Turn on backlight */
               Lcd_Backlight(BacklightOn);
            }
            else
            {
               /* Turn off backlight */
               Lcd_Backlight(BacklightOff);
            }
            break;
            
         case LCD_DISPLAY_CLEAR_SCR:
            /* Clear screen */
            Lcd_WriteCommand(CLEAR_DISPLAY);
            break;
            
         case LCD_DISPLAY_LINE_1:
            /* Cursor home position */
            Lcd_WriteCommand(CURSOR_HOME);            
            break;
            
         case LCD_DISPLAY_LINE_2:
            /* Cursor line 2 position  */
            Lcd_WriteCommand(CURSOR_LINE_2);            
            break; 
            
         case LCD_WRITE_LINE_1:
            /* Cursor home position */
            Lcd_WriteCommand(CURSOR_HOME);               
            /* Clear buffer */
            ClearBuffer(LcdWriteBuffer,20);
            /* Fill buffer */
            for (Index = 0; Index < CmdBuffer[PAR_1_INDEX]; Index++)
            {
               LcdWriteBuffer[Index] = CmdBuffer[Index + 2];
            }
            /* Add termination */
            LcdWriteBuffer[Index + 3] = NULL;            
            /* Write LCD */
            Lcd_WriteString(LcdWriteBuffer);            
            break;
            
         case LCD_WRITE_LINE_2:
            /* Cursor line 2 position  */
            Lcd_WriteCommand(CURSOR_LINE_2);   
            /* Clear buffer */
            ClearBuffer(LcdWriteBuffer,20);
            /* Fill buffer */
            for (Index = 0; Index < CmdBuffer[PAR_1_INDEX]; Index++)
            {
               LcdWriteBuffer[Index] = CmdBuffer[Index + 2];
            }
            /* Add termination */
            LcdWriteBuffer[Index + 3] = NULL;            
            /* Write LCD */
            Lcd_WriteString(LcdWriteBuffer);            
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
* Description:	Main app task.
* Date:		02/07/17
************************************************************************/
void App_MainTask (UINT8 Options)
{    
   WpuChType WpuCh = {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF};
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

         /* Set charge pump shutdown control pin as output */
         BSP_CP_SHDN_TRIS = LINE_DIRECTION_OUTPUT;
         /* Enable charge pump */
         BSP_CP_SHDN = LINE_STATE_HIGH;         
         
         /* Set pull-ups status */ 
         WpuCh.WpuC3 = CH_PU_ENABLED;
         WpuCh.WpuC4 = CH_PU_ENABLED;
         WpuCh.WpuC5 = CH_PU_ENABLED;
         WpuCh.WpuC6 = CH_PU_ENABLED;
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
         
         /* Init LCD */
         Lcd_SetConfig();
         Lcd_InitLcdInterface(); 
         
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
         
         /* Welcome screen */
         Lcd_WriteCommand(CURSOR_HOME);
         Lcd_WriteConstString(" Mercury System ");
         Lcd_WriteCommand(CURSOR_LINE_2);
         Lcd_WriteConstString("EB210: Lcd Board");
         Lcd_Backlight(BacklightOn);         
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
  