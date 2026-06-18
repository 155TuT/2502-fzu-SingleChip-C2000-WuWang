//#############################################################################
//
//! \file    User_Component/myTimer/myTimer.c
//!
//! \brief   CPU TIMER
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
#include "User_Component/myTimer/myTimer.h"


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions
// (1) module Initial
//! \brief     myTimer module initial
//! \param[in] None
//! \param[out] None
void myTimer_initial(void)
{

}
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     myTimer Pin configure
//! \param[in] None
//! \param[out] None
void myTimer_pinConfigure(void)
{

}
//
// (2.2) module function configure
//! \brief     myTimer function configure
//! \param[in] None
//! \param[out] None
void myTimer_functionConfigure(void)
{
	//void TIMER_stop(TIMER_Handle timerHandle);
	//void TIMER_setDecimationFactor(TIMER_Handle timerHandle,
	//                               const uint16_t decFactor);
	//void TIMER_setPeriod(TIMER_Handle timerHandle,
	//                     const uint32_t period);
	//void TIMER_reload(TIMER_Handle timerHandle);
	//void TIMER_start(TIMER_Handle timerHandle);
	TIMER_stop(myTimer0);
	TIMER_setDecimationFactor(myTimer0, 1);   //0+1个时钟周期
//	TIMER_setPeriod(myTimer0, 60000000L);
	TIMER_setPeriod(myTimer0, 30000L-1);    //60000000*1/60000000=1ms
//	TIMER_setPeriod(myTimer0, 60000L-1);    //60000000*1/60000000=1s

	TIMER_reload(myTimer0);
	TIMER_start(myTimer0);

}
//
// (2.3) module Event configure
//! \brief     myTimer Event configure
//! \param[in] None
//! \param[out] None
void myTimer_eventConfigure(void)
{

}

                      
// **************************************************************************
// the API functions



// end of file
