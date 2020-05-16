/************************************************************************
*                       Modem BT Low Level Driver                       *
*************************************************************************
* FileName:         mdm_bt_drv.h                                        *
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
* F.Ficili     20/05/17    1.0          First release                   *
************************************************************************/

#ifndef MDM_BT_DRV_H
#define	MDM_BT_DRV_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

#if (MODEM_USED == BT_MDM)
/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Driver main function period in ms */
#define MDM_BT_DRIVER_PERIOD_MS                             ((UINT16)(10))

/* BT module status */
#define MODULE_IDLE                                         ((UINT8)(0))
#define MODULE_INIT                                         ((UINT8)(1))
#define MODULE_READY                                        ((UINT8)(2))

/* BT module modes */
#define COMM_MODE                                           ((UINT8)(0))
#define AT_MODE                                             ((UINT8)(1))
/* Initial mode */
#define BT_MDM_INITIAL_MODE                                 COMM_MODE

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Modem Mode */
typedef enum _BtModemMode
{
   MdmBt_ComMode,
   MdmBt_AtMode,
} BtModemMode;

/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* BT modem main function */
void MdmBt_DrvMainFn (UINT8 Options);
/* API to set BT modem mode */
void MdmBt_SetModemMode (BtModemMode Mode);
/* Get BT Modem status */
UINT8 MdmBt_GetModemStatus (void);
/* Get BT Modem mode */
UINT8 MdmBt_GetModemMode (void);

#endif /* MODEM_USED == BT_MDM */

#endif	/* MDM_BT_DRV_H */

