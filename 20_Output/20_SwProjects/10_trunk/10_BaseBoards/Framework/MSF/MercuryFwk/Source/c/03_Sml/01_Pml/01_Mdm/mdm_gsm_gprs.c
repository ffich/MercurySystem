/************************************************************************
*                           Modem GSM GPRS                              *
*************************************************************************
* FileName:         mdm_gsm_gprs.c                                      *
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
* F.Ficili     11/09/16    1.0          First release                   *
************************************************************************/

/************************************************************************
* Notes: Typical Strings 
* 
* Uart_WriteConstString(1,"AT+IPR=115200\r");      --> Set baud rate
* Uart_WriteConstString(1,"AT+CPIN=\"3144\"\r");   --> Unlock with pin
* Uart_WriteConstString(1,"ATD00393925814985;\r"); --> Make phone call
* Uart_WriteConstString(1,"ATE0\r");               --> Echo mode off
* 
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "mdm_gsm_gprs.h"

#if (MODEM_USED == GSM_GPRS_MDM)
/************************************************************************
* Defines
************************************************************************/
/* SMS RI pulse ms */
#define MDM_ON_DELAY_MS                                  (((UINT16)(3000))/MDM_TASK_PERIOD_MS)
/* Delay for config steps */
#define MDM_CFG_STEP_DELAY_MS                            (((UINT16)(1000))/MDM_TASK_PERIOD_MS)
/* Check delay */
#define MDM_IN_CHECK_DELAY_MS                            2000u
#define MDM_FINAL_DELAY_MS                               1000u

/* Modem Events */
#define EVENTS_NUM                                       ((UINT8)(3))
#define RING_STRING_IDX                                  ((UINT8)(0))
#define SMS_STRING_IDX                                   ((UINT8)(1))
#define CALL_READY_STRING_IDX                            ((UINT8)(2))

/* Modem constants */
#define PHONE_CALL_UART_MSG_LEN                          ((UINT8)(25))
#define PIN_MSG_LEN                                      ((UINT8)(13))

#define LOW_BAUD                                         1
#define HIGH_BAUD                                        2
#define BAUD                                             LOW_BAUD

#define GSM_GPRS_MDM_LOW_BAUD_RATE                       ((UINT32)(9600))
#define GSM_GPRS_MDM_HIGH_BAUD_RATE                      ((UINT32)(115200))

/************************************************************************
* Typedefs
************************************************************************/
/* TurnOnMdmState */
typedef enum _TurnOnMdmState
{
   TurnOnDelay,
   TurnOnModuleReq,
   Stop
} TurnOnMdmStateType;

/* GetSms state machine state type */
typedef enum _GetSmsState
{
   GetSms_CheckReq,
   GetSms_SendAtCmd,
   GetSms_StoreHeader,
   GetSms_StoreText,   
} GetSmsStateType;

/* Modem config state type */
typedef enum _MdmConfigState
{
   MdmCfg_Init,
   MdmCfg_ChangeBaud,
   MdmCfg_SetMdmEchoMode,
   MdmCfg_SetMdmSmsMode,
   MdmCfg_Delay,
   MdmCfg_UpdateMdmSts,
   MdmCfg_Stop
} MdmConfigStateType;

typedef enum _CheckMdmStsType
{
   MdmCh_SetInitialDelay,
   MdmCh_InitialDelay,          
   MdmCh_SendATE1,
   MdmCh_WaitEcho,
   MdmCh_Delay,
   MdmCh_Done   
} CheckMdmStsType;

typedef enum _SendSms_StateType
{
   SendSms_WaitEvt,
   SendSms_CmgsCmd,
   SendSms_EnterCmd,
   SendSms_WaitConf,
   SendSms_SendSmsText, 
   SendSms_Delay,        
} SendSms_StateType;

/* Sms struct type */
typedef struct _Sms
{
   UINT8 HeaderBuffer[HEADER_BUFF_LEN];
   UINT8 MessageBuffer[MESSAGE_BUFF_LEN];
   UINT8 HeadLen;
   UINT8 TextLen;
} SmsRxType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* UART configuration variable */
static UartConfigType UartConfig;
/* Sms */
static SmsRxType Mdm_SmsRx;
/* Com check status */
static UINT8 ComChecCompleted = STD_FALSE;
/* Send SMS service locals */
static UINT8 SmsLen;
static UINT8 PhoneNumbLen;
static UINT8 PhoneNumber[20];
static UINT8 MessageText[50];

/* Local Events */
EventStructureType SendSmsEvent = {EventIdle};

/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Global modem status */
MdmStatusType MdmStatus = ModemOff;
/* Events */
EventStructureType RingEvent = {EventIdle};
EventStructureType SmsEvent = {EventIdle};
EventStructureType GetSmsText = {EventIdle};
EventStructureType SmsTxtReady = {EventIdle};
EventStructureType MdmStsReady = {EventIdle};
EventStructureType NewStringReceived = {EventIdle};
EventStructureType NewMdmEvtParserRequest = {EventIdle};
EventStructureType NewStringMdmTerminal = {EventIdle};

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
void TurnOnModem (void);
void ConfigureModem (void);
void UartSetConfig (void);
void GetSmsSm (void);
void SendSmsSm (void);
void ModemEvtParser (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/
/************************************************************************
* Function:     UartSetConfig
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Set the Uart module configuration.
* Date:         11/09/16
************************************************************************/
void UartSetConfig (void)
{   
   UartConfig.UartEnable = SERIAL_PORT_ENABLED;
   UartConfig.UartMode = ASYNC_MODE;
#if (BAUD == LOW_BAUD)
   UartConfig.BaudRateMode = BAUD_RATE_8_BIT;
#elif (BAUD == HIGH_BAUD)
   UartConfig.BaudRateMode = BAUD_RATE_16_BIT;
#endif   
#if (BAUD == LOW_BAUD)
   UartConfig.HighBaudMode = LOW_SPEED;
#elif (BAUD == HIGH_BAUD)
   UartConfig.HighBaudMode = HIGH_SPEED;  
#endif   
   UartConfig.Tx9bitMode = TX_9_BIT_DISABLED;
   UartConfig.Rx9bitMode = RX_9_BIT_DISABLED;
   UartConfig.TxEnable = TX_ENABLED;
   UartConfig.RxEnable = RX_ENABLED;
   UartConfig.SendBreakEnable = UART_NOT_SEND_BREAK;
   UartConfig.AutobaudEnable = AUTOBAUD_DIS;
   UartConfig.WakeUpEnable = UART_WAKE_UP_DIS;
   UartConfig.TxInterruptEnable = TX_INTERRUPT_DISABLED;
   UartConfig.RxInterruptEnable = RX_INTERRUPT_ENABLED;
   UartConfig.McuBusClock = SYSTEM_FREQUENCY_VALUE_HZ;
#if (BAUD == LOW_BAUD)   
   UartConfig.DesiredBaudRate = GSM_GPRS_MDM_LOW_BAUD_RATE;
#elif (BAUD == HIGH_BAUD)   
   UartConfig.DesiredBaudRate = GSM_GPRS_MDM_HIGH_BAUD_RATE;
#endif    
}

/************************************************************************
* Function:     UartSetConfigLow
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Set the Uart module configuration for low baud rate.
* Date:         11/09/16
************************************************************************/
void UartSetConfigLow (void)
{   
   UartConfig.UartEnable = SERIAL_PORT_ENABLED;
   UartConfig.UartMode = ASYNC_MODE;
   UartConfig.BaudRateMode = BAUD_RATE_8_BIT;
   UartConfig.HighBaudMode = LOW_SPEED; 
   UartConfig.Tx9bitMode = TX_9_BIT_DISABLED;
   UartConfig.Rx9bitMode = RX_9_BIT_DISABLED;
   UartConfig.TxEnable = TX_ENABLED;
   UartConfig.RxEnable = RX_ENABLED;
   UartConfig.SendBreakEnable = UART_NOT_SEND_BREAK;
   UartConfig.AutobaudEnable = AUTOBAUD_DIS;
   UartConfig.WakeUpEnable = UART_WAKE_UP_DIS;
   UartConfig.TxInterruptEnable = TX_INTERRUPT_DISABLED;
   UartConfig.RxInterruptEnable = RX_INTERRUPT_ENABLED;
   UartConfig.McuBusClock = SYSTEM_FREQUENCY_VALUE_HZ; 
   UartConfig.DesiredBaudRate = GSM_GPRS_MDM_LOW_BAUD_RATE;
}

/************************************************************************
* Function:     UartSetConfigHigh
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Set the Uart module configuration for high baud rate.
* Date:         11/09/16
************************************************************************/
void UartSetConfigHigh (void)
{  
   UartConfig.UartEnable = SERIAL_PORT_ENABLED;
   UartConfig.UartMode = ASYNC_MODE;
   UartConfig.BaudRateMode = BAUD_RATE_16_BIT;  
   UartConfig.HighBaudMode = HIGH_SPEED;   
   UartConfig.Tx9bitMode = TX_9_BIT_DISABLED;
   UartConfig.Rx9bitMode = RX_9_BIT_DISABLED;
   UartConfig.TxEnable = TX_ENABLED;
   UartConfig.RxEnable = RX_ENABLED;
   UartConfig.SendBreakEnable = UART_NOT_SEND_BREAK;
   UartConfig.AutobaudEnable = AUTOBAUD_DIS;
   UartConfig.WakeUpEnable = UART_WAKE_UP_DIS;
   UartConfig.TxInterruptEnable = TX_INTERRUPT_DISABLED;
   UartConfig.RxInterruptEnable = RX_INTERRUPT_ENABLED;
   UartConfig.McuBusClock = SYSTEM_FREQUENCY_VALUE_HZ;
   UartConfig.DesiredBaudRate = GSM_GPRS_MDM_HIGH_BAUD_RATE;   
}

/************************************************************************
* Function:     CheckModem
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Check if the modem is on or not.
* Date:         03/06/17
************************************************************************/
void CheckModem (void)
{
   static CheckMdmStsType CheckMdmSts = MdmCh_SetInitialDelay;
   static SwTimerType Timeout = SW_TIMER_INIT_DIS;
   
   switch (CheckMdmSts)
   {
      case MdmCh_SetInitialDelay:
         /* Start timeout timer */
         OsTmr_StartTimer(&Timeout,2000);         
         /* Switch state */
         CheckMdmSts = MdmCh_InitialDelay;         
         break;   
         
      case MdmCh_InitialDelay:
         /* Check timer */
         if (OsTmr_GetTimerStatus(&Timeout) == SwTimerExpired)
         {         
            /* Switch state */
            CheckMdmSts = MdmCh_SendATE1;               
         }                  
         break;  
      
      case MdmCh_SendATE1:
         /* Set echo mode off */
         Uart_WriteConstString(1,"AT\r");
         /* Start timeout timer */
         OsTmr_StartTimer(&Timeout,MDM_IN_CHECK_DELAY_MS);         
         /* Switch state */
         CheckMdmSts = MdmCh_WaitEcho;
         break;
                         
      case MdmCh_WaitEcho:
         /* If a new string is received on Mdm UART */
         if (ReceiveEvt(&NewStringReceived))
         {
            /* In case of positive resp. */
            if ((RxBuffer[0] == 'O') && (RxBuffer[1] == 'K'))
            {
               /* Generate modem on pulse request */
               GenerateEvt(&MdmOnOffPulseRequest);
               /* Start timeout timer */
               OsTmr_StartTimer(&Timeout,MDM_FINAL_DELAY_MS);                   
               /* Switch state */
               CheckMdmSts = MdmCh_Delay;
            }
         }
         else if (OsTmr_GetTimerStatus(&Timeout) == SwTimerExpired)
         {
            /* Update modem status */
            MdmStatus = ModemOff;
            /* Update ComChecCompleted flag */
            ComChecCompleted = STD_TRUE;
            /* Switch state */
            CheckMdmSts = MdmCh_Done;            
         }
         else
         {
            /* Nothing */
         }            
         break;
         
      case MdmCh_Delay:
         /* Check timer */
         if (OsTmr_GetTimerStatus(&Timeout) == SwTimerExpired)
         {
            /* Update modem status */
            MdmStatus = ModemOff;
            /* Update ComChecCompleted flag */
            ComChecCompleted = STD_TRUE;            
            /* Switch state */
            CheckMdmSts = MdmCh_Done;                    
         }                  
         break;         
         
      case MdmCh_Done:
         /* Do nothing */
         break;
         
      default:
         break;         
   }
}  

/************************************************************************
* Function:     TurnOnModemDelay
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Function for modem on request generation.
* Date:         17/09/16
************************************************************************/
void TurnOnModem (void)
{  
   static TurnOnMdmStateType TurnOnMdmState = TurnOnDelay;
   static UINT16 Counter = 0;
   
   switch (TurnOnMdmState)
   {
      case TurnOnDelay:
         /* Inc counter */
         Counter++;     
         /* Wait for initial delay */
         if (Counter >= MDM_ON_DELAY_MS)
         {
            /* Swithc state */
            TurnOnMdmState = TurnOnModuleReq;
         }         
         break;
         
      case TurnOnModuleReq:
         /* Generate modem on pulse request */
         GenerateEvt(&MdmOnOffPulseRequest);
         /* Update modem status */
         MdmStatus = ModemOn;
         /* Swithc state */
         TurnOnMdmState = Stop;         
         break;
         
      case Stop:
         break;
         
      default:
         break;
   }
}

/************************************************************************
* Function:     ConfigureModem
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Send defined configuration to the modem
* Date:         18/09/16
************************************************************************/
void ConfigureModem (void)
{
   static MdmConfigStateType MdmConfigState = MdmCfg_Init;
   static MdmConfigStateType NextState = MdmCfg_Init;
   static UINT16 DelayValueMs = 0;
   static UINT16 DelayCounter = 0;
   
   switch (MdmConfigState)
   {
      case MdmCfg_Init:
         /* Check modem status */
         if (MdmStatus == ModemReady)
         {
            /* Switch state */
            MdmConfigState = MdmCfg_ChangeBaud;
         }
         break;         

      case MdmCfg_ChangeBaud:
         /* Change module baud to 9600 */
         Uart_WriteConstString(1,"AT+IPR=9600\r");    
         /* Align UART baud */
         UartSetConfigLow();
         /* Re-Init UART Interface */
         Uart_InitUartModule(1,&UartConfig);             
         /* Set delay */
         DelayValueMs = MDM_CFG_STEP_DELAY_MS;
         /* Set next state and switch */
         NextState = MdmCfg_SetMdmEchoMode;         
         MdmConfigState = MdmCfg_Delay;           
         break;
         
      case MdmCfg_SetMdmEchoMode:        
         /* Set echo mode off */
         Uart_WriteConstString(1,"ATE0\r");
         /* Set delay */
         DelayValueMs = MDM_CFG_STEP_DELAY_MS;
         /* Set next state and switch */
         NextState = MdmCfg_UpdateMdmSts;         
         MdmConfigState = MdmCfg_Delay;         
         break;
         
      case MdmCfg_Delay:
         /* Inc counter */
         DelayCounter++;
         /* Check delay */
         if (DelayCounter >= DelayValueMs)
         {
            /* Reset counter */
            DelayCounter = 0;
            /* Go to next state */
            MdmConfigState = NextState;
         }         
         break;
                                   
      case MdmCfg_UpdateMdmSts:
         /* Update Modem status */
         MdmStatus = ModemConfigured;
         /* Modem ready event */
         GenerateEvt(&MdmStsReady);
         /* Swithc status */
         MdmConfigState = MdmCfg_Stop;         
         break;                 
         
      case MdmCfg_Stop:
         break;
         
      default:
         break;         
   }
}  

/************************************************************************
* Function:     ModemEvtParser
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Function to parse modem events.
* Date:         17/09/16
************************************************************************/
void ModemEvtParser (void)
{
   UINT8 RingString[] = {'R','I','N','G'}; 
   UINT8 SmsString[] = {'+','C','M','T','I'};
   UINT8 CallReadyString[] = {'C','a','l','l',' ','R','e','a','d','y'};   
   UINT8 CompareIdx = 0;
   UINT8 Match = FALSE;

   /* If a new string is received */
   if (ReceiveEvt(&NewMdmEvtParserRequest))
   {   
      for (CompareIdx = 0; CompareIdx < EVENTS_NUM; CompareIdx++)
      {
         switch (CompareIdx)
         {
            case RING_STRING_IDX:
               /* Check string */
               if (StringCompare(RingString, RxBuffer, sizeof(RingString)))
               {
                  /* String match */
                  GenerateEvt(&RingEvent);
                  /* Update match */
                  Match = TRUE;                  
               }
               break;

            case SMS_STRING_IDX:
               /* Check string */
               if (StringCompare(SmsString, RxBuffer, sizeof(SmsString)))
               {
                  /* String match */
                  GenerateEvt(&SmsEvent);
                  /* Update match */
                  Match = TRUE;   
               }
               break;
               
            case CALL_READY_STRING_IDX:
               /* Check string */               
               if (StringCompare(CallReadyString, RxBuffer, sizeof(CallReadyString)))                  
               {
                  /* Update modem status */
                  MdmStatus = ModemReady;
                  /* Update match */
                  Match = TRUE;   
               }
               break;               

            default:
               break;               
         }
      }
           
      /* Check overall result */
      if (Match == TRUE)
      {
#if (TERMINAL_MODE != TRANSPARENT_MODE)  
         /* Clear buffer */
         Uart_ClearRxBuffer();
#endif
      }      
   }
}

/************************************************************************
* Function:     GetSmsSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Function to get the content of an SMS.
* Date:         18/09/16
************************************************************************/
void GetSmsSm (void)
{   
   static GetSmsStateType GetSmsState = GetSms_CheckReq;
   
   /* If the modem is configured */
   if (MdmStatus == ModemConfigured)
   {
      switch (GetSmsState)
      {
         case GetSms_CheckReq:
            /* Check for request */
            if (ReceiveEvt(&GetSmsText))
            {
               /* Swithc state */
               GetSmsState = GetSms_SendAtCmd;            
            }         
            break;

         case GetSms_SendAtCmd:
            /* Send SMS read AT cmd */
            Uart_WriteConstString(1,"AT+CMGR=1\r");
            /* Swithc state */
            GetSmsState = GetSms_StoreHeader;              
            break;

         case GetSms_StoreHeader:
            /* If a new string is received on Mdm UART */
            if (ReceiveEvt(&NewStringReceived))
            {
               if (RxBuffer[0] != 0)
               {
                  /* Copy Header */
                  StringCopy(RxBuffer,Mdm_SmsRx.HeaderBuffer,RxLen);
                  /* Update Header Length */
                  Mdm_SmsRx.HeadLen = RxLen;
                  /* Swithc state */
                  GetSmsState = GetSms_StoreText;                 
               }              
            }
            break;

         case GetSms_StoreText:
            /* If a new string is received on Mdm UART */
            if (ReceiveEvt(&NewStringReceived))
            {
               /* Copy Text */
               StringCopy(RxBuffer,Mdm_SmsRx.MessageBuffer,RxLen);
               /* Update Text Length */
               Mdm_SmsRx.TextLen = RxLen;               
               /* Swithc state */
               GetSmsState = GetSms_CheckReq;
               /* Send decode complete evt */
               GenerateEvt(&SmsTxtReady);
            }         
            break;

         default:
            break;         
      }    
   }
}

/************************************************************************
* Function:     SendSmsSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Function to send and SMS to a specific number.
* Date:         27/10/17
************************************************************************/
void SendSmsSm (void)
{
   static SendSms_StateType SendSms_State = SendSms_WaitEvt;
   static SendSms_StateType SendSms_NextState = SendSms_WaitEvt;   
   static SwTimerType SwTimer = SW_TIMER_INIT_EN;   
   UINT8 UartMessage[50];
   
   /* If the modem is configured */
   if (MdmStatus == ModemConfigured)
   {   
      switch (SendSms_State)
      {
         case SendSms_WaitEvt:
            /* If send sms event is received */
            if (ReceiveEvt(&SendSmsEvent))
            {
               /* Switch state */
               SendSms_State = SendSms_CmgsCmd;
            }
            break;

         case SendSms_CmgsCmd:
            /* Add AT message header */
            StringCopy((void*)("AT+CMGS=\""),UartMessage,9);
            /* Add phone number */
            StringCopy(PhoneNumber,&UartMessage[9],PhoneNumbLen);
            /* Add AT message tail */
            UartMessage[PhoneNumbLen + 9] = '"';         
            UartMessage[PhoneNumbLen + 10] = '\r';         
            /* Send CMGS command */
            Uart_WriteStringLen(UART_MODULE_1, UartMessage,(PhoneNumbLen + 11));          
            /* Switch state with delay */
            OsTmr_StartTimer(&SwTimer,2500);         
            SendSms_State = SendSms_Delay;  
            SendSms_NextState = SendSms_EnterCmd; 
            break;

         case SendSms_EnterCmd:
            /* Add enter character */
            UartMessage[0] = '\n'; 
            UartMessage[1] = '\r';         
            /* Send enter command */
            Uart_WriteStringLen(UART_MODULE_1, UartMessage,2);                   
            /* Switch state */
            SendSms_State = SendSms_WaitConf;           
            break;

         case SendSms_WaitConf:
            /* If a new string is received on Mdm UART */
            if (ReceiveEvt(&NewStringReceived))
            {
               /* If the promt character is received */
               if (RxBuffer[0] == '>')
               {
                  /* Switch state */
                  SendSms_State = SendSms_SendSmsText;                 
               }
            }
            break;

         case SendSms_SendSmsText:
            /* Copy sms text */
            StringCopy(MessageText,UartMessage,SmsLen);   
            /* Add tail */
            UartMessage[SmsLen] = 0x1A;
            /* Send enter command */
            Uart_WriteStringLen(UART_MODULE_1, UartMessage,(SmsLen + 1));              
            /* Switch state */
            SendSms_State = SendSms_WaitEvt;           
            break;

         case SendSms_Delay:
            /* Check if the timer expired */
            if (OsTmr_GetTimerStatus(&SwTimer) == SwTimerExpired)
            {
               /* Switch state */
               SendSms_State = SendSms_NextState;
            }         
            break;         

         default:
            break;         
      }
   }
}   

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     Mdm_MainTask
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage Modem communication.
* Date:         11/09/16
************************************************************************/
void Mdm_MainTask (UINT8 Options)
{    
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* Set UART configuration */
         UartSetConfig();
         /* Init UART Interface */
         Uart_InitUartModule(1,&UartConfig);
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Check modem status */
         CheckModem();
         /* If the modem is off... */
         if ((ComChecCompleted == STD_TRUE) && (MdmStatus == ModemOff))
         {
            /* ...turn it on */
            TurnOnModem();         
         }
         /* Configure modem */
         ConfigureModem();
         /* Modem Evt messages parser */
         ModemEvtParser();
         /* SMS get state machine */
         GetSmsSm();
         /* SMS send state machine */
         SendSmsSm();
         break;

      /* Default */
      default:
         break;
   }
}

/************************************************************************
* Function:     Mdm_PinUnlock
* Input:        UINT8 *PIN
* Output:       None
* Author:       F.Ficili
* Description:  API to to unlock the SIM using PIN.
* Date:         07/10/16
************************************************************************/
void Mdm_PinUnlock (const UINT8 *PIN)
{
   UINT8 UartMessage[PIN_MSG_LEN];
   UINT8 Index = 0;
   
   /* Add AT message header */
   UartMessage[0] = 'A';
   UartMessage[1] = 'T';
   UartMessage[2] = '+';
   UartMessage[3] = 'C';
   UartMessage[4] = 'P';
   UartMessage[5] = 'I';   
   UartMessage[6] = 'N';
   UartMessage[7] = '=';    
   
   /* Copy phone number */
   for (Index = 0; Index < 4; Index++)
   {
      UartMessage[Index + 8] = *(PIN + Index);
   }
   
   /* Add AT message tail */
   UartMessage[Index + 8] = 0x0d;
   UartMessage[Index + 9] = NULL;
   
   /* Send the PIN */
   Uart_WriteString(1, UartMessage);
}

/************************************************************************
* Function:     Mdm_SetSmsFormat
* Input:        UINT8 TextFormat
* Output:       None
* Author:       F.Ficili
* Description:  Function to set the SMS format type (text ON/OFF).
* Date:         04/06/17
************************************************************************/
void Mdm_SetSmsFormat (UINT8 TextFormat)
{
   UINT8 UartMessage[20];
   UINT8 Index = 0;
   
   /* Add AT message header */
   UartMessage[0] = 'A';
   UartMessage[1] = 'T';
   UartMessage[2] = '+';
   UartMessage[3] = 'C';
   UartMessage[4] = 'M';
   UartMessage[5] = 'G';   
   UartMessage[6] = 'F';
   UartMessage[7] = '=';
   
   /* Select tetxt format */
   if (TextFormat == STD_ON)
   {
      UartMessage[8] = '1';
   }
   else
   {
      UartMessage[8] = '0';
   }      
   /* Add tail */
   UartMessage[9] = '\r';   
   UartMessage[10] = NULL;   
   
   /* Set SMS format */
   Uart_WriteString(1, UartMessage);
}

/************************************************************************
* Function:     Mdm_RequestSmsData
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  API to request the SMS data to the modem.
* Date:         25/09/16
************************************************************************/
void Mdm_RequestSmsData (void)
{
   /* Request sms text */
   GenerateEvt(&GetSmsText);   
}  

/************************************************************************
* Function:     Mdm_GetSmsData
* Input:        UINT8 *MessageText
* Output:       GetSmsStatusType 
* Author:       F.Ficili
* Description:  API to get the SMS data to the modem. The data must be 
*               requested before using the Mdm_RequestSmsData API.
* Date:         25/09/16
************************************************************************/
GetSmsStatusType Mdm_GetSmsData (UINT8 *MessageText)
{
   GetSmsStatusType GetSmsStatus = SmsDataNotReady;
   
   /* Once the SMS is decocded... */
   if (ReceiveEvt(&SmsTxtReady))
   {
      /* ...Copy text data */
      StringCopy(Mdm_SmsRx.MessageBuffer,MessageText,Mdm_SmsRx.TextLen);
      /* Update status */
      GetSmsStatus = SmsDataReady;
   }

   /* Return status */
   return GetSmsStatus; 
}

/************************************************************************
* Function:     Mdm_SendSmsData
* Input:        UINT8 *PhoneNmb
*               UINT8 PhoneNmbLen
*               UINT8 *MsgTxt
*               UINT8 MsgTxtLen 
* Output:       None
* Author:       F.Ficili
* Description:  Function to send the SMS triggered by application.
* Date:         27/10/17
************************************************************************/
void Mdm_SendSmsData (UINT8 *PhoneNmb, UINT8 PhoneNmbLen, UINT8 *MsgTxt, UINT8 MsgTxtLen)
{
   /* Copy Phone Number */
   StringCopy(PhoneNmb,PhoneNumber,PhoneNmbLen);
   PhoneNumbLen = PhoneNmbLen;
   /* Copy SMS text  */
   StringCopy(MsgTxt,MessageText,MsgTxtLen);   
   SmsLen = MsgTxtLen;
   /* Genertae send event */
   GenerateEvt(&SendSmsEvent);
}

/************************************************************************
* Function:     Mdm_IsSmsReceived
* Input:        None
* Output:       SmsRxStsType
* Author:       F.Ficili
* Description:  API to check if an SMS has been received.
* Date:         31/12/18
************************************************************************/
SmsRxStsType Mdm_IsSmsReceived (void)
{
   SmsRxStsType Ret = SmsNotReceived;
   
   /* Check the ring event */
   if (ReceiveEvt(&SmsEvent))
   {
      Ret = SmsReceived;
   }
   
   return Ret;
}

/************************************************************************
* Function:     Mdm_MakePhoneCall
* Input:        UINT8 *PhoneNumb
*               UINT8 PhoneNumbLen 
* Output:       None
* Author:       F.Ficili
* Description:  Function to make a phone call to specific number.
* Date:         25/09/16
************************************************************************/
void Mdm_MakePhoneCall (UINT8 *PhoneNumb, UINT8 PhoneNumbLen)
{
   UINT8 UartMessage[PHONE_CALL_UART_MSG_LEN];
   UINT8 Index = 0;
   
   /* Add AT message header */
   UartMessage[0] = 'A';
   UartMessage[1] = 'T';
   UartMessage[2] = 'D';
   
   /* Copy phone number */
   for (Index = 0; Index < PhoneNumbLen; Index++)
   {
      UartMessage[Index + 3] = *(PhoneNumb + Index);
   }
   
   /* Add AT message tail */
   UartMessage[Index + 3] = ';';
   UartMessage[Index + 4] = 0x0d;
   UartMessage[Index + 5] = NULL;
   
   /* Make the phone call */
   Uart_WriteString(1, UartMessage);
}

/************************************************************************
* Function:     Mdm_HangPhoneCall
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Function to close a phone call.
* Date:         25/09/16
************************************************************************/
void Mdm_HangPhoneCall (void)
{
   /* Hang the phone call */
   Uart_WriteConstString(1, "ATH\r");
}

/************************************************************************
* Function:     Mdm_GetPhoneCall
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Function to answer a phone call.
* Date:         25/09/16
************************************************************************/
void Mdm_GetPhoneCall (void)
{
   /* Answer to the phone call */
   Uart_WriteConstString(1, "ATA\r");
}

/************************************************************************
* Function:     Mdm_IsRinging
* Input:        None
* Output:       RingStsType
* Author:       F.Ficili
* Description:  API to check if the phone is ringing.
* Date:         31/12/18
************************************************************************/
RingStsType Mdm_IsRinging (void)
{
   RingStsType Ret = PhoneNotRinging;
   
   /* Check the ring event */
   if (ReceiveEvt(&RingEvent))
   {
      Ret = PhoneRinging;
   }
   
   return Ret;
}

#endif /* MODEM_USED == GSM_GPRS_MDM */