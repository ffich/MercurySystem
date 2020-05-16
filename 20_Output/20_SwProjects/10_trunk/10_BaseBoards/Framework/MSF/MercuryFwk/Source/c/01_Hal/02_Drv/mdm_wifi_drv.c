/************************************************************************
*                       Modem WiFi Low Level Driver                     *
*************************************************************************
* FileName:         mdm_wifi_drv.c                                      *
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
* F.Ficili     23/09/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "mdm_wifi_drv.h"

#if (MODEM_USED == WIFI_MDM)
/************************************************************************
* Defines
************************************************************************/
/* WIFI module status */
#define MODULE_IDLE                                         ((UINT8)(0))
#define MODULE_INIT                                         ((UINT8)(1))
#define MODULE_READY                                        ((UINT8)(2))
/* Initial delay */
#define WIFI_MDM_INITIAL_DELAY_MS                           (((UINT16)(200))/MDM_WIFI_DRIVER_PERIOD_MS)

/************************************************************************
* Typedefs
************************************************************************/
/* Start modem state machine state type */
typedef enum _WifiModeSmStateType
{
   WifiMdm_Autostart,  
   WifiMdm_Idle,
   WifiMdm_Reset,
   WifiMdm_Delay,           
   WifiMdm_RestartMdm,
} WifiModeSmStateType;

/************************************************************************
* LOCAL Variables
************************************************************************/
/* Modem Status */
UINT8 WifiMdmStatus = MODULE_IDLE;

/************************************************************************
* GLOBAL Variables
************************************************************************/
/* Events */
EventStructureType StartWifiModem = {EventIdle};

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/


/************************************************************************
* LOCAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     MdmWifi_StartWifiModemSm
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Start the Wifi modem.
* Date:         23/09/17
************************************************************************/
void MdmWifi_StartWifiModemSm (void)
{
   static WifiModeSmStateType WifiModeSmState = WifiMdm_Autostart;
   static UINT16 Counter = 0;
   
   switch (WifiModeSmState)
   {
      case WifiMdm_Autostart:   
         /* Set mdm pin DDR */
         BSP_WIFI_MDM_RESET_TRIS = LINE_DIRECTION_OUTPUT;
         /* Reset modem */
         BSP_WIFI_MDM_RESET = LINE_STATE_LOW;               
         /* Generate start event */
         GenerateEvt(&StartWifiModem);
         /* Switch state */
         WifiModeSmState = WifiMdm_Idle;             
         break;
         
      case WifiMdm_Idle:
         /* Check start event */
         if (ReceiveEvt(&StartWifiModem))
         {          
            /* Reset modem */
            BSP_WIFI_MDM_RESET = LINE_STATE_LOW;               
            /* Update status */
            WifiMdmStatus = MODULE_INIT;
            /* Switch state */
            WifiModeSmState = WifiMdm_Delay;               
         }
         break;

      case WifiMdm_Delay:
         /* Increment counter */
         Counter++;
         /* Check counter */
         if (Counter >= WIFI_MDM_INITIAL_DELAY_MS)
         {
            /* Reset */
            Counter = 0;             
            /* Switch state */
            WifiModeSmState = WifiMdm_RestartMdm;            
         }
         break;

      case WifiMdm_RestartMdm:  
         /* Activate modem */
         BSP_WIFI_MDM_RESET = LINE_STATE_HIGH;            
         /* Update status */
         WifiMdmStatus = MODULE_READY;         
         /* Switch state */
         WifiModeSmState = WifiMdm_Idle;                
         break;
         
      default:
         break;         
   }
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     MdmWifi_DrvMainFn
* Input:        UINT8 Options
* Output:       None
* Author:       F.Ficili
* Description:  Wifi Modem driver periodic function.
* Date:         23/09/17
************************************************************************/
void MdmWifi_DrvMainFn (UINT8 Options)
{
   /* Start modem state machine */
   MdmWifi_StartWifiModemSm();
}

/************************************************************************
* Function:     MdmWifi_RestartModem
* Input:        BtModemMode Mode
* Output:       None
* Author:       F.Ficili
* Description:  Restart Wifi Modem.
* Date:         23/09/17
************************************************************************/
void MdmWifi_RestartModem (void)
{
   /* Generate start event */
   GenerateEvt(&StartWifiModem);
}

#endif /* MODEM_USED == WIFI_MDM */