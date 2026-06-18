
//#############################################################################
//
//! \file    User_Component/LED_Gpio/LED_Gpio.h
//!
//! \brief   LED control by GPIO
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


#ifndef _KEY_GPIO_H_
#define _KEY_GPIO_H_


// **************************************************************************
// the includes
#include <stdint.h>

// driver
#include "F2802x_Component/F2802x_Device.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef TARGET_GLOBAL
   #define TARGET_EXT 	extern
#else
   #define TARGET_EXT
#endif


/*------- hardware description of the example module   -------------*/
// For example
// The module derived from GPIO
//#define   EXAMPLE_obj   myGpio          //here myGpio is defined in
                                          //F2802x_Component/F2802x_Device.h
//#define   EXAMPLE_PIN   GPIO_Number_??  //pin

#define   KEY_Gpio_obj      myGpio          //object
#define   KEY1           GPIO_Number_12  //pin

#define   KEY_PRESS_SHORT    1  //pin
#define   KEY_PRESS_LONG     2  //pin
#define   KEY_PRESS_TWICE    3  //pin
#define   KEY_PRESS_NO       0
#define   KEYPRESSED         1
TARGET_EXT uint16_t key_counter;
TARGET_EXT uint16_t key_status;
TARGET_EXT uint16_t key_status1;
TARGET_EXT uint16_t key_status2;
TARGET_EXT uint16_t key_on;
TARGET_EXT uint16_t KeyCount;
TARGET_EXT uint16_t key_two_touch;
TARGET_EXT uint32_t key_two_msecond;
// the function prototypes
//
// (1) module Initial
//! \brief     LED_GPIO module initial
//! \param[in] None
//! \param[out] None

TARGET_EXT void KEY_GPIO_initial(void);
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     LED_GPIO Pin configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void KEY_GPIO_pinConfigure(void);
//
// (2.2) module function configure
//! \brief     LED_GPIO function configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void KEY_GPIO_functionConfigure(void);
//
// (2.3) module Event configure
//! \brief     LED_GPIO Event configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void KEY_GPIO_eventConfigure(void);
//
/*******************************************/

/*------- API description of the example module   -------------*/
// EXAMPLE API
// the function prototypes
//! \brief     EXAMPLE API
//! \param[in] None
//! \param[out] None
//TARGET_EXT void EXAMPLE_API(void);

//! \brief      LED on
//! \param[in]  None
//! \param[out] None

TARGET_EXT void KEY_ID(void);            //按键识别程序

TARGET_EXT  uint16_t inline KEY_STATUS(GPIO_Number_e key)
{
   return GPIO_getData(KEY_Gpio_obj, key);
}
//! \brief      LED off
//! \param[in]  None
//! \param[out] None



/*-------end of API description -------------*/


/*------- Globals of the example module   -------------*/
// the globals

//! \brief Defines the Variable
//TARGET_EXT uint16_t  Example;

/*------- end of globals  -------------*/


/*------- Defines of the example module   -------------*/
// the defines

//! \brief Defines the constant
//#define EXAMPLE_DEF

/*------- end of defines  -------------*/



#ifdef __cplusplus
}
#endif // extern "C"


#endif  // end of _LED_GPIO_H_ definition

