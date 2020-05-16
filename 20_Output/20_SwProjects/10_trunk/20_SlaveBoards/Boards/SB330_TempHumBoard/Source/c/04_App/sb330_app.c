/************************************************************************
*                             SB330 APP                                 *
*************************************************************************
* FileName:         sb330_app.c                                         *
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
* F.Ficili     14/01/18    1.0.0        First release                   *
* F.Ficili     06/01/19    1.0.1        Corrected a bug on temp & hum   *
*                                       String paddiong.                * 
* F.Ficili     13/01/19    1.1.0        Updated dip-switch managment    *      
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "sb330_app.h"

/************************************************************************
* Defines
************************************************************************/
//#define TEMP_SIGN_TEST

/* Wpu set time */
#define WPU_SET_TIMEOUT_MS                            ((UINT16)(100)/APP_TASK_PERIOD_MS)
#define READING_TMOUT_MS                              ((UINT16)(4000)/APP_TASK_PERIOD_MS)

#define CHECKSUM_FAILED                               0u
#define CHECKSUM_OK                                   1u

/* Queries */
#define REQUEST_TEMPERATURE_RAW                       ((UINT8)(0x60))
#define REQUEST_TEMPERATURE_RAW_LEN                   ((UINT8)(2))

#define REQUEST_TEMPERATURE_ASCII                     ((UINT8)(0x61))
#define REQUEST_TEMPERATURE_ASCII_LEN                 ((UINT8)(5))

#define REQUEST_HUMIDITY_RAW                          ((UINT8)(0x62))
#define REQUEST_HUMIDITY_RAW_LEN                      ((UINT8)(2))

#define REQUEST_HUMIDITY_ASCII                        ((UINT8)(0x63))
#define REQUEST_HUMIDITY_ASCII_LEN                    ((UINT8)(4))

#define REQUEST_CHECKSUM_STS                          ((UINT8)(0x64))
#define REQUEST_CHECKSUM_LEN                          ((UINT8)(1))
/* Parameters */
#define TEMP_RAW_INT_IDX                              ((UINT8)(0))
#define TEMP_RAW_DEC_IDX                              ((UINT8)(1))
#define HUM_RAW_INT_IDX                               ((UINT8)(0))
#define HUM_RAW_DEC_IDX                               ((UINT8)(1))

/* Ranges */
#define HUM_MAX                                       1000
#define HUM_MIN                                       0
#define TEMP_MAX                                      800
#define TEMP_MIN                                      -400

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
/* Temperature Integral&Decimal */
UINT8 TempSign = 0;
INT8 TemperatureInt = 0;
UINT8 TemperatureDec = 0;
/* Humidity Integral&Decimal */
UINT8 HumidityInt = 0;
UINT8 HumidityDec = 0;
/* Temperature */
INT16 Temperature = 0;
/* Humidity */
UINT16 Humidity = 0;
/* Checksum status */
UINT8 ChecksumSts = CHECKSUM_FAILED;
/* Temp&Hum strings */
UINT8 TemperatureIntStr[3];
UINT8 TemperatureDecStr[3];
UINT8 TemperatureStr[5];
UINT8 HumidityIntStr[3];
UINT8 HumidityDecStr[3];
UINT8 HumidityStr[5];

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
         case REQUEST_TEMPERATURE_RAW:
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = TemperatureInt;
            RepBuffer[REP_PAR_2_INDEX] = TemperatureDec;
            /* Set response lenght */
            RepBufferLen = REQUEST_TEMPERATURE_RAW_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);            
            break;
            
         case REQUEST_TEMPERATURE_ASCII:
            /* Pre-load the buffer value */
            SetBuffer(TemperatureStr,'0',5);            
            /* Chain convert temp data */
            itoa(TemperatureIntStr,TemperatureInt,10);
            itoa(TemperatureDecStr,TemperatureDec,10);     
            strcpy(TemperatureStr,TemperatureIntStr);
            strcat(TemperatureStr,".");
            strcat(TemperatureStr,TemperatureDecStr);                                     
            /* Copy data to reply buffer */
            memcpy(RepBuffer,TemperatureStr,REQUEST_TEMPERATURE_ASCII_LEN);            
            /* Set response lenght */
            RepBufferLen = REQUEST_TEMPERATURE_ASCII_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);                 
            break;

         case REQUEST_HUMIDITY_RAW:
            /* Prepare response */                     
            RepBuffer[REP_PAR_1_INDEX] = HumidityInt;
            RepBuffer[REP_PAR_2_INDEX] = HumidityDec;              
            /* Set response lenght */
            RepBufferLen = REQUEST_HUMIDITY_RAW_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);             
            break;

         case REQUEST_HUMIDITY_ASCII: 
            /* Pre-load the buffer value */
            SetBuffer(HumidityStr,'0',5);
            /* Chain convert hum data */
            itoa(HumidityIntStr,HumidityInt,10);
            itoa(HumidityDecStr,HumidityDec,10);
            strcpy(HumidityStr,HumidityIntStr);
            strcat(HumidityStr,".");
            strcat(HumidityStr,HumidityDecStr);                
            /* Copy data to reply buffer */
            memcpy(RepBuffer,HumidityStr,REQUEST_HUMIDITY_ASCII_LEN);                
            /* Set response lenght */
            RepBufferLen = REQUEST_HUMIDITY_ASCII_LEN;
            /* Notify */
            GenerateEvt(&I2cReplyPacketReady);                 
            break;

         case REQUEST_CHECKSUM_STS:
            /* Prepare response */
            RepBuffer[REP_PAR_1_INDEX] = ChecksumSts;
            /* Set response lenght */
            RepBufferLen = REQUEST_CHECKSUM_LEN;
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
* Author:       F.Ficili
* Description:  Application main task.
* Date:         31/07/16
************************************************************************/
void App_MainTask (UINT8 Options)
{    
   WpuChType WpuCh = {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF};
   AnalogChSelType AnalogChSel = {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF};
   AdcConfigType AdcConfig;      
   static UINT8 Counter = 0;
   
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
         break;

      /* System Normal operation Phase */
      case RunningState:         
         /* Inc counter */
         Counter++;
         /* Check counter */
         if (Counter > READING_TMOUT_MS)
         {            
            /* Reset counter */
            Counter = 0;          
            /* Verify Checksum */   
            if (Dht_GetSensorData())
            {
#if (SENSOR_TYPE == DHT11) 
               /* Check for ranges (temp min is not checked) */
               if ((Sensor.Data.HumidityInt <= (HUM_MAX/10)) &&
                  (Sensor.Data.HumidityInt >= (HUM_MIN/10)))
               {       
#ifdef TEMP_SIGN_TEST
                  Sensor.Data.TemperatureHigh |= 0x80;
#endif               
                  /* Upate Temp&Humidity and CksSts data */
                  TemperatureInt = Sensor.Data.TemperatureInt;
                  TemperatureDec = Sensor.Data.TemperatureDec;
                  HumidityInt = Sensor.Data.HumidityInt;
                  HumidityDec = Sensor.Data.HumidityDec;
                  ChecksumSts = CHECKSUM_OK;                  
               }          

#endif   /* (SENSOR_TYPE == DHT11) */  

#if (SENSOR_TYPE == DHT22)    
               /* Check for ranges (temp min is not checked) */
               if (((Sensor.Data.HumidityLow | (Sensor.Data.HumidityHigh << 8)) <= HUM_MAX) &&
                  ((Sensor.Data.HumidityLow | (Sensor.Data.HumidityHigh << 8)) >= HUM_MIN) &&
                  ((Sensor.Data.TemperatureLow | ((Sensor.Data.TemperatureHigh & 0x7F) << 8)) <= TEMP_MAX))
               {           
#ifdef TEMP_SIGN_TEST
                  Sensor.Data.TemperatureHigh |= 0x80;
#endif                         
                  /* Get sign */   
                  TempSign = (Sensor.Data.TemperatureHigh & 0x80) >> 7;                   
                  /* Upate Temp&Humidity and CksSts data */    
                  Temperature = Sensor.Data.TemperatureLow | ((Sensor.Data.TemperatureHigh & 0x7F) << 8);
                  Humidity = Sensor.Data.HumidityLow | (Sensor.Data.HumidityHigh << 8);               
                  /* Get integer and decimal part */
                  TemperatureInt = Temperature/10;       
                  TemperatureDec = Temperature%10;
                  HumidityInt = Humidity/10;
                  HumidityDec = Humidity%10;                              
                  ChecksumSts = CHECKSUM_OK;
                  /* Adjust sign if needed */
                  if (TempSign)
                  {
                     TemperatureInt = -TemperatureInt;
                  }                       
               }
#endif   /* (SENSOR_TYPE == DHT22) */                                                
            }
            else
            {
               /* Cheksuma just failed */
               ChecksumSts = CHECKSUM_FAILED;
            }
         }                  
         /* Main app state machine */
         AppSm();
         break;

      /* Default */
      default:
         break;
   }
}
  