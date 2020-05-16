/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/
#ifndef BOOT18FXXJXX_H
#define BOOT18FXXJXX_H

/** P U B L I C  P R O T O T Y P E S *****************************************/
void UserInit(void);
void ProcessIO(void);
void ClearWatchdog(void);
void DisableUSBandExecuteLongDelay(void);


//Vector remapping/absolute address constants
#define REMAPPED_APPLICATION_RESET_VECTOR       0x2000
//#define REMAPPED_APPLICATION_HIGH_ISR_VECTOR    0x1008        //See VectorRemap.asm
//#define REMAPPED_APPLICATION_LOW_ISR_VECTOR     0x1018        //See VectorRemap.asm
#define BOOTLOADER_ABSOLUTE_ENTRY_ADDRESS       0x001C  //Execute a "goto 0x001C" inline assembly instruction, if you want to enter the bootloader mode from the application via software

#define APP_SIGNATURE_ADDRESS                   0x2006  //0x1006 and 0x1007 contains the "signature" WORD, indicating successful erase/program/verify operation
#define APP_SIGNATURE_VALUE                     0xABCD  //leet "GOOD", implying that the erase/program was a success and the bootloader intentionally programmed the APP_SIGNATURE_ADDRESS with this value
#define APP_VERSION_ADDRESS                     0x2016  //0x1016 and 0x1017 should contain the application image firmware version number
#endif //BOOT18FXXJXX_H
