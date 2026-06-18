//#############################################################################
//
//! \file:  "User_Component/LED_Gpio/LED_Gpio.c"
//! \brief: GPIO for LED
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
#include "User_Component/LED_Gpio/LED_Gpio.h"


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
void LED_GPIO_initial(void)
{
	LED_off(LED1);
	LED_off(LED2);
	LED_off(LED3);
	LED_off(LED4);
	LED[0]=GPIO_Number_0;
	LED[1]=GPIO_Number_1;
	LED[2]=GPIO_Number_2;
	LED[3]=GPIO_Number_3;

}
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     LED_GPIO Pin configure
//! \param[in] None
//! \param[out] None
void LED_GPIO_pinConfigure(void)
{
	// 1. set mode
	//void GPIO_setMode(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Mode_e mode);
	GPIO_setMode(LED_Gpio_obj, LED1, GPIO_0_Mode_GeneralPurpose);
	GPIO_setMode(LED_Gpio_obj, LED2, GPIO_1_Mode_GeneralPurpose);
	GPIO_setMode(LED_Gpio_obj, LED3, GPIO_2_Mode_GeneralPurpose);
	GPIO_setMode(LED_Gpio_obj, LED4, GPIO_3_Mode_GeneralPurpose);

	// 2. set pullup
	//void GPIO_setPullUp(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_PullUp_e pullUp);
	GPIO_setPullUp(LED_Gpio_obj, LED1, GPIO_PullUp_Disable);
	GPIO_setPullUp(LED_Gpio_obj, LED2, GPIO_PullUp_Disable);
	GPIO_setPullUp(LED_Gpio_obj, LED3, GPIO_PullUp_Disable);
	GPIO_setPullUp(LED_Gpio_obj, LED4, GPIO_PullUp_Disable);

	// 3. set direction
	//void GPIO_setDirection(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Direction_e direction);
	GPIO_setDirection(LED_Gpio_obj, LED1, GPIO_Direction_Output);
	GPIO_setDirection(LED_Gpio_obj, LED2, GPIO_Direction_Output);
	GPIO_setDirection(LED_Gpio_obj, LED3, GPIO_Direction_Output);
	GPIO_setDirection(LED_Gpio_obj, LED4, GPIO_Direction_Output);

}
//
// (2.2) module function configure
//! \brief     LED_GPIO function configure
//! \param[in] None
//! \param[out] None
void LED_GPIO_functionConfigure(void)
{

}
//
// (2.3) module Event configure
//! \brief     LED_GPIO Event configure
//! \param[in] None
//! \param[out] None
void LED_GPIO_eventConfigure(void)
{

}
// ********************************************
void Delay(uint32_t time)
{
    while(time--) ;
}


void LED_Display(void)           //灯闪烁控制
{
    LED_on(LED1);
    Delay(500000L);
    LED_off(LED1);
    Delay(500000L);
}


void LED_Display1(void)          //矩阵方式实现显示
{
    int c;int d;
    int a[4][5]={{0,0,1,0,1},{0,1,0,1,0},{0,1,0,1,0},{0,0,1,0,1}};   //五种状态
    for(c=0;c<=4;c++)
            {
                for(d=0;d<=3;d++)
                {
                    if(a[d][c]==1)
                        LED_on(LED[d]);
                    else
                        LED_off(LED[d]);
                }
                Delay(50000L);
            }

}

void LED_Display2(void)                      //矩阵方式实现流水灯
{
    int c;int d;
    int a[4][5]={{0,1,0,0,0},{0,0,1,0,0},{0,0,0,1,0},{0,0,0,0,1}};
    for(c=0;c<=4;c++)
            {
                for(d=0;d<=3;d++)
                {
                    if(a[d][c]==1)
                        LED_on(LED[d]);
                    else
                        LED_off(LED[d]);
                }
                Delay(50000L);
            }

}

// ********************************************
void LED_Display3(void)              //流水灯
{

    unsigned char i;
    for(i=0;i<4;i++)
    {
        LED_on(LED[i]);
        Delay(50000L);
        LED_off(LED[i]);
        Delay(50000L);
    }
}

                      
// **************************************************************************
// the API functions



// end of file
