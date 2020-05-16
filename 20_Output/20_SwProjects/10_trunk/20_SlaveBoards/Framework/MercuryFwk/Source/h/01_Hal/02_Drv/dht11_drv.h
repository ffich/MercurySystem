/************************************************************************
*                          	DHTxx Driver                             	*
*************************************************************************
* FileName:         dhtxx_drv.h                                    		*
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

#ifndef DHT_XX_DRV_H
#define DHT_XX_DRV_H

/************************************************************************
* Includes
************************************************************************/


/************************************************************************
* EXPORTED Defines
************************************************************************/
#define DHT11                                           1u    
#define DHT22                                           2u
#define SENSOR_TYPE                                     DHT22

/************************************************************************
* EXPORTED Typedef
************************************************************************/
/* Sensor union */
typedef union _DhtSensor
{
    
#if (SENSOR_TYPE == DHT11)
    /* DHT11 Data Structure */
    struct _SensorDataType
    {
        UINT8 HumidityInt;
        UINT8 HumidityDec;
        UINT8 TemperatureInt;
        UINT8 TemperatureDec;
        UINT8 Checksum;
    } Data;
#endif
#if (SENSOR_TYPE == DHT22)
    /* DHT22 Data Structure */
    struct _SensorDataType
    {
        UINT8 HumidityHigh;
        UINT8 HumidityLow;
        UINT8 TemperatureHigh;
        UINT8 TemperatureLow;        
        UINT8 Checksum;
    } Data;
#endif    
    
    /* Sensor Array */
    UINT8 Array[5];
} DhtSensorType;
       
/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Exported sensor variable */
extern DhtSensorType Sensor;

/************************************************************************
* EXPORTED Functions
************************************************************************/
void Dht_RequestData (void);
UINT8 Dht_ReadByte (void);
void Dht_ReadData (void);
UINT8 Dht_CalculateChecksum (DhtSensorType Sens);
UINT8 Dht_GetSensorData (void);

#endif	/* DHT_XX_DRV_H */

