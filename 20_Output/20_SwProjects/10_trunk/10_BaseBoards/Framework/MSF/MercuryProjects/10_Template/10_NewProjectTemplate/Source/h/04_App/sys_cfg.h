/************************************************************************
*                          System Config                                *
*************************************************************************
* FileName:         sys_cfg.h                                           *
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
* F.Ficili     30/04/18    1.0          First release                   *
************************************************************************/

#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

/************************************************************************
* Includes
************************************************************************/
/* Used derivative */
#include "pic18f46j50.h"

/************************************************************************
* System Timing
************************************************************************/
/* CPU clock frequency (Hz) */
#define SYSTEM_FREQUENCY_VALUE_HZ                       ((UINT32)(48000000))
/* Peripheral clock frequency (Hz) */
#define PER_FREQUENCY_VALUE_HZ                          (SYSTEM_FREQUENCY_VALUE_HZ/(UINT32)(4))

/************************************************************************
* Compiler
************************************************************************/
#define XC8_COMPILER                                    1u
#define XC16_COMPILER                                   2u
#define XC32_COMPILER                                   3u

#define COMPILER_USED                                   XC8_COMPILER  

/************************************************************************
* Usb Config
************************************************************************/
#define USB_STS                                         STD_ON 

/************************************************************************
* Usb Class
************************************************************************/
#define USB_CLASS_CDC                                   0u 
#define USB_CLASS_HID                                   1u 

#define USB_CLASS_USED                                  USB_CLASS_CDC 

/************************************************************************
* Term Task Config
************************************************************************/
#define TERM_TASK_STS                                   STD_ON 

/************************************************************************
* Term Mode
************************************************************************/
#define COMMAND_MODE                                    1u
#define TRANSPARENT_MODE                                2u

#define TERMINAL_MODE                                   COMMAND_MODE

/************************************************************************
* Wdg Task Config
************************************************************************/
#define WDG_STS                                         STD_ON 

/************************************************************************
* Wifi Modem Type Config
************************************************************************/
#define STD_ESP8266_MODULE                              0u 
#define UPANEL_MODULE                                   1u 

#define WIFI_MODULE                                     STD_ESP8266_MODULE

/************************************************************************
* APP Task Config
************************************************************************/
#define APP_TASK_STS                                    STD_ON 
#define APP_TASK_SYMB                                   MyApp_Task 
#define APP_TASK_PERIOD_MS                              MY_APP_TASK_PERIOD_MS   

/************************************************************************
* Modem Type Config
************************************************************************/
#define NO_MDM                                          0u
#define GSM_GPRS_MDM                                    1u
#define BT_MDM                                          2u
#define WIFI_MDM                                        3u

#define MODEM_USED                                      NO_MDM 

#endif	/* SYSTEM_CONFIG_H */