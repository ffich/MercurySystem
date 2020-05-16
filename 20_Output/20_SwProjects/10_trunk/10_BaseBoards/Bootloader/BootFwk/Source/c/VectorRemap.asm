;/*******************************************************************************
;Copyright 2016 Microchip Technology Inc. (www.microchip.com)
;
;Licensed under the Apache License, Version 2.0 (the "License");
;you may not use this file except in compliance with the License.
;You may obtain a copy of the License at
;
;    http://www.apache.org/licenses/LICENSE-2.0
;
;Unless required by applicable law or agreed to in writing, software
;distributed under the License is distributed on an "AS IS" BASIS,
;WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;See the License for the specific language governing permissions and
;limitations under the License.
;
;To request to license the code under the MLA license (www.microchip.com/mla_license), 
;please contact mla_licensing@microchip.com
;*******************************************************************************/

    ;//High priority interrupt vector remapping
#ifdef __XC8__
    PSECT HiVector,class=CODE,delta=1,abs
#endif
        org 0x08
    goto 0x2008     ;Resides at 0x0008 (hardware high priority interrupt vector), and causes PC to jump to 0x1008 upon a high priority interrupt event


    ;//Low priority interrupt vector remapping, as well as bootloader mode absolute
    ;//entry point (located at 0x001C).
#ifdef __XC8__
    PSECT LoVector,class=CODE,delta=1,abs
#endif
        org 0x18
    goto    0x2018  ;Resides at 0x0018 (hardware low priority interrupt vector), and causes PC to jump to 0x1018 upon a low priority interrupt event
    goto    0x30    ;Resides at 0x001C  //Serves as absolute entry point from application program into the bootloader mode



    end