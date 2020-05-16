/************************************************************************
*                       Modem Bluetooth Commands                        *
*************************************************************************
* FileName:         mdm_bt_cmd.h                                        *
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
* F.Ficili     27/04/19    1.0          First release                   *
************************************************************************/

#ifndef MDM_BT_CMD_H
#define MDM_BT_CMD_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

#if (MODEM_USED == BT_MDM)
/************************************************************************
* EXPORTED Defines
************************************************************************/


/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* BT mode type */
typedef enum _BtModuleModeType
{
   MODE_SLAVE = 0,
   MODE_MASTER,
   MODE_SLAVE_LOOP,
} BtModuleModeType;

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Set BT Modem AT mode */
void MdmBt_SetAtMode (void);
/* Set BT Modem COM mode */
void MdmBt_SetComMode (void);
/* Reset the BT Modem. This comand only works on AT mode */
void MdmBt_ModemReset (void);
/* Set the BT module name. This comand only works on AT mode */
void MdmBt_SetModuleName (UINT8* Name);
#endif   /* MODEM_USED == BT_MDM */

#endif	/* MDM_BT_CMD_H */

