/************************************************************************
*                          HID Devices                             	*
*************************************************************************
* FileName:         hid_devices.h                                       *
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
* F.Ficili     27/05/17    1.0          First release                   *
************************************************************************/

#ifndef HID_DEVICES_H
#define	HID_DEVICES_H

/************************************************************************
* Includes
************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include "usb_device.h"
#include "usb_device_hid.h"
#include "os_sched.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/


/************************************************************************
* EXPORTED Typedef
************************************************************************/


/************************************************************************
* EXPORTED Variables
************************************************************************/
/* Mouse buttons */
extern UINT8 Mouse_Btn1;
extern UINT8 Mouse_Btn2;
extern UINT8 Mouse_Btn3;

/************************************************************************
* EXPORTED Functions
************************************************************************/

/*********************************************************************
* Function: void APP_DeviceMouseInitialize(void);
*
* Overview: Initializes the demo code
*
* PreCondition: None
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceMouseInitialize(void);

/*********************************************************************
* Function: void APP_DeviceMouseStart(void);
*
* Overview: Starts running the demo.
*
* PreCondition: The device should be configured into the configuration
*   that contains the custome HID interface.  The APP_DeviceMouseInitialize()
*   function should also have been called before calling this function.
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceMouseStart();

/*********************************************************************
* Function: void APP_DeviceMouseTasks(void);
*
* Overview: Keeps the demo running.
*
* PreCondition: The demo should have been initialized and started via
*   the APP_DeviceMouseInitialize() and APP_DeviceMouseStart() demos
*   respectively.
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceMouseTasks();

bool APP_DeviceMouseEventHandler(unsigned int event, void *pdata, size_t size);

void APP_DeviceMouseSOFHandler(void);


#endif	/* HID_DEVICES_H */

