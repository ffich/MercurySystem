/************************************************************************
*                           Hw Profile                                  *
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
#define BSP_LED_1_TRIS                                   TRISDbits.TRISD4
#define BSP_LED_2_TRIS                                   TRISDbits.TRISD5
#define BSP_LED_3_TRIS                                   TRISDbits.TRISD6

/* Resource Alias */
#define BSP_LED_1                                        PORTDbits.RD4
#define BSP_LED_2                                        PORTDbits.RD5
#define BSP_LED_3                                        PORTDbits.RD6

/************************************************************************
* User Buttons
************************************************************************/
/* Data direction register */
#define BSP_BTN_1_TRIS                                   TRISDbits.TRISD7

/* Resource Alias */
#define BSP_BTN_1                                        PORTDbits.RD7

/************************************************************************
* Generic I/O
************************************************************************/
/* Data direction register */
#define BSP_INT_0_TRIS                                   TRISBbits.TRISB0
#define BSP_INT_1_TRIS                                   TRISDbits.TRISD3

/* Resource Alias */
#define BSP_INT_0                                        PORTBbits.RB0
#define BSP_INT_1                                        PORTDbits.RD3

/************************************************************************
* I2C Modules
************************************************************************/
/* Data direction register */
#define BSP_I2C_1_SDA_TRIS                               TRISBbits.TRISB5
#define BSP_I2C_1_SCL_TRIS                               TRISBbits.TRISB4

/* Resource Alias */
#define BSP_I2C_2_SDA_TRIS                               TRISDbits.TRISD1
#define BSP_I2C_2_SCL_TRIS                               TRISDbits.TRISD0

/************************************************************************
* UART Modules
************************************************************************/
/* Data direction register */
#define BSP_UART_1_TX_TRIS                               TRISCbits.TRISC6
#define BSP_UART_1_RX_TRIS                               TRISCbits.TRISC7

/* Resource Alias */
#define BSP_UART_2_TX_TRIS                               TRISCbits.TRISC0
#define BSP_UART_2_RX_TRIS                               TRISCbits.TRISC1

/************************************************************************
* Modem Interface
************************************************************************/
/* Data direction register */
#define BSP_MDM_RESET_TRIS                               TRISEbits.TRISE0
#define BSP_MDM_ON_OFF_TRIS                              TRISEbits.TRISE1
#define BSP_MDM_RI_TRIS                                  TRISEbits.TRISE2

/* Resource Alias */
#define BSP_MDM_RESET                                    PORTEbits.RE0
#define BSP_MDM_ON_OFF                                   PORTEbits.RE1
#define BSP_MDM_RI                                       PORTEbits.RE2

/************************************************************************
* BT Modem Interface
************************************************************************/
/* Data direction register */
#define BSP_BT_MDM_RESET_TRIS                            TRISEbits.TRISE0
#define BSP_BT_MDM_MODE_TRIS                             TRISCbits.TRISC2

/* Resource Alias */
#define BSP_BT_MDM_RESET                                 LATEbits.LATE0
#define BSP_BT_MDM_MODE                                  LATCbits.LATC2

/************************************************************************
* Wifi Modem Interface
************************************************************************/
/* Data direction register */
#define BSP_WIFI_MDM_RESET_TRIS                          TRISEbits.TRISE0

/* Resource Alias */
#define BSP_WIFI_MDM_RESET                               LATEbits.LATE0

/************************************************************************
* GPIO
************************************************************************/
/* Data direction register */
#define BSP_IO_0_TRIS                                   TRISDbits.TRISD0
#define BSP_IO_1_TRIS                                   TRISDbits.TRISD1
#define BSP_IO_2_TRIS                                   TRISCbits.TRISC0
#define BSP_IO_3_TRIS                                   TRISCbits.TRISC1
#define BSP_IO_4_TRIS                                   TRISBbits.TRISB0
#define BSP_IO_5_TRIS                                   TRISDbits.TRISD3
#define BSP_IO_6_TRIS                                   TRISBbits.TRISB5
#define BSP_IO_7_TRIS                                   TRISBbits.TRISB4
#define BSP_IO_8_TRIS                                   TRISAbits.TRISA2
#define BSP_IO_9_TRIS                                   TRISAbits.TRISA3
#define BSP_IO_10_TRIS                                  TRISBbits.TRISB7
#define BSP_IO_11_TRIS                                  TRISBbits.TRISB6

/* Resource Alias */
#define BSP_IO_0                                        LATDbits.LATD0
#define BSP_IO_1                                        LATDbits.LATD1
#define BSP_IO_2                                        LATCbits.LATC0
#define BSP_IO_3                                        LATCbits.LATC1
#define BSP_IO_4                                        LATBbits.LATB0
#define BSP_IO_5                                        LATDbits.LATD3
#define BSP_IO_6                                        LATBbits.LATB5
#define BSP_IO_7                                        LATBbits.LATB4
#define BSP_IO_8                                        LATAbits.LATA2
#define BSP_IO_9                                        LATAbits.LATA3
#define BSP_IO_10                                       LATBbits.LATB7
#define BSP_IO_11                                       LATBbits.LATB6

/************************************************************************
* Execution time measure options
************************************************************************/
#define USE_EXEC_TIME_MEAS_PIN                          STD_OFF

#define EXEC_TIME_MEAS_PIN_TRIS                         BSP_IO_4_TRIS
#define EXEC_TIME_MEAS_PIN                              BSP_IO_4

#endif	/* HW_PROFILE_H */

