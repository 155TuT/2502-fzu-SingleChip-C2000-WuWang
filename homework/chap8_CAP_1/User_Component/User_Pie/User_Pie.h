
//#############################################################################
//
//! \file   User_Component/User_Pie/User_Pie.h
//!
//! \brief  PIE
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


#ifndef _USER_PIE_H_
#define _USER_PIE_H_


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
//
// the function prototypes
//
// (1) module Initial
//! \brief     User_Pie module initial
//! \param[in] None
//! \param[out] None
TARGET_EXT void User_Pie_initial(void);
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     User_Pie Pin configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void User_Pie_pinConfigure(void);
//
// (2.2) module function configure
//! \brief     User_Pie function configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void User_Pie_functionConfigure(void);
//
// (2.3) module Event configure
//! \brief     User_Pie Event configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void User_Pie_eventConfigure(void);
//
/*******************************************/

/*------- API description of the example module   -------------*/
// EXAMPLE API
// the function prototypes
//! \brief     EXAMPLE API
//! \param[in] None
//! \param[out] None
//TARGET_EXT void EXAMPLE_API(void);

//! \brief     User_Pie API
//! \param[in] None
//! \param[out] None
TARGET_EXT void inline User_Pie_start(void)
{
	//Enable Global Interrupts
	CPU_enableGlobalInts(myCpu);
}

//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None

TARGET_EXT interrupt void myCap_CAPINT_isr(void);     //redefined in Isr.h

TARGET_EXT interrupt void KEY_XINT1_isr(void);     //redefined in Isr.h

//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
TARGET_EXT interrupt void myTimer_CpuTimer0_isr(void);     //redefined in Isr.h


//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
TARGET_EXT interrupt void LED_EPWM1_isr(void);     //redefined in Isr.h

//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
TARGET_EXT interrupt void myAdc_ADCINT_isr(void);     //redefined in Isr.h




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


#endif  // end of _USER_PIE_H_ definition

