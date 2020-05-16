/************************************************************************
*                          Neopixel Driver                             	*
*************************************************************************
* FileName:         neopixel_drv.h                                    	*
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

#ifndef NEOPIXEL_DRV_H
#define NEOPIXEL_DRV_H

/************************************************************************
* Includes
************************************************************************/


/************************************************************************
* EXPORTED Defines
************************************************************************/


/************************************************************************
* EXPORTED Typedef
************************************************************************/


/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
void Npdrv_Ch1_WriteNeopixelColor (UINT8 PixNumb, UINT8 Green, UINT8 Red, UINT8 Blue);
void Npdrv_Ch2_WriteNeopixelColor (UINT8 PixNumb, UINT8 Green, UINT8 Red, UINT8 Blue);
void Npdrv_Ch3_WriteNeopixelColor (UINT8 PixNumb, UINT8 Green, UINT8 Red, UINT8 Blue);

#endif	/* NEOPIXEL_DRV_H */

