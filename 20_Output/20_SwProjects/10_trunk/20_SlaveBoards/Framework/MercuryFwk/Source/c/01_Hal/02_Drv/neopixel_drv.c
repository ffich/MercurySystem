/************************************************************************
*                          Neopixel Driver                             	*
*************************************************************************
* FileName:         neopixel_drv.c                                    	*
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
* F.Ficili     27/01/18    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "hw_profile.h"
#include "mcu_drv.h"

/************************************************************************
* Defines
************************************************************************/
#define CH_1_PIN                                   BSP_NEOPIXEL_1_PIN
#define CH_2_PIN                                   BSP_NEOPIXEL_2_PIN
#define CH_3_PIN                                   BSP_NEOPIXEL_3_PIN

#define T0H                                        2u
#define T0L                                        7u
#define T1H                                        6u
#define T1L                                        3u

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
* Function:     Npdrv_Ch1_WriteNeopixelColor
* Input:        UINT8 PixNumb
*               UINT8 Green
*               UINT8 Red
*               UINT8 Blue  
* Output:       None
* Author:       F.Ficili
* Description:  Write a complete color for a set of pixels for CH1. This function is
*               highly optimized for speed. 
* Date:         27/01/18
************************************************************************/
void Npdrv_Ch1_WriteNeopixelColor (UINT8 PixNumb, UINT8 Green, UINT8 Red, UINT8 Blue)
{
   UINT8 i,j;
   UINT8 GreenBitArray[8];
   UINT8 RedBitArray[8];
   UINT8 BlueBitArray[8];             
   
   for (i = 0; i < 8; i++)
   {
      GreenBitArray[i] = ((Green >> (7 - i)) & 0x01);
      RedBitArray[i] = ((Red >> (7 - i)) & 0x01);
      BlueBitArray[i] = ((Blue >> (7 - i)) & 0x01);      
   }
   
   /* Critical section: enter */
   Mcu_DisableInterrupts();
   
   CH_1_PIN = STD_OFF;
   __delay_ms(1);
   
   for (j = 0; j < PixNumb; j++)
   {
      /* Write Green */
      for (i = 0; i < 8; i++)
      {
         if (GreenBitArray[i] == 0)
         {
            CH_1_PIN = STD_ON;
            _delay(T0H);
            CH_1_PIN = STD_OFF;
            _delay(T0L);      
         }
         else
         {
            CH_1_PIN = STD_ON;
            _delay(T1H);
            CH_1_PIN = STD_OFF;
            _delay(T1L);     
         }      
      }
   
      /* Write Red */
      for (i = 0; i < 8; i++)
      {
         if (RedBitArray[i] == 0)
         {
            CH_1_PIN = STD_ON;
            _delay(T0H);
            CH_1_PIN = STD_OFF;
            _delay(T0L);      
         }
         else
         {
            CH_1_PIN = STD_ON;
            _delay(T1H);
            CH_1_PIN = STD_OFF;
            _delay(T1L);     
         }         
      }  
      
      /* Write Blue */
      for (i = 0; i < 8; i++)
      {   
         if (BlueBitArray[i] == 0)
         {
            CH_1_PIN = STD_ON;
            _delay(T0H);
            CH_1_PIN = STD_OFF;
            _delay(T0L);      
         }
         else
         {
            CH_1_PIN = STD_ON;
            _delay(T1H);
            CH_1_PIN = STD_OFF;
            _delay(T1L);     
         }     
      }   
   }
    
   /* Critical section: leave */
   Mcu_EnableInterrupts();   
}

/************************************************************************
* Function:     Npdrv_Ch2_WriteNeopixelColor
* Input:        UINT8 PixNumb
*               UINT8 Green
*               UINT8 Red
*               UINT8 Blue  
* Output:       None
* Author:       F.Ficili
* Description:  Write a complete color for a set of pixels for CH2. This function is
*               highly optimized for speed. 
* Date:         31/03/18
************************************************************************/
void Npdrv_Ch2_WriteNeopixelColor (UINT8 PixNumb, UINT8 Green, UINT8 Red, UINT8 Blue)
{
   UINT8 i,j;
   UINT8 GreenBitArray[8];
   UINT8 RedBitArray[8];
   UINT8 BlueBitArray[8];             
   
   for (i = 0; i < 8; i++)
   {
      GreenBitArray[i] = ((Green >> (7 - i)) & 0x01);
      RedBitArray[i] = ((Red >> (7 - i)) & 0x01);
      BlueBitArray[i] = ((Blue >> (7 - i)) & 0x01);      
   }
   
   /* Critical section: enter */
   Mcu_DisableInterrupts();
   
   for (j = 0; j < PixNumb; j++)
   {
      /* Write Green */
      for (i = 0; i < 8; i++)
      {
         if (GreenBitArray[i] == 0)
         {
            CH_2_PIN = STD_ON;
            _delay(T0H);
            CH_2_PIN = STD_OFF;
            _delay(T0L);      
         }
         else
         {
            CH_2_PIN = STD_ON;
            _delay(T1H);
            CH_2_PIN = STD_OFF;
            _delay(T1L);     
         }      
      }
   
      /* Write Red */
      for (i = 0; i < 8; i++)
      {
         if (RedBitArray[i] == 0)
         {
            CH_2_PIN = STD_ON;
            _delay(T0H);
            CH_2_PIN = STD_OFF;
            _delay(T0L);      
         }
         else
         {
            CH_2_PIN = STD_ON;
            _delay(T1H);
            CH_2_PIN = STD_OFF;
            _delay(T1L);     
         }            
      }  
      
      /* Write Blue */
      for (i = 0; i < 8; i++)
      {   
         if (BlueBitArray[i] == 0)
         {
            CH_2_PIN = STD_ON;
            _delay(T0H);
            CH_2_PIN = STD_OFF;
            _delay(T0L);      
         }
         else
         {
            CH_2_PIN = STD_ON;
            _delay(T1H);
            CH_2_PIN = STD_OFF;
            _delay(T1L);     
         }         
      }   
   }
    
   /* Critical section: leave */
   Mcu_EnableInterrupts();   
}

/************************************************************************
* Function:     Npdrv_Ch3_WriteNeopixelColor
* Input:        UINT8 PixNumb
*               UINT8 Green
*               UINT8 Red
*               UINT8 Blue  
* Output:       None
* Author:       F.Ficili
* Description:  Write a complete color for a set of pixels for CH3. This function is
*               highly optimized for speed. 
* Date:         31/03/18
************************************************************************/
void Npdrv_Ch3_WriteNeopixelColor (UINT8 PixNumb, UINT8 Green, UINT8 Red, UINT8 Blue)
{
   UINT8 i,j;
   UINT8 GreenBitArray[8];
   UINT8 RedBitArray[8];
   UINT8 BlueBitArray[8];             
   
   for (i = 0; i < 8; i++)
   {
      GreenBitArray[i] = ((Green >> (7 - i)) & 0x01);
      RedBitArray[i] = ((Red >> (7 - i)) & 0x01);
      BlueBitArray[i] = ((Blue >> (7 - i)) & 0x01);      
   }
   
   /* Critical section: enter */
   Mcu_DisableInterrupts();
   
   for (j = 0; j < PixNumb; j++)
   {
      /* Write Green */
      for (i = 0; i < 8; i++)
      {
         if (GreenBitArray[i] == 0)
         {
            CH_3_PIN = STD_ON;
            _delay(T0H);
            CH_3_PIN = STD_OFF;
            _delay(T0L);      
         }
         else
         {
            CH_3_PIN = STD_ON;
            _delay(T1H);
            CH_3_PIN = STD_OFF;
            _delay(T1L);     
         }      
      }
   
      /* Write Red */
      for (i = 0; i < 8; i++)
      {
         if (RedBitArray[i] == 0)
         {
            CH_3_PIN = STD_ON;
            _delay(T0H);
            CH_3_PIN = STD_OFF;
            _delay(T0L);      
         }
         else
         {
            CH_3_PIN = STD_ON;
            _delay(T1H);
            CH_3_PIN = STD_OFF;
            _delay(T1L);     
         }            
      }  
      
      /* Write Blue */
      for (i = 0; i < 8; i++)
      {   
         if (BlueBitArray[i] == 0)
         {
            CH_3_PIN = STD_ON;
            _delay(T0H);
            CH_3_PIN = STD_OFF;
            _delay(T0L);      
         }
         else
         {
            CH_3_PIN = STD_ON;
            _delay(T1H);
            CH_3_PIN = STD_OFF;
            _delay(T1L);     
         }         
      }   
   }
    
   /* Critical section: leave */
   Mcu_EnableInterrupts();   
}