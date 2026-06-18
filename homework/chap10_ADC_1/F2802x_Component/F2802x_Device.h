//#############################################################################
//
//! \file:  "F2802x_Device.h"
//! \brief: include;my***
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

#ifndef F2802X_DEVICE_H
#define F2802X_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TARGET_GLOBAL
   #define TARGET_EXT 	extern
#else
   #define TARGET_EXT
#endif

//includes
#include "F2802x_Component/include/adc.h"
#include "F2802x_Component/include/clk.h"
#include "F2802x_Component/include/flash.h"
#include "F2802x_Component/include/gpio.h"
#include "F2802x_Component/include/pie.h"
#include "F2802x_Component/include/pll.h"
#include "F2802x_Component/include/timer.h"
#include "F2802x_Component/include/wdog.h"
#include "F2802x_Component/include/pwm.h"
#include "F2802x_Component/include/cap.h"
#include "F2802x_Component/include/sci.h"

// F2802x Device Define

TARGET_EXT CLK_Handle myClk;
TARGET_EXT CPU_Handle myCpu;
TARGET_EXT PLL_Handle myPll;
TARGET_EXT WDOG_Handle myWDog;
TARGET_EXT PIE_Handle myPie;
TARGET_EXT GPIO_Handle myGpio;
TARGET_EXT TIMER_Handle myTimer0;
TARGET_EXT ADC_Handle myAdc;
TARGET_EXT CAP_Handle myCap;
TARGET_EXT PWM_Handle myPwm1;
TARGET_EXT PWM_Handle myPwm2;

TARGET_EXT void System_authorize(void);    //system authorize

#ifdef __cplusplus
}
#endif /* extern "C" */



#endif   // end of _F2802X_DEVICE_H_ definition



