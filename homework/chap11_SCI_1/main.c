//#############################################################################
//
//! \file:  main.c
//! \brief: main file
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

#define TARGET_GLOBAL 1

#include "Application\app.h"


void main(void)
{
    //1. System runtime environment
	User_System_pinConfigure();
	User_System_functionConfigure();
	User_System_eventConfigure();
	User_System_initial();

	//2. Module
    //2.1 LED_Gpio
    LED_GPIO_pinConfigure();
    LED_GPIO_functionConfigure();
    LED_GPIO_eventConfigure();
	LED_GPIO_initial();

    //2.2 KEY
    //KEY_pinConfigure();
    //KEY_functionConfigure();
    //KEY_eventConfigure();
    //KEY_initial();

    //2.3 myTimer
//    myTimer_pinConfigure();
//    myTimer_functionConfigure();
//    myTimer_eventConfigure();
//    myTimer_initial();

    //2.4 LED_PWM
    //LED_PWM_pinConfigure();
    //LED_PWM_functionConfigure();
    //LED_PWM_eventConfigure();
    //LED_PWM_initial();

    //2.5 myCap
    //myCap_pinConfigure();
    //myCap_functionConfigure();
    //myCap_eventConfigure();
    //myCap_initial();

    //2.6 myAdc
    //myADC_pinConfigure();
    //myADC_functionConfigure();
    //myADC_eventConfigure();
    //myADC_initial();

    //2.7 mySci
    mySCI_pinConfigure();
    mySCI_functionConfigure();
    mySCI_eventConfigure();
    mySCI_initial();

    //3. PIE runtime environment(if use interrupt)
    User_Pie_pinConfigure();
    User_Pie_functionConfigure();
    User_Pie_eventConfigure();
    User_Pie_initial();
//
//	//4. the global interrupt start (if use interrupt)
     User_Pie_start();

	//5. main LOOP
    for( ; ; )
    {
    	// call API from app.h
    	//LED_Control();
    	//KEY_LED_Control();
     //SCI_Test();  //查询方式,要关闭中断,aci码显示
    	SCI_Test1();   //查询方式,要关闭中断,把串口收到的数据发送回去
    //	SCI_ExchageData();//中断 非FIFO模式 ,发送端要加上十六进制的“00”
//        mySci->SCITXBUF = 1;
//       mySci->SCITXBUF = 2;
//        mySci->SCITXBUF = 3;
//       mySci->SCITXBUF = 4;
//        mySci->SCITXBUF = 5;
//        mySci->SCITXBUF = 6;
    	GPIO_toggle(LED_Gpio_obj,LED1);
    	Delay(200000);

    }

}

