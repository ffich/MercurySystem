/************************************************************************
*                             Os Services                               *
*************************************************************************
* FileName:         os_ser.h                                            *
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
#if (EVENT_OPTION == BASIC_EVENT_OPTION)
/* Basic Event Options */
#define NO_MASK                                 ((UINT8)(0))
#define MASK_RECEIVE                            ((UINT8)(1))
#define MASK_GENERATE                           ((UINT8)(2))
#define MASK_ALL                                ((UINT8)(3))
#endif

/************************************************************************
* EXPORTED Typedef
************************************************************************/
#if (EVENT_OPTION == BASIC_EVENT_OPTION)
/* Event Status */
typedef enum _EventStatusEnum
{
    EventIdle,
    EventReceived,
    EventSent,
    EventNotSent,
} EventStatusType;

/* Event Structure */
typedef struct _EventStructure
{
    UINT8 EventOptions;
    EventStatusType EventStatus;
} EventStructureType;
#endif

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
#if (EVENT_OPTION == BASIC_EVENT_OPTION)
/* Generate events */
EventStatusType GenerateEvt (EventStructureType *Event);
/* Receive events */
EventStatusType ReceiveEvt (EventStructureType *Event);
/* Mask reception of a particular event */
void MaskEvtRec (EventStructureType *Event);
/* Mask send of a particular event */
void MaskEvtSend (EventStructureType *Event);
/* Mask a particular event */
void MaskEvt (EventStructureType *Event);
/* Unmask a particular event */
void UnmaskEvt (EventStructureType *Event);
#endif

#endif /* OS_SER_H */