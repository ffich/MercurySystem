/************************************************************************
*                          Modem GSM GPRS Driver                        *
*************************************************************************
* FileName:         mdm_drv.h                                           *
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

#ifndef MDM_GSM_GPRS_DRV_H
#define	MDM_GSM_GPRS_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

#if (MODEM_USED == GSM_GPRS_MDM)
/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Driver main function period in ms */
#define MDM_DRIVER_PERIOD_MS                                ((UINT16)(10))

/************************************************************************
* EXPORTED Typedef
************************************************************************/

typedef enum _MdmRiType
{
   NoEvents,
   IncomingCall,
   IncomingSms,
} MdmRiType;

/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Global RI status */
extern MdmRiType Mmd_RingIndicator;
/* Events */
extern EventStructureType ActivateRiDetection;
extern EventStructureType NewSmsEvent;
extern EventStructureType IncomingCallEvent;
extern EventStructureType CallClosedEvent;
extern EventStructureType MdmOnOffPulseRequest;
extern EventStructureType MdmResetRequest;

/************************************************************************
* EXPORTED Functions
************************************************************************/
MdmRiType Mdm_GetModemRiSts (void);
void Mdm_DrvMainFn (UINT8 Options);

#endif /* MODEM_USED == GSM_GPRS_MDM */

#endif	/* MDM_GSM_GPRS_DRV_H */
