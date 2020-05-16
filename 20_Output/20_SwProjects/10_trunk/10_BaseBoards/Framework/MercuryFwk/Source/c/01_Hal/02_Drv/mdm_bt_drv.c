/************************************************************************
*                       Modem BT Low Level Driver                       *
*************************************************************************
* FileName:         mdm_bt_drv.c                                        *
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

/************************************************************************
* Includes
************************************************************************/
#include "mdm_bt_drv.h"

#if (MODEM_USED == BT_MDM)
/************************************************************************
* Defines
************************************************************************/
/* Initial delay */
#define BT_MDM_INITIAL_DELAY_MS                             (((UINT16)(200))/MDM_BT_DRIVER_PERIOD_MS)

/************************************************************************
* Typedefs
************************************************************************/
/* Start modem state machine state type */
typedef enum _BtModeSmStateType
{
   BtMdm_Autostart,  
   BtMdm_Idle,
   BtMdm_SetMode,
   BtMdm_Delay,           
   BtMdm_RestartMdm,
} BtModeSmStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* Modem mode */
static UINT8 BtMdmMode = BT_MDM_INITIAL_MODE;
/* Modem Status */
static UINT8 BtMdmStatus = MODULE_IDLE;

/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Events */
EventStructureType StartBtModem = {EventIdle};

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/


/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     MdmBt_StartBtModemSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Start the BT modem.
* Date:         20/05/16
************************************************************************/
void MdmBt_StartBtModemSm (void)
{
   static BtModeSmStateType BtModeSmState = BtMdm_Autostart;
   static UINT16 Counter = 0;
   
   switch (BtModeSmState)
   {
      case BtMdm_Autostart:   
         /* Generate start event */
         GenerateEvt(&StartBtModem);
         /* Switch state */
         BtModeSmState = BtMdm_Idle;             
         break;
         
      case BtMdm_Idle:
         /* Check start event */
         if (ReceiveEvt(&StartBtModem))
         {
            /* Update status */
            BtMdmStatus = MODULE_INIT;
            /* Switch state */
            BtModeSmState = BtMdm_SetMode;
         }
         break;
         
      case BtMdm_SetMode:
         /* Set mdm pin DDR */
         BSP_BT_MDM_RESET_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_BT_MDM_MODE_TRIS = LINE_DIRECTION_OUTPUT;         
         /* Set mode pin */
         BSP_BT_MDM_MODE = BtMdmMode;                  
         /* Set reset line low */
         BSP_BT_MDM_RESET = LINE_STATE_LOW;                  
         /* Switch state */
         BtModeSmState = BtMdm_Delay;                  
         break;

      case BtMdm_Delay:
         /* Increment counter */
         Counter++;
         /* Check counter */
         if (Counter >= BT_MDM_INITIAL_DELAY_MS)
         {
            /* Reset */
            Counter = 0;
            /* Reset modem */
            BSP_BT_MDM_RESET = LINE_STATE_HIGH;                  
            /* Switch state */
            BtModeSmState = BtMdm_RestartMdm;            
         }
         break;

      case BtMdm_RestartMdm:
         /* Update status */
         BtMdmStatus = MODULE_READY;         
         /* Switch state */
         BtModeSmState = BtMdm_Idle;                
         break;
         
      default:
         break;         
   }
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     MdmBt_DrvMainFn
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Bt Modem driver periodic function.
* Date:         20/05/16
************************************************************************/
void MdmBt_DrvMainFn (UINT8 Options)
{
   /* Start modem state machine */
   MdmBt_StartBtModemSm();
}

/************************************************************************
* Function:     MdmBt_SetModemMode
* Input:        BtModemMode Mode
* Output:       None
* Author:       F.Ficili
* Description:  Set BT Modem mode.
* Date:         20/05/16
************************************************************************/
void MdmBt_SetModemMode (BtModemMode Mode)
{
   /* Set internal mode variable */
   if (Mode == MdmBt_ComMode)
   {
      /* Comm mode (pairing) */
      BtMdmMode = COMM_MODE;
   }
   else
   {
      /* AT mode */
      BtMdmMode = AT_MODE;   
   }
   
   /* Generate start event */
   GenerateEvt(&StartBtModem);
}

/************************************************************************
* Function:     MdmBt_GetModemStatus
* Input:        None
* Output:       UINT8 --> MODULE_IDLE, MODULE_INIT, MODULE_READY
* Author:       F.Ficili
* Description:  Get BT Modem status.
* Date:         20/05/16
************************************************************************/
UINT8 MdmBt_GetModemStatus (void)
{
   return BtMdmStatus;
}

/************************************************************************
* Function:     MdmBt_GetModemMode
* Input:        None
* Output:       UINT8 --> COMM_MODE, AT_MODE
* Author:       F.Ficili
* Description:  Get BT Modem mode.
* Date:         20/05/16
************************************************************************/
UINT8 MdmBt_GetModemMode (void)
{
   return BtMdmMode;
}

#endif /* MODEM_USED == BT_MDM */