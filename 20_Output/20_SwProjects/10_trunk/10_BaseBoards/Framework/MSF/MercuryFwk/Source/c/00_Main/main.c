/************************************************************************
*                               Main                                    *
*************************************************************************
* FileName:         main.c                                              *
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
* F.Ficili     23/07/16    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
*************************************************************************/
#include "hal.h"
#include "os_sched.h"
#include "config_bits.h"

/************************************************************************
* Defines
************************************************************************/


/************************************************************************
* Global Variables
************************************************************************/

/************************************************************************
* Local Prototypes
************************************************************************/

/************************************************************************
* Main Function
************************************************************************/

/************************************************************************
* Function:     main
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Main function
* Date:         23/07/16
************************************************************************/
INT16 main(void)
{
    /* Call System Initialization */
    SystemInit();

    /* Call Main Scheduler */
    Sch_MainScheduler();
    
    /* execution should never reach this point */
    return EXIT_SUCCESS;
}
