/************************************************************************
*                              	APP                              		*
*************************************************************************
* FileName:         app.c                                      			*
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
* F.Ficili     09/07/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "app.h"

/************************************************************************
* Defines
************************************************************************/
#define I2C_TX_BUFFER_LEN												10u

/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/
static UINT8 I2cTxBuffer[I2C_TX_BUFFER_LEN];  

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
* Function:     MyApp_Task
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  My app main task.
* Date:         09/10/16
************************************************************************/
void MyApp_Task (UINT8 Options)
{    
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* Make app init. if necesary */ 
         Led_SetLedBlinkTime(LED_1,100,900);
         Led_SetLedStatus(LED_1, LED_STS_BLINK);
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* App periodic calls */
         
         break;

      /* Default */
      default:
         break;
   }
}