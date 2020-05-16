/************************************************************************
*                        	Modem Wifi HTTP                             *
*************************************************************************
* FileName:         mdm_wifi_http.c                                     *
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
* F.Ficili     14/04/19    1.1          Added:                          *
*                                       - MdmWifiHttp_MdmJoinNetwork    *
*                                       - MdmWifiHttp_GetMethod         *  
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "mdm_wifi_http.h"

#if (MODEM_USED == WIFI_MDM)
/************************************************************************
* Defines
************************************************************************/
/* Init delay constants */
#define JOIN_AP_DELAY_MS                                 5000u
#define START_COMM_DELAY_MS                              10000u
#define INIT_DELAY_MS                                    500u

/* Methods delays constants */
#define GET_REQ_DEL_MS                                   100u
#define POST_REQ_DEL_MS                                  100u

/* Buffers len constants */
#define AP_BUFF_LEN                                      50u
#define PWD_BUFF_LEN                                     50u
#define HTTP_SER_STRING_LEN                              200u
#define HOST_BUFF_LEN                                    20u

/* HTTP GET request */
#define GET_REQ                                          "GET "

/* HTTP POST request */
#define POST_REQ                                         "POST "

/* Connection ID */
#define CONN_ID                                          0u
/* Target Port */
#define PORT                                             80

/************************************************************************
* Typedefs
************************************************************************/
/* Init Modem return type */
typedef enum _InitModemRetType
{
   InitMdm_InProgress,
   InitMdm_Completed
} InitModemRetType;

/* Init mdm state type */
typedef enum _InitMdmStateType
{
   InMdmJn_Start,
   InMdmJn_SetModemMode,
   InMdmJn_InitMdmJoinAp,
   InMdmJn_InitMdmSetTransferMode,
   InMdmJn_InitMdmSetConnMode,
   InMdmJn_InitMdmStop,
   InMdmJn_InitMdmDelay                                 
} InitMdmStateType;

/* Get SM state type */
typedef enum _GetStateType
{
   Get_StartConn,
   Get_GetReqData,
   Get_GetReqHost,
   Get_GetReqSend,
   Get_ConnClose,
   Get_ReqCompleted,
   Get_Delay,                                
} GetStateType;

/* Get return type */
typedef enum _GetRetType
{
   Get_InProgress,
   Get_Completed,
} GetRetType;

/* Post SM state type */
typedef enum _PostStateType
{
   Post_StartConn,
   Post_PostReqData,
   Post_PostReqHost,
   Post_PostReqSend,
   Post_ConnClose,
   Post_ReqCompleted,
   Post_Delay,                                
} PostStateType;

/* Post return type */
typedef enum _PostRetType
{
   Post_InProgress,
   Post_Completed,
} PostRetType;


/************************************************************************
* LOCAL Variables
************************************************************************/
/* Events */
static EventStructureType StartJoinNetworkInit;
static EventStructureType GetMethodEvt;
static EventStructureType PostMethodEvt;

/* Local buffers */
static UINT8 LocalAccessPoint[AP_BUFF_LEN];
static UINT8 LocalPassword[PWD_BUFF_LEN];
static UINT8 LocalHttpSerString[HTTP_SER_STRING_LEN];
static UINT8 LocalHost[HOST_BUFF_LEN];

/* SM last states */
static InitModemRetType InitMdmLastOutputState;
static GetRetType GetMethodLastOutputState;
static PostRetType PostMethodLastOutputState;

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
* Function:     MdmJoinNetworkSm
* Input:        None
* Output:       InitModemRetType
* Author:       F.Ficili
* Description:  Local state machine for initialize the wifi modem in Join Network mode.
* Date:         14/04/19
************************************************************************/
InitModemRetType MdmJoinNetworkSm (void)
{
   static InitMdmStateType InitMdm_State = InMdmJn_Start;
   static InitMdmStateType InitMdm_NextState = InMdmJn_Start;
   static SwTimerType InitMdm_SwTimer = SW_TIMER_INIT_EN;
   static InitModemRetType InitMdmSts = InitMdm_InProgress;
   
   switch (InitMdm_State)
   {
      case InMdmJn_Start:
         /* Check activation event */
         if (ReceiveEvt(&StartJoinNetworkInit))
         {
            /* Set init in progress */
            InitMdmSts = InitMdm_InProgress;                  
            /* Switch state with delay */
            OsTmr_StartTimer(&InitMdm_SwTimer,JOIN_AP_DELAY_MS);
            InitMdm_State = InMdmJn_InitMdmDelay;
            InitMdm_NextState = InMdmJn_SetModemMode;               
         }           
         break; 
                         
      case InMdmJn_SetModemMode:
         /* Set WiFi Mode --> Station Mode */
         MdmWifiCmd_SetWifiMode(STATION_MODE);
         /* Switch state with delay */
         OsTmr_StartTimer(&InitMdm_SwTimer,INIT_DELAY_MS); 
         InitMdm_State = InMdmJn_InitMdmDelay;
         InitMdm_NextState = InMdmJn_InitMdmJoinAp;              
         break;
         
      case InMdmJn_InitMdmJoinAp:                 
         /* Join access point */
         MdmWifiCmd_JoinAccessPoint(LocalAccessPoint, LocalPassword);
         /* Switch state with delay */
         OsTmr_StartTimer(&InitMdm_SwTimer,START_COMM_DELAY_MS);
         InitMdm_State = InMdmJn_InitMdmDelay;
         InitMdm_NextState = InMdmJn_InitMdmSetTransferMode;              
         break;
         
      case InMdmJn_InitMdmSetTransferMode:
         /* Set tx mode */
         MdmWifiCmd_SetTransferMode(TX_MODE_NORMAL);
         /* Switch state with delay */
         OsTmr_StartTimer(&InitMdm_SwTimer,INIT_DELAY_MS);
         InitMdm_State = InMdmJn_InitMdmDelay;
         InitMdm_NextState = InMdmJn_InitMdmSetConnMode;           
         break;         
                  
      case InMdmJn_InitMdmSetConnMode:
         /* Set tx mode */
         MdmWifiCmd_SetConnectionMode(CONN_MODE_MULTIPLE);
         /* Switch state with delay */
         OsTmr_StartTimer(&InitMdm_SwTimer,INIT_DELAY_MS);
         InitMdm_State = InMdmJn_InitMdmDelay;
         InitMdm_NextState = InMdmJn_InitMdmStop;              
         break;  
         
      case InMdmJn_InitMdmStop:
         /* Set status as completed */
         InitMdmSts = InitMdm_Completed;
         InitMdm_State = InMdmJn_Start;         
         break;
         
      case InMdmJn_InitMdmDelay:
         /* Check if the timer expired */
         if (OsTmr_GetTimerStatus(&InitMdm_SwTimer) == SwTimerExpired)
         {
            /* Switch state */
            InitMdm_State = InitMdm_NextState;
         }
         break;          
         
      default:
         break;
   }  
   
   return InitMdmSts;
}

/************************************************************************
* Function:     HttpGetMethodSm
* Input:        None
* Output:       GetRetType
* Author:       F.Ficili
* Description:  Local state machine for the implementation of HTTP GET Method.
* Date:         14/04/19
************************************************************************/
GetRetType HttpGetMethodSm (void)
{
   static GetStateType GetSmState =  Get_StartConn;
   static GetStateType GetSmNextState =  Get_StartConn;
   static SwTimerType Get_SwTimer = SW_TIMER_INIT_EN;   
   static GetRetType Ret = Get_InProgress;   
   UINT8 Data[HTTP_SER_STRING_LEN + 10];
   UINT8 Size;      
      
   switch (GetSmState)
   {
      case Get_StartConn:
         /* Check activation event */
         if (ReceiveEvt(&GetMethodEvt))
         {
            /* Ret in progress */
            Ret = Get_InProgress;
            /* Start Connection */
            MdmWifiCmd_StartConnection(CONN_MODE_MULTIPLE,CONN_ID,PROTOCOL_TCP,(void*)LocalHost,PORT);
            /* Switch state with delay */
            OsTmr_StartTimer(&Get_SwTimer,GET_REQ_DEL_MS);
            GetSmState = Get_Delay;
            GetSmNextState = Get_GetReqData; 
         }                      
         break;
         
      case Get_GetReqData:      
         /* Build string */
         ClearBuffer(Data,sizeof(Data));      
         strcpy(Data,GET_REQ);
         strcat(Data,LocalHttpSerString);        
         Size = strlen(Data);
         /* GET Request */
         MdmWifiCmd_SendData(CONN_MODE_MULTIPLE,CONN_ID,Size,(void*)Data);             
         /* Switch state with delay */
         OsTmr_StartTimer(&Get_SwTimer,GET_REQ_DEL_MS);
         GetSmState = Get_Delay;
         GetSmNextState = Get_GetReqHost;            
         break;
         
      case Get_GetReqHost:
         /* Build string */
         ClearBuffer(Data,sizeof(Data));  
         strcpy(Data,"Host: ");           
         strcat(Data,(void*)LocalHost);   
         Size = strlen(Data);
         /* GET Request */
         MdmWifiCmd_SendData(CONN_MODE_MULTIPLE,CONN_ID,Size,(void*)Data);          
         /* Switch state with delay */
         OsTmr_StartTimer(&Get_SwTimer,GET_REQ_DEL_MS);
         GetSmState = Get_Delay;
         GetSmNextState = Get_GetReqSend;           
         break;
         
      case Get_GetReqSend:
         /* GET Request */
         MdmWifiCmd_SendData(CONN_MODE_MULTIPLE,CONN_ID,0,(void*)"");
         /* Switch state with delay */
         OsTmr_StartTimer(&Get_SwTimer,GET_REQ_DEL_MS);
         GetSmState = Get_Delay;
         GetSmNextState = Get_ReqCompleted;            
         break;
         
      case Get_ReqCompleted: 
         /* Req. completed */
         Ret = Get_Completed;
         GetSmState = Get_StartConn;
         break;
         
      case Get_Delay:
         /* Check if the timer expired */
         if (OsTmr_GetTimerStatus(&Get_SwTimer) == SwTimerExpired)
         {
            /* Switch state */
            GetSmState = GetSmNextState;
         }
         break;       
         
      default:
         break;          
   }
   
   return Ret;   
}

/************************************************************************
* Function:     HttpPostMethodSm
* Input:        None
* Output:       PostRetType
* Author:       F.Ficili
* Description:  Local state machine for the implementation of HTTP POST Method.
* Date:         14/04/19
************************************************************************/
PostRetType HttpPostMethodSm (void)
{
   static PostStateType PostSmState =  Post_StartConn;
   static PostStateType PostSmNextState =  Post_StartConn;
   static SwTimerType Post_SwTimer = SW_TIMER_INIT_EN;   
   static PostRetType Ret = Post_InProgress; 
   UINT8 Data[HTTP_SER_STRING_LEN + 10];
   UINT8 Size;       
      
   switch (PostSmState)
   {
      case Post_StartConn:
         /* Check activation event */
         if (ReceiveEvt(&PostMethodEvt))
         {
            /* Ret in progress */
            Ret = Post_InProgress;
            /* Start Connection */
            MdmWifiCmd_StartConnection(CONN_MODE_MULTIPLE,CONN_ID,PROTOCOL_TCP,(void*)LocalHost,PORT);
            /* Switch state with delay */
            OsTmr_StartTimer(&Post_SwTimer,POST_REQ_DEL_MS);
            PostSmState = Post_Delay;
            PostSmNextState = Post_PostReqData; 
         }                      
         break;
         
      case Post_PostReqData:      
         /* Build string */
         ClearBuffer(Data,sizeof(Data));      
         strcpy(Data,POST_REQ);
         strcat(Data,LocalHttpSerString);        
         Size = strlen(Data);
         /* GET Request */
         MdmWifiCmd_SendData(CONN_MODE_MULTIPLE,CONN_ID,Size,(void*)Data);             
         /* Switch state with delay */
         OsTmr_StartTimer(&Post_SwTimer,POST_REQ_DEL_MS);
         PostSmState = Post_Delay;
         PostSmNextState = Post_PostReqHost;            
         break;
         
      case Post_PostReqHost:
         /* Build string */
         ClearBuffer(Data,sizeof(Data));  
         strcpy(Data,"Host: ");           
         strcat(Data,(void*)LocalHost);   
         Size = strlen(Data);
         /* POST Request */
         MdmWifiCmd_SendData(CONN_MODE_MULTIPLE,CONN_ID,Size,(void*)Data);          
         /* Switch state with delay */
         OsTmr_StartTimer(&Post_SwTimer,POST_REQ_DEL_MS);
         PostSmState = Post_Delay;
         PostSmNextState = Post_PostReqSend;           
         break;
         
      case Post_PostReqSend:
         /* POST Request */
         MdmWifiCmd_SendData(CONN_MODE_MULTIPLE,CONN_ID,0,(void*)"");
         /* Switch state with delay */
         OsTmr_StartTimer(&Post_SwTimer,POST_REQ_DEL_MS);
         PostSmState = Post_Delay;
         PostSmNextState = Post_ReqCompleted;            
         break;
         
      case Post_ReqCompleted: 
         /* Req. completed */
         Ret = Post_Completed;
         PostSmState = Post_StartConn;
         break;
         
      case Post_Delay:
         /* Check if the timer expired */
         if (OsTmr_GetTimerStatus(&Post_SwTimer) == SwTimerExpired)
         {
            /* Switch state */
            PostSmState = PostSmNextState;
         }
         break;       
         
      default:
         break;          
   }
   
   return Ret;   
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     MdmWifiHttp_MainTask
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Manage wifi HTTP main function task.
* Date:         06/11/17
************************************************************************/
void MdmWifiHttp_MainTask (UINT8 Options)
{
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:

         break;

      /* System Normal operation Phase */
      case RunningState:
         /* Init modem and join an existing network */
         InitMdmLastOutputState = MdmJoinNetworkSm();
         /* Get method */
         GetMethodLastOutputState = HttpGetMethodSm();
         /* Post method */
         PostMethodLastOutputState = HttpPostMethodSm();
         break;

      /* Default */
      default:
         break;
   }   
}   

/************************************************************************
* Function:     MdmWifiHttp_MdmJoinNetwork
* Input:        const UINT8 *AccessPoint
*               const UINT8 *Password 
* Output:       None
* Author:       F.Ficili
* Description:  API to initialize the wifi modem in Join Network mode.
* Date:         14/04/19
************************************************************************/
void MdmWifiHttp_MdmJoinNetwork (const UINT8 *AccessPoint, const UINT8 *Password)
{
   /* Copy local data */
   strcpy(LocalAccessPoint, AccessPoint);
   strcpy(LocalPassword, Password);   
   
   /* Send event */
   GenerateEvt(&StartJoinNetworkInit);
}

/************************************************************************
* Function:     MdmWifiHttp_IsMdmJoinNetworkCompleted
* Input:        None
* Output:       UINT8
* Author:       F.Ficili
* Description:  API to get if MdmWifiHttp_MdmJoinNetwork is completed.
* Date:         14/04/19
************************************************************************/
UINT8 MdmWifiHttp_IsMdmJoinNetworkCompleted (void)
{
   UINT8 Ret = STD_FALSE;
   
   if (InitMdmLastOutputState == InitMdm_Completed)
   {
      Ret = STD_TRUE;
   }
   else
   {
      Ret = STD_FALSE;
   }
   
   return Ret;
}

/************************************************************************
* Function:     MdmWifiHttp_GetMethod
* Input:        const UINT8 *GetString
*               const UINT8 *Host 
* Output:       None
* Author:       F.Ficili
* Description:  API to request a GET method service.
* Date:         14/04/19
************************************************************************/
void MdmWifiHttp_GetMethod (const UINT8 *GetString, const UINT8 *Host)
{
   /* TODO: add check for string length */
   
   /* Copy local data */
   strcpy(LocalHttpSerString, GetString);
   strcpy(LocalHost, Host);   
   
   /* Send event */
   GenerateEvt(&GetMethodEvt);   
}

/************************************************************************
* Function:     MdmWifiHttp_IsGetMethodCompleted
* Input:        None
* Output:       UINT8
* Author:       F.Ficili
* Description:  API to get if MdmWifiHttp_GetMethod is completed.
* Date:         14/04/19
************************************************************************/
UINT8 MdmWifiHttp_IsGetMethodCompleted (void)
{
   UINT8 Ret = STD_FALSE;
   
   if (GetMethodLastOutputState == Get_Completed)
   {
      Ret = STD_TRUE;
   }
   else
   {
      Ret = STD_FALSE;
   }
   
   return Ret;
}

/************************************************************************
* Function:     MdmWifiHttp_GetMethod
* Input:        const UINT8 *PostString
*               const UINT8 *Host 
* Output:       None
* Author:       F.Ficili
* Description:  API to request a POST method service.
* Date:         14/04/19
************************************************************************/
void MdmWifiHttp_PostMethod (const UINT8 *PostString, const UINT8 *Host)
{
   /* TODO: add check for string length */   
   
   /* Copy local data */
   strcpy(LocalHttpSerString, PostString);
   strcpy(LocalHost, Host);   
   
   /* Send event */
   GenerateEvt(&PostMethodEvt);      
}

/************************************************************************
* Function:     MdmWifiHttp_IsPostMethodCompleted
* Input:        None
* Output:       UINT8
* Author:       F.Ficili
* Description:  API to get if MdmWifiHttp_PostMethod is completed.
* Date:         14/04/19
************************************************************************/
UINT8 MdmWifiHttp_IsPostMethodCompleted (void)
{
   UINT8 Ret = STD_FALSE;
   
   if (PostMethodLastOutputState == Post_Completed)
   {
      Ret = STD_TRUE;
   }
   else
   {
      Ret = STD_FALSE;
   }
   
   return Ret;
}

#endif /* MODEM_USED == WIFI_MDM */