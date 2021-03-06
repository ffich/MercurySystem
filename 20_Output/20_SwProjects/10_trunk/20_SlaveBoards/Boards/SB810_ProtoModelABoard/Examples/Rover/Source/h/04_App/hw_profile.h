/************************************************************************
*                          Hw Profile SB140                             *
*************************************************************************
* FileName:         hw_profile.h                                        *
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

#ifndef HW_PROFILE_H
#define	HW_PROFILE_H

/************************************************************************
* Includes
************************************************************************/
#include "xc.h"
#include "hal.h"

/************************************************************************
* Hardware Basic Defines
************************************************************************/
/* Data direction */
#define LINE_DIRECTION_INPUT                             ((UINT8)(1))
#define LINE_DIRECTION_OUTPUT                            ((UINT8)(0))

/* Digital state */
#define LINE_STATE_HIGH                                  ((UINT8)(1))
#define LINE_STATE_LOW                                   ((UINT8)(0))

/************************************************************************
* Device Hardware Profile
************************************************************************/

/************************************************************************
* User LEDs
************************************************************************/
/* Data direction register */
#define BSP_LED_1_TRIS                                   TRISBbits.TRISB7

/* Resource Alias */
#define BSP_LED_1                                        LATBbits.LATB7

/************************************************************************
* IOs
************************************************************************/
/* Data direction register */
#define BSP_RA5_TRIS                                     TRISAbits.TRISA5
#define BSP_RA4_TRIS                                     TRISAbits.TRISA4
#define BSP_RA2_TRIS                                     TRISAbits.TRISA2
#define BSP_RB5_TRIS                                     TRISBbits.TRISB5
#define BSP_RC0_TRIS                                     TRISCbits.TRISC0
#define BSP_RC1_TRIS                                     TRISCbits.TRISC1
#define BSP_RC2_TRIS                                     TRISCbits.TRISC2
#define BSP_RC7_TRIS                                     TRISCbits.TRISC7

/* LAT registers */
#define BSP_RA5_LAT                                      LATAbits.LATA5
#define BSP_RA4_LAT                                      LATAbits.LATA4
#define BSP_RA2_LAT                                      LATAbits.LATA2
#define BSP_RB5_LAT                                      LATBbits.LATB5
#define BSP_RC0_LAT                                      LATCbits.LATC0
#define BSP_RC1_LAT                                      LATCbits.LATC1
#define BSP_RC2_LAT                                      LATCbits.LATC2
#define BSP_RC7_LAT                                      LATCbits.LATC7

/* PORT registers */
#define BSP_RA5_PORT                                     PORTAbits.RA5
#define BSP_RA4_PORT                                     PORTAbits.RA4
#define BSP_RA2_PORT                                     PORTAbits.RA2
#define BSP_RB5_PORT                                     PORTBbits.RB5
#define BSP_RC0_PORT                                     PORTCbits.RC0
#define BSP_RC1_PORT                                     PORTCbits.RC1
#define BSP_RC2_PORT                                     PORTCbits.RC2
#define BSP_RC7_PORT                                     PORTCbits.RC7

/************************************************************************
* DipSwitch
************************************************************************/
/* Data direction register */
#define BSP_DIPSWITCH_1_TRIS                             TRISCbits.TRISC5
#define BSP_DIPSWITCH_2_TRIS                             TRISCbits.TRISC4
#define BSP_DIPSWITCH_3_TRIS                             TRISCbits.TRISC3
#define BSP_DIPSWITCH_4_TRIS                             TRISCbits.TRISC6

/* Resource Alias */
#define BSP_DIPSWITCH_1                                  PORTCbits.RC5
#define BSP_DIPSWITCH_2                                  PORTCbits.RC4
#define BSP_DIPSWITCH_3                                  PORTCbits.RC3
#define BSP_DIPSWITCH_4                                  PORTCbits.RC6

/************************************************************************
* I2C Modules
************************************************************************/
/* Data direction register */
#define BSP_I2C_1_SDA_TRIS                               TRISBbits.TRISB6
#define BSP_I2C_1_SCL_TRIS                               TRISBbits.TRISB4

#endif	/* HW_PROFILE_H */

