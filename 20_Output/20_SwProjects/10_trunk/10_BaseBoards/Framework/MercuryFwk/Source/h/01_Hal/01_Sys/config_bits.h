/************************************************************************
*                               Common                                  *
*************************************************************************
* FileName:         common.h                                            *
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
* F.Ficili     23/07/16    1.0          First release                   *
************************************************************************/

#ifndef CONFIG_BITS_H
#define	CONFIG_BITS_H

#include <xc.h>

//#define MERGED_CFG

#ifndef MERGED_CFG
// CONFIG1L
#pragma config WDTEN = OFF                  // Watchdog Timer (Disabled - Controlled by SWDTEN bit)
#pragma config PLLDIV = 2                   // PLL Prescaler Selection bits (Divide by 2 (8 MHz oscillator input))
#pragma config STVREN = ON                  // Stack Overflow/Underflow Reset (Enabled)
#pragma config XINST = OFF                  // Extended Instruction Set (Disabled)

// CONFIG1H
#pragma config CPUDIV = OSC1                // CPU System Clock Postscaler (No CPU system clock divide)
#pragma config CP0 = OFF                    // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = HSPLL                  // Oscillator (HS+PLL, USB-HS+PLL)
#pragma config T1DIG = ON                   // T1OSCEN Enforcement (Secondary Oscillator clock source may not be selected)
#pragma config LPT1OSC = OFF                // Low-Power Timer1 Oscillator (Low-power operation)
#pragma config FCMEN = ON                   // Fail-Safe Clock Monitor (Enabled)
#pragma config IESO = ON                    // Internal External Oscillator Switch Over Mode (Enabled)

// CONFIG2H
#pragma config WDTPS = 2048                // Watchdog Postscaler (1:2048)

// CONFIG3L
#pragma config DSWDTOSC = INTOSCREF         // DSWDT Clock Select (DSWDT uses INTRC)
#pragma config RTCOSC = T1OSCREF            // RTCC Clock Select (RTCC uses LP OSC)
#pragma config DSBOREN = OFF                // Deep Sleep BOR (Disabled)
#pragma config DSWDTEN = OFF                // Deep Sleep Watchdog Timer (Disabled)
#pragma config DSWDTPS = G2                 // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = ON                 // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set once)
#pragma config MSSP7B_EN = MSK7             // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_63               // Write/Erase Protect Page Start/End Location (WrSecondary Oscillator clock source may be selectedite Protect Program Flash Page 63)
#pragma config WPEND = PAGE_WPFP            // Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
#pragma config WPCFG = OFF                  // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)

// CONFIG4H
#pragma config WPDIS = OFF                  // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)
#endif

#endif	/* CONFIG_BITS_H */




