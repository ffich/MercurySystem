/************************************************************************
*                          	DHTxx Driver                             	*
*************************************************************************
* FileName:         dhtxx_drv.c                                    		*
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
* F.Ficili     31/03/18    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "hw_profile.h"
#include "mcu_drv.h"
#include "dht11_drv.h"

/************************************************************************
* Defines
************************************************************************/
#define TIMING_PROTECTION                                STD_ON

#define DHT11_DDR                                        BSP_SENS_COM_TRIS
#define DHT11_PIN                                        BSP_SENS_COM

/* Define _XTAL_FREQ to use built-in delay routine:
__delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
__delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))
 */
#define _XTAL_FREQ                                       SYSTEM_FREQUENCY_VALUE_HZ

/* Delay constant 1: 40ms for DHT11, 5ms for DHT22 */
#if (SENSOR_TYPE == DHT11)
#define START_SEQ_DEALY_1_MS                            20u
#endif

#if (SENSOR_TYPE == DHT22)
#define START_SEQ_DEALY_1_MS                             5u
#endif

/* Delay constant 2: 20-40us for DHTxx  */
#define START_SEQ_DEALY_2_US                             30u

#define DHT_READ_CNT_PERIOD_US                           10u
#define DHT_READ_DELAY_US                                (30u/DHT_READ_CNT_PERIOD_US)

#define HUM_INT_INDEX                                    0u
#define HUM_DEC_INDEX                                    1u
#define TEMP_INT_INDEX                                   2u
#define TEMP_DEC_INDEX                                   3u
#define CHECKSUM_INDEX                                   4u

/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/


/************************************************************************
* GLOBAL Variables
************************************************************************/
DhtSensorType Sensor;

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
* Function:     Dht_RequestData
* Input:        None 
* Output:       None
* Author:       F.Ficili
* Description:  Request data to DHTxx sensor.
* Date:         31/03/18
************************************************************************/
void Dht_RequestData (void)
{
	/* Set pin as output */
   DHT11_DDR = LINE_DIRECTION_OUTPUT;
   
   /* Line low for at least 18ms and then high for 40 us */
   DHT11_PIN = LINE_STATE_LOW;
   __delay_ms(START_SEQ_DEALY_1_MS);
   DHT11_PIN = LINE_STATE_HIGH;
   __delay_us(START_SEQ_DEALY_2_US); 
   DHT11_PIN = LINE_STATE_LOW;
   
   /* The DHT11 acknowledge is 54us low and 80us high */
   DHT11_DDR = LINE_DIRECTION_INPUT;
   /* 1: low-to-high transition */
   while (DHT11_PIN == LINE_STATE_LOW);  
   /* 2: high-to-low transition */
   while (DHT11_PIN == LINE_STATE_HIGH);
}

/************************************************************************
* Function:     Dht_ReadByte
* Input:        None 
* Output:       UINT8
* Author:       F.Ficili
* Description:  Read 8-bit data from DHTxx sensor.
* Date:         31/03/18
************************************************************************/
UINT8 Dht_ReadByte (void)
{
   UINT8 Index;
   UINT8 Data = 0x00;
   UINT16 Counter = 0;
   
   for (Index = 0; Index < 8; Index++)
   {
      /* Wait until the low pulse is done */
      while (DHT11_PIN == LINE_STATE_LOW);
      /* Reset counter */
      Counter = 0;      
      /* Wait until the high pulse is done */
      while ((DHT11_PIN == LINE_STATE_HIGH) && (Counter < 100))
      {
         __delay_us(DHT_READ_CNT_PERIOD_US);
         Counter++;         
      }
      /* check counter */
      if (Counter > DHT_READ_DELAY_US)
      {
         /* It's a '1' bit */
         Data |= 0x01 << (7 - Index);
      }
   }
   
   /* Return data value */
   return Data;
}

/************************************************************************
* Function:     Dht_ReadData
* Input:        None 
* Output:       None
* Author:       F.Ficili
* Description:  Read the complete data set from DHTxx sensor.
* Date:         01/04/18
************************************************************************/
void Dht_ReadData (void)
{
   UINT8 Index;
   
   /* Get data from sensor */
   for (Index = 0; Index < 5; Index++)
   {
      Sensor.Array[Index] = Dht_ReadByte();
   }
}

/************************************************************************
* Function:     Dht_CalculateChecksum
* Input:        DhtSensorType Sens 
* Output:       UINT8
* Author:       F.Ficili
* Description:  Calculate checksum from DHTxx data.
* Date:         01/04/18
************************************************************************/
UINT8 Dht_CalculateChecksum (DhtSensorType Sens)
{
   UINT8 Index;
   UINT8 Checksum;
   UINT8 Ret;   
   
   /* Calculate Checksum */
   Checksum = 0;            
   for (Index = 0; Index < 4; Index++)
   {
      Checksum += Sens.Array[Index];
   }

   /* Verify Checksum */   
   if (Checksum == Sens.Data.Checksum)
   {
      Ret = TRUE;
   }
   else
   {
      Ret = FALSE;
   }
   
   return Ret;
}

/************************************************************************
* Function:     Dht_GetSensorData
* Input:        None 
* Output:       UINT8
* Author:       F.Ficili
* Description:  Get the complete data set from DHTxx sensor.
* Date:         01/04/18
************************************************************************/
UINT8 Dht_GetSensorData (void)
{
   UINT8 Ret;
   
#if (TIMING_PROTECTION == STD_ON)
   /* Critical area: ON */
   Mcu_DisableSchedulerInt();
#endif   
   /* Request data sequence */
   Dht_RequestData();   
   /* Read data from sensor */
   Dht_ReadData();
#if (TIMING_PROTECTION == STD_ON)   
   /* Critical area: OFF */ 
   Mcu_EnableSchedulerInt();
#endif      
   
   if (Dht_CalculateChecksum(Sensor))
   {
      Ret = TRUE;
   }
   else
   {
      Ret = FALSE;
   }
   
   return Ret;
}