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
* F.Ficili     05/06/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "sb810_app.h"

/************************************************************************
* Defines
************************************************************************/
/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                            ((UINT16)(200)/APP_TASK_PERIOD_MS)

/* Queries */
#define REQUEST_TEMP                                  ((UINT8)(0x61))
#define REQUEST_TEMP_LEN                              ((UINT8)(4))

#define SAMPLE_NUM                                    20
#define CAL_VALUE_C                                   ((double)(3.5))              

/************************************************************************
* Typedefs
************************************************************************/
/* RB01 main app state */
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
/* Temperature */
static volatile UINT16 TempRes;
static volatile double TempCel;
static UINT8 TempCelStr[50];
static UINT16 TempResArray[SAMPLE_NUM]; 

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
void MyFtoa(double number,char * string, int precision);
void GetTemperature (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     ManageProtocol
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	Manage mercury protocol (standard and app-specific services).
* Date:         06/08/16
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
         case REQUEST_TEMP:
            /* Prepare response */
            for (Index = 0; Index < REQUEST_TEMP_LEN; Index++)
            {
               RepBuffer[Index] = TempCelStr[Index];
            }
            /* Set response lenght */
            RepBufferLen = REQUEST_TEMP_LEN;
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
* Description:	App main state machine.
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
* Function:     GetTemperature
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	Get temp and convert.
* Date:         29/09/17
************************************************************************/
void GetTemperature (void)
{
   UINT8 Index;   
   double Val1, Val2;   
   
   /* Windowing */
   for (Index = 0; Index < SAMPLE_NUM - 1; Index++)
   {
      TempResArray[Index] = TempResArray[Index + 1];
   }

   /* Start conversion */
   Adc_StartConversion();
   /* Wait conversion completed */
   while (Adc_GetConvSts() == AdcConvInProgress);
   /* Get result */
   TempResArray[SAMPLE_NUM - 1] = Adc_GetConvResult();      
      
   /* Average value calculation */
   for (Index = 0; Index < SAMPLE_NUM; Index++)
   {
      TempRes += TempResArray[Index];
   }
   
   TempRes = TempRes/SAMPLE_NUM;
   
   /* Temperature in °C */
   Val1 = (((double)3.3)/((double)1024));
   Val2 = (Val1 * (double)TempRes);         
   TempCel = (double)((Val2 - (double)(0.5))/((double)0.01));  
   /* Apply calibration */
   TempCel = TempCel - CAL_VALUE_C;
   /* Conversion to ASCII */
   MyFtoa(TempCel,TempCelStr,100); 
}
  
/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     App_MainTask
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 App main task.
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
         /* Initialize sens DDRs */
         //BSP_RA2_TRIS = LINE_DIRECTION_INPUT;
         
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
         
         /* Analog Channels configurations */
         AnalogChSel.AnSelA0An0 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelA1An1 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelA2An2 = CH_SEL_ANALOG;
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
         
         /* Adc initialization */
         AdcConfig.AdcConvClock = ADC_FRC;
         AdcConfig.AdcIntEnabled = OFF;
         AdcConfig.AdcNegRef = ADC_N_VREF_VSS;
         AdcConfig.AdcPosRef = ADC_P_VREF_VDD;
         AdcConfig.AdcResultFormat = ADC_RIGHT_JUST;
         AdcConfig.AdcEnable = ON;
         Adc_InitAdcModule(&AdcConfig);
         /* Adc channel selection */
         Adc_SelectChannel(CH_AN2);
         break;

      /* System Normal operation Phase */
      case RunningState:    
         /* Get sens temp */
         GetTemperature();                    
         /* Main app state machine */
         AppSm();         
         break;

      /* Default */
      default:
         break;
   }
}
