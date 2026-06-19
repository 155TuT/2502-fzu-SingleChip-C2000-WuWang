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


void LED_Control1(void)
{

    unsigned char i;
    for(i=0;i<4;i++)
    {
        LED_on(LED[i]);
        Delay(10000L);
        LED_off(LED[i]);
        Delay(10000L);
    }
}



void LED_KEY_Control(void)
{
    if(  KEY_STATUS(KEY1)==1  )
    {
        LED_on(LED1);
    }
    else
    {
        LED_off(LED1);

    }

}


void KEY_Control_LED(void)                    //显示程序在app层
{
    switch(key_status)
    {
       case 1:   LED_DISPLAY1();break;   //LED1亮暗显示
       case 2:   LED_DISPLAY2();break;   //流水灯显示
       case 3:   LED_DISPLAY3();break;   //分组交替显示
       default:  break;
    }

}

void KEY_Control_LED1(void)                    //显示程序在用户层
{
    switch(key_status)
    {
       case 1:   LED_DISPLAY11();break;   //LED1亮暗显示
       case 2:   LED_DISPLAY22();break;   //流水灯显示
       case 3:   LED_DISPLAY33();break;   //分组交替显示
       default:  break;
    }

}





void LED_KEY_short_long(void)       //长按短按切换显示
{
//    if(key_status==KEY_PRESS_SHORT)
//    {
//        LED_DISPLAY1();            //跑马灯
//    }
//    if(key_status==KEY_PRESS_LONG)
//    {
//        LED_DISPLAY2();            //两组灯切换
//    }

    switch(key_status)
    {
    case 0:
        LED_DISPLAY1();            //跑马灯
        break;
    case 1:
        LED_DISPLAY2();            //两组灯切换
        break;
    default:
        break;

    }

}



void LED_DISPLAY1(void)
{
    LED_on(LED1);
    Delay(2000L);          //板子仿真设置1000000；proteus仿真设置1000
    LED_off(LED1);
    Delay(2000L);
}


void LED_DISPLAY2(void)         //流水灯
{

    unsigned char i;
    for(i=0;i<4;i++)
    {
        LED_on(LED[i]);
        Delay(2000L);
        LED_off(LED[i]);
        Delay(2000L);
    }
}






void LED_DISPLAY3(void)     //两个灯切换显示
{
    LED_on(LED1);
    LED_on(LED4);
    Delay(2000L);
    LED_off(LED1);
    LED_off(LED4);
    Delay(2000L);

    LED_on(LED2);
    LED_on(LED3);
    Delay(2000L);
    LED_off(LED2);
    LED_off(LED3);
    Delay(2000L);

}







// end of file
