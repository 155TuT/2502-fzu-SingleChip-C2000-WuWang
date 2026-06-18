
/*
 * main.c
 */

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
//    LED_GPIO_pinConfigure();
//    LED_GPIO_functionConfigure();
//    LED_GPIO_eventConfigure();
//	LED_GPIO_initial();

	//按键模块初始化
//	  KEY_GPIO_pinConfigure();
//	  KEY_GPIO_functionConfigure();
//	  KEY_GPIO_eventConfigure();
//	  KEY_GPIO_initial();

	  LED_PWM_eventConfigure();
	  LED_PWM_functionConfigure();
	  LED_PWM_pinConfigure();
	  LED_PWM_initial();


	    //2.3 myTimer
//	    myTimer_pinConfigure();
//	    myTimer_functionConfigure();
//	    myTimer_eventConfigure();
//	    myTimer_initial();

	    //2.5 myCap
	     myCap_pinConfigure();
	     myCap_functionConfigure();
	     myCap_eventConfigure();
	     myCap_initial();


	    //3. PIE runtime environment(if use interrupt)
	    User_Pie_initial();
	    User_Pie_pinConfigure();
	    User_Pie_functionConfigure();
	    User_Pie_eventConfigure();

	    //4. the global interrupt start (if use interrupt)
	    User_Pie_start();

	//3. PIE runtime environment(if use interrupt)

	//4. the global interrupt start (if use interrupt)

	//5. main LOOP
    for( ; ; )
    {
    	// call API from app.h
    //	LED_Control();
    //	myGpio->GPACLEAR = 1;
//        myGpio->GPACLEAR = 2;
//        myGpio->GPACLEAR = 4;
//        myGpio->GPACLEAR = 8;

  //      key_status=myGpio->GPADAT;
    //    KEY_ID();                  //按键检测

     //   KEY_Control_LED1();        //按键处理,显示在用户层


  //      KEY_ID();                  //按键检测
    //    LED_DISPLAY2();
     //   LED_Control2();

     //   KEY_Control_LED2();        //按键处理
 //       LED_DISPLAY();
      //  KEY_Control_LED3();        //按键处理
      //  LED_DISPLAY5();

    }


}

