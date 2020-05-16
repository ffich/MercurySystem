/************************************************************************
*                             Terminal                                  *
*************************************************************************
* FileName:         term.c                                              *
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
* F.Ficili     21/05/17    1.0          First release                   *
* F.Ficili     01/11/17    1.1          Fixed a bug on terminal I2C     *
*                                       read. Added Timeout on read.    *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "term.h"
#include "stdlib.h"

#if (TERM_TASK_STS == STD_ON) 
/************************************************************************
* Defines
************************************************************************/
#define USB_BUFFER_SIZE                                     200u
#define TERM_BUFFER_SIZE                                    50u
#define PAYLOAD_BUFFER_SIZE                                 20u

/* Bytes offset */
#define OP_TYPE_OFF_BYTE_0                                  0u             
#define OP_TYPE_OFF_BYTE_1                                  OP_TYPE_OFF_BYTE_0 + 1u    /* 1 */ 
#define OP_TYPE_OFF_BYTE_2                                  OP_TYPE_OFF_BYTE_1 + 1u    /* 2 */
#define OP_OFF_BYTE                                         OP_TYPE_OFF_BYTE_2 + 1u    /* 3 */
#define LEN_OFF_BYTE                                        OP_OFF_BYTE + 2u           /* 5 */
#define OPT_OFF_BYTE                                        OP_OFF_BYTE + 2u           /* 5 */
#define ADDR_OFF_BYTE                                       LEN_OFF_BYTE + 3u          /* 8 */
#define OPT_SEL_OFF_BYTE                                    LEN_OFF_BYTE + 3u          /* 8 */
#define PAYLOAD_OFF_IDX                                     ADDR_OFF_BYTE + 3u         /* 11 */

/* System options */
#define FORMAT_OPT_BINARY                                   0u
#define FORMAT_OPT_ASCII                                    1u

#define DEC                                                 0u
#define HEX                                                 1u                     
#define BASE                                                HEX  

#define DATA_SIZE                                           50
#define STR_SIZE                                            4

#define TERM_TO_MS                                          3000u
#define TO_STRING                                           " Lazy device...you've been timed-out!!!\r\n" 

/************************************************************************
* Typedefs
************************************************************************/
/* Main SM state */
typedef enum _TermMainSmStateType
{
   Term_StartTmr,
   Term_InitDelay,
   Term_InitState,
   Term_WelcomeState,
   Term_InstDescState,
   Term_FwkVerState,
   Term_FwkBuildDateState,
   Term_SysInitState,
   Term_FinalState   
} TermMainSmStateType;

/* Terminal state type */
typedef enum _TermStateType
{
   Term_GetData,
   Term_PrepSlvI2cResp,
   Term_ReadSlvI2cData,
   Term_ReadSlvUartData,
} TermStateType;

/* Payload type */
typedef struct _PayloadType
{
   UINT8 Byte[4];
} PayloadType;

/************************************************************************
* LOCAL Variables
************************************************************************/
static const UINT8 Welcome[] =  {
                                 "*************************************************\r\n"
                                 "                  MERCURY SYSTEM\r\n"
                                 "*************************************************\r\n"                                     
                                };

static const UINT8 MsgFormat[] = {
                                 "Host message format: \r\n"    
                                 "[op_type]:[op]:[len]:[addr]:[payload]\r\n"
                                 "\r\n" 
                                 "[op_type] = SO,SB\r\n"
                                 "[op] = R: read,W: write\r\n"
                                 "[len] = 2 byte ASCII\r\n"
                                 "[addr] = 2 byte ASCII\r\n"
                                 "[payload] = max 20 byte ASCII\r\n"
                                 "\r\n"  
/* Terminal Mode */                                         
#if (TERMINAL_MODE == COMMAND_MODE)
                                 "Terminal Mode: COMMAND MODE\r\n"
#elif (TERMINAL_MODE == TRANSPARENT_MODE)  
                                 "Terminal Mode: TRANSPARENT MODE\r\n"        
#endif                           
/* Modem Type */                                           
#if (MODEM_USED == NO_MDM)
                                 "Modem Type: None\r\n"
#elif (MODEM_USED == GSM_GPRS_MDM)  
                                 "Modem Type: GSM/GPRS\r\n"                        
#elif (MODEM_USED == BT_MDM)  
                                 "Modem Type: BluetoothE\r\n"        
#elif (MODEM_USED == WIFI_MDM)  
                                 "Modem Type: WiFi\r\n"        
#endif                                           
                                 "\r\n"                                             
                                 };

static const UINT8 MsgReady[] =  {
                                 "\r\nSystem Ready... \r\n\r\n"   
                                 };

/************************************************************************
* GLOBAL Variables
************************************************************************/


/************************************************************************
* LOCAL Function Prototypes
************************************************************************/


/************************************************************************
* LOCAL Function Implementations
************************************************************************/

#if ((MODEM_USED == BT_MDM) && (USB_CLASS_USED == USB_CLASS_CDC))
/************************************************************************
* Function:     BtTerminal
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  BT Terminal task.
* Date:         23/09/17
************************************************************************/
void BtTerminal (void)
{
   static UINT8 UsbBuffer[USB_BUFFER_SIZE];
   static UINT8 UartBuffer[USB_BUFFER_SIZE];
   UINT8 UartIndex = 0;
   UINT8 ReceivedBytes = 0;
   UINT8 Index;
   
   /* Return if USB is not ready */
   if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl == 1)) 
   {
      return;   
   }
   
   /* Get data */
   ReceivedBytes = getsUSBUSART(UsbBuffer,USB_BUFFER_SIZE);
   /* Buffer is not empty */
   if (ReceivedBytes > 0)
   {
      /* Special command mode */
      if (UsbBuffer[0] == '#')
      {
         if (UsbBuffer[1] == 'A')
         {
            /* AT mode */
            MdmBt_SetModemMode(AT_MODE);
            MdmBt_UartSetConfigAtMode();
         }
         else if (UsbBuffer[1] == 'C')
         {
            /* COMM mode */
            MdmBt_SetModemMode(COMM_MODE); 
            MdmBt_UartSetConfigComMode();
         }
      }
      else
      {
         /* Fill Uart buffer */
         for (Index = 0; Index < ReceivedBytes; Index++)
         {
            UartBuffer[UartIndex] = UsbBuffer[UartIndex];
            UartIndex++;         
         }      
         /* Add \r\n (required by the HC05 module) */
         MdmBt_SendBtMsgLen(UartBuffer,UartIndex);
         ClearBuffer(UartBuffer, sizeof(UartBuffer));         
      }                  
   }
   
   /* Check for new messages */
   if (MdmBt_ReceiveBtMsgTerminal(UartBuffer,&UartIndex) == BtMsg_Received)
   {
      /* Add line feed and newline */
      UartBuffer[UartIndex] = '\r';
      UartBuffer[UartIndex + 1] = '\n';
      /* If USB is Tx ready */
      if(USBUSARTIsTxTrfReady())
      {
         putUSBUSART(UartBuffer,(UartIndex + 2));
      }          
   }
}
#endif

#if ((MODEM_USED == WIFI_MDM) && (USB_CLASS_USED == USB_CLASS_CDC))
/************************************************************************
* Function:     WifiTerminal
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 WiFi Terminal task.
* Date:         23/09/17
************************************************************************/
void WifiTerminal (void)
{
   static UINT8 UsbBuffer[USB_BUFFER_SIZE];
   static UINT8 UartBuffer[USB_BUFFER_SIZE];
   static UINT8 FirstRecFlag = FALSE;
   UINT8 UartIndex = 0;
   UINT8 ReceivedBytes = 0;
   UINT8 Index;
   
   /* Return if USB is not ready */
   if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl == 1)) 
   {
      return;   
   }
   
   /* Get data */
   ReceivedBytes = getsUSBUSART(UsbBuffer,USB_BUFFER_SIZE);
   /* Buffer is not empty */
   if (ReceivedBytes > 0)
   {      
      /* Special command mode */
      if (UsbBuffer[0] == '!')
      {
         if (UsbBuffer[1] == 'R')
         {
            /* Restart modem */
            MdmWifi_RestartModem();
         }         
         else if (UsbBuffer[1] == 'N')
         {
            UartBuffer[0] = '\r';
            UartBuffer[1] = '\n';
            MdmWifi_SendWifiMsgLen(UartBuffer,2);
            ClearBuffer(UartBuffer,2);
         }
      }
      else
      {
         /* Fill Uart buffer */
         for (Index = 0; Index < ReceivedBytes; Index++)
         {
            UartBuffer[UartIndex] = UsbBuffer[UartIndex];
            UartIndex++;         
         }      
         MdmWifi_SendWifiMsgLen(UartBuffer,(UartIndex));
         ClearBuffer(UartBuffer, sizeof(UartBuffer));            
      }                   
   }
   
   /* If a new Wifi packet is received */
   if (MdmWifi_ReceiveWifiMsgTerminal(UartBuffer,&UartIndex) == WifiMsg_Received)
   {
      if (FirstRecFlag == FALSE)
      {
         FirstRecFlag = TRUE;    
      }
      else
      {
         /* Add line feed and newline */
         UartBuffer[UartIndex] = '\r';
         UartBuffer[UartIndex + 1] = '\n';   
         
         /* If USB is Tx ready */
         if(USBUSARTIsTxTrfReady())
         {
            putUSBUSART(UartBuffer,(UartIndex + 2));         
         }            
      }         
   }   
}
#endif

#if ((MODEM_USED == GSM_GPRS_MDM) && (USB_CLASS_USED == USB_CLASS_CDC))
/************************************************************************
* Function:     GsmGprsTerminal
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  GSM/GPRS Terminal task.
* Date:         25/10/17
************************************************************************/
void GsmGprsTerminal (void)
{
   static UINT8 UsbBuffer[USB_BUFFER_SIZE];
   static UINT8 UartBuffer[USB_BUFFER_SIZE];
   static UINT8 FirstRecFlag = FALSE;
   static UINT8 SMS = FALSE;
   UINT8 UartIndex = 0;
   UINT8 ReceivedBytes = 0;
   UINT8 Index;
   
   /* Return if USB is not ready */
   if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl == 1)) 
   {
      return;   
   }
   
   /* Get data */
   ReceivedBytes = getsUSBUSART(UsbBuffer,USB_BUFFER_SIZE);
   /* Buffer is not empty */
   if (ReceivedBytes > 0)
   {           
      /* Special command mode */
      if (UsbBuffer[0] == '#')
      {
         if (UsbBuffer[1] == 'S')
         {
            SMS = TRUE;
         }
      }
      else
      {
         /* Fill Uart buffer */
         for (Index = 0; Index < ReceivedBytes; Index++)
         {
            UartBuffer[UartIndex] = UsbBuffer[UartIndex];
            UartIndex++;         
         }      
         
         if (SMS == TRUE)
         {
            SMS = FALSE;
            UartBuffer[UartIndex - 1] = 0x1A;                
         }
         Uart_WriteStringLen(UART_MODULE_1,UartBuffer,UartIndex);
         ClearBuffer(UartBuffer, sizeof(UartBuffer));          
      }                  
   }
   
   /* If a new Mdm GSM/GPRS packet is received */
   if (ReceiveEvt(&NewStringMdmTerminal))
   {
      /* Add line feed and newline */
      RxBuffer[RxLen] = '\r';
      RxBuffer[RxLen + 1] = '\n';         
      /* If USB is Tx ready */
      if(USBUSARTIsTxTrfReady())
      {
         putUSBUSART(RxBuffer,(RxLen + 2));
      }            
   }   
}
#endif

#if ((USB_CLASS_USED == USB_CLASS_CDC) && (TERMINAL_MODE == TRANSPARENT_MODE))
/************************************************************************
* Function:     Terminal
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Terminal task. 
* Date:         21/05/17
************************************************************************/
void Terminal (void)
{
#if (MODEM_USED == BT_MDM)   
   /* BT Terminal task */
   BtTerminal();
#endif 
   
#if (MODEM_USED == WIFI_MDM)   
   /* Wifi Terminal task */
   WifiTerminal();
#endif    
   
#if (MODEM_USED == GSM_GPRS_MDM)   
   /* GSM/GPRS Terminal task */
   GsmGprsTerminal();
#endif    
}
#endif

#if ((USB_CLASS_USED == USB_CLASS_CDC) && (TERMINAL_MODE == COMMAND_MODE))
/************************************************************************
* Function:     Terminal
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 Terminal task. 
*               Host message format:
*               [op_type]:[op]:[len]:[addr]:[payload]
*               [op_type] = SO, SB, MB
*               [op] = R: read, W: write   
*               [len] = 2 byte ascii
*               [addr] = 2 byte ascii 
*               [payload] = max 20 byte ascii (depends on len patrameter)
*               
*               Example: SB:W:01:01:20 
* Date:         21/05/17
************************************************************************/
void Terminal (void)
{
   static TermStateType TermState = Term_GetData;
   UINT8 UsbRxBuffer[USB_BUFFER_SIZE];
   UINT8 UsbTxBuffer[USB_BUFFER_SIZE];   
   UINT8 TermTxBuffer[TERM_BUFFER_SIZE];
   static UINT8 TermRxBuffer[TERM_BUFFER_SIZE];   
   UINT8 Index;
   static UINT8 Len[4];
   static UINT8 PackageLen;
   static UINT8 Addr[4];
   static UINT8 SlaveAddr;
   static PayloadType Payload[PAYLOAD_BUFFER_SIZE];
   UINT8 ReceivedBytes = 0;
   /* System Options */
   static UINT8 FormatOption = FORMAT_OPT_BINARY;
   static UINT8 EchoOption = STD_ON;
   static SwTimerType SwTimer;
   
   /* Return if USB is not ready */
   if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
   
   /* Main switch */
   switch (TermState)
   {
      case Term_GetData:
         /* Get data from USB CDC */
         ReceivedBytes = getsUSBUSART(UsbRxBuffer,USB_BUFFER_SIZE);
         
         /* Buffer is not empty */
         if (ReceivedBytes > 0)
         {                       
            /* Check echo option */
            if (EchoOption == STD_ON)
            {
               /* Echo */
               if(USBUSARTIsTxTrfReady())
               {
                  UsbRxBuffer[ReceivedBytes] = '\r';
                  UsbRxBuffer[ReceivedBytes + 1] = '\n';
                  putUSBUSART(UsbRxBuffer,ReceivedBytes + 2);               
               }            
            }
            
            /* Check request type */
            if ((UsbRxBuffer[OP_TYPE_OFF_BYTE_0] == 'S')  && 
                (UsbRxBuffer[OP_TYPE_OFF_BYTE_1] == 'O')  && 
                (UsbRxBuffer[OP_TYPE_OFF_BYTE_2] == ':'))
            {
               /* Manage system options */
               if ((UsbRxBuffer[OP_OFF_BYTE] == 'G') && (UsbRxBuffer[OP_OFF_BYTE + 1] == ':'))
               {
                  /* Data Format */
                  if ((UsbRxBuffer[OPT_OFF_BYTE] == 'D') && (UsbRxBuffer[OPT_OFF_BYTE + 1] == 'F'))
                  {
                     if (UsbRxBuffer[OPT_SEL_OFF_BYTE] == 'A')
                     {
                        FormatOption = FORMAT_OPT_ASCII;
                     }
                     else if (UsbRxBuffer[OPT_SEL_OFF_BYTE] == 'B')
                     {
                        FormatOption = FORMAT_OPT_BINARY;
                     }                                          
                  }   
                  /* Echo Mode */
                  else if ((UsbRxBuffer[OPT_OFF_BYTE] == 'E') && (UsbRxBuffer[OPT_OFF_BYTE + 1] == 'M'))
                  {
                     if (UsbRxBuffer[OPT_SEL_OFF_BYTE] == '1')
                     {
                        EchoOption = STD_ON;
                     }
                     else if (UsbRxBuffer[OPT_SEL_OFF_BYTE] == '0')
                     {
                        EchoOption = STD_OFF;
                     }                                          
                  }                    
                  else
                  {
                     /* Do nothing */
                  }
               }                     
            }                            
            else if ((UsbRxBuffer[OP_TYPE_OFF_BYTE_0] == 'S')  && 
                     (UsbRxBuffer[OP_TYPE_OFF_BYTE_1] == 'B')  && 
                     (UsbRxBuffer[OP_TYPE_OFF_BYTE_2] == ':'))
            {            
               if ((UsbRxBuffer[OP_OFF_BYTE] == 'W') && (UsbRxBuffer[OP_OFF_BYTE + 1] == ':'))
               {              
                  /* Get Lenght */
                  Len[0] = UsbRxBuffer[LEN_OFF_BYTE];
                  Len[1] = UsbRxBuffer[LEN_OFF_BYTE + 1];
                  /* Get I2C Address */
                  Addr[0] = UsbRxBuffer[ADDR_OFF_BYTE];
                  Addr[1] = UsbRxBuffer[ADDR_OFF_BYTE + 1];                 
                  /* Make conversions */
                  PackageLen = atoi((void*)Len);
                  SlaveAddr = xtoi((void*)Addr);                  
                  /* Get Payload */
                  for (Index = 0; Index <= PackageLen; Index++)
                  {                                                      
                     Payload[Index].Byte[0] = UsbRxBuffer[2*Index + PAYLOAD_OFF_IDX];
                     Payload[Index].Byte[1] = UsbRxBuffer[2*Index + PAYLOAD_OFF_IDX + 1];
                  }
                  /* Convert Payload */
                  for (Index = 0; Index <= PackageLen; Index++)
                  {                                           
#if (BASE == DEC)
                     TermTxBuffer[Index] = atoi((void*)&(Payload[Index]));
#elif (BASE == HEX)
                     TermTxBuffer[Index] = strtol((void*)&(Payload[Index]), NULL, 16);
#endif
                  }                  

                  /* Send I2C message */
                  I2cSlv_SendI2cMsg(TermTxBuffer, SlaveAddr, PackageLen);              
               }
               else if ((UsbRxBuffer[OP_OFF_BYTE] == 'R') && (UsbRxBuffer[OP_OFF_BYTE + 1] == ':'))
               {
                  /* Get Lenght */
                  Len[0] = UsbRxBuffer[LEN_OFF_BYTE];
                  Len[1] = UsbRxBuffer[LEN_OFF_BYTE + 1];
                  /* Get I2C Address */
                  Addr[0] = UsbRxBuffer[ADDR_OFF_BYTE];
                  Addr[1] = UsbRxBuffer[ADDR_OFF_BYTE + 1];                 
                  /* Make conversions */
                  PackageLen = atoi((void*)Len);
                  SlaveAddr = xtoi((void*)Addr);                     
                  
                  /* Make a read request */
                  I2cSlv_ReceiveI2cMsg(TermRxBuffer, SlaveAddr, PackageLen);   
                               
                  /* Switch to read I2C data state */
                  TermState = Term_PrepSlvI2cResp;
               }            
            }
            else if ((UsbRxBuffer[OP_TYPE_OFF_BYTE_0] == 'M')  && 
                     (UsbRxBuffer[OP_TYPE_OFF_BYTE_1] == 'B')  && 
                     (UsbRxBuffer[OP_TYPE_OFF_BYTE_2] == ':'))
            {
                  /* TO_DO: add modem write */
            }
            
            /* ------------------------------ */
            /* -- Modems Events Management -- */
            /* ------------------------------ */
            
            /* BT Modem */
            #if (MODEM_USED == BT_MDM)
            /* If a new BT packet is received */
            if (ReceiveEvt(&(NewBtPacket)))
            {
               /* Add Tail */
               BtPacket.RxBuffer[BtPacket.RxByteNum] = '\r';
               BtPacket.RxBuffer[BtPacket.RxByteNum + 1] = '\n';
               /* Put data on USB */
               if(USBUSARTIsTxTrfReady())
               {
                   putUSBUSART(BtPacket.RxBuffer,(BtPacket.RxByteNum + 2));
               }      
            }  
            #endif

            /* GSM/GPRS Modem */
            #if (MODEM_USED == GSM_GPRS_MDM)

            #endif   
            
            /* WiFi Modem */
            #if (MODEM_USED == WIFI_MDM)

            #endif                
            
         }         
         break;
         
      case Term_PrepSlvI2cResp:         
         /* Prepare reply header */
         if(USBUSARTIsTxTrfReady())
         {
            UsbTxBuffer[0] = 'S';
            UsbTxBuffer[1] = 'B';
            UsbTxBuffer[2] = '[';
            UsbTxBuffer[3] = Addr[0];
            UsbTxBuffer[4] = Addr[1];
            UsbTxBuffer[5] = ']';
            UsbTxBuffer[6] = ':';
            putUSBUSART(UsbTxBuffer,7);

            /* Switch to read I2C data state */
            TermState = Term_ReadSlvI2cData;            
         }      
         break;
         
      case Term_ReadSlvI2cData:
         /* If the message is arrived */
         if (I2cSlv_I2cReadMsgSts() == MessageReceived)
         {
            /* Check format option */
            if (FormatOption == FORMAT_OPT_ASCII)
            {
               /* Convert data */ 
               for (Index = 0; Index <= PackageLen; Index++)
               {    
                  itoa(((void*)&(Payload[Index])),TermRxBuffer[Index],10);
               }            
            }

            /* Transfer data */
            if(USBUSARTIsTxTrfReady())
            {
               /* Check format option */
               if (FormatOption == FORMAT_OPT_ASCII)
               {       
                  Payload[(2*PackageLen)].Byte[3] = '\r';
                  Payload[(2*PackageLen)].Byte[2] = '\n';                  
                  /* Send back data over USB */
                  putUSBUSART((void*)Payload,(2*PackageLen) + 4);
               }
               else
               {
                  TermRxBuffer[PackageLen] = '\r';
                  TermRxBuffer[PackageLen + 1] = '\n';
                  /* Send back data over USB */
                  putUSBUSART(TermRxBuffer,PackageLen + 2);                     
               }                
               /* Switch to get data state */
               TermState = Term_GetData;                      
            }                               
         } 
         else
         {
            /* Check if the terminal timeout expifred */
            if (OsTmr_Wait(&SwTimer,TERM_TO_MS) == DelayExpired)
            {                  
               /* Prepare reply header */
               if(USBUSARTIsTxTrfReady())
               {                                   
                  strcpy(UsbTxBuffer,TO_STRING);
                  putUSBUSART(UsbTxBuffer,sizeof(TO_STRING)-1);    
                  /* Switch initial state */
                  TermState = Term_GetData;                 
               }                  
            }
         }
         break;
         
      case Term_ReadSlvUartData:
         break;
         
      default:
         break;
   }   
}
#endif


#if (USB_CLASS_USED == USB_CLASS_CDC)
/************************************************************************
* Function:     Term_MainSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 Terminal main state machine.
* Date:         21/05/17
************************************************************************/
void Term_MainSm (void)
{
   static TermMainSmStateType TermMainSmState = Term_StartTmr;
   static SwTimerType TermDelay = SW_TIMER_INIT_EN;
   static UINT8 Data[DATA_SIZE];
   UINT8 FwkMajorStr[STR_SIZE];
   UINT8 FwkMinorStr[STR_SIZE];
   UINT8 FwkBuildStr[STR_SIZE];   
   UINT8 FwkBuildDayStr[STR_SIZE];
   UINT8 FwkBuildMonthStr[STR_SIZE];  
   UINT8 FwkBuildYearStr[STR_SIZE];    

   switch (TermMainSmState)
   {
      case Term_StartTmr:
         /* Start 1s timer */
         OsTmr_StartTimer(&TermDelay,2000);          
         /* Switch state */
         TermMainSmState = Term_InitDelay;         
         break;
         
      case Term_InitDelay:
         /* Check timer */
         if (OsTmr_GetTimerStatus(&TermDelay) == SwTimerExpired)
         {
            /* Switch state */
            TermMainSmState = Term_InitState;            
         }         
         break;         
      
      case Term_InitState:
         /* Check USB status */
         if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl == 1))
         {
            /* Do nothing */
         }  
         else
         {
            /* Switch state */
            TermMainSmState = Term_WelcomeState;
         }
         break;
         
      case Term_WelcomeState:
         /* Check if the transfer is allowed */
         if(USBUSARTIsTxTrfReady())
         {
            /* Send welcome string */
            putUSBUSART((void*)Welcome,(sizeof(Welcome) - 1));
            /* Switch state */
            TermMainSmState = Term_InstDescState;             
         }
         break;
         
      case Term_InstDescState:
         /* Check if the transfer is allowed */
         if(USBUSARTIsTxTrfReady())
         {
            /* Send MsgFormat string */
            putUSBUSART((void*)MsgFormat,(sizeof(MsgFormat) - 1));
            /* Switch state */
            TermMainSmState = Term_FwkVerState;             
         }         
         break;
         
      case Term_FwkVerState:
         /* Check if the transfer is allowed */
         if(USBUSARTIsTxTrfReady())
         {
            /* Clear data buffer */
            memset(Data,0,DATA_SIZE);            
            /* Buil framework version string */
            utoa(FwkMajorStr,FRAMEWORK_VERSION_MAJOR,10);
            utoa(FwkMinorStr,FRAMEWORK_VERSION_MINOR,10);
            utoa(FwkBuildStr,FRAMEWORK_VERSION_BUILD,10);
            strcpy(Data,"Framework Version: ");
            strcat(Data,FwkMajorStr);
            strcat(Data,".");
            strcat(Data,FwkMinorStr);
            strcat(Data,".");
            strcat(Data,FwkBuildStr);            
            strcat(Data,"\r\n");            
            /* Send MsgFormat string */
            putUSBUSART((void*)Data,strlen(Data));
            /* Switch state */
            TermMainSmState = Term_FwkBuildDateState;             
         }                  
         break;         
         
      case Term_FwkBuildDateState:
         /* Check if the transfer is allowed */
         if(USBUSARTIsTxTrfReady())
         {
            /* Clear data buffer */
            memset(Data,0,DATA_SIZE);
            /* Add build date */
            sprintf(Data,"Build Date: %s\r\n",FRAMEWORK_BUILD_DATE);                        
            /* Send MsgFormat string */
            putUSBUSART((void*)Data,strlen(Data));
            /* Switch state */
            TermMainSmState = Term_SysInitState;             
         }                  
         break;         
                                    
      case Term_SysInitState:
         /* Check if the transfer is allowed */
         if(USBUSARTIsTxTrfReady())
         {
            /* Send MsgFormat string */
            putUSBUSART((void*)MsgReady,(sizeof(MsgReady) - 1));
            /* Switch state */
            TermMainSmState = Term_FinalState;             
         }                   
         break;
         
      case Term_FinalState:
         /* Run terminal */
         Terminal();
         break;   
         
      default:
         break;         
   }
}   
#endif

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     Term_MainTask
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Terminal main task.
* Date:         21/05/17
************************************************************************/
void Term_MainTask (UINT8 Options)
{       
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:                           
         /* Nothing to do */
         break;

      /* System Normal operation Phase */
      case RunningState:
         #if (USB_CLASS_USED == USB_CLASS_CDC)
         /* Main state machine */
         Term_MainSm(); 
         #endif        
         break;

      /* Default */
      default:
         break;
   }
}
#endif