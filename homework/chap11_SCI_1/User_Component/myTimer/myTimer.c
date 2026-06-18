//#############################################################################
//
//! \file:  "User_Component/myTimer/myTimer.c"
//! \brief: TIMER function configuration
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
	Hour = 15;
	Minute = 35;
	Second = 0;
	HalfSecond = 0;

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
	TIMER_setDecimationFactor(myTimer0, 0);
	TIMER_setPeriod(myTimer0, 30000000L);
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
