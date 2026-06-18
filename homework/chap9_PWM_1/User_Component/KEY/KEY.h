
//#############################################################################
//
//! \file:  "User_Component/KEY/KEY.h"
//! \brief: KEY
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
//############################################################################


#ifndef _KEY_H_
#define _KEY_H_


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
#define   KEY_obj      myGpio          //object
#define   KEY1         GPIO_Number_12  //pin

// the function prototypes
//
// (1) module Initial
//! \brief     KEY module initial
//! \param[in] None
//! \param[out] None
TARGET_EXT void KEY_initial(void);
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     KEY Pin configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void KEY_pinConfigure(void);
//
// (2.2) module function configure
//! \brief     KEY function configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void KEY_functionConfigure(void);
//
// (2.3) module Event configure
//! \brief     KEY Event configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void KEY_eventConfigure(void);
//
/*******************************************/

/*------- API description of the example module   -------------*/
// EXAMPLE API
// the function prototypes
//! \brief     EXAMPLE API
//! \param[in] None
//! \param[out] None
//TARGET_EXT void EXAMPLE_API(void);

//! \brief     Get Key Status
//! \param[in] key
//! \param[out] None
TARGET_EXT uint16_t inline GetKeyStatus(GPIO_Number_e key)
{
	return GPIO_getData(KEY_obj, key);

}

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
#define    KEYPRESSED   1

/*------- end of defines  -------------*/



#ifdef __cplusplus
}
#endif // extern "C"


#endif  // end of _KEY_H_ definition

