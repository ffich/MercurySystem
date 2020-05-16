/************************************************************************
*                          Standard Services                            *
*************************************************************************
* FileName:         std_ser.c                                           *
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
* F.Ficili     06/06/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "std_ser.h"
#include "stdlib.h"
#include "string.h"

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
/* LED behavour status */
UINT8 LedBehaviourSts = LED_DEFAULT_BEHAV;

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
* Function:     SetLedBehavSts
* Input:        UINT8 LedBehav
* Output:       None
* Author:       F.Ficili
* Description:	 Update led behaviour status on request.
* Date:         06/06/17
************************************************************************/
void SetLedBehavSts (UINT8 LedBehav)
{
   /* Check Request  */
   switch (LedBehav)
   {
      case LED_ALWAYS_OFF:
         /* Set status to off */
         Led_SetLedStatus(LED_1,LED_STS_OFF);
         /* Update global variable */
         LedBehaviourSts = LED_ALWAYS_OFF;
         break;
         
      case LED_I2C_PULSE:
         /* Set LED pulse behaviour */
         Led_SetLedPulseTime(LED_1,50);
         /* Update global variable */
         LedBehaviourSts = LED_I2C_PULSE;         
         break;
         
      case LED_BLINK:
         /* Set blink time */
         Led_SetLedBlinkTime(LED_1,50,950);
         /* Set status to blink */
         Led_SetLedStatus(LED_1,LED_STS_BLINK);         
         /* Update global variable */
         LedBehaviourSts = LED_BLINK;           
         break;

      case LED_DIRECT:
         /* Update global variable */
         LedBehaviourSts = LED_DIRECT;            
         break;

      default:
         break;         
   }  
}

/************************************************************************
* Function:     ControlLedDirect
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 Directly control LED status.
* Date:         06/06/17
************************************************************************/
void ControlLedDirect (void)
{
   /* Check if allowed */
   if (LedBehaviourSts == LED_DIRECT)
   {
      /* Control Led blink */
      if (CmdBuffer[PAR_1_INDEX] == ON)
      {
         Led_SetLedStatus(LED_1,LED_STS_ON);
      }  
      else
      {
         Led_SetLedStatus(LED_1,LED_STS_OFF);
      }      
   }
}

/************************************************************************
* Function:     BoardReset
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 Reset the board.
* Date:         06/06/17
************************************************************************/
void BoardReset (void)
{
   /* Reset the board */
   Mcu_Reset();
}

/************************************************************************
* Function:     LowPowerMode
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 Enter low power mode.
* Date:         06/06/17
************************************************************************/
void LowPowerMode (void)
{
   /* Put all pin in low state */
   LATA = 0x00;
   #if defined(__16F1829)
      LATB = 0x00;
   #endif
   LATC = 0x00;   
   /* Disable Wdg */
   Mcu_SetWdgSts(OFF);
   /* Enter low power mode */
   asm ("SLEEP");
   
   /* ... Sleep Mode, waiting for wake-up ... */
   
   /* Re-Enable Wdg after wake-up */
   Mcu_SetWdgSts(ON);   
}

/************************************************************************
* Function:     GetDieTemperature
* Input:        None
* Output:       UINT16 (result of die temp conversion)
* Author:       F.Ficili
* Description:	 Get the DIE temperature 
* Date:         01/01/18
************************************************************************/
UINT16 GetDieTemperature (void)
{
   UINT16 DieTempRes; 
  
   /* Adc channel selection */
   Adc_SelectChannel(TEMP_SENS);   
   /* Start conversion */
   Adc_StartConversion();
   /* Wait conversion completed */
   while (Adc_GetConvSts() == AdcConvInProgress);
   /* Get result */
   DieTempRes = Adc_GetConvResult();
   
   return DieTempRes;
}

/************************************************************************
* Function:     StdS_ManageStdServices
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 Manage standard protocol commands.
* Date:         22/06/17
************************************************************************/
void StdS_ManageStdServices (void)
{
   UINT16 DieTemp;
   UINT8 DieTempStr[4];
   
   /* Check command */
   switch (CmdBuffer[CMD_INDEX]) 
   {         
      case PING_CMD:
         /* Copy echo string */
         StringCopy(PING_ECHO,RepBuffer,PING_ECHO_LEN);
         /* Set response lenght */
         RepBufferLen = PING_ECHO_LEN;         
         /* Notify */
         GenerateEvt(&I2cReplyPacketReady);            
         break;  

      case BOARD_ID_CMD:
         /* Copy Board ID string */
         StringCopy((void*)BOARD_ID,RepBuffer,BOARD_ID_LEN);
         /* Set response lenght */
         RepBufferLen = BOARD_ID_LEN;
         /* Notify */
         GenerateEvt(&I2cReplyPacketReady);                
         break;   

      case FWK_VERSION_CMD:
         /* Send FW version */         
         StringCopy((void*)FWK_VERSION,RepBuffer,FWK_VERSION_LEN);
         /* Set response lenght */
         RepBufferLen = FWK_VERSION_LEN;
         /* Notify */
         GenerateEvt(&I2cReplyPacketReady);                    
         break; 
         
      case APP_VERSION_CMD:
         /* Send APP version */
         StringCopy((void*)APP_VERSION,RepBuffer,APP_VERSION_LEN);
         /* Set response lenght */
         RepBufferLen = APP_VERSION_LEN;
         /* Notify */
         GenerateEvt(&I2cReplyPacketReady);           
         
         break;          

      case BOARD_RESET_CMD:
         /* Reset the board */
         BoardReset();
         break;  

      case BOARD_LP_MODE_CMD:
         /* Low power mode */
         LowPowerMode();
         break;  

      case CONTROL_LED_BEHAV_CMD:
         /* Update led behav */
         SetLedBehavSts(CmdBuffer[PAR_1_INDEX]);
         break;

      case CONTROL_LED_DIRECT_CMD:
         /* Control LED status */
         ControlLedDirect();
         break;  
         
      case GET_DIE_TEMP_RAW_CMD:
         /* Get DIE temperature raw */
         DieTemp = GetDieTemperature();         
         /* Prepare response */
         RepBuffer[0] = ((DieTemp & 0xFF00) >> 8);
         RepBuffer[1] = (DieTemp & 0x00FF);       
         /* Set response lenght */
         RepBufferLen = GET_DIE_TEMP_RAW_LEN;
         /* Notify */
         GenerateEvt(&I2cReplyPacketReady);         
         break;          
         
      case GET_DIE_TEMP_ASCII_CMD:
         /* Get DIE temperature raw */
         DieTemp = GetDieTemperature();
         /* Calculate roughly corresponding temp value in Celsius */
         DieTemp = (DieTemp/8) - 40;
         /* Converto to ASCII */
         utoa(DieTempStr,DieTemp,10);
         /* Copy data into response buffer */
         StringCopy(DieTempStr,RepBuffer,GET_DIE_TEMP_ASCII_LEN);         
         /* Set response lenght */
         RepBufferLen = GET_DIE_TEMP_ASCII_LEN;
         /* Notify */
         GenerateEvt(&I2cReplyPacketReady);         
         break;           

      default:
         break;
   }   
}

/************************************************************************
* Function:     StdS_SetI2cAddress
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 Set the board I2c Address.
* Date:         13/01/19
************************************************************************/
void StdS_SetI2cAddress (void)
{
   #if (ADDRESS_MODE == ADDR_MODE_DIP_SWITCH)
      /* Get addr from dip-switch */
      I2cSlaveAddress |= (!BSP_DIPSWITCH_4 << 3);
      I2cSlaveAddress |= (!BSP_DIPSWITCH_3 << 2);
      I2cSlaveAddress |= (!BSP_DIPSWITCH_2 << 1);
      I2cSlaveAddress |= (!BSP_DIPSWITCH_1 << 0);
   #endif

   #if (ADDRESS_MODE == ADDR_MODE_HARDCODED)
      /* Get Hard-coded address */
      I2cSlaveAddress = I2C_HARDCODED_SLAVE_ADDRESS;
   #endif       
}
