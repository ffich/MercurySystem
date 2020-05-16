/************************************************************************
*                       Modem Bluetooth Commands                        *
*************************************************************************
* FileName:         mdm_bt_cmd.c                                        *
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

/************************************************************************
* Includes
************************************************************************/
#include "mdm_bt_cmd.h"

#if (MODEM_USED == BT_MDM)
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
* Function:     MdmBt_SetAtMode
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  API used to set BT Modem AT mode.
* Date:         27/04/19
************************************************************************/
void MdmBt_SetAtMode (void)
{
   /* Set the correct UART cfg */
   MdmBt_UartSetConfigAtMode();
   /* Set the AT mode */
   MdmBt_SetModemMode(MdmBt_AtMode);
}

/************************************************************************
* Function:     MdmBt_SetComMode
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  API used to set BT Modem COM mode.
* Date:         27/04/19
************************************************************************/
void MdmBt_SetComMode (void)
{
   /* Set the correct UART cfg */
   MdmBt_UartSetConfigComMode();   
   /* Set the COM mode */
   MdmBt_SetModemMode(MdmBt_ComMode);
}

/************************************************************************
* Function:     MdmBt_ModemReset
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  API used to reset the BT Modem. This command only works on AT mode.
* Date:         27/04/19
************************************************************************/
void MdmBt_ModemReset (void)
{      
   UINT8 Data[20];
   
   /* Check if the mode is the correct one */
   if (MdmBt_GetModemMode() == AT_MODE)
   {
      /* Perepare data buffer */
      strcpy(Data,"AT+RESET");
      /* Add tail */
      strcat(Data,"\r\n");
      strcat(Data,NULL);        
      /* Send command */
      MdmBt_SendBtMsg(Data);
   }
}

/************************************************************************
* Function:     MdmBt_SetModuleName
* Input:        UINT8* Name
* Output:       None
* Author:       F.Ficili
* Description:  API used to set the BT module name. This command only works on AT mode.
* Date:         27/04/19
************************************************************************/
void MdmBt_SetModuleName (UINT8* Name)
{
   UINT8 Data[40];
   
   /* Check if the mode is the correct one */
   if (MdmBt_GetModemMode() == AT_MODE)
   {
      /* Perepare data buffer */      
      strcpy(Data,"AT+NAME=");
      strcat(Data,Name);
      /* Add tail */
      strcat(Data,"\r\n");
      strcat(Data,NULL);    
      /* Send command */
      MdmBt_SendBtMsg(Data);      
   }   
}  

/************************************************************************
* Function:     MdmBt_SetModuleMode
* Input:        BtModuleModeType Mode
* Output:       None
* Author:       F.Ficili
* Description:  API used to set the BT module Mode. This command only works on AT mode.
* Date:         27/04/19
************************************************************************/
void MdmBt_SetModuleMode (BtModuleModeType Mode)
{
   UINT8 Data[20];
   
   /* Check if the mode is the correct one */
   if (MdmBt_GetModemMode() == AT_MODE)
   {
      /* Perepare data buffer */
      switch (Mode)
      {
         case MODE_SLAVE:
            strcpy(Data,"AT+ROLE=0");
            break;
            
         case MODE_MASTER:
            strcpy(Data,"AT+ROLE=1");
            break;
            
         case MODE_SLAVE_LOOP:
            strcpy(Data,"AT+ROLE=2");
            break;
            
         default:
            break;
      }
      /* Add tail */
      strcat(Data,"\r\n");
      strcat(Data,NULL);    
      /* Send command */
      MdmBt_SendBtMsg(Data);      
   }    
}
#endif   /* MODEM_USED == BT_MDM */