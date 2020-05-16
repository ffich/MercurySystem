/************************************************************************
*                          Special Functions                            *
*************************************************************************
* FileName:         sf.c                                                *
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
* F.Ficili     26/04/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "sf.h"

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
* Function:     StringCompare
* Input:        UINT8 *String1
*               UINT8 *String2
*               UINT16 Len   
* Output:       UINT8 --> 0: no match; 1: match
* Author:       F.Ficili
* Description:  Function to compare two strings.
* Date:         17/09/16
************************************************************************/
UINT8 StringCompare (UINT8 *String1, UINT8 *String2, UINT16 Len)
{
   UINT16 Index = 0;
   UINT8 Result = TRUE;
   
   /* Compare string */
   for (Index = 0; Index < Len; Index++)
   {
      if (String1[Index] != String2[Index])
      {
         Result = FALSE;
         break;
      }         
   }
   
   /* Return result */
   return Result;
}

/************************************************************************
* Function:     StringCopy
* Input:        UINT8 *SourceStr
*               UINT8 *DestStr
*               UINT16 Len   
* Output:       None
* Author:       F.Ficili
* Description:  Function to copy a string from a source (DestStr = SourceStr).
* Date:         18/09/16
************************************************************************/
void StringCopy (UINT8 *SourceStr, UINT8 *DestStr, UINT16 Len)
{
   UINT16 Index = 0;
   
   /* Compare string */
   for (Index = 0; Index < Len; Index++)
   {
      DestStr[Index] = SourceStr[Index]; 
   }
}

/************************************************************************
* Function:     ClearBuffer
* Input:        UINT8 *Buffer
*               UINT16 Len 
* Output:       None
* Author:       F.Ficili
* Description:	 Clear the buffer content.
* Date:         20/05/17
************************************************************************/
void ClearBuffer (UINT8 *Buffer, UINT16 Len)
{
   UINT16 Index = 0;
   
   /* Clear buffer */
   for (Index = 0; Index <= Len; Index++)
   {
      *(Buffer + Index) = 0;
   }
}