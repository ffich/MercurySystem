/************************************************************************
*                       External Interrupt                             	*
*************************************************************************
* FileName:         ext_int.c                                   			*
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
* F.Ficili     17/03/19    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "mcu_drv.h"
#include "ext_int.h"

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
* Function:     ExtInt_SetInt0Action
* Input:        ExtInt_CallbackType IsrCallback                     
* Output:       None
* Author:       F.Ficili
* Description:  API used to register action to service the Int0 external interrupt.
* Date:         17/03/19
************************************************************************/
void ExtInt_SetInt0Action (ExtInt_CallbackType Callback)
{
   /* Callback registration */
   Mcu_RegisterInt0IsrCallback(Callback);
}

/************************************************************************
* Function:     ExtInt_SetInt1Action
* Input:        ExtInt_CallbackType IsrCallback                     
* Output:       None
* Author:       F.Ficili
* Description:  API used to register action to service the Int0 external interrupt.
* Date:         17/03/19
************************************************************************/
void ExtInt_SetInt1Action (ExtInt_CallbackType Callback)
{
   /* Callback registration */
   Mcu_RegisterInt1IsrCallback(Callback);
}