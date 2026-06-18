//#############################################################################
//
//! \file  Application\app.c
//!
//! \brief  application
//
//  Author:
//  Group:          C2000
//  Target Device:  TMS320F2802x
//
//  (C) Copyright 2014, Texas Instruments, Inc.
//#############################################################################
// $TI Release: f2802x Support Library v210 $
// $Release Date: Mon Sep 17 09:13:31 CDT 2012 $
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


void KEY_Control_LED3(void)                    //显示程序在app层
{
    switch(key_status)
    {
       case KEY_PRESS_SHORT:
           LED_TIME_DELAY=LED_TIME_DELAY-1;     //短按
           key_status = KEY_PRESS_NO;
           break;
       case KEY_PRESS_LONG:
           LED_TIME_DELAY=LED_TIME_DELAY+1;     //长按
           key_status = KEY_PRESS_NO;
           break;
       case KEY_PRESS_TWICE:                     //双击
           display_mode=!display_mode;
           key_status = KEY_PRESS_NO;;
           break;
       default:  break;
    }

}












void KEY_Control_LED(void)                    //显示程序在app层
{
    switch(key_status)
    {
       case 1:   LED_TIME_DELAY=1;break;
       case 2:   LED_TIME_DELAY=5;break;
       case 3:   LED_TIME_DELAY=10;break;
       default:  break;
    }

}

void KEY_Control_LED1(void)                    //显示程序在用户层
{
    switch(key_status)
    {
       case 1:   LED_DISPLAY11();break;
       case 2:   LED_DISPLAY33();break;
       case 3:   LED_DISPLAY22();break;
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
