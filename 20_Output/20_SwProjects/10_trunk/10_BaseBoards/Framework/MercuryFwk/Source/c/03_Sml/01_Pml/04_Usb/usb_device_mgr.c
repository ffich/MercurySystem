/************************************************************************
*                          USB Device Manager                           *
*************************************************************************
* FileName:         usb_device_mgr.c                                    *
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

/************************************************************************
* Includes
************************************************************************/
#include "usb_device_mgr.h"

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
* Function:     UsbDevice_MainTask
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage USB task.
* Date:         19/09/16
************************************************************************/
void UsbDevice_MainTask (UINT8 Options)
{    
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* USB device initialization */        
         USBDeviceInit();
         USBDeviceAttach();
         break;

      /* System Normal operation Phase */
      case RunningState:
         /* USB periodic task */
         USBDeviceTasks();
         /* Class-specific tasks */
#if (USB_CLASS_USED == USB_CLASS_CDC)
         /* CDC tx service */
         CDCTxService();
#endif               
#if (USB_CLASS_USED == USB_CLASS_HID)
         /* HID tx service */
         APP_DeviceMouseTasks();
#endif                                    
         break;

      /* Default */
      default:
         break;
   }
}

bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch( (int) event )
    {
        case EVENT_TRANSFER:
            break;

        case EVENT_SOF:
            /* We are using the SOF as a timer to time the LED indicator.  Call
             * the LED update function here. */
            #if (USB_CLASS_USED == USB_CLASS_HID)
               //APP_DeviceMouseSOFHandler();
            #endif
            break;

        case EVENT_SUSPEND:
            /* Update the LED status for the suspend event. */
            //APP_LEDUpdateUSBStatus();
            break;

        case EVENT_RESUME:
            /* Update the LED status for the resume event. */
            //APP_LEDUpdateUSBStatus();
            break;

        case EVENT_CONFIGURED:
            /* When the device is configured, we can (re)initialize the 
             * demo code. */
            //APP_DeviceCDCEmulatorInitialize();
            #if (USB_CLASS_USED == USB_CLASS_CDC)
               CDCInitEP();
            #endif
            #if (USB_CLASS_USED == USB_CLASS_HID)
               APP_DeviceMouseInitialize();
            #endif
            break;

        case EVENT_SET_DESCRIPTOR:
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The HID driver
             * needs to check to see if the request was for it. */
            #if (USB_CLASS_USED == USB_CLASS_CDC)
               USBCheckCDCRequest();
            #endif
            #if (USB_CLASS_USED == USB_CLASS_HID)
               USBCheckHIDRequest();
            #endif
            break;

        case EVENT_BUS_ERROR:
            break;

        case EVENT_TRANSFER_TERMINATED:
            break;

        default:
            break;
    }
    return true;
}