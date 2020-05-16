/************************************************************************
*                             Os Services                               *
*************************************************************************
* FileName:         syst_ser.h                                          *
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
* F.Ficili     24/07/16    1.0          First release                   *
************************************************************************/

#ifndef OS_SER_H
#define OS_SER_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/


/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Event Status */
typedef enum EventStatusEnum
{
   EventIdle,
   EventReceived,
   EventSent,
   EventNotSent,
} EventStatusType;

/* Event Structure */
typedef struct EventStructure
{
   EventStatusType EventStatus;
} EventStructureType;

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Generate events */
EventStatusType GenerateEvt (EventStructureType *Event);
/* Receive events */
EventStatusType ReceiveEvt (EventStructureType *Event);

#endif /* OS_SER_H */