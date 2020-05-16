/************************************************************************
*                             RTCC Driver                              	*
*************************************************************************
* FileName:         rtcc_drv.c                                          *
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
* F.Ficili     19/04/17    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "rtcc_drv.h"

/************************************************************************
* Defines
************************************************************************/
/* 1 and 0 defines */
#define SET_TO_ZERO                                            0U
#define SET_TO_ONE                                             1U

/* Shift defines */
#define SHIFT_4                                                4U

/* Masks defines */
#define LOW_NIBBLE_MASK                                        0x0F
#define HIGH_NIBBLE_MASK                                       0xF0

/************************************************************************
* Macros
************************************************************************/
/* Macro for BCD to Dec conversion */
#define Bcd2Dec(x)                        (((x & HIGH_NIBBLE_MASK) >> SHIFT_4) * 10) + (x & LOW_NIBBLE_MASK)
/* Macro for Dec to BCD conversion */
#define Dec2Bcd(x)                        (((x/10) << SHIFT_4) & HIGH_NIBBLE_MASK) | ((x % 10) & LOW_NIBBLE_MASK)

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
void RtccIrqInit (IntPriorityType Priority);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/
/************************************************************************
* Function:     RtccIrqInit
* Input:        UINT8 Priority                    
* Output:       None
* Author:       F.Ficili
* Description:  Initialize Rtcc Interrupts.
* Date:         20/04/17
************************************************************************/
void RtccIrqInit (IntPriorityType Priority)
{
   /* Clear Interrupt flag */
   PIR3bits.RTCCIF = STD_OFF;
   /* Set Interrupt priority */
   IPR3bits.RTCCIP = Priority;
   /* Set Interrupt flag */
   PIE3bits.RTCCIE = STD_ON;
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     Rtcc_RtcConfig
* Input:        RtccConfigType *RtcConfig                     
* Output:       None
* Author:       F.Ficili
* Description:  Configure the RTCC (Real Time Clock Calendar) module.
* Date:         19/04/17
************************************************************************/
void Rtcc_RtcConfig(RtccConfigType *RtcConfig)
{
   /* Rtc out pin enable */
   RTCCFGbits.RTCOE = RtcConfig->RtcOutPinEnable;   
   /* RTCC Seconds Clock Output Select bits */
   PADCFG1bits.RTSECSEL0 = ((RtcConfig->RtcOutputSelectBit) & 0x01);
   PADCFG1bits.RTSECSEL1 = ((RtcConfig->RtcOutputSelectBit >> 1) & 0x01);   
   /* If is requested to enable interrupts */
   if (RtcConfig->IntEnable == EnableInt)
   {
      /* Enable it and set the priority */
      RtccIrqInit(RtcConfig->IntPriority);
   }
}

/************************************************************************
* Function:     Rtcc_AlarmConfig
* Input:        RtccAlarmConfigType *AlarmConfig                    
* Output:       None
* Author:       F.Ficili
* Description:  Configure the RTC alarm.
* Date:         20/04/17
************************************************************************/
void Rtcc_AlarmConfig (RtccAlarmConfigType *AlarmConfig)
{
   /* Rtcc Alarm enable */
   ALRMCFGbits.ALRMEN = AlarmConfig->RtcAlarmEnable;
   /* Rtcc chime enable */
   ALRMCFGbits.CHIME = AlarmConfig->RtcChimeEnable;
   /* Rtcc alarm mask */
   ALRMCFGbits.AMASK = AlarmConfig->AlarmMask;
   /* Rtcc alarm repeat */
   ALRMRPTbits.ARPT = AlarmConfig->AlarmRepeat;   
}

/************************************************************************
* Function:     Rtcc_SetRtcStatus
* Input:        UINT8 Status                     
* Output:       None
* Author:       F.Ficili
* Description:  Set the RTC status (enabled or disabled).
* Date:         19/04/17
************************************************************************/
void Rtcc_SetRtcStatus (UINT8 Status)
{
   /* Enable/Disable on-chip RTCC */
   RTCCFGbits.RTCEN = Status;
}

/************************************************************************
* Function:     Rtcc_UnlockRtc
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  Unlock RTCC using the specific sequence.
* Date:         19/04/17
************************************************************************/
void Rtcc_UnlockRtc (void)
{
   /* Unlock RTCC sequence */
   asm("movlb 0x0F");
   asm("movlw 0x55");
   asm("movwf EECON2");
   asm("movlw 0xAA");
   asm("movwf EECON2");
   /* Replaces asm("bsf RTCCFG,5"); */
   RTCCFGbits.RTCWREN = STD_ON;
}

/************************************************************************
* Function:     Rtcc_LockRtc
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  Lock RTCC using the specific sequence.
* Date:         19/04/17
************************************************************************/
void Rtcc_LockRtc (void)
{
   /* Lock RTCC sequence */
   asm("movlb 0x0F");
   asm("movlw 0x55");
   asm("movwf EECON2");
   asm("movlw 0xAA");
   asm("movwf EECON2"); 
   /* Replaces asm("bcf RTCCFG,5"); */
   RTCCFGbits.RTCWREN = STD_OFF;
}

/************************************************************************
* Function:     Rtcc_GetRtccValue
* Input:        RtccValPointerType RtccPtrVal                     
* Output:       UINT8
* Author:       F.Ficili
* Description:  Get the requested RTCC value (Year, Month, Day, Weekday,
*               Hours, Minutes, Seconds). Register mapping table 17-3 
*               PIC18F46J50 DS. 
* Date:         20/04/17
************************************************************************/
UINT8 Rtcc_GetRtccValue (RtccValPointerType RtccPtrVal)
{
   /* Service */
   UINT8 RegValue;
   UINT8 RetValue;
   
   switch (RtccPtrVal)
   {
      case ValPtrYear:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ONE;
         RTCCFGbits.RTCPTR0 = SET_TO_ONE;           
         /* Get value */
         RegValue = RTCVALL;
         /* Convert the value */
         RetValue = Bcd2Dec(RegValue);
         break;
         
      case ValPtrMonth:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ONE;         
         RTCCFGbits.RTCPTR0 = SET_TO_ZERO;  
         /* Get value */
         RegValue = RTCVALH;
         /* Convert the value */
         RetValue = Bcd2Dec(RegValue);         
         break;

      case ValPtrDay:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ONE;         
         RTCCFGbits.RTCPTR0 = SET_TO_ZERO;  
         /* Get value */
         RegValue = RTCVALL;
         /* Convert the value */
         RetValue = Bcd2Dec(RegValue);              
         break;
         
      case ValPtrWeekday:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ZERO;         
         RTCCFGbits.RTCPTR0 = SET_TO_ONE;  
         /* Get value */
         RetValue = RTCVALH;           
         break;

      case ValPtrHours:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ZERO;         
         RTCCFGbits.RTCPTR0 = SET_TO_ONE;  
         /* Get value */
         RegValue = RTCVALL;
         /* Convert the value */
         RetValue = Bcd2Dec(RegValue);            
         break;
         
      case ValPtrMinutes:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ZERO;         
         RTCCFGbits.RTCPTR0 = SET_TO_ZERO;  
         /* Get value */
         RegValue = RTCVALH;
         /* Convert the value */
         RetValue = Bcd2Dec(RegValue);            
         break;
         
      case ValPtrSeconds:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ZERO;         
         RTCCFGbits.RTCPTR0 = SET_TO_ZERO;  
         /* Get value */
         RegValue = RTCVALL;
         /* Convert the value */
         RetValue = Bcd2Dec(RegValue);   
         break;         

      default:
         break;
   }
   
   /* Return value */
   return RetValue;
}

/************************************************************************
* Function:     Rtcc_GetRtccAlarm
* Input:        RtccAlrmPointerType RtccPtrAlrm                     
* Output:       UINT8
* Author:       F.Ficili
* Description:  Get the requested RTCC alarm (Month, Day, Weekday,
*               Hours, Minutes, Seconds). Register mapping table 17-4 
*               PIC18F46J50 DS. 
* Date:         20/04/17
************************************************************************/
UINT8 Rtcc_GetRtccAlarm (RtccAlrmPointerType RtccPtrAlrm)
{
   /* Service */
   UINT8 RegValue;
   UINT8 AlrmValue;
   
   switch (RtccPtrAlrm)
   {
      case AlrmPtrMonth:
         /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ONE;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ZERO;  
         /* Get value */
         RegValue = ALRMVALH;
         /* Convert the value */
         AlrmValue = Bcd2Dec(RegValue);            
         break;

      case AlrmPtrDay:
         /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ONE;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ZERO;  
         /* Get value */
         RegValue = ALRMVALL;
         /* Convert the value */
         AlrmValue = Bcd2Dec(RegValue);             
         break;
         
      case AlrmPtrWeekday:
        /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ZERO;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ONE;  
         /* Get value */
         AlrmValue = ALRMVALH;                 
         break;

      case AlrmPtrHours:
         /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ZERO;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ONE;  
         /* Get value */
         RegValue = ALRMVALL;
         /* Convert the value */
         AlrmValue = Bcd2Dec(RegValue);            
         break;
         
      case AlrmPtrMinutes:
        /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ZERO;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ZERO;  
         /* Get value */
         RegValue = ALRMVALH;
         /* Convert the value */
         AlrmValue = Bcd2Dec(RegValue);             
         break;
         
      case AlrmPtrSeconds:
        /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ZERO;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ZERO;  
         /* Get value */
         RegValue = ALRMVALL;
         /* Convert the value */
         AlrmValue = Bcd2Dec(RegValue);           
         break;         

      default:
         break;
   }
   
   /* Return value */
   return AlrmValue;
}

/************************************************************************
* Function:     Rtcc_SetRtccValue
* Input:        RtccValPointerType RtccPtrVal
*               UINT8 Value                      
* Output:       None
* Author:       F.Ficili
* Description:  Set the pointed RTCC value (Year, Month, Day, Weekday,
*               Hours, Minutes, Seconds).
* Date:         20/04/17
************************************************************************/
void Rtcc_SetRtccValue (RtccValPointerType RtccPtrVal, UINT8 Value)
{
   switch (RtccPtrVal)
   {
      case ValPtrYear:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ONE;
         RTCCFGbits.RTCPTR0 = SET_TO_ONE;           
         /* Set register value */
         RTCVALL = Dec2Bcd(Value);
         break;
         
      case ValPtrMonth:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ONE;
         RTCCFGbits.RTCPTR0 = SET_TO_ZERO;           
         /* Set register value */
         RTCVALH = Dec2Bcd(Value);         
         break; 
         
      case ValPtrDay:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ONE;
         RTCCFGbits.RTCPTR0 = SET_TO_ZERO;           
         /* Set register value */
         RTCVALL = Dec2Bcd(Value);         
         break;
         
      case ValPtrWeekday:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ZERO;         
         RTCCFGbits.RTCPTR0 = SET_TO_ONE;           
         /* Set register value */
         RTCVALH = Dec2Bcd(Value);         
         break;   

      case ValPtrHours:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ZERO;
         RTCCFGbits.RTCPTR0 = SET_TO_ONE;           
         /* Set register value */
         RTCVALL = Dec2Bcd(Value);         
         break;
         
      case ValPtrMinutes:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ZERO;
         RTCCFGbits.RTCPTR0 = SET_TO_ZERO;           
         /* Set register value */
         RTCVALH = Dec2Bcd(Value);         
         break;   

      case ValPtrSeconds:
         /* Set cfg pointer */
         RTCCFGbits.RTCPTR1 = SET_TO_ZERO;         
         RTCCFGbits.RTCPTR0 = SET_TO_ZERO;           
         /* Set register value */
         RTCVALL = Dec2Bcd(Value);         
         break;
         
      default:
         break;                     
   }
}

/************************************************************************
* Function:     Rtcc_SetRtccAlarm
* Input:        RtccValPointerType RtccPtrAlrm
*               UINT8 Value                      
* Output:       None
* Author:       F.Ficili
* Description:  Set the pointed RTCC Alarm (Month, Day, Weekday,
*               Hours, Minutes, Seconds).
* Date:         20/04/17
************************************************************************/
void Rtcc_SetRtccAlarm (RtccAlrmPointerType RtccPtrAl, UINT8 Value)
{
   switch (RtccPtrAl)
   {
      case AlrmPtrMonth:
         /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ONE;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ZERO;  
         /* Set alarm register */   
         ALRMVALH = Dec2Bcd(Value);               
         break;
         
      case AlrmPtrDay:
         /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ONE;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ZERO;
         /* Set alarm register */   
         ALRMVALL = Dec2Bcd(Value);          
         break;

      case AlrmPtrWeekday:
         /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ZERO;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ONE;  
         /* Set alarm register */   
         ALRMVALH = Dec2Bcd(Value);        
         break;
         
      case AlrmPtrHours:
         /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ZERO;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ONE;  
         /* Set alarm register */   
         ALRMVALL = Dec2Bcd(Value);                
         break;    

      case AlrmPtrMinutes:
         /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ZERO;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ZERO;  
         /* Set alarm register */   
         ALRMVALH = Dec2Bcd(Value);                
         break;    

      case AlrmPtrSeconds:
         /* Set cfg pointer */
         ALRMCFGbits.ALRMPTR1 = SET_TO_ZERO;         
         ALRMCFGbits.ALRMPTR0 = SET_TO_ZERO;  
         /* Set alarm register */   
         ALRMVALL = Dec2Bcd(Value);              
         break;    

      default:
         break;         
   }
}

/************************************************************************
* Function:     Rtcc_RtcCalibrate
* Input:        INT8 Cal                     
* Output:       None
* Author:       F.Ficili
* Description:  Calibrate the RTCC module.
* Date:         20/04/17
************************************************************************/
void Rtcc_RtcCalibrate (INT8 Cal)
{
   /* Set calibration register value */
   RTCCAL = Cal;
}

/************************************************************************
* Function:     Rtcc_RtcIsr
* Input:        None                     
* Output:       None
* Author:       F.Ficili
* Description:  Interrutp Service Routine for RTCC.
* Date:         20/04/17
************************************************************************/
void Rtcc_RtcIsr (void)
{
   /* Clear RTCC flag */
   PIR3bits.RTCCIF = STD_OFF;
   
   /* Check if the callback function has been correctly registered */
   if (RtccIsrCallback != NULL)
   {
      /* Call callback funtion */
      RtccIsrCallback();   
   }   
}

/************************************************************************
* Function:     Rtcc_RegisterRtcIsrCallback
* Input:        Rtcc_CallbackType IsrCallback                     
* Output:       None
* Author:       F.Ficili
* Description:  Register callback function to service the ISR.
* Date:         20/04/17
************************************************************************/
void Rtcc_RegisterRtcIsrCallback (Rtcc_CallbackType IsrCallback)
{
   /* Callback registration */
   RtccIsrCallback = IsrCallback;
}