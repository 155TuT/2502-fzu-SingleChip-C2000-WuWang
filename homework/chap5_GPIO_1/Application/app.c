//#############################################################################
//
//! \file:  "Application/app.c"
//! \brief: Application program
//  Group:  C2000
//  Target Device:  TMS320F2802x
//
//  Author:  Cai Fenghuang，Wang Wu，Jiang Jiahui.
//
//  (C) Copyright 2021, Embedded Principles and Application Course Group，Fuzhou University.
//
//  $Release Date: 2021.6 $
//#############################################################################
//  作者：蔡逢煌、王武、江加辉
//  版权：福州大学，嵌入式原理及应用课程组
//  配套教材：《微控制器原理及应用---基于TI C2000实时微控制器》
//#############################################################################

// **************************************************************************
// the includes
#include "Application/app.h"


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions

//! \brief     LED Control
//! \param[in] None
//! \param[out] None


//! \brief     LED Control by KEY
//! \param[in] None
//! \param[out] None


//**************************************************************************
void KEY_Control_LED(void)                    //按键控制简易程序
{
    if( KEYPRESSED == GetKeyStatus(KEY1))
    {
        LED_on(LED1);
    }
    else
    {
        LED_off(LED1);
    }
}

// **************************************************************************
void KEY_Control_LED_1(void)                 //短按、长按、双击
{

  if(key_status == KEY_PRESS_SHORT)         //短按
  {
      LED_toggle(LED1);
  }
  if(key_status == KEY_PRESS_LONG)         //长按
  {
      LED_toggle(LED2);
  }

  if(key_status == KEY_PRESS_TWICE)       //双击
  {
      LED_toggle(LED3);
  }
  key_status = KEY_PRESS_NO;              //避免重复处理
}

// **************************************************************************
void KEY_Control_LED_2(void)            //按键控制两种不同显示方式
{

  if(KeyCount% 2 ==0)
  {
      LED_Display1();
  }
  if(KeyCount% 2== 1)
  {
      LED_Display3();
  }
}
//**************************************************************************
void KEY_Control_LED_3(void)                     //按键抗抖动
{
    if(KEYPRESSED != GetKeyStatus(KEY1))
    {
        key_counter=0;
    }
    if( KEYPRESSED == GetKeyStatus(KEY1))
    {
        key_counter++;
        if( key_counter >= 65500)
            {
                LED_toggle(LED1);
                key_counter=0;
            }
    }
}
//**************************************************************************
//
//void KEY4_Control_LED(void)
//{
//    if(KEYPRESSED != GetKeyStatus(KEY1))
//       {
//           key_counter1=0;
//           key_counter2=0;
//       }
//       if( KEYPRESSED == GetKeyStatus(KEY1))
//       {
//           key_counter1++;
//           key_counter2++;
//           if( key_counter1 >= 100)
//               {
//                   LED_toggle(LED1);
//                   key_counter1=0;
//               }
//           if( key_counter2 >= 65500)
//               {
//                   LED_toggle(LED2);
//                   key_counter2=0;
//               }
//
//       }
//
//    if((KEYPRESSED == GetKeyStatus(KEY1)) &&( 0 == Flag) )
//    {
//        Flag = 1;
//    }
//
//    if((KEYPRESSED != GetKeyStatus(KEY1)) &&( 1 == Flag) )
//    {
//        Flag = 0;
//        KeyCount ++;
//        if(3 < KeyCount)
//        {
//            KeyCount = 0;
//            LED_toggle(LED3);
//        }
//    }
//
//}
//
//


// end of file
