//#############################################################################
//
//! \file  User_Component/myCap/myCap.c
//!
//! \brief   ECAP
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
#include <User_Component/User_CAP/User_CAP.h>


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions
// (1) module Initial
//! \brief     myCap module initial
//! \param[in] None
//! \param[out] None
void myCap_initial(void)
{
	myCapVal1 = 0;
	myCapVal2 = 0;
	myCapVal3 = 0;

}

//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     myCap Pin configure
//! \param[in] None
//! \param[out] None
void myCap_pinConfigure(void)
{
	//1. GPIO setup(Input)
	//void GPIO_setMode(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Mode_e mode);
	//void GPIO_setPullUp(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_PullUp_e pullUp);
	//void GPIO_setDirection(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Direction_e direction);
	//void GPIO_setQualification(GPIO_Handle gpioHandle, const GPIO_Number_e gpioNumber, const GPIO_Qual_e qualification);
	//void GPIO_setQualificationPeriod(GPIO_Handle gpioHandle, const GPIO_Number_e gpioNumber, const uint16_t period);
	GPIO_setMode(myGpio, GPIO_Number_5, GPIO_5_Mode_ECAP1);
	GPIO_setPullUp(myGpio, GPIO_Number_5, GPIO_PullUp_Disable);
	GPIO_setDirection(myGpio, GPIO_Number_5, GPIO_Direction_Input);
	GPIO_setQualification(myGpio, GPIO_Number_5, GPIO_Qual_ASync);
}
//
// (2.2) module function configure
//! \brief     myCap function configure
//! \param[in] None
//! \param[out] None
void myCap_functionConfigure(void)
{
	//1. mode
	//void CAP_setCapContinuous(CAP_Handle capHandle);
	CAP_setCapContinuous(myCap);

	//2. stopWrap
	//void CAP_setStopWrap(CAP_Handle capHandle, const CAP_Stop_Wrap_e stopWrap);
	CAP_setStopWrap(myCap, CAP_Stop_Wrap_CEVT3);

	//3. Event Polarity
	//void CAP_setCapEvtPolarity(CAP_Handle capHandle, const CAP_Event_e event, const CAP_Polarity_e polarity);
	CAP_setCapEvtPolarity(myCap, CAP_Event_1, CAP_Polarity_Rising);
	CAP_setCapEvtPolarity(myCap, CAP_Event_2, CAP_Polarity_Falling);
	CAP_setCapEvtPolarity(myCap, CAP_Event_2, CAP_Polarity_Rising);

	//4. Event Reset
	//void CAP_setCapEvtReset(CAP_Handle capHandle, const CAP_Event_e event, const CAP_Reset_e reset);
	CAP_setCapEvtReset(myCap, CAP_Event_1, CAP_Reset_Enable);
	CAP_setCapEvtReset(myCap, CAP_Event_2, CAP_Reset_Disable);
	CAP_setCapEvtReset(myCap, CAP_Event_3, CAP_Reset_Enable);

	//5. enable Capture Load
	//void CAP_enableCaptureLoad(CAP_Handle capHandle);
	CAP_enableCaptureLoad(myCap);


}
//
// (2.3) module Event configure
//! \brief     myCap Event configure
//! \param[in] None
//! \param[out] None
void myCap_eventConfigure(void)
{
    //1. interrupt source configure
 	//void CAP_enableTimestampCounter(CAP_Handle capHandle);
	CAP_enableTimestampCounter(myCap);      //捕获计数器开始计数

}
//


// **************************************************************************
// the API functions



// end of file
