/************************************************************************
*                               Test Task                               *
*************************************************************************
* FileName:         test_task.c                                         *
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
* F.Ficili     30/07/16    1.0          First release                   *
************************************************************************/

/************************************************************************
* Includes
************************************************************************/
#include "test.h"

#if (TEST_TASK_STS == STD_ON)   
/************************************************************************
* Defines
************************************************************************/
/* Perform App Test */
//#define APP_TEST
//#define USB_TEST
//#define I2C_TX_TEST
//#define MDM_TEST
//#define TERMINAL_TEST
//#define MDM_TEST
//#define BT_TEST
//#define SMS_TEST

/* I2C Counter timeout */
#define I2C_TX_TIMEOUT_MS                                ((UINT16)(200)/TEST_TASK_PERIOD_MS)
/* Timeouts */
#define I2C_TMOUT_200_MS                                 ((UINT16)(200)/TEST_TASK_PERIOD_MS)
#define I2C_TMOUT_250_MS                                 ((UINT16)(250)/TEST_TASK_PERIOD_MS)
#define I2C_TMOUT_500_MS                                 ((UINT16)(500)/TEST_TASK_PERIOD_MS)
#define I2C_TMOUT_2000_MS                                ((UINT16)(2000)/TEST_TASK_PERIOD_MS)
#define I2C_TMOUT_5000_MS                                ((UINT16)(5000)/TEST_TASK_PERIOD_MS)
/* I2C slave address #1 */
#define SLAVE_1_ADDRESS                                  ((UINT8)(0x01))
#define SLAVE_2_ADDRESS                                  ((UINT8)(0x02))
/* I2c cmd */
#define SB01_SET_RELAY_STS                               ((UINT8)(0x50))
#define SB01_REQUEST_RELAY_STS                           ((UINT8)(0x60))

/************************************************************************
* Typedefs
************************************************************************/


/************************************************************************
* LOCAL Variables
************************************************************************/
static UINT8 RelayB1, RelayB2 = 0;
static UINT8 Relay1, Relay2, Relay3, Relay4 = 0;
static UINT8 TxBuffer[] = {0x00, 0x00};  
static UINT8 I2cReady = FALSE;

/************************************************************************
* GLOBAL Variables
************************************************************************/


/************************************************************************
* LOCAL Function Prototypes
************************************************************************/
void I2cTxTest (void);

/************************************************************************
* LOCAL Function Implementations
************************************************************************/
/************************************************************************
* Function:     Led_MainTask
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:	 Execute I2C Test.
* Date:         30/07/16
************************************************************************/
void I2cTxTest (void)
{
   #define INIT_DELAY   0x00
   #define WRITE_1      0x10
   #define WRITE_2      0x11
   #define WRITE_3      0x12
   #define WRITE_4      0x13
   #define WRITE_5      0x14
   #define READ_1       0x20
   #define READ_2       0x21
   #define STOP         0x30   
   
   static UINT16 Counter = 0;
   static UINT8 Addr = 0x01;
   static UINT8 Cmd = 0;
   static UINT8 TxBuffer[] = {0x00, 0x00};
   static UINT8 RxBuffer[] = {0x00, 0x00}; 
   static UINT8 State = INIT_DELAY;
  
      
#ifdef WRITE_TEST         
   Counter++;
   /* Check counter */
   if (Counter > I2C_TX_TIMEOUT_MS)
   {
      if (BSP_BTN_1 == 1)
      {
         /* Reset counter */
         Counter = 0;
         /* Increment command */
         Cmd++;
         /* Reset */
         if (Cmd > 4)
         {
            Cmd = 0;
            /* Increment Address */
            Addr++;
         }
         
         if (Addr > 2)
         {
            Addr = 1;
         }
         
         /* Binary counter */
         TxBuffer[0] = 0x50;
         TxBuffer[1] = Cmd;
         
         /* Send I2C message */
         I2cSlv_SendI2cMsg(TxBuffer, Addr, sizeof(TxBuffer));      
      }
   } 
#endif
   
#ifdef READ_TEST   
   if (BSP_BTN_1 == 0)
   {   
   
   switch (State)
   {
      case INIT_DELAY:
         Counter++;
         if (Counter >= I2C_TMOUT_200_MS)
         {
            Counter = 0;
            State = WRITE_5;
         }
         break;
         
      case WRITE_1:
         TxBuffer[0] = 0x50;
         TxBuffer[1] = 0x02;         
         /* Send I2C message */
         I2cSlv_SendI2cMsg(TxBuffer, SLAVE_1_ADDRESS, 2);  
         State = WRITE_2;
         break;  
         
      case WRITE_2:
         Counter++;
         if (Counter >= I2C_TMOUT_2000_MS)
         {
            Counter = 0;
            State = WRITE_3;
         }
         break;

      case WRITE_3:
         TxBuffer[0] = 0x50;
         TxBuffer[1] = 0x01;        
         /* Send I2C message */
         I2cSlv_SendI2cMsg(TxBuffer, SLAVE_1_ADDRESS, 2);  
         State = WRITE_4;
         break;
         
      case WRITE_4:
         Counter++;
         if (Counter >= I2C_TMOUT_200_MS)
         {
            Counter = 0;
            State = WRITE_5;
         }
         break;         
         
      case WRITE_5:
         TxBuffer[0] = 0x60;        
         /* Send I2C message */
         I2cSlv_SendI2cMsg(TxBuffer, SLAVE_1_ADDRESS, 1);  
         State = READ_1;
         break;
         
      case READ_1:
         Counter++;
         if (Counter >= I2C_TMOUT_2000_MS)
         {
            Counter = 0;
            State = READ_2;
         }            

      case READ_2:
         I2cSlv_ReceiveI2cMsg(RxBuffer, SLAVE_1_ADDRESS, sizeof(RxBuffer) + 1);         
         if (I2cSlv_I2cReadMsgSts() == MessageReceived)
         {
            State = STOP;         
         }
         break;
         
      case STOP:         
         Counter++;
         if (Counter >= I2C_TMOUT_500_MS)
         {
            Counter = 0;
            State = INIT_DELAY;
         }            
         break;                  
   }
   }
#endif   
} 

#ifdef USB_TEST
/************************************************************************
* Function:     UsbTest
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Simple USB test
* Date:         19/09/16
************************************************************************/
void UsbTest (void)
{
   static UINT16 Counter = 0;
   UINT8 MessageBuffer[4];
   
   if (I2cReady == FALSE)
   {
      I2cReady = TRUE;
      I2cSlv_SendI2cMsg(TxBuffer, SLAVE_2_ADDRESS, sizeof(TxBuffer));
   }   
   
   Counter++;
   if (Counter >= 1)
   {
      Counter = 0;

      if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

      if (getsUSBUSART(MessageBuffer,4))
      {
         /* Set Relay1 and Relay2 values */
         if (MessageBuffer[1] == '1')
         {
            if (MessageBuffer[3] == '1')
            {
               Relay1 = 1;
            }
            else if (MessageBuffer[3] == '2')
            {
               Relay1 = !Relay1;
            }            
            else
            {
               Relay1 = 0;
            }
            
         /* Set global variable */
         RelayB1 = Relay1 | (Relay2 << 1);

         TxBuffer[0] = SB01_SET_RELAY_STS;
         TxBuffer[1] = RelayB1;
         /* Send I2C message */
         I2cSlv_SendI2cMsg(TxBuffer, SLAVE_2_ADDRESS, sizeof(TxBuffer));              
            
         }
         else if (MessageBuffer[1] == '2')
         {
            if (MessageBuffer[3] == '1')
            {
               Relay2 = 1;
            }
            else if (MessageBuffer[3] == '2')
            {
               Relay2 = !Relay2;
            }
            else
            {
               Relay2 = 0;
            }
            
         /* Set global variable */
         RelayB1 = Relay1 | (Relay2 << 1);

         TxBuffer[0] = SB01_SET_RELAY_STS;
         TxBuffer[1] = RelayB1;
         /* Send I2C message */
         I2cSlv_SendI2cMsg(TxBuffer, SLAVE_2_ADDRESS, sizeof(TxBuffer)); 
         
         }
         
         else if (MessageBuffer[1] == '3')
         {
            if (MessageBuffer[3] == '1')
            {
               Relay3 = 1;
            }
            else if (MessageBuffer[3] == '2')
            {
               Relay3 = !Relay3;
            }
            else
            {
               Relay3 = 0;
            }
            
         /* Set global variable */
         RelayB2 = (Relay3 << 1);

         TxBuffer[0] = SB01_SET_RELAY_STS;
         TxBuffer[1] = RelayB2;
         /* Send I2C message */
         I2cSlv_SendI2cMsg(TxBuffer, SLAVE_2_ADDRESS, sizeof(TxBuffer));              
            
         }                  
      }      
   }
}
#endif

#ifdef BT_TEST
/************************************************************************
* Function:     UsbTest
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Simple USB test
* Date:         19/09/16
************************************************************************/
void MdmTest (void)
{
   UINT8 MessageBuffer[2];
   
   if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

   if (getsUSBUSART(MessageBuffer,3))
   {
      if (MessageBuffer[0] == 'C')
      {  
         MdmBt_SetModemMode(MdmBt_ComMode);
      }
      else if (MessageBuffer[0] == 'A')
      {
         MdmBt_SetModemMode(MdmBt_AtMode);
      }
      else
      {
      
      }      
   }   
}

void Terminal (void)
{
#define USB_BUFFER_SIZE 25
   
   static UINT8 UsbRx[USB_BUFFER_SIZE];
   static UINT8 UsbTx[USB_BUFFER_SIZE];
   static UINT8 UartRx[USB_BUFFER_SIZE];
   static UINT8 UartTx[USB_BUFFER_SIZE];
   static UINT8 UartTxIndex = 0;
   UINT8 ReceivedBytes = 0;
   UINT8 Index;
   
   if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
   
   /* Get data */
   ReceivedBytes = getsUSBUSART(UsbRx,USB_BUFFER_SIZE);
   /* Buffer is not empty */
   if (ReceivedBytes > 0)
   {
      if (UsbRx[0] == '#')
      {
         UartTx[UartTxIndex] = '\r';
         UartTx[UartTxIndex + 1] = '\n';
         UartTx[UartTxIndex + 2] = NULL;
         Uart_WriteString(1,UartTx);
         UartTxIndex = 0;
      }
      else
      {
         for (Index = 0; Index < ReceivedBytes; Index++)
         {
            UartTx[UartTxIndex] = UsbRx[UartTxIndex];
            UartTxIndex++;         
         }
      }
   }
   
   if (ReceiveEvt(&(NewBtPacket)))
   {
      BtPacket.RxBuffer[BtPacket.RxByteNum] = '\r';
      BtPacket.RxBuffer[BtPacket.RxByteNum + 1] = '\n';
      if(USBUSARTIsTxTrfReady())
      {
          putUSBUSART(BtPacket.RxBuffer,(BtPacket.RxByteNum + 2));
      }      
   }
}

/************************************************************************
* Function:     BtRelayTest
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Simple USB test
* Date:         19/09/16
************************************************************************/
void BtRelayTest (void)
{
   static UINT16 Counter = 0;
   UINT8 MessageBuffer[4];
   
   if (I2cReady == FALSE)
   {
      I2cReady = TRUE;
      I2cSlv_SendI2cMsg(TxBuffer, SLAVE_2_ADDRESS, sizeof(TxBuffer));
   }   
   
   if (ReceiveEvt(&(BtPacket.PacketRxEvt)))
   {
      /* Set Relay1 and Relay2 values */
      if (BtPacket.RxBuffer[1] == '1')
      {
         if (BtPacket.RxBuffer[3] == '1')
         {
            Relay1 = 1;
         }
         else if (BtPacket.RxBuffer[3] == '2')
         {
            Relay1 = !Relay1;
         }            
         else
         {
            Relay1 = 0;
         }          
      }
      else if (BtPacket.RxBuffer[1] == '2')
      {
         if (BtPacket.RxBuffer[3] == '1')
         {
            Relay2 = 1;
         }
         else if (BtPacket.RxBuffer[3] == '2')
         {
            Relay2 = !Relay2;
         }
         else
         {
            Relay2 = 0;
         }
      }  
      else if (BtPacket.RxBuffer[1] == '3')
      {
         if (BtPacket.RxBuffer[3] == '1')
         {
            Relay3 = 1;
         }
         else if (BtPacket.RxBuffer[3] == '2')
         {
            Relay3 = !Relay3;
         }
         else
         {
            Relay3 = 0;
         }
      }
      else if (BtPacket.RxBuffer[1] == '4')
      {
         if (BtPacket.RxBuffer[3] == '1')
         {
            Relay4 = 1;
         }
         else if (BtPacket.RxBuffer[3] == '2')
         {
            Relay4 = !Relay4;
         }
         else
         {
            Relay4 = 0;
         }
      }        
      
      /* Set global variable */
      RelayB1 = Relay1 | (Relay2 << 1) | (Relay3 << 2) | (Relay4 << 3);

      TxBuffer[0] = SB01_SET_RELAY_STS;
      TxBuffer[1] = RelayB1;
      /* Send I2C message */
      I2cSlv_SendI2cMsg(TxBuffer, SLAVE_2_ADDRESS, sizeof(TxBuffer));       
   }         
}
#endif

#ifdef SMS_TEST
/************************************************************************
* Function:     SendSmsTest
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Simple USB test
* Date:         27/10/17
************************************************************************/
void SendSmsTest (void)
{
   static UINT8 counter = 0;
   
   counter++;
   
   if (counter == 40)
   {
      /* SMS text mode on */
      Mdm_SetSmsFormat(1);
   }
   
   if (counter == 50)
   {
      #define NUMBER  "+393296348287"
      #define MESSAGE "Ciao ANNA_FALCO!"
      /* Send SMS to my number */
      Mdm_SendSmsData(((void*)NUMBER),sizeof(NUMBER),((void*)MESSAGE),sizeof(MESSAGE));
   }  
}
#endif



void GpioTest (void)
{   
   static UINT8 UsbRx[25];
   static UINT8 UsbTx[25];
   static UINT8 UartTxIndex = 0;
   UINT8 ReceivedBytes = 0;
   UINT8 Index;
   
   if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
   
   /* Get data */
   ReceivedBytes = getsUSBUSART(UsbRx,25);
   /* Buffer is not empty */
   if (ReceivedBytes > 0)
   {
      if (UsbRx[0] == '0')
      {
         BSP_IO_2 = 0;
         BSP_IO_3 = 0;
         BSP_IO_4 = 0;
         BSP_IO_5 = 0;         
      }
      else if (UsbRx[0] == '1')
      {
         BSP_IO_2 = 1;
      }
      else if (UsbRx[0] == '2')
      {
         BSP_IO_3 = 1;
      }      
      else if (UsbRx[0] == '3')
      {
         BSP_IO_4 = 1;
      }      
      else if (UsbRx[0] == '4')
      {
         BSP_IO_5 = 1;
      }            
   }   
}

/************************************************************************
* GLOBAL Function Implementations
************************************************************************/

/************************************************************************
* Function:     Test_MainTask
* Input:        None
* Output:       None
* Author:       F.Ficili
* Description:  Manage Test task.
* Date:         30/07/16
************************************************************************/
void Test_MainTask (UINT8 Options)
{    
   switch (SystemState)
   {
      /* System Initialization Phase */
      case InitializationState:
         /* Init user button */
         BSP_BTN_1_TRIS = LINE_DIRECTION_INPUT;
         
         BSP_IO_2_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_IO_3_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_IO_4_TRIS = LINE_DIRECTION_OUTPUT;
         BSP_IO_5_TRIS = LINE_DIRECTION_OUTPUT;         
                  
         break;

      /* System Normal operation Phase */
      case RunningState:       
         
         #ifdef I2C_TX_TEST
            /* I2c test */
            I2cTxTest();
         #endif

         #ifdef APP_TEST 
            /* App test */
            AppTest();
         #endif         

         #ifdef USB_TEST 
            /* Usb test */
            UsbTest();                  
         #endif     

         #ifdef MDM_TEST 
            /* Mdm test */
            MdmTest();
         #endif             

         #ifdef BT_TEST 
            Terminal();
            BtRelayTest();
         #endif     

         #ifdef SMS_TEST          
            SendSmsTest();
         #endif   
            
         GpioTest();
         break;

      /* Default */
      default:
         break;
   }
}  

#endif /* (TEST_TASK_STS == STD_ON) */