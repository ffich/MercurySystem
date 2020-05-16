/************************************************************************
*                          Standard Services                            *
*************************************************************************
* FileName:         std_ser.h                                           *
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
* F.Ficili     06/06/17    1.0          First release                   *
* F.Ficili     13/01/19    1.1          Added StdS_SetI2cAddress func   *
*                                       and fixed a bug on dip-switch   *
*                                       read.    
************************************************************************/

#ifndef STD_SER_H
#define	STD_SER_H

/************************************************************************
* Includes
************************************************************************/
#include "os_sched.h"

/************************************************************************
* EXPORTED Defines
************************************************************************/
/* Board Identifier */
#define BOARD_ID                                            BOARD_NAME

#define FWK_VERSION_MAJOR                                   1u
#define FWK_VERSION_MINOR                                   0u
#define FWK_VERSION_FIX                                     0u

#define FWK_VERSION                                         "01.01.00"

/* Command Protocol indexes */
#define CMD_INDEX                                           ((UINT8)(0))
#define PAR_1_INDEX                                         ((UINT8)(1))
#define PAR_2_INDEX                                         ((UINT8)(2))
#define PAR_3_INDEX                                         ((UINT8)(3))
#define PAR_4_INDEX                                         ((UINT8)(4))

/* Replyes Protocol indexes */
#define REP_PAR_1_INDEX                                     ((UINT8)(0))
#define REP_PAR_2_INDEX                                     ((UINT8)(1))
#define REP_PAR_3_INDEX                                     ((UINT8)(2))
#define REP_PAR_4_INDEX                                     ((UINT8)(3))

/* Commands */
#define PING_CMD                                            ((UINT8)(0x10))

#define BOARD_ID_CMD                                        ((UINT8)(0x20))
#define FWK_VERSION_CMD                                     ((UINT8)(0x21))
#define APP_VERSION_CMD                                     ((UINT8)(0x22))

#define BOARD_RESET_CMD                                     ((UINT8)(0x30))
#define BOARD_LP_MODE_CMD                                   ((UINT8)(0x31))

#define CONTROL_LED_BEHAV_CMD                               ((UINT8)(0x41))
#define CONTROL_LED_DIRECT_CMD                              ((UINT8)(0x42))
#define GET_DIE_TEMP_RAW_CMD                                ((UINT8)(0x45))
#define GET_DIE_TEMP_ASCII_CMD                              ((UINT8)(0x46))

/* Replies */
#define PING_ECHO                                           (void*)("ECHO")
#define PING_ECHO_LEN                                       ((UINT8)(4))
#define GET_DIE_TEMP_RAW_LEN                                ((UINT8)(2))
#define GET_DIE_TEMP_ASCII_LEN                              ((UINT8)(2))

#define BOARD_ID_LEN                                        ((UINT8)(5))
#define FWK_VERSION_LEN                                     ((UINT8)(8))
#define APP_VERSION_LEN                                     ((UINT8)(8))

/* LED behavior Parameters */
#define LED_ALWAYS_OFF                                      ((UINT8)(0))
#define LED_I2C_PULSE                                       ((UINT8)(1))
#define LED_BLINK                                           ((UINT8)(2))
#define LED_DIRECT                                          ((UINT8)(3))

/* Setting for LED default behaviour */
#define LED_DEFAULT_BEHAV                                   LED_I2C_PULSE

/************************************************************************
* EXPORTED Typedef
************************************************************************/


/************************************************************************
* EXPORTED Variables
************************************************************************/
/* LED behavour status */
extern UINT8 LedBehaviourSts;

/************************************************************************
* EXPORTED Functions
************************************************************************/
/* Update led blink status on request */
void UpdateLebBlinkSts (void);
/* Directly control LED status */
void ControlLedDirect (void);
/* Reset the board */
void BoardReset (void);
/* Enter low power mode */
void LowPowerMode (void);
/* Manage standard protocol commands */
void StdS_ManageStdServices (void);
/* Set the LED default behav */
void SetLedBehavSts (UINT8 LedBehav);
/* Set I2c address of the board */
void StdS_SetI2cAddress (void);

#endif	/* STD_SER_H */

