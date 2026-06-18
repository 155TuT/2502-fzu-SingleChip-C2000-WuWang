/*
 * Button_GPIO.c
 *
 *  Created on: 2020Äê2ÔÂ20ÈÕ
 *      Author: Administrator
 */
#include "User_Component/Button_GPIO/Button_GPIO.h"


void Button_GPIO_pinConfigure(void)
{   //1. set pullup
    //void GPIO_setPullUp(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_PullUp_e pullUp);
    GPIO_setPullUp(myGpio, Button1, GPIO_PullUp_Disable);

    //2. set mode
    //void GPIO_setMode(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Mode_e mode);
    GPIO_setMode(myGpio, Button1, GPIO_12_Mode_GeneralPurpose);


    //3. set direction
    //void GPIO_setDirection(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Direction_e direction);
    GPIO_setDirection(myGpio, Button1, GPIO_Direction_Input);


    GPIO_setQualification(myGpio, Button1, GPIO_Qual_ASync );
}

uint16_t Button_State_Read(void)
{
    return(GPIO_getData(myGpio, Button1));
}

