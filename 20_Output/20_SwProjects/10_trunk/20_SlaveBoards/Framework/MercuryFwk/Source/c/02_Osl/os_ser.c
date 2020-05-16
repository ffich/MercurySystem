/************************************************************************
*                             Os Services                               *
*************************************************************************
* FileName:         os_ser.c                                            *
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

/************************************************************************
* Includes
************************************************************************/
#include "os_ser.h"

/************************************************************************
* Defines
************************************************************************/


/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/


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
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     void GenerateEvt (EventStructureType *Event)
* Input:        EventStructureType *Event
* Output:       EventStatusType
* Author:       F.Ficili	
* Description:	Generate event.    
* Date:         24/07/16
************************************************************************/
EventStatusType GenerateEvt (EventStructureType *Event)
{
   /* Event Status */
   EventStatusType EventStatus = EventIdle;

   if ((Event->EventOptions != MASK_GENERATE) && (Event->EventOptions != MASK_ALL))
   {
      /* Send the event */
      Event->EventStatus = EventSent;
      /* Update event status */
      EventStatus = EventSent;
   }
   else
   {
      /* Update event status */
      EventStatus = EventNotSent;
   }

   /* Return event status */
   return(EventStatus);
}
	
/************************************************************************
* Function:     void ReceiveEvt (EventStructureType *Event)
* Input:        EventStructureType *Event
* Output:       EventStatusType
* Author:       F.Ficili	
* Description:	Receive event.  
* Date:         24/07/16
************************************************************************/
EventStatusType ReceiveEvt (EventStructureType *Event)
{
   /* Event Status */
   EventStatusType EventStatus = EventIdle;

   if ((Event->EventOptions != MASK_RECEIVE) && (Event->EventOptions != MASK_ALL))
   {
      /* Check event status */
      if (Event->EventStatus == EventSent)
      {
         /* The event is now received */
         Event->EventStatus = EventReceived;
         /* Update event status */
         EventStatus = EventReceived;
      }
      else
      {
         /* Update event status */
         EventStatus = EventIdle;
      }
   }

   /* Return event status */
   return(EventStatus);
}

/************************************************************************
* Function:     void MaskEvtRec (EventStructureType *Event)
* Input:        EventStructureType *Event
* Output:       void
* Author:       F.Ficili
* Description:	Mask reception of a particular event.
* Date:         24/07/16
************************************************************************/
void MaskEvtRec (EventStructureType *Event)
{
   /* Mask event reception */
   Event->EventOptions = MASK_RECEIVE;
}

/************************************************************************
* Function:     void MaskEvtSend (EventStructureType *Event)
* Input:        EventStructureType *Event
* Output:       void
* Author:       F.Ficili
* Description:	Mask send of a particular event.
* Date:         24/07/16
************************************************************************/
void MaskEvtSend (EventStructureType *Event)
{
   /* Mask event send */
   Event->EventOptions = MASK_GENERATE;
}

/************************************************************************
* Function:     void MaskEvt (EventStructureType *Event)
* Input:        EventStructureType *Event
* Output:       void
* Author:       F.Ficili
* Description:	Mask a particular event.
* Date:         24/07/16
************************************************************************/
void MaskEvt (EventStructureType *Event)
{
   /* Mask event */
   Event->EventOptions = MASK_ALL;
}

/************************************************************************
* Function:     void UnmaskEvt (EventStructureType *Event)
* Input:        EventStructureType *Event
* Output:       void
* Author:       F.Ficili
* Description:	Unmask a particular event.
* Date:         24/07/16
************************************************************************/
void UnmaskEvt (EventStructureType *Event)
{
   /* Mask event */
   Event->EventOptions = NO_MASK;
}

