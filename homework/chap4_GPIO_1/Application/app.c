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
void LED_Control(void)
{
    LED_on(LED1);
    Delay(100000L);
    LED_off(LED1);
    Delay(100000L);
}


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




void LED_Control2(void)
{

	unsigned char i;
	for(i=0;i<2;i++)
	{
		LED_on(LED[i*2]);
		LED_on(LED[i*2+1]);
		Delay(100000L);
		LED_off(LED[i*2]);
		LED_off(LED[i*2+1]);
	    Delay(100000L);
	}
}
//void LED_Control3(void)
//{
//    unsigned char i;
//    for(i=0;i<4;i++)
//    {
//    myGpio->GPACLEAR=(uint32_t)1 << i;
//    Delay(1000000L);
//    myGpio->GPASET=(uint32_t)1 << i;
//    Delay(1000000L);
//    }
//}




// end of file
