/************************************************************************
*                          Modem HTTP Interface                         *
*************************************************************************
* FileName:         mdm_gsm_gprs_http.c                                 *
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
* F.Ficili     25/04/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "mdm_gsm_gprs_http.h"

#if (MODEM_USED == GSM_GPRS_MDM)
/************************************************************************
* Defines
************************************************************************/
#define HTTP_OP_ALARM_ID                                    0u
#define CONN_TIMEOUT_MS                                     20000u
#define HTTP_AL_PR_MS                                       1u

#define GET_READ_OPTION                                     STD_OFF
/************************************************************************
* Typedefs
************************************************************************/
/* Gprs init type */
typedef enum _GprsNetInitType
{
   GprsIn_Idle,
   GprsIn_SetConType,
   GprsIn_SetApn,
   GprsIn_EnGprs,
   GprsIn_GetIp,        
   GprsIn_Done,
   GprsIn_Fail,
   GprsIn_WaitMdmResp,
   GprsIn_WaitGetIp,
} GprsNetInitType;

/* Get method state type */
typedef enum _GetMethodType
{
   GetM_Idle,
   GetM_HttpInit,
   GetM_HttpSsl,
   GetM_SetBearerProf,
   GetM_SetUrlAddr,
   GetM_PerfGetAction,
   GetM_ReqActionConf,
   GetM_WaitActionConf,           
   GetM_HttpRead,
   GetM_HttpTerminate,
   GetM_WaitMdmResp,
   GetM_HttpWait,
   GetM_Wait,
   GetM_ReadGetData,
   GetM_Done,
   GetM_Fail,           
} GetMethodType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* Local copies for Apn and ApnLen */
UINT8 *ApnLocal;
UINT8 ApnLenLocal;

/* Local copies for HttpGetData */
HttpGetDataType HttpGetDataLocal;

/* Connections timeout */
SwTimerType ConnTimeout = SW_TIMER_INIT_DIS;
SwTimerType ConnWait = SW_TIMER_INIT_DIS;

/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Net status */
GprsOutStsType GprsNetworkSts = GprsSts_NotInit;
/* Get Method out status */
GetMethodStsType HttpGetMethodSts = GetMOut_Idle;

/* Events */
EventStructureType InitNetwork = {EventIdle};
EventStructureType GetMethod = {EventIdle};
EventStructureType PostMethod = {EventIdle};

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
GprsOutStsType InitGprsNetwork (UINT8 *Apn, UINT8 ApnLen);
GprsOutStsType HttpGetMethod (HttpGetDataType *GetData);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     InitGprsNetwork
* Input:        UINT8 *Apn
*               UINT8 ApnLen 
* Output:       GprsOutStsType
* Author:       F.Ficili
* Description:  Init the GPRS network.
* Date:         25/04/17
************************************************************************/
GprsOutStsType InitGprsNetwork (UINT8 *Apn, UINT8 ApnLen)
{
   static GprsNetInitType GprsNetInitState = GprsIn_Idle;
   static GprsNetInitType GprsNetInitNextState = GprsIn_Idle;
   static GprsOutStsType GprsInitSts = GprsSts_InProg;
   UINT8 UartMessage[50];
   UINT8 Index = 0;   
   
   switch(GprsNetInitState)
   {
      case GprsIn_Idle:
         /* If received init event */
         if (ReceiveEvt(&InitNetwork))
         {
            /* Switch state */
            GprsNetInitState = GprsIn_SetConType;
         }
         break;
      case GprsIn_SetConType:
         /* Set connection type */
         Uart_WriteConstString(1, "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r");
         /* Wait modem response and set new state */
         GprsNetInitState = GprsIn_WaitMdmResp;
         GprsNetInitNextState = GprsIn_SetApn;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);         
         /* Net init in progress */
         GprsInitSts = GprsSts_InProg;
         break;
         
      case GprsIn_SetApn:
         /* Clear buffer */
         ClearBuffer(UartMessage, sizeof(UartMessage));           
         /* Set header  */
         StringCopy((void*)("AT+SAPBR=3,1,\"APN\",\""),UartMessage,20);    
         /* Copy APN */
         for (Index = 0; Index < ApnLen; Index++)
         {
            UartMessage[Index + 20] = *(Apn + Index);
         }
         /* Add AT message tail */
         UartMessage[Index + 20] = '"';
         UartMessage[Index + 21] = '\r';
         UartMessage[Index + 22] = NULL;
         /* Write uart message */
         Uart_WriteString(1, UartMessage);
         /* Wait modem response and set new state */
         GprsNetInitState = GprsIn_WaitMdmResp;
         GprsNetInitNextState = GprsIn_EnGprs;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);
         /* Net init in progress */
         GprsInitSts = GprsSts_InProg;         
         break;
         
      case GprsIn_EnGprs:
         /* Enable GPRS */
         Uart_WriteConstString(1, "AT+SAPBR=1,1\r");
         /* Wait modem response and set new state */
         GprsNetInitState = GprsIn_WaitMdmResp;
         GprsNetInitNextState = GprsIn_Done;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);         
         /* Net init in progress */
         GprsInitSts = GprsSts_InProg;         
         break;
         
      case GprsIn_GetIp:
         /* Request IP Address */
         Uart_WriteConstString(1, "AT+SAPBR=2,1\r");
         /* Wait modem response and set new state */
         GprsNetInitState = GprsIn_WaitGetIp;
         GprsNetInitNextState = GprsIn_Done;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);         
         /* Net init in progress */
         GprsInitSts = GprsSts_InProg;         
         break;         
         
      case GprsIn_Done:
         /* Net init completed */
         GprsInitSts = GprsSts_Completed;   
         GprsNetInitState = GprsIn_Idle;     
         break;
         
      case GprsIn_WaitMdmResp:         
         /* If a new string is received on Mdm UART */
         if (ReceiveEvt(&NewStringReceived))
         {
            /* In case of positive resp. */
            if ((RxBuffer[0] == 'O') && (RxBuffer[1] == 'K'))
            {
               /* Go to nex state */
               GprsNetInitState = GprsNetInitNextState;
               /* Clear buffer */
               ClearBuffer(RxBuffer,RX_BUFFER_LENGTH);               
            }
         }
         else if (OsTmr_GetTimerStatus(&ConnTimeout) == SwTimerExpired)
         {
            /* Go to failed state */
            GprsNetInitState = GprsIn_Fail;            
         }
         break;   
         
      case GprsIn_WaitGetIp:         
         /* If a new string is received on Mdm UART */
         if (ReceiveEvt(&NewStringReceived))
         {
            /* In case of positive resp. */
            if ((RxBuffer[0] == '+') && (RxBuffer[1] == 'S'))
            {
               /* Go to nex state */
               GprsNetInitState = GprsNetInitNextState;
               /* Clear buffer */
               ClearBuffer(RxBuffer,RX_BUFFER_LENGTH);               
            }
         }
         else if (OsTmr_GetTimerStatus(&ConnTimeout) == SwTimerExpired)
         {
            /* Go to failed state */
            GprsNetInitState = GprsIn_Fail;            
         }
         break;          
         
      case GprsIn_Fail:
         /* Net init in failed */
         GprsInitSts = GprsSts_Failed;  
         GprsNetInitState = GprsIn_Idle;         
         break;         
                  
      default:
         break;
   }
   
   /* Return status */
   return GprsInitSts;
}

/************************************************************************
* Function:     HttpGetMethod
* Input:        HttpGetDataType *GetData 
* Output:       GetMethodStsType
* Author:       F.Ficili
* Description:  HTTP GET method state machine.
* Date:         26/04/17
************************************************************************/
GetMethodStsType HttpGetMethod (HttpGetDataType *GetData)
{
   static GetMethodType GetMethodState = GetM_Idle;
   static GetMethodType GetMethodNextState = GetM_Idle;
   static GetMethodStsType GetMOutSts = GetMOut_Idle;
   UINT8 UartMessage[200];
   UINT8 Index = 0;      
   
   switch (GetMethodState)
   {
      case GetM_Idle:
         /* If the get method evt is received */
         if (ReceiveEvt(&GetMethod))
         {
            /* Switch state */
            GetMethodState = GetM_HttpInit;
         }
         break;
      case GetM_HttpInit:
         /* Init Http */
         Uart_WriteConstString(1, "AT+HTTPINIT\r");
         /* Wait modem response and set new state */
         GetMethodState = GetM_WaitMdmResp;
         GetMethodNextState = GetM_HttpSsl;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);
         /* Out status */
         GetMOutSts = GetMOut_InProg;
         break;
         
      case GetM_HttpSsl:
         /* Init Http */
         Uart_WriteConstString(1, "AT+HTTPSSL=1\r");
         /* Wait modem response and set new state */
         GetMethodState = GetM_WaitMdmResp;
         GetMethodNextState = GetM_SetBearerProf;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);
         /* Out status */
         GetMOutSts = GetMOut_InProg;
         break;      
                           
      case GetM_SetBearerProf:
         /* Init Http */
         Uart_WriteConstString(1, "AT+HTTPPARA=\"CID\",1\r");
         /* Wait modem response and set new state */
         GetMethodState = GetM_WaitMdmResp;
         GetMethodNextState = GetM_SetUrlAddr;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);
         /* Out status */
         GetMOutSts = GetMOut_InProg;         
         break;
         
      case GetM_SetUrlAddr:
         /* Clear buffer */
         ClearBuffer(UartMessage, sizeof(UartMessage));
         /* Set header  */
         StringCopy((void*)("AT+HTTPPARA=\"URL\",\""),UartMessage,19);    
         /* Copy URL */
         for (Index = 0; Index < GetData->UrlLen; Index++)
         {
            UartMessage[Index + 19] = *(GetData->Url + Index);
         }
         /* Add AT message tail */
         UartMessage[Index + 19] = '"';
         UartMessage[Index + 20] = '\r';
         UartMessage[Index + 21] = NULL;
         /* Write uart message */
         Uart_WriteString(1, UartMessage);   
         /* Wait modem response and set new state */
         GetMethodState = GetM_WaitMdmResp;
         GetMethodNextState = GetM_PerfGetAction;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);
         /* Out status */
         GetMOutSts = GetMOut_InProg;         
         break;
         
      case GetM_PerfGetAction:
         /* Init Http */
         Uart_WriteConstString(1, "AT+HTTPACTION=0\r");
         /* Wait modem response and set new state */
         GetMethodState = GetM_WaitMdmResp;
         GetMethodNextState = GetM_ReqActionConf;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);             
         break;
         
      case GetM_ReqActionConf:
         /* Wait action confirmation */
         GetMethodState = GetM_WaitActionConf;
/* Read option static config */         
#if (GET_READ_OPTION == STD_OFF)
         GetMethodNextState = GetM_HttpTerminate;
#elif (GET_READ_OPTION == STD_ON)
         GetMethodNextState = GetM_HttpRead;
#endif
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);                    
         break;         
         
      case GetM_HttpRead:
         /* Init Http */
         Uart_WriteConstString(1, "AT+HTTPREAD\r");
         /* Wait modem response and set new state */
         GetMethodState = GetM_ReadGetData;
         GetMethodNextState = GetM_HttpTerminate;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);
         /* Out status */
         GetMOutSts = GetMOut_InProg;         
         break;
         
      case GetM_HttpTerminate:
         /* Init Http */
         Uart_WriteConstString(1, "AT+HTTPTERM\r");
         /* Wait modem response and set new state */
         GetMethodState = GetM_WaitMdmResp;
         GetMethodNextState = GetM_Done;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnTimeout,CONN_TIMEOUT_MS);
         /* Out status */
         GetMOutSts = GetMOut_InProg;         
         break;
         
      case GetM_WaitMdmResp:
         /* If a new string is received on Mdm UART */
         if (ReceiveEvt(&NewStringReceived))
         {
            /* In case of positive resp. */
            if ((RxBuffer[0] == 'O') && (RxBuffer[1] == 'K'))
            {
               /* Go to nex state */
               GetMethodState = GetMethodNextState;
               /* Clear buffer */
               ClearBuffer(RxBuffer,RX_BUFFER_LENGTH);
            }
         }
         else if (OsTmr_GetTimerStatus(&ConnTimeout) == SwTimerExpired)
         {
            /* Go to failed state */
            GetMethodState = GetM_Fail;            
         }         
         break;
         
      case GetM_WaitActionConf:
         /* If a new string is received on Mdm UART */
         if (ReceiveEvt(&NewStringReceived))
         {
            /* In case of positive resp. */
            if ((RxBuffer[0] == '+') && (RxBuffer[1] == 'H'))
            {
               /* Go to nex state */
               GetMethodState = GetMethodNextState;
               /* Clear buffer */
               ClearBuffer(RxBuffer,RX_BUFFER_LENGTH);               
            }            
         }    
         else if (OsTmr_GetTimerStatus(&ConnTimeout) == SwTimerExpired)
         {
            /* Go to failed state */
            GetMethodState = GetM_Fail;            
         }         
         break;         
         
      case GetM_HttpWait:
         /* Wait modem response and set new state */
         GetMethodState = GetM_Wait;
         GetMethodNextState = GetM_PerfGetAction;
         /* Start timeout timer */
         OsTmr_StartTimer(&ConnWait,1000);
         /* Out status */
         GetMOutSts = GetMOut_InProg;                 
         break;         
         
      case GetM_Wait:
         /* Wait and then terminate */
         if (OsTmr_GetTimerStatus(&ConnWait) == SwTimerExpired)
         {
            /* Go to nex state */
            GetMethodState = GetMethodNextState;       
         }              
         break;               

      case GetM_ReadGetData:
         /* If a new string is received on Mdm UART */
         if (ReceiveEvt(&NewStringReceived))
         {
            /* In case of positive resp. */
            if ((RxBuffer[0] == '+') && (RxBuffer[1] == 'H'))
            {
               /* Go to nex state */
               GetMethodState = GetMethodNextState;
               /* Copy Rx buffer */
               StringCopy(RxBuffer,GetData->RespBuffer,RX_BUFFER_LENGTH);               
               /* Clear buffer */
               ClearBuffer(RxBuffer,RX_BUFFER_LENGTH);               
            }                  
         }         
         else if (OsTmr_GetTimerStatus(&ConnTimeout) == SwTimerExpired)
         {
            /* Go to failed state */
            GetMethodState = GetM_Fail;            
         }     
         break; 
         
      case GetM_Done:
         /* Go to next state */
         GetMethodState = GetM_Idle;
         /* Out status */
         GetMOutSts = GetMOut_Completed;         
         break;
         
      case GetM_Fail:
         /* Go to GetM_HttpInit state */
         GetMethodState = GetM_Idle;
         /* Out status */
         GetMOutSts = GetMOut_Failed;         
         break;             
         
      default:
         break;
   }
   
   return GetMOutSts;
}

#ifdef USE_INIT_GPRS_CBK
/************************************************************************
* Function:     InitGprsAlrmCallback
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Callback for init gprs network.
* Date:         26/04/17
************************************************************************/
void InitGprsAlrmCallback (void)
{
   if (InitGprsNetwork(ApnLocal,ApnLenLocal) != GprsSts_InProg)
   {
      /* Re-Set alarm */
      OsAlrm_SetOsAlarm(HTTP_OP_ALARM_ID,HTTP_AL_PR_MS,InitGprsAlrmCallback);   
   }
   else
   {
      /* Do nothing */
   }
}
#endif

#ifdef USE_GET_METHOD_CBK
/************************************************************************
* Function:     HttpGetMethodCallback
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Callback for HTTP GET method.
* Date:         26/04/17
************************************************************************/
void HttpGetMethodCallback (void)
{
   if (HttpGetMethod(&HttpGetDataLocal) != GprsSts_InProg)
   {
      /* Re-Set alarm */
      OsAlrm_SetOsAlarm(HTTP_OP_ALARM_ID,HTTP_AL_PR_MS,HttpGetMethodCallback);   
   }
   else
   {
      /* Do nothing */
   }
}
#endif

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     MdmHttp_MainTask
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage HTTP functions main task.
* Date:         25/04/17
************************************************************************/
void MdmHttp_MainTask (UINT8 Options)
{    
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:

         break;

      /* System Normal operation Phase */
      case RunningState:
         /* If the modem is configured run Gprs/Http FSM */
         if (MdmStatus == ModemConfigured)
         {
            /* Network init state machine */
            GprsNetworkSts = InitGprsNetwork(ApnLocal,ApnLenLocal);         
            /* Get method state machine */
            HttpGetMethodSts = HttpGetMethod(&HttpGetDataLocal);         
         }
         break;

      /* Default */
      default:
         break;
   }
}

/************************************************************************
* Function:     MdmHttp_InitGprsNetwork
* Input:        UINT8 *Apn
*               UINT8 ApnLen 
* Output:       None
* Author:       F.Ficili
* Description:  Start GPRS network initialization.
* Date:         26/04/17
************************************************************************/
void MdmHttp_InitGprsNetwork (UINT8 *Apn, UINT8 ApnLen)
{
   /* Save local data */
   ApnLocal = Apn;   
   ApnLenLocal = ApnLen;
   
   /* Send event */
   GenerateEvt(&InitNetwork);
}

/************************************************************************
* Function:     MdmHttp_SetSsl
* Input:        UINT8 Ssl
* Output:       None
* Author:       F.Ficili
* Description:  Set SSL option.
* Date:         26/04/17
************************************************************************/
void MdmHttp_SetSsl (UINT8 Ssl)
{
   /* */
   if (Ssl == STD_TRUE)
   {
      /* Enable SSL */
      Uart_WriteConstString(1, "AT+HTTPSSL=1\r");   
   }
   else
   {
      /* Disable SSL */
      Uart_WriteConstString(1, "AT+HTTPSSL=0\r");      
   }
}

/************************************************************************
* Function:     MdmHttp_GetMethod
* Input:        HttpGetDataType *GetData 
* Output:       None
* Author:       F.Ficili
* Description:  Perform an HTTP GET method.
* Date:         26/04/17
************************************************************************/
void MdmHttp_GetMethod (HttpGetDataType *GetData)
{
   /* Copy data to local structure */
   HttpGetDataLocal.Url = GetData->Url;
   HttpGetDataLocal.UrlLen = GetData->UrlLen;           
   HttpGetDataLocal.RespBuffer = GetData->RespBuffer;
   
   /* Send event */
   GenerateEvt(&GetMethod);
}

/************************************************************************
* Function:     MdmHttp_QueryIpAddr
* Input:        IpAddrType *Ip
* Output:       None
* Author:       F.Ficili
* Description:  Request the assigned Ip address.
* Date:         26/04/17
************************************************************************/
void MdmHttp_QueryIpAddr (IpAddrType *Ip)
{
   
}

/************************************************************************
* Function:     MdmHttp_PostMethod
* Input:        HttpPostDataType *PostData
* Output:       None
* Author:       F.Ficili
* Description:  Perform an HTTP POST method.
* Date:         26/04/17
************************************************************************/
void MdmHttp_PostMethod (HttpPostDataType *PostData)
{

}

#endif /* MODEM_USED == GSM_GPRS_MDM */