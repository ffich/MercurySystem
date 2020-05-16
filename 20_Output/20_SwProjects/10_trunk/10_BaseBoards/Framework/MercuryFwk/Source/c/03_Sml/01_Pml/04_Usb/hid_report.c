/************************************************************************
*                             HID Report                             	*
*************************************************************************
* FileName:         hid_report.c                                        *
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

/************************************************************************
* Includes
************************************************************************/
#include <stdint.h>
#include "usb_config.h"

#if (USB_CLASS_USED == USB_CLASS_HID)
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
/*******************************************************************************
 * HID Report Descriptor - this describes the data format of the reports that
 * are sent between the host and the device.
 *
 * In this example there are only one INPUT report.  This report descriptor can
 * be generated using the HID descriptor tool available at www.usb.org.
 ******************************************************************************/
const struct{uint8_t report[HID_RPT01_SIZE];}hid_rpt01=
{
    {
        0x05, 0x01, /* Usage Page (Generic Desktop)             */
        0x09, 0x02, /* Usage (Mouse)                            */
        0xA1, 0x01, /* Collection (Application)                 */
        0x09, 0x01, /*  Usage (Pointer)                         */
        0xA1, 0x00, /*  Collection (Physical)                   */
        0x05, 0x09, /*      Usage Page (Buttons)                */
        0x19, 0x01, /*      Usage Minimum (01)                  */
        0x29, 0x03, /*      Usage Maximum (03)                  */
        0x15, 0x00, /*      Logical Minimum (0)                 */
        0x25, 0x01, /*      Logical Maximum (1)                 */
        0x95, 0x03, /*      Report Count (3)                    */
        0x75, 0x01, /*      Report Size (1)                     */
        0x81, 0x02, /*      Input (Data, Variable, Absolute)    */
        0x95, 0x01, /*      Report Count (1)                    */
        0x75, 0x05, /*      Report Size (5)                     */
        0x81, 0x01, /*      Input (Constant)    ;5 bit padding  */
        0x05, 0x01, /*      Usage Page (Generic Desktop)        */
        0x09, 0x30, /*      Usage (X)                           */
        0x09, 0x31, /*      Usage (Y)                           */
        0x15, 0x81, /*      Logical Minimum (-127)              */
        0x25, 0x7F, /*      Logical Maximum (127)               */
        0x75, 0x08, /*      Report Size (8)                     */
        0x95, 0x02, /*      Report Count (2)                    */
        0x81, 0x06, /*      Input (Data, Variable, Relative)    */
        0xC0, 0xC0  /* End Collection,End Collection            */
    }
};

/************************************************************************
* LOCAL Function Prototypes
************************************************************************/


/************************************************************************
* LOCAL Function Implementations
************************************************************************/


/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

#endif /* (USB_CLASS_USED == USB_CLASS_HID) */