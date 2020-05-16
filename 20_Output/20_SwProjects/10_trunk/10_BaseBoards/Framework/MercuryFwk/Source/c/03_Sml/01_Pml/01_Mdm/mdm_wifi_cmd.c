/************************************************************************
*                        	Modem Wifi Commands                         *
*************************************************************************
* FileName:         mdm_wifi_cmd.c                                      *
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
* F.Ficili     06/11/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "mdm_wifi_cmd.h"
#include "string.h"

#if (MODEM_USED == WIFI_MDM)
/************************************************************************
* Defines
************************************************************************/
#define ADDRESS_LEN                                      ((UINT8)(25))
#define LOCAL_BUFFER_LEN                                 ((UINT8)(100))

/************************************************************************
* Typedefs
************************************************************************/
typedef enum _StartConnStateType
{
   StCon_WaitEvt,
   StCon_SendStartCmd,
   StCon_CheckCmdRes,   
} StartConnStateType;

typedef enum _WifiSendDataStateType
{
   WiFiSnd_WaitEvt,
   WiFiSnd_SendDataCmd,
   WiFiSnd_WaitPrompt,
   WiFiSnd_SendPayload,
   WiFiSnd_ErrorState,       
} WifiSendDataStateType;

typedef enum _WifiReceiveDataStateType
{
   WiFiRcv_WaitEvt,
   WiFiRcv_GetData,
} WifiReceiveDataStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* Local connection data */
UINT8 ModeLocal;
UINT8 IdLocal;
UINT8 ProtocolLocal;
UINT8 PortLocal;
UINT8 ConnStatus;
UINT8 LenLocal;
UINT8 AddressLocal[ADDRESS_LEN];
UINT8 BufferLocal[LOCAL_BUFFER_LEN];

/* Start connection event */
EventStructureType StartConnection;
EventStructureType WifiSendData;
EventStructureType WifiReceiveData;

/************************************************************************
* GLOBAL Variables
************************************************************************/


/************************************************************************
* LOCAL Function Prototypes
************************************************************************/


/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     StartConnectionSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Start connection handling state machine.
* Date:         07/11/17
************************************************************************/
void StartConnectionSm (void)
{
   static StartConnStateType StartConnState = StCon_WaitEvt;
   UINT8 Data[LOCAL_BUFFER_LEN];  
   UINT8 DataIndex;
   UINT8 PortAscii[4];
   UINT8 IdAscii[1];
   
   switch (StartConnState)
   {
      case StCon_WaitEvt:
         /* If the StartConnection event is received */
         if (ReceiveEvt(&StartConnection))
         {
            StartConnState = StCon_SendStartCmd;
         }
         break;
         
      case StCon_SendStartCmd:
         /* String Cleanup */
         ClearBuffer(Data,LOCAL_BUFFER_LEN);         
         /* Build string */
         strcpy(Data,"AT+CIPSTART=");
         /* Add Id if multiple connections */
         if (ModeLocal == CONN_MODE_MULTIPLE)
         {
            utoa(IdAscii,IdLocal,10);
            strcat(Data,IdAscii);
            strcat(Data,",");
         } 
         strcat(Data,"\"");
         /* Add protocol */
         if (ProtocolLocal == PROTOCOL_UDP)
         { 
            strcat(Data,"UDP");
         }  
         else
         {
            strcat(Data,"TCP");
         }
         strcat(Data,"\"");
         strcat(Data,",");
         strcat(Data,"\"");
         strcat(Data,AddressLocal);
         strcat(Data,"\"");         
         /* Convert port to ASCII */
         utoa(PortAscii,PortLocal,10);
         strcat(Data,",");
         strcat(Data,PortAscii);         
         /* Add tail */
         strcat(Data,"\r\n");
         strcat(Data,NULL);
         /* Send data to modem */
         MdmWifi_SendWifiMsg(Data); 
         
         /* String Cleanup */
         ClearBuffer(AddressLocal,sizeof(AddressLocal));
         
         /* Switch state */
         StartConnState = StCon_CheckCmdRes;                  
         break;
         
      case StCon_CheckCmdRes:
         /* If a new Wifi packet is received */
         if (MdmWifi_ReceiveWifiMsg(Data,&DataIndex) == WifiMsg_Received) 
         {
            /* Discard echo */
            if ((Data[0] != 'A') && (Data[1] != 'T'))
            {
               /* Check for a server positive response */
               if (strstr(Data,(void*)"CONNECT") != NULL)
               {
                  ConnStatus = CONN_ACCEPTED;
               }
               else
               {
                  ConnStatus = CONN_REFUSED;
               }      
            
               /* Switch state */
               StartConnState = StCon_WaitEvt;                  
            }            
         }
         break;
         
      default:
         break;
   }   
}

/************************************************************************
* Function:     SendDataSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Send data state machine.
* Date:         07/11/17
************************************************************************/
void SendDataSm (void)
{
   static WifiSendDataStateType WifiSendDataState = WiFiSnd_WaitEvt;
   UINT8 Data[LOCAL_BUFFER_LEN + 3];  
   UINT8 DataIndex;
   UINT8 IdAscii[1];
   UINT8 LenAscii[4]; 
   static SwTimerType SwTimer = SW_TIMER_INIT_EN;   
   
   switch (WifiSendDataState)
   {
      case WiFiSnd_WaitEvt:
         /* If the WifiSendData event is received */
         if (ReceiveEvt(&WifiSendData))
         {
            WifiSendDataState = WiFiSnd_SendDataCmd;
         }         
         break;
         
      case WiFiSnd_SendDataCmd:
         /* Build string */
         strcpy(Data,"AT+CIPSEND=");
         /* Add Id if multiple connections */
         if (ModeLocal == CONN_MODE_MULTIPLE)
         {
            utoa(IdAscii,IdLocal,10);
            strcat(Data,IdAscii);
            strcat(Data,",");
         }          
         /* Add tail to len value */
         LenLocal = LenLocal + 2;         
         utoa(LenAscii,LenLocal,10);
         strcat(Data,LenAscii);    
         
         /* Add tail */
         strcat(Data,"\r\n");
         strcat(Data,NULL);
         /* Send data to modem */
         MdmWifi_SendWifiMsg(Data); 
         /* Start SW timer */
         OsTmr_StartTimer(&SwTimer,2000);
         
         /* Switch state */
         WifiSendDataState = WiFiSnd_WaitPrompt;                                        
         break;
         
      case WiFiSnd_WaitPrompt:
         /* If a new Wifi packet is received */
         if (MdmWifi_ReceiveWifiMsg(Data,&DataIndex) == WifiMsg_Received) 
         {
            /* Check for the prompt */
            if (strstr(Data,(void*)">") != NULL)
            {
               /* Switch state */
               WifiSendDataState = WiFiSnd_SendPayload;                 
            }
         } 
         else if (OsTmr_GetTimerStatus(&SwTimer) == SwTimerExpired)
         {
            /* Switch state */
            WifiSendDataState = WiFiSnd_WaitEvt;              
         }
         break;
         
      case WiFiSnd_SendPayload:
         /* Build string */
         strcpy(Data,BufferLocal);
         /* Add tail */
         strcat(Data,"\r\n");
         strcat(Data,NULL);         
         
         /* Send payload to modem */
         MdmWifi_SendWifiMsg(Data); 
         
         /* Switch state */
         WifiSendDataState = WiFiSnd_WaitEvt;             
         break;                          
         
      default:
         break;   
   }
}

/************************************************************************
* Function:     ReceiveDataSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Receive data state machine.
* Date:         29/04/18
************************************************************************/
void ReceiveDataSm (void)
{
   static WifiReceiveDataStateType WifiReceiveDataState = WiFiRcv_WaitEvt; 

   
   switch (WifiReceiveDataState)
   {
      case WiFiRcv_WaitEvt:
         /* If the WifiReceiveNetData event is received */
         if (ReceiveEvt(&WifiReceiveNetData))
         {
            WifiReceiveDataState = WiFiRcv_GetData;
         }            
         break;
         
      case WiFiRcv_GetData:
         /* Generate receive event */
         GenerateEvt(&WifiReceiveData);
         /* Go back to wait event state */
         WifiReceiveDataState = WiFiRcv_WaitEvt;                  
         break;
         
      default:
         break;
   }
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     MdmWifiCmd_MainTask
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Manage wifi cmd main function task.
* Date:         06/11/17
************************************************************************/
void MdmWifiCmd_MainTask (UINT8 Options)
{
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* No initializations needed */
         break;

      /* System Normal operation Phase */
      case RunningState:         
         /* Start connection SM */
         StartConnectionSm();         
         /* Send data SM */
         SendDataSm();
         /* Receive data SM */
         ReceiveDataSm();
         break;

      /* Default */
      default:
         break;
   }   
}   

/************************************************************************
* Function:     MdmWifiCmd_RestartModem
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  API used to restart Wifi Modem.
* Date:         06/11/17
************************************************************************/
void MdmWifiCmd_RestartModem (void)
{        
   /* Send restart command to modem */
   MdmWifi_SendWifiMsg((void*)"AT+RST\r\n");
}

/************************************************************************
* Function:     MdmWifiCmd_RestartModem
* Input:        UINT8
* Output:       UINT8 (STD_OK or STD_NOT_OK)
* Author:       F.Ficili
* Description:  API used to set the modem wifi mode (1: Station, 2: SoftAP,
*               3: SoftAP + Station). 
* Date:         22/12/17
************************************************************************/
UINT8 MdmWifiCmd_SetWifiMode (UINT8 WifiMode)
{
   UINT8 Data[20];
   UINT8 Ret = STD_OK;

   /* Build string */
   strcpy(Data,"AT+CWMODE=");   
   
   /* Check requested mode */
   if (WifiMode == STATION_MODE)
   {
      /* Station mode */
      strcat(Data,"1");     
   }
   else if (WifiMode == SOFT_AP_MODE)
   {
      /* SoftAP mode */
      strcat(Data,"2");
   }
   else if (WifiMode == SOFT_AP_AND_STATION_MODE)
   {
      /* SoftAP + Station mode */
      strcat(Data,"3");
   }   
   else
   {
      Ret = STD_NOT_OK;
   }
   
   /* Add tail */
   strcat(Data,"\r\n");
   strcat(Data,NULL);   
   
   /* Send command string */
   MdmWifi_SendWifiMsg(Data); 
   
   /* Return operation result */
   return Ret;   
} 

/************************************************************************
* Function:     MdmWifiCmd_JoinAccessPoint
* Input:        const UINT8 *AccessPoint
*               const UINT8 *Password
* Output:       None
* Author:       F.Ficili
* Description:  API used to join an existing access point with password.
* Date:         06/11/17
************************************************************************/
void MdmWifiCmd_JoinAccessPoint (const UINT8 *AccessPoint, const UINT8 *Password)
{
   UINT8 Data[100];
      
   /* Build string */
   strcpy(Data,"AT+CWJAP=");
   strcat(Data,"\"");
   strcat(Data,AccessPoint);
   strcat(Data,"\",\"");
   strcat(Data,Password);
   strcat(Data,"\"");
   strcat(Data,"\r\n");
   strcat(Data,NULL);
   
   /* Send join access point string */
   MdmWifi_SendWifiMsg(Data);      
}

/************************************************************************
* Function:     MdmWifiCmd_QuitAccessPoint
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  API used to quit a previously joined access point.
*               This function does not wait for nay reply, so to check
*               if the de-join sucvceded is up to the user. 
* Date:         06/11/17
************************************************************************/
void MdmWifiCmd_QuitAccessPoint (void)
{        
   /* Send quit access point command to modem */
   MdmWifi_SendWifiMsg((void*)"AT+CWQAP\r\n");
}

/************************************************************************
* Function:     MdmWifiCmd_SetTransferMode
* Input:        UINT8 Mode
* Output:       UINT8 (STD_OK or STD_NOT_OK)
* Author:       F.Ficili
* Description:  API used to set the transmission mode (normal or unvarnished).
* Date:         06/11/17
************************************************************************/
UINT8 MdmWifiCmd_SetTransferMode (UINT8 Mode)
{
   UINT8 Data[20];
   UINT8 Ret = STD_OK;

   /* Build string */
   strcpy(Data,"AT+CIPMODE=");   
   
   /* Check requested mode */
   if (Mode == TX_MODE_NORMAL)
   {
      /* Normal mode */
      strcat(Data,"0");     
   }
   else if (Mode == TX_MODE_UNVARNISHED)
   {
      /* Unvarnished mode */
      strcat(Data,"1");
   }
   else
   {
      Ret = STD_NOT_OK;
   }
   
   /* Add tail */
   strcat(Data,"\r\n");
   strcat(Data,NULL);   
   
   /* Send command string */
   MdmWifi_SendWifiMsg(Data); 
   
   /* Return operation result */
   return Ret;   
} 

/************************************************************************
* Function:     MdmWifiCmd_SetConnectionMode
* Input:        UINT8 Mode
* Output:       UINT8 (STD_OK or STD_NOT_OK)
* Author:       F.Ficili
* Description:  API used to set the connection mode (single or multiple).
* Date:         06/11/17
************************************************************************/
UINT8 MdmWifiCmd_SetConnectionMode (UINT8 Mode)
{
   UINT8 Data[20];
   UINT8 Ret = STD_OK;

   /* Build string */
   strcpy(Data,"AT+CIPMUX=");   
   
   /* Check requested mode */
   if (Mode == CONN_MODE_SINGLE)
   {
      /* Single transmision mode */
      strcat(Data,"0");     
   }
   else if (Mode == CONN_MODE_MULTIPLE)
   {
      /* Multiple transmisions mode */
      strcat(Data,"1");
   }
   else
   {
      Ret = STD_NOT_OK;
   }
   
   /* Add tail */
   strcat(Data,"\r\n");
   strcat(Data,NULL);   
   
   /* Send command string */
   MdmWifi_SendWifiMsg(Data); 
   
   /* Return operation result */
   return Ret;     
}

/************************************************************************
* Function:     MdmWifiCmd_StartConnection
* Input:        UINT8 Mode
*               UINT8 Id 
*               UINT8 Protocol
*               UINT8 *Address
*               UINT8 Port 
* Output:       None
* Author:       F.Ficili
* Description:  API used to start a TCP or UDP connection.
* Date:         07/11/17
************************************************************************/
void MdmWifiCmd_StartConnection (UINT8 Mode, UINT8 Id, UINT8 Protocol, UINT8 *Address, UINT8 Port)
{
   /* Check mode */
   if (Mode == CONN_MODE_SINGLE)
   {
      /* Copy data */
      ModeLocal = CONN_MODE_SINGLE;
      ProtocolLocal = Protocol;
      strcpy(AddressLocal,Address);
      PortLocal = Port;
   }
   else
   {
      /* Copy data */
      ModeLocal = CONN_MODE_MULTIPLE;
      IdLocal = Id;
      ProtocolLocal = Protocol;
      strcpy(AddressLocal,Address); 
      PortLocal = Port;
   }     
   
   /* Send start connection event */
   GenerateEvt(&StartConnection);
}

/************************************************************************
* Function:     MdmWifiCmd_SendData
* Input:        UINT8 Mode
*               UINT8 Id 
*               UINT8 Len
*               UINT8 *Buffer  
* Output:       None
* Author:       F.Ficili
* Description:  API used to send a TCP or UDP packet. 
* Date:         07/11/17
************************************************************************/
void MdmWifiCmd_SendData (UINT8 Mode, UINT8 Id, UINT8 Len, UINT8 *Buffer)
{
   /* Check mode */
   if (Mode == CONN_MODE_SINGLE)
   {
      /* Copy data */
      ModeLocal = CONN_MODE_SINGLE;
      LenLocal = Len;
      strcpy(BufferLocal,Buffer);
   }
   else
   {
      /* Copy data */
      ModeLocal = CONN_MODE_MULTIPLE;
      IdLocal = Id;
      LenLocal = Len;
      strcpy(BufferLocal,Buffer);      
   }     
   
   /* Send SendData event */
   GenerateEvt(&WifiSendData);     
}

/************************************************************************
* Function:     MdmWifiCmd_CloseConnection
* Input:        UINT8 Mode
*               UINT8 Id 
* Output:       None
* Author:       F.Ficili
* Description:  API used to close a connection.
* Date:         07/11/17
************************************************************************/
void MdmWifiCmd_CloseConnection (UINT8 Mode, UINT8 Id)
{
   UINT8 Data[20]; 
   UINT8 IdAscii[1];
   
   /* Check mode */
   if (Mode == CONN_MODE_SINGLE)
   {
      /* Build string */
      strcpy(Data,"AT+CIPCLOSE");         
   }
   else
   {
      /* Build string */
      strcpy(Data,"AT+CIPCLOSE="); 
      /* Concatenate connection ID with ASCII conversion */      
      utoa(IdAscii,Id,10);
      strcat(Data,IdAscii);
   }  
   
   /* Add tail */
   strcat(Data,"\r\n");
   strcat(Data,NULL);   

   /* Send command string */
   MdmWifi_SendWifiMsg(Data);    
}

/************************************************************************
* Function:     MdmWifiCmd_ConfigureSoftAPMode
* Input:        void* ssid
*               void* pwd
*               UINT8 chid
*               UINT8 enc
* Output:       UINT8 (STD_OK or STD_NOT_OK)
* Author:       F.Ficili
* Description:  API used to configure the softAP.
* Date:         02/11/18
************************************************************************/
UINT8 MdmWifiCmd_ConfigureSoftAPMode(void* ssid, void* pwd, UINT8 chid, UINT8 enc)
{
   UINT8 Data[100];
   UINT8 Ret = STD_OK;
   UINT8 ChIdAscii[5];
   UINT8 EncAscii[5];

   /* Build string */
   strcpy(Data,"AT+CWSAP="); 
   strcat(Data,"\"");
   strcat(Data,ssid);
   strcat(Data,"\",\"");
   strcat(Data,pwd);
   strcat(Data,"\",");
   utoa(ChIdAscii,chid,10);
   strcat(Data,ChIdAscii);
   strcat(Data,",");
   utoa(EncAscii,enc,10);
   strcat(Data,EncAscii);
   
   /* Add tail */
   strcat(Data,"\r\n");
   strcat(Data,NULL);   
   
   /* Send command string */
   MdmWifi_SendWifiMsg(Data); 
   
   /* Return operation result */
   return Ret;     
}

/************************************************************************
* Function:     MdmWifiCmd_ConfigureSoftAPIpAddress
* Input:        void* ip
* Output:       UINT8 (STD_OK or STD_NOT_OK)
* Author:       F.Ficili
* Description:  API used to configure the sofAP IP Address.
* Date:         02/11/18
************************************************************************/
UINT8 MdmWifiCmd_ConfigureSoftAPIpAddress(void* ip)
{
   UINT8 Data[40];
   UINT8 Ret = STD_OK;

   /* Build string */
   strcpy(Data,"AT+CIPAP="); 
   strcat(Data,"\"");
   strcat(Data,ip);
   strcat(Data,"\"");
   
   /* Add tail */
   strcat(Data,"\r\n");
   strcat(Data,NULL);   
   
   /* Send command string */
   MdmWifi_SendWifiMsg(Data); 
   
   /* Return operation result */
   return Ret;     
}

/************************************************************************
* Function:     MdmWifiCmd_ConfigureServer
* Input:        UINT8 Mode
*               UINT16 Port 
* Output:       UINT8 (STD_OK or STD_NOT_OK)
* Author:       F.Ficili
* Description:  API used to configure or delete a server.
* Date:         06/11/17
************************************************************************/
UINT8 MdmWifiCmd_ConfigureServer (UINT8 Mode, UINT16 Port)
{
   UINT8 Data[20];
   UINT8 Ret = STD_OK;
   UINT8 PortAscii[10];

   /* Build string */
   strcpy(Data,"AT+CIPSERVER=");   
   
   /* Check requested mode */
   if (Mode == DELETE_SERVER)
   {
      /* Single transmision mode */
      strcat(Data,"0");     
   }
   else if (Mode == CREATE_SERVER)
   {
      /* Multiple transmisions mode */
      strcat(Data,"1");
   }
   else
   {
      Ret = STD_NOT_OK;
   }
   
   strcat(Data,",");
   /* Add port */
   utoa(PortAscii,Port,10);
   strcat(Data,PortAscii);
   
   /* Add tail */
   strcat(Data,"\r\n");
   strcat(Data,NULL);   
   
   /* Send command string */
   MdmWifi_SendWifiMsg(Data); 
   
   /* Return operation result */
   return Ret;     
}

/************************************************************************
* Function:     MdmWifiCmd_ReceiveWifiMsg
* Input:        UINT8* RxBuffer
*               UINT8* DataLenght
* Output:       WifiDataRecType
* Author:       F.Ficili
* Description:  API used to check if a bunch of data is received and get data.
* Date:         30/04/18
************************************************************************/
WifiDataRecType MdmWifiCmd_ReceiveWifiMsg (UINT8* RxBuffer, UINT8* DataLenght)
{
   WifiDataRecType Ret;
   
   /* If an Rx event is received */
   if (ReceiveEvt(&WifiReceiveData))
   {
      /* Copy data in Rx buffer */
      StringCopy(WifiRxPacket.RxBuffer, RxBuffer, WifiRxPacket.RxByteNum);
      /* Update data len */
      *DataLenght = WifiRxPacket.RxByteNum;
      /* Message received */
      Ret = WiFiRcv_DataReceived;
   }
   else
   {
      /* Message not received */
      Ret = WiFiRcv_DataNotReceived;   
   }
   
   return Ret;   
}

/* EXAMPLE OF HTTP DATA USING TCP
 * 
 * MdmWifiCmd_StartConnection(1,4,PROTOCOL_TCP,(void*)"dweet.io",80);
 * |
 * MdmWifiCmd_SendData(1,4,47,(void*)"GET /dweet/for/ff_MeteoStation?Temperature=30.5");
 * |
 * MdmWifiCmd_SendData(1,4,14,(void*)"Host: dweet.io");
 * |
 * MdmWifiCmd_SendData(1,4,0,(void*)"");
 */

#endif /* MODEM_USED == WIFI_MDM */



