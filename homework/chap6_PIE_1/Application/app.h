//#############################################################################
//
//! \file:  "Application/app.h"
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


#ifndef _APP_H_
#define _APP_H_


// **************************************************************************
// the includes
#include <stdint.h>

// driver
#include "User_Component/User_Device.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifndef TARGET_GLOBAL
   #define TARGET_EXT 	extern
#else
   #define TARGET_EXT
#endif


/*------- API description of the example module   -------------*/
// EXAMPLE API
// the function prototypes
//! \brief     EXAMPLE API
//! \param[in] None
//! \param[out] None
//TARGET_EXT void EXAMPLE_API(void);

//! \brief     LED Control
//! \param[in] None
//! \param[out] None


TARGET_EXT void LED_KEY_short_long(void);
TARGET_EXT void KEY_Control_LED(void);
TARGET_EXT void KEY_Control_LED1(void);
TARGET_EXT void LED_Control1(void);


TARGET_EXT void LED_DISPLAY1(void);
TARGET_EXT void LED_DISPLAY2(void);
TARGET_EXT void LED_DISPLAY3(void);
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


#endif  // end of _APP_H_ definition

