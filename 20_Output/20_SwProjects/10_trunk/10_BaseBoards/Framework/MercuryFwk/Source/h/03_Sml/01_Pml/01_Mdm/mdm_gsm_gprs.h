/************************************************************************
*                           Modem GSM GPRS                              *
*************************************************************************
* FileName:         mdm_gsm_gprs.h                                      *
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

#ifndef MDM_GSM_GPRS_H
#define	MDM_GSM_GPRS_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"
#include "mdm_gsm_gprs_http.h"

#if (MODEM_USED == GSM_GPRS_MDM)
/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Task period in ms */
#define MDM_TASK_PERIOD_MS                               ((UINT16)(1))

/* Get SMS constants */
#define HEADER_BUFF_LEN                                  ((UINT8)(80))
#define MESSAGE_BUFF_LEN                                 ((UINT8)(50))

/* SMS text mode */
#define SMS_MODE_TEXT_OFF                                ((UINT8)(0))
#define SMS_MODE_TEXT_ON                                 ((UINT8)(1))

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Modem status type */
typedef enum _MdmStatus
{
   ModemOff,
   ModemOn,
   ModemReady,
   ModemConfigured,
   ModemSleep
} MdmStatusType;

/* Get sms status */
typedef enum _GetSmsStatus
{
   SmsDataNotReady,
   SmsDataReady,
} GetSmsStatusType;

/* Ring status */
typedef enum _RingStsType
{
   PhoneNotRinging = 0,
   PhoneRinging
} RingStsType;

/* SMS reception status */
typedef enum _SmsRxStsType
{
   SmsNotReceived = 0,
   SmsReceived
} SmsRxStsType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Global modem status */
extern MdmStatusType MdmStatus;
/* Events */
extern EventStructureType RingEvent;
extern EventStructureType SmsEvent;
extern EventStructureType GetSmsText;
extern EventStructureType SmsTxtReady;
extern EventStructureType MdmStsReady;
extern EventStructureType NewStringReceived;
extern EventStructureType NewMdmEvtParserRequest;
extern EventStructureType NewStringMdmTerminal;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Modem task */
void Mdm_MainTask (UINT8 Options);

/* -- SIM general API -- */
/* API to to unlock the SIM using PIN */
void Mdm_PinUnlock (const UINT8 *PIN);

/* -- Phone calls API -- */
/* API to make a phone call to specific number */
void Mdm_MakePhoneCall (UINT8 *PhoneNumb, UINT8 PhoneNumbLen);
/* API to close a phone call */
void Mdm_HangPhoneCall (void);
/* API to get a phone call */
void Mdm_GetPhoneCall (void);
/* API to check if the phone is ringing */
RingStsType Mdm_IsRinging (void);

/* -- SMS API -- */
/* API to set the SMS format type (text ON/OFF) */
void Mdm_SetSmsFormat (UINT8 TextFormat);
/* API to request the SMS data to the modem */
void Mdm_RequestSmsData (void);
/* API to get the SMS data from the modem */
GetSmsStatusType Mdm_GetSmsData (UINT8 *MessageText);
/* API to send an SMS */
void Mdm_SendSmsData (UINT8 *PhoneNmb, UINT8 PhoneNmbLen, UINT8 *MsgTxt, UINT8 MsgTxtLen);
/* API to check if an SMS has been received */
SmsRxStsType Mdm_IsSmsReceived (void);

#endif /* MODEM_USED == GSM_GPRS_MDM */

#endif	/* MDM_GSM_GPRS_H */