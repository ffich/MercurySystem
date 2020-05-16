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
* F.Ficili     13/01/19    1.1          Updated dip-switch managment    *      
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

/* Analog channel list */
#define ADC_CH_0                                      CH_AN2
#define ADC_CH_1                                      CH_AN4
#define ADC_CH_2                                      CH_AN9
#define ADC_CH_3                                      CH_AN11

#define DIG_CH_0                                      0u
#define DIG_CH_1                                      1u
#define DIG_CH_2                                      2u
#define DIG_CH_3                                      3u

#define DIG_CH_0_TRIS                                 BSP_RC2_TRIS
#define DIG_CH_1_TRIS                                 BSP_RC1_TRIS
#define DIG_CH_2_TRIS                                 BSP_RA4_TRIS
#define DIG_CH_3_TRIS                                 BSP_RA5_TRIS

#define DIG_CH_0_LAT                                  BSP_RC2_LAT
#define DIG_CH_1_LAT                                  BSP_RC1_LAT
#define DIG_CH_2_LAT                                  BSP_RA4_LAT
#define DIG_CH_3_LAT                                  BSP_RA5_LAT

#define DIG_CH_0_PORT                                 BSP_RC2_PORT
#define DIG_CH_1_PORT                                 BSP_RC1_PORT
#define DIG_CH_2_PORT                                 BSP_RA4_PORT
#define DIG_CH_3_PORT                                 BSP_RA5_PORT

#define REQ_ANA_CH_0_RAW                              ((UINT8)(0x70))
#define REQ_ANA_CH_1_RAW                              ((UINT8)(0x71))
#define REQ_ANA_CH_2_RAW                              ((UINT8)(0x72))
#define REQ_ANA_CH_3_RAW                              ((UINT8)(0x73))
#define REQ_ANA_CH_X_RAW_LEN                          ((UINT8)(2))

#define REQ_ANA_CH_0_ASCII                            ((UINT8)(0x80))
#define REQ_ANA_CH_1_ASCII                            ((UINT8)(0x81))
#define REQ_ANA_CH_2_ASCII                            ((UINT8)(0x82))
#define REQ_ANA_CH_3_ASCII                            ((UINT8)(0x83))
#define REQ_ANA_CH_X_ASCII_LEN                        ((UINT8)(4))

#define SET_DIG_CH_DIR                                ((UINT8)(0x50))
#define SET_DIG_CH_DIR_LEN                            ((UINT8)(3))

#define SET_DIG_CH_OUT_STS                            ((UINT8)(0x51))
#define SET_DIG_CH_OUT_STS_LEN                        ((UINT8)(3))

#define REQ_DIG_CH_0_RAW                              ((UINT8)(0x60))
#define REQ_DIG_CH_1_RAW                              ((UINT8)(0x61))
#define REQ_DIG_CH_2_RAW                              ((UINT8)(0x62))
#define REQ_DIG_CH_3_RAW                              ((UINT8)(0x63))
#define REQ_DIG_CH_X_RAW_LEN                          ((UINT8)(1))

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
UINT16 AdcGetValueCh (UINT8 Ch);
void GetAnalogRaw (UINT8 Ch);
void GetAnalogAscii (UINT8 Ch);
void GetDigitalRaw (UINT8 Ch);
void SetDigitalDir (UINT8 Ch);
void SetDigitalOutSts (UINT8 Ch);

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

   
   /* If a write evt is received */
   if (ReceiveEvt(&I2cWriteRequest))
   {
      /* Check standard commands */
      StdS_ManageStdServices();               
      /* Check command */
      switch (CmdBuffer[CMD_INDEX]) 
      {
         case REQ_ANA_CH_0_RAW:
            /* Analog raw ch 0 */
            GetAnalogRaw(ADC_CH_0);
            break;
            
         case REQ_ANA_CH_0_ASCII:
            /* Analog ASCII ch 0 */
            GetAnalogAscii(ADC_CH_0);
            break;            
            
         case REQ_ANA_CH_1_RAW:
            /* Analog raw ch 1 */
            GetAnalogRaw(ADC_CH_1);            
            break;
            
         case REQ_ANA_CH_1_ASCII:
            /* Analog ASCII ch 1 */
            GetAnalogAscii(ADC_CH_1);
            break;               
            
         case REQ_ANA_CH_2_RAW:
            /* Analog raw ch 2 */
            GetAnalogRaw(ADC_CH_2);            
            break;
            
         case REQ_ANA_CH_2_ASCII:
            /* Analog ASCII ch 2 */
            GetAnalogAscii(ADC_CH_2);
            break;               
            
         case REQ_ANA_CH_3_RAW:
            /* Analog raw ch 3 */
            GetAnalogRaw(ADC_CH_3);            
            break; 
            
         case REQ_ANA_CH_3_ASCII:
            /* Analog ASCII ch 3 */
            GetAnalogAscii(ADC_CH_3);
            break;               
            
         case REQ_DIG_CH_0_RAW:
            /* Digital raw ch 0 */
            GetDigitalRaw(DIG_CH_0);
            break;
            
         case REQ_DIG_CH_1_RAW:
            /* Digital raw ch 1 */
            GetDigitalRaw(DIG_CH_1);            
            break;
            
         case REQ_DIG_CH_2_RAW:
            /* Digital raw ch 2 */
            GetDigitalRaw(DIG_CH_2);            
            break;
            
         case REQ_DIG_CH_3_RAW:
            /* Digital raw ch 3 */
            GetDigitalRaw(DIG_CH_3);            
            break;            
            
         case SET_DIG_CH_DIR: 
            /* Set TRIS register for digital channels */
            SetDigitalDir(CmdBuffer[PAR_1_INDEX]);
            break;
            
         case SET_DIG_CH_OUT_STS: 
            /* Set LAT register for digital channels */
            SetDigitalOutSts(CmdBuffer[PAR_1_INDEX]);            
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
* Function:     AdcGetValueCh
* Input:        UINT8 Ch
* Output:       UINT16
* Author:       F.Ficili
* Description:	 Return the conversion result for selected channel.
* Date:         08/10/18
************************************************************************/
UINT16 AdcGetValueCh (UINT8 Ch)
{
   UINT16 Result;
   
   /* Select Channel */
   Adc_SelectChannel(Ch);
   /* Start conversion */
   Adc_StartConversion();
   /* Wait conversion completed */
   while (Adc_GetConvSts() == AdcConvInProgress);
   /* Get result */
   Result = Adc_GetConvResult();      
   
   return Result;
}

/************************************************************************
* Function:     GetAnalogRaw
* Input:        UINT8 Ch
* Output:       void
* Author:       F.Ficili
* Description:	 Prepare response to analog raw data request.
* Date:         08/10/18
************************************************************************/
void GetAnalogRaw (UINT8 Ch)
{
   UINT8 i,j;
   UINT16 AdcData;
   
   /* Get conversion result */
   AdcData = AdcGetValueCh(Ch);
   AdcData = AdcGetValueCh(Ch);
   /* Clear buffer */
   memset(RepBuffer,0,I2C_TX_BUFFER_DIM);
   /* Prepare response */
   for (i = 0,j = 1; i < REQ_ANA_CH_X_RAW_LEN; i++,j--)
   {
      RepBuffer[i] |= (AdcData >> (8*j)) & 0x00FF;
   }
   /* Set response lenght */
   RepBufferLen = REQ_ANA_CH_X_RAW_LEN;
   /* Notify */
   GenerateEvt(&I2cReplyPacketReady);    
}

/************************************************************************
* Function:     GetAnalogAscii
* Input:        UINT8 Ch
* Output:       void
* Author:       F.Ficili
* Description:	 Prepare response to analog raw data request.
* Date:         08/10/18
************************************************************************/
void GetAnalogAscii (UINT8 Ch)
{
   UINT16 AdcData; 
   
   /* Get conversion result */
   AdcData = AdcGetValueCh(Ch);
   AdcData = AdcGetValueCh(Ch);
   /* Clear buffer */
   memset(RepBuffer,0,I2C_TX_BUFFER_DIM);
   /* Prepare response */
   utoa(RepBuffer,AdcData,10);
   strcat(RepBuffer,"    ");
   /* Set response lenght */
   RepBufferLen = REQ_ANA_CH_X_ASCII_LEN;
   /* Notify */
   GenerateEvt(&I2cReplyPacketReady);    
}

/************************************************************************
* Function:     GetDigitalRaw
* Input:        UINT8 Ch
* Output:       void
* Author:       F.Ficili
* Description:	 Prepare response to digital raw data request.
* Date:         08/10/18
************************************************************************/
void GetDigitalRaw (UINT8 Ch)
{
   switch (Ch)
   {
      case DIG_CH_0:
         RepBuffer[0] = DIG_CH_0_PORT;
         break;
         
      case DIG_CH_1:
         RepBuffer[0] = DIG_CH_1_PORT;
         break;

      case DIG_CH_2:
         RepBuffer[0] = DIG_CH_2_PORT;
         break;

      case DIG_CH_3:
         RepBuffer[0] = DIG_CH_3_PORT;
         break;         
         
      default:
         break;
   }
   
   /* Set response lenght */
   RepBufferLen = REQ_DIG_CH_X_RAW_LEN;
   /* Notify */
   GenerateEvt(&I2cReplyPacketReady);          
}

/************************************************************************
* Function:     SetDigitalDir
* Input:        UINT8 Ch
* Output:       void
* Author:       F.Ficili
* Description:	 Set the TRIS register of digital channels.
* Date:         08/10/18
************************************************************************/
void SetDigitalDir (UINT8 Ch)
{
   switch (Ch)
   {
      case DIG_CH_0:
         DIG_CH_0_TRIS = CmdBuffer[PAR_2_INDEX];
         break;
         
      case DIG_CH_1:
         DIG_CH_1_TRIS = CmdBuffer[PAR_2_INDEX];
         break;

      case DIG_CH_2:
         DIG_CH_2_TRIS = CmdBuffer[PAR_2_INDEX];
         break;

      case DIG_CH_3:
         DIG_CH_3_TRIS = CmdBuffer[PAR_2_INDEX];
         break;         
         
      default:
         break;
   }
}

/************************************************************************
* Function:     SetDigitalOutSts
* Input:        UINT8 Ch
* Output:       void
* Author:       F.Ficili
* Description:	 Set the LAT register of digital channels.
* Date:         08/10/18
************************************************************************/
void SetDigitalOutSts (UINT8 Ch)
{
   switch (Ch)
   {
      case DIG_CH_0:
         DIG_CH_0_LAT = CmdBuffer[PAR_2_INDEX];
         break;
         
      case DIG_CH_1:
         DIG_CH_1_LAT = CmdBuffer[PAR_2_INDEX];
         break;

      case DIG_CH_2:
         DIG_CH_2_LAT = CmdBuffer[PAR_2_INDEX];
         break;

      case DIG_CH_3:
         DIG_CH_3_LAT = CmdBuffer[PAR_2_INDEX];
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
* Description:  Main app task.
* Date:         02/07/17
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
         AnalogChSel.AnSelC0An4 = CH_SEL_ANALOG;
         AnalogChSel.AnSelC1An5 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelC2An6 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelC3An7 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelC6An8 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelC7An9 = CH_SEL_ANALOG;
         AnalogChSel.AnSelB4An10 = CH_SEL_DIGITAL;
         AnalogChSel.AnSelB5An11 = CH_SEL_ANALOG;         
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
         
         /* Main app state machine */
         AppSm();         
         break;

      /* Default */
      default:
         break;
   }
}
  