//#############################################################################
//
//! \file  User_Component/LED_Gpio/LED_Gpio.c
//!
//! \brief LED control by GPIO
//!
//  Quthor:
//  Group:          C2000
//  Target Device:  TMS320F2802x
//
//  (C) Copyright 2015, Fuzhou University.
//#############################################################################
// $TI Release: f2802x Support Library v210 $
// $Release Date: Mon Sep 17 09:13:31 CDT 2012 $
//#############################################################################

// **************************************************************************
// the includes
#include "User_Component/key_Gpio/key_Gpio.h"


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions
// (1) module Initial
//! \brief     LED_GPIO module initial
//! \param[in] None
//! \param[out] None
void KEY_GPIO_initial(void)
{
    key_status =0;
    key_status1=0;
    key_status2=0;
    key_on=0;
    key_two_msecond=0;
}
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     LED_GPIO Pin configure
//! \param[in] None
//! \param[out] None
void KEY_GPIO_pinConfigure(void)
{
	// 1. set mode
	//void GPIO_setMode(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Mode_e mode);
	GPIO_setMode(KEY_Gpio_obj, KEY1, GPIO_12_Mode_GeneralPurpose);


	// 2. set pullup
	//void GPIO_setPullUp(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_PullUp_e pullUp);
	GPIO_setPullUp(KEY_Gpio_obj, KEY1, GPIO_PullUp_Disable);


	// 3. set direction
	//void GPIO_setDirection(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Direction_e direction);
	GPIO_setDirection(KEY_Gpio_obj, KEY1, GPIO_Direction_Input);

	//4.滤波
	GPIO_setQualification(KEY_Gpio_obj,KEY1,GPIO_Qual_Sample_6);
	GPIO_setQualificationPeriod(KEY_Gpio_obj,KEY1,250);


}
//
// (2.2) module function configure
//! \brief     LED_GPIO function configure
//! \param[in] None
//! \param[out] None
void KEY_GPIO_functionConfigure(void)
{

}
//
// (2.3) module Event configure
//! \brief     LED_GPIO Event configure
//! \param[in] None
//! \param[out] None
void KEY_GPIO_eventConfigure(void)
{
    PIE_setExtIntPolarity(myPie, CPU_ExtIntNumber_1, PIE_ExtIntPolarity_RisingEdge);

    //  void GPIO_setExtInt(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const CPU_ExtIntNumber_e intNumber);
    GPIO_setExtInt(KEY_Gpio_obj, KEY1, CPU_ExtIntNumber_1);

}


// **************************************************************************
// the API functions

void KEY_ID(void)
{
    if(KEYPRESSED == KEY_STATUS(KEY1) && (key_on ==0))         //按键按下,等待释放.一次按键处理一次。
        {
          key_on = 1;
        }

    if((KEYPRESSED != KEY_STATUS(KEY1))&& (key_on==1) )          //按键释放,开始处理
        {
            key_on =0;

            if(key_counter>=3)  key_counter=0;
            key_counter++;
            key_status=key_counter;

        }
}

// end of file
