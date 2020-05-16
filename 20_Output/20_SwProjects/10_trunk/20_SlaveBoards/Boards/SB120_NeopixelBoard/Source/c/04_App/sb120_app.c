/************************************************************************
*                             SB120 APP                                 *
*************************************************************************
* FileName:         sb120_app.c                                         *
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
#include "sb120_app.h"

/************************************************************************
* Defines
************************************************************************/
#define DOUBLE_WRITE
#define DOUBLE_WRITE_DELAY_MS                         ((UINT8)(10))

/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                            ((UINT16)(100)/APP_TASK_PERIOD_MS)

/* Commands */
#define CH_1_WRITE_PIXEL                              ((UINT8)(0x50))
#define CH_1_SET_PIXEL_COLORS                         ((UINT8)(0x51))
#define CH_1_SET_PIXEL_NUMBER                         ((UINT8)(0x52))

#define CH_2_WRITE_PIXEL                              ((UINT8)(0x53))
#define CH_2_SET_PIXEL_COLORS                         ((UINT8)(0x54))
#define CH_2_SET_PIXEL_NUMBER                         ((UINT8)(0x55))

#define CH_3_WRITE_PIXEL                              ((UINT8)(0x56))
#define CH_3_SET_PIXEL_COLORS                         ((UINT8)(0x57))
#define CH_3_SET_PIXEL_NUMBER                         ((UINT8)(0x58))

/* Queries */

/* Parameters */
#define NPX_OFF                                       ((UINT8)(0))
#define NPX_ON                                        ((UINT8)(1))

/* Other */
#define NPX_CH_NUMB                                   ((UINT8)(3))
#define NPX_CH_1                                      ((UINT8)(0))
#define NPX_CH_2                                      ((UINT8)(1))
#define NPX_CH_3                                      ((UINT8)(2))
#define PIXEL_DEFAULT_NUMBER                          ((UINT8)(1))

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

typedef struct _NeopixelChType
{
   UINT8 NpxCh_PixelNumber;
   UINT8 NpxCh_PixelGreen;
   UINT8 NpxCh_PixelRed;
   UINT8 NpxCh_PixelBlue;           
} NeopixelChType;

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
/* Neopixel Ch structure */
NeopixelChType NeopixelChannels[NPX_CH_NUMB];

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
   UINT8 PixelIndex; 
   
   /* If a write evt is received */
   if (ReceiveEvt(&I2cWriteRequest))
   {
      /* Check standard commands */
      StdS_ManageStdServices();         
      /* Check command */
      switch (CmdBuffer[CMD_INDEX]) 
      {
         /* CH_1 */
         case CH_1_WRITE_PIXEL:
            /* Check action */
            if (CmdBuffer[PAR_1_INDEX] == NPX_ON)
            {
               /* Write pixels */
               Npdrv_Ch1_WriteNeopixelColor(NeopixelChannels[NPX_CH_1].NpxCh_PixelNumber
                                          ,NeopixelChannels[NPX_CH_1].NpxCh_PixelGreen
                                          ,NeopixelChannels[NPX_CH_1].NpxCh_PixelRed
                                          ,NeopixelChannels[NPX_CH_1].NpxCh_PixelBlue);
                              
#ifdef DOUBLE_WRITE
               __delay_ms(DOUBLE_WRITE_DELAY_MS);
               Npdrv_Ch1_WriteNeopixelColor(NeopixelChannels[NPX_CH_1].NpxCh_PixelNumber
                                          ,NeopixelChannels[NPX_CH_1].NpxCh_PixelGreen
                                          ,NeopixelChannels[NPX_CH_1].NpxCh_PixelRed
                                          ,NeopixelChannels[NPX_CH_1].NpxCh_PixelBlue);    
#endif
            }
            else if (CmdBuffer[PAR_1_INDEX] == NPX_OFF)
            {
               /* Switch pixel off */
               Npdrv_Ch1_WriteNeopixelColor(NeopixelChannels[NPX_CH_1].NpxCh_PixelNumber,0,0,0);
#ifdef DOUBLE_WRITE   
               __delay_ms(DOUBLE_WRITE_DELAY_MS);               
               Npdrv_Ch1_WriteNeopixelColor(NeopixelChannels[NPX_CH_1].NpxCh_PixelNumber,0,0,0);
#endif               
            }
            else
            {
               /* Do nothing */
            }                        
            break;   
            
         case CH_1_SET_PIXEL_COLORS:
            /* Set colors */
            NeopixelChannels[NPX_CH_1].NpxCh_PixelGreen = CmdBuffer[PAR_1_INDEX];
            NeopixelChannels[NPX_CH_1].NpxCh_PixelRed = CmdBuffer[PAR_2_INDEX];
            NeopixelChannels[NPX_CH_1].NpxCh_PixelBlue = CmdBuffer[PAR_3_INDEX];
            break;               
                                                
         case CH_1_SET_PIXEL_NUMBER:
            /* Set number */
            NeopixelChannels[NPX_CH_1].NpxCh_PixelNumber = CmdBuffer[PAR_1_INDEX];
            break;
            
         /* CH_2 */   
         case CH_2_WRITE_PIXEL:
            
            /* Check action */
            if (CmdBuffer[PAR_1_INDEX] == NPX_ON)
            {
               /* Write pixels */
               Npdrv_Ch2_WriteNeopixelColor(NeopixelChannels[NPX_CH_2].NpxCh_PixelNumber
                                          ,NeopixelChannels[NPX_CH_2].NpxCh_PixelGreen
                                          ,NeopixelChannels[NPX_CH_2].NpxCh_PixelRed
                                          ,NeopixelChannels[NPX_CH_2].NpxCh_PixelBlue); 
#ifdef DOUBLE_WRITE     
               __delay_ms(DOUBLE_WRITE_DELAY_MS);  
               Npdrv_Ch2_WriteNeopixelColor(NeopixelChannels[NPX_CH_2].NpxCh_PixelNumber
                                          ,NeopixelChannels[NPX_CH_2].NpxCh_PixelGreen
                                          ,NeopixelChannels[NPX_CH_2].NpxCh_PixelRed
                                          ,NeopixelChannels[NPX_CH_2].NpxCh_PixelBlue);   
#endif               
            }
            else if (CmdBuffer[PAR_1_INDEX] == NPX_OFF)
            {
               /* Switch pixel off */
               Npdrv_Ch1_WriteNeopixelColor(NeopixelChannels[NPX_CH_2].NpxCh_PixelNumber,0,0,0); 
#ifdef DOUBLE_WRITE   
               __delay_ms(DOUBLE_WRITE_DELAY_MS);  
               Npdrv_Ch1_WriteNeopixelColor(NeopixelChannels[NPX_CH_2].NpxCh_PixelNumber,0,0,0);
#endif
            }
            else
            {
               /* Do nothing */
            }              
            break;   
            
         case CH_2_SET_PIXEL_COLORS:                   
            /* Set colors */
            NeopixelChannels[NPX_CH_2].NpxCh_PixelGreen = CmdBuffer[PAR_1_INDEX];
            NeopixelChannels[NPX_CH_2].NpxCh_PixelRed = CmdBuffer[PAR_2_INDEX];
            NeopixelChannels[NPX_CH_2].NpxCh_PixelBlue = CmdBuffer[PAR_3_INDEX];
            break;               
                                                
         case CH_2_SET_PIXEL_NUMBER:
            /* Set number */
            NeopixelChannels[NPX_CH_2].NpxCh_PixelNumber = CmdBuffer[PAR_1_INDEX];
            break; 
            
         /* CH_3 */   
         case CH_3_WRITE_PIXEL:            
            /* Check action */
            if (CmdBuffer[PAR_1_INDEX] == NPX_ON)
            {
               /* Write pixels */
               Npdrv_Ch3_WriteNeopixelColor(NeopixelChannels[NPX_CH_3].NpxCh_PixelNumber
                                          ,NeopixelChannels[NPX_CH_3].NpxCh_PixelGreen
                                          ,NeopixelChannels[NPX_CH_3].NpxCh_PixelRed
                                          ,NeopixelChannels[NPX_CH_3].NpxCh_PixelBlue);  
#ifdef DOUBLE_WRITE    
               __delay_ms(DOUBLE_WRITE_DELAY_MS);  
               Npdrv_Ch3_WriteNeopixelColor(NeopixelChannels[NPX_CH_3].NpxCh_PixelNumber
                                          ,NeopixelChannels[NPX_CH_3].NpxCh_PixelGreen
                                          ,NeopixelChannels[NPX_CH_3].NpxCh_PixelRed
                                          ,NeopixelChannels[NPX_CH_3].NpxCh_PixelBlue); 
#endif
            }
            else if (CmdBuffer[PAR_1_INDEX] == NPX_OFF)
            {
               /* Switch pixel off */
               Npdrv_Ch1_WriteNeopixelColor(NeopixelChannels[NPX_CH_3].NpxCh_PixelNumber,0,0,0); 
#ifdef DOUBLE_WRITE     
               __delay_ms(DOUBLE_WRITE_DELAY_MS);  
               Npdrv_Ch1_WriteNeopixelColor(NeopixelChannels[NPX_CH_3].NpxCh_PixelNumber,0,0,0);              
#endif               
            }
            else
            {
               /* Do nothing */            
            }                         
            break;   
            
         case CH_3_SET_PIXEL_COLORS:
            /* Set colors */
            NeopixelChannels[NPX_CH_3].NpxCh_PixelGreen = CmdBuffer[PAR_1_INDEX];
            NeopixelChannels[NPX_CH_3].NpxCh_PixelRed = CmdBuffer[PAR_2_INDEX];
            NeopixelChannels[NPX_CH_3].NpxCh_PixelBlue = CmdBuffer[PAR_3_INDEX];
            break;               
                                                
         case CH_3_SET_PIXEL_NUMBER:
            /* Set number */
            NeopixelChannels[NPX_CH_3].NpxCh_PixelNumber = CmdBuffer[PAR_1_INDEX];
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
   AnalogChSelType AnalogChSel = {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF};
   AdcConfigType AdcConfig;
   UINT8 Index;
   
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* Neopixel pin */
         BSP_NEOPIXEL_PIN_1_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_NEOPIXEL_PIN_2_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_NEOPIXEL_PIN_3_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_NEOPIXEL_1_PIN = STD_OFF;
         BSP_NEOPIXEL_2_PIN = STD_OFF;
         BSP_NEOPIXEL_3_PIN = STD_OFF;
         
         /* Initialize dip-switch  DDRs */
         BSP_DIPSWITCH_1_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_2_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_3_TRIS = LINE_DIRECTION_INPUT;
         BSP_DIPSWITCH_4_TRIS = LINE_DIRECTION_INPUT;
         
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
         
         /* Init neopixel structure */
         for (Index = 0; Index < NPX_CH_NUMB; Index++)
         {
            NeopixelChannels[Index].NpxCh_PixelNumber = PIXEL_DEFAULT_NUMBER;
            NeopixelChannels[Index].NpxCh_PixelGreen = 0x00;
            NeopixelChannels[Index].NpxCh_PixelRed = 0x00;
            NeopixelChannels[Index].NpxCh_PixelBlue = 0x00;
         }         
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
  