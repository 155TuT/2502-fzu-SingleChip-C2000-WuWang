//#############################################################################
//
//! \file:  "User_Component/myCap/myCap.c"
//! \brief: CAP function configuration
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
#include "User_Component/myCap/myCap.h"


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
	CAP_setStopWrap(myCap, CAP_Stop_Wrap_CEVT2);

	//3. Event Polarity
	//void CAP_setCapEvtPolarity(CAP_Handle capHandle, const CAP_Event_e event, const CAP_Polarity_e polarity);
	CAP_setCapEvtPolarity(myCap, CAP_Event_1, CAP_Polarity_Rising);
	CAP_setCapEvtPolarity(myCap, CAP_Event_2, CAP_Polarity_Falling);

	//4. Event Reset
	//void CAP_setCapEvtReset(CAP_Handle capHandle, const CAP_Event_e event, const CAP_Reset_e reset);
	CAP_setCapEvtReset(myCap, CAP_Event_1, CAP_Reset_Enable);
	CAP_setCapEvtReset(myCap, CAP_Event_2, CAP_Reset_Disable);

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
	CAP_enableTimestampCounter(myCap);

}
//

                      
// **************************************************************************
// the API functions



// end of file
