/************************************************************************
*                          Modem HTTP Interface                         *
*************************************************************************
* FileName:         mdm_gsm_gprs_http.h                                 *
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

#ifndef MDM_HTTP_H
#define	MDM_HTTP_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

#if (MODEM_USED == GSM_GPRS_MDM)
/************************************************************************
* EXPORTED Defines
************************************************************************/
#define MDM_HTTP_TASK_PERIOD_MS                             ((UINT16)(1))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
typedef enum _GprsOutStsType
{
   GprsSts_NotInit,
   GprsSts_InProg,
   GprsSts_Completed,
   GprsSts_Failed,
} GprsOutStsType; 

typedef enum _GetMethodStsType
{
   GetMOut_Idle,
   GetMOut_InProg,
   GetMOut_Completed,
   GetMOut_Failed,
} GetMethodStsType; 

/* Ip address type */
typedef struct _IpAddrType
{
   UINT8 Byte0;
   UINT8 Byte1;
   UINT8 Byte2;
   UINT8 Byte3;   
} IpAddrType;

/* Http get data type */
typedef struct _HttpGetDataType
{
   UINT8 *Url;
   UINT8 UrlLen;
   UINT8 *RespBuffer;
} HttpGetDataType;

/* Http post data type */
typedef struct _HttpPostDataType
{
   UINT8 *Url;
   UINT8 UrlLen;
   UINT8 *DataBuffer;   
   UINT8 *RespBuffer;
   UINT16 Timeout;
} HttpPostDataType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Net status */
extern GprsOutStsType GprsNetworkSts;
/* Get Method out status */
extern GetMethodStsType HttpGetMethodSts;
/* Events */
extern EventStructureType InitNetwork;
extern EventStructureType GetMethod;
extern EventStructureType PostMethod;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Manage HTTP functions main task */
void MdmHttp_MainTask (UINT8 Options);
/* Start GPRS network initialization */
void MdmHttp_InitGprsNetwork (UINT8 *Apn, UINT8 ApnLen);
/* Set SSL option */
void MdmHttp_SetSsl (UINT8 Ssl);
/* Request the assigned Ip address */
void MdmHttp_QueryIpAddr (IpAddrType *Ip);
/* Perform an HTTP GET method */
void MdmHttp_GetMethod (HttpGetDataType *GetData);
/* Perform an HTTP POST method */
void MdmHttp_PostMethod (HttpPostDataType *PostData);

#endif /* MODEM_USED == GSM_GPRS_MDM */

#endif	/* MDM_HTTP_H */

