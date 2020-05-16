/************************************************************************
*                          USB Device Task                              *
*************************************************************************
* FileName:         usb_device_task.h                                   *
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
* F.Ficili     19/09/16    1.0          First release                   *
************************************************************************/

#ifndef USB_DEVICE_TASK_H
#define	USB_DEVICE_TASK_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"
#include "usb_device.h"

/* Class selection */
#if (USB_CLASS_USED == USB_CLASS_CDC)
   #include "usb_device_cdc.h"
#endif /* (USB_CLASS_USED == USB_CLASS_CDC) */

#if (USB_CLASS_USED == USB_CLASS_HID)
   #include "usb_device_hid.h"
   #include "hid_devices.h"
#endif /* (USB_CLASS_USED == USB_CLASS_HID) */

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Task period in ms */
#define USB_DEV_TASK_PERIOD_MS                           ((UINT16)(1))

/************************************************************************
* EXPORTED Typedef
************************************************************************/


/************************************************************************
* EXPORTED Variables
************************************************************************/


/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Usb Device task */
void UsbDevice_MainTask (UINT8 Options);

#endif	/* USB_DEVICE_TASK_H */

