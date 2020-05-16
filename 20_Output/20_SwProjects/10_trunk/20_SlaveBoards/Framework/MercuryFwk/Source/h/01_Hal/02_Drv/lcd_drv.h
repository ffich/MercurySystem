/************************************************************************
*                           External LCD driver                         *
*************************************************************************
* FileName:         lcd_drv.h                                           *
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
* F.Ficili     06/09/15     1.0          First release                  *
* F.Ficili     03/07/17     1.0          Porting for MS EB210           *
************************************************************************/

#ifndef LCD_DRIVER_H
#define	LCD_DRIVER_H

/************************************************************************
* Includes
************************************************************************/
#include "hal.h"

#ifdef USE_EB210
/************************************************************************
* Defines
************************************************************************/
/* Data direction registers */
#define LCD_D4_TRIS                                     BSP_LCD_D4_TRIS
#define LCD_D5_TRIS                                     BSP_LCD_D5_TRIS
#define LCD_D6_TRIS                                     BSP_LCD_D6_TRIS
#define LCD_D7_TRIS                                     BSP_LCD_D7_TRIS
#define LCD_EN_TRIS                                     BSP_LCD_EN_TRIS
#define LCD_RS_TRIS                                     BSP_LCD_RS_TRIS
#define LCD_RW_TRIS                                     BSP_LCD_RW_TRIS
#define LCD_BKL_TRIS                                    BSP_LCD_BL_TRIS

/* Port's pins */
#define LCD_D4                                          BSP_LCD_D4_LAT
#define LCD_D5                                          BSP_LCD_D5_LAT
#define LCD_D6                                          BSP_LCD_D6_LAT
#define LCD_D7                                          BSP_LCD_D7_LAT
#define LCD_EN                                          BSP_LCD_EN_LAT
#define LCD_RS                                          BSP_LCD_RS_LAT
#define LCD_RW                                          BSP_LCD_RW_LAT
#define LCD_BKL                                         BSP_LCD_BL_LAT

/* Delays constant */
#define ENABLE_PULSE_DELAY_1                            ((UINT16)(200))
#define ENABLE_PULSE_DELAY_2                            ((UINT16)(200))
#define INIT_DELAY                                      ((UINT16)(2000))
#define STATE_DELAY                                     ((UINT16)(2000))

/* -- HD44780 parameters -- */
/* Entry mode parameters */
#define ENTRY_MODE_CMD                                  ((UINT8)(0b00000100))
#define EM_ID_SHIFT                                     ((UINT8)(1))
#define EM_ID_DEC_CURSOR_POS                            ((UINT8)(0x00))
#define EM_ID_INC_CURSOR_POS                            ((UINT8)(0x01))
#define EM_S_SHIFT                                      ((UINT8)(0))
#define EM_S_NO_DISPLAY_SHIFT                           ((UINT8)(0x00))
#define EM_S_DISPLAY_SHIFT                              ((UINT8)(0x01))

/* Display control parameters */
#define DISPLAY_CONTROL_CMD                             ((UINT8)(0b00001000))
#define DC_D_SHIFT                                      ((UINT8)(2))
#define DC_D_DISPLAY_OFF                                ((UINT8)(0x00))
#define DC_D_DISPLAY_ON                                 ((UINT8)(0x01))
#define DC_C_SHIFT                                      ((UINT8)(1))
#define DC_C_CURSOR_OFF                                 ((UINT8)(0x00))
#define DC_C_CURSOR_ON                                  ((UINT8)(0x01))
#define DC_B_SHIFT                                      ((UINT8)(0))
#define DC_B_BLINK_OFF                                  ((UINT8)(0x00))
#define DC_B_BLINK_ON                                   ((UINT8)(0x01))

/* Cursor control parameters */
#define CURSOR_CONTROL_CMD                              ((UINT8)(0b00010000))
#define CC_SC_SHIFT                                     ((UINT8)(3))
#define CC_SC_MOVE_CURSOR                               ((UINT8)(0x00))
#define CC_SC_SHIFT_DISPLAY                             ((UINT8)(0x01))
#define CC_RL_SHIFT                                     ((UINT8)(2))
#define CC_RL_SHIFT_LEFT                                ((UINT8)(0x00))
#define CC_RL_SHIFT_RIGHT                               ((UINT8)(0x01))

/* Function set parameters */
#define FUNCTION_SET_CMD                                ((UINT8)(0b00100000))
#define FS_DL_SHIFT                                     ((UINT8)(4))
#define FS_DL_INT_4_BIT                                 ((UINT8)(0x00))
#define FS_DL_INT_8_BIT                                 ((UINT8)(0x01))
#define FS_N_SHIFT                                      ((UINT8)(3))
#define FS_N_1_LINE                                     ((UINT8)(0x00))
#define FS_N_2_LINE                                     ((UINT8)(0x01))
#define FS_F_SHIFT                                      ((UINT8)(2))
#define FS_F_5_X_8_DOTS                                 ((UINT8)(0x00))
#define FS_F_5_X_10_DOTS                                ((UINT8)(0x01))

/* Display command */
#define CLEAR_DISPLAY                                   ((UINT8)(0x01))
#define CURSOR_HOME                                     ((UINT8)(0x02))
#define CURSOR_LINE_2                                   ((UINT8)(0xC0))

/* Special defines */
#define ALIGN_SPECIAL_CHAR                              ((UINT8)(0x30))
#define INT_MODE_8_BIT_INIT                             ((UINT8)(0x30))
#define INT_MODE_4_BIT_INIT                             ((UINT8)(0x20))

/* Mask & Shift */
#define BIT_7_MASK                                      ((UINT8)(0b10000000))
#define BIT_6_MASK                                      ((UINT8)(0b01000000))
#define BIT_5_MASK                                      ((UINT8)(0b00100000))
#define BIT_4_MASK                                      ((UINT8)(0b00010000))
#define BIT_3_MASK                                      ((UINT8)(0b00001000))
#define BIT_2_MASK                                      ((UINT8)(0b00000100))
#define BIT_1_MASK                                      ((UINT8)(0b00000010))
#define BIT_0_MASK                                      ((UINT8)(0b00000001))

#define BITWISE_SHIFT_7                                 ((UINT8)(7))
#define BITWISE_SHIFT_6                                 ((UINT8)(6))
#define BITWISE_SHIFT_5                                 ((UINT8)(5))
#define BITWISE_SHIFT_4                                 ((UINT8)(4))
#define BITWISE_SHIFT_3                                 ((UINT8)(3))
#define BITWISE_SHIFT_2                                 ((UINT8)(2))
#define BITWISE_SHIFT_1                                 ((UINT8)(1))

/************************************************************************
* Global Variables
************************************************************************/


/************************************************************************
* Typedef
************************************************************************/
/* LCD backlight */
typedef enum _BacklSts
{
   BacklightOff,
   BacklightOn
} BacklStsType;

/* Init LCD enum */
typedef enum _LcdInitState
{
   InitDelayState,
   InitHwIntState,
   LcsStsAlign1,
   LcsStsAlign2,
   LcsStsAlign3,
   LcsStsAlign4,	 
   FunctionSetState,
   DisplayControlState,
   DisplaySetState,
   EntryModeState,
   ClearDisplayState,
   InitCompleteState,
   LcdWaitState,
} LcdInitStateType;

/* Lcd config type */
typedef struct _LcdConfig
{
   UINT8 EmInitCursorPosIncDec;
   UINT8 EmDisplayShiftOnOff;
   UINT8 DcDisplayOnOff;
   UINT8 DcCursorOnOff;
   UINT8 DcBlinkOnOff;
   UINT8 CcCursorMove;
   UINT8 CcShiftLeftRight;
   UINT8 FsIntType;
   UINT8 FsLineNumb;
   UINT8 FsFontType;
} LcdConfigType;

/************************************************************************
* Function
************************************************************************/
void Lcd_SetConfig (void);
void Lcd_InitHwInterface(void);
LcdInitStateType Lcd_InitLcdInterface (void);
void Lcd_WriteCommand (UINT8 CommandToWrite);
void Lcd_WriteChar (UINT8 CharToWrite);
void Lcd_WriteConstString (const UINT8 *ConstString);
void Lcd_WriteString (UINT8 *String);
void Lcd_Backlight(BacklStsType Backlight);

#endif /* USE_EB210 */

#endif	/* LCD_DRIVER_H */

