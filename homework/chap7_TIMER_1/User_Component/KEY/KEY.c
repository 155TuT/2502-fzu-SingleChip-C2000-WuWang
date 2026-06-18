//#############################################################################
//
//! \file:  "User_Component/KEY/KEY.c"
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
//#############################################################################

// **************************************************************************
// the includes
#include "User_Component/KEY/KEY.h"


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions
// (1) module Initial
//! \brief     KEY module initial
//! \param[in] None
//! \param[out] None
void KEY_initial(void)
{

}
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     KEY Pin configure
//! \param[in] None
//! \param[out] None
void KEY_pinConfigure(void)
{
	// 1. set mode
	//void GPIO_setMode(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Mode_e mode);
	GPIO_setMode(KEY_obj, KEY1, GPIO_12_Mode_GeneralPurpose);

	// 2. set pullup
	//void GPIO_setPullUp(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_PullUp_e pullUp);
	GPIO_setPullUp(KEY_obj, KEY1, GPIO_PullUp_Disable);

	//3. set direction
	//void GPIO_setDirection(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Direction_e direction);
	GPIO_setDirection(KEY_obj, KEY1, GPIO_Direction_Input);

	//4. set Qualification
	//void GPIO_setQualification(GPIO_Handle gpioHandle, const GPIO_Number_e gpioNumber, const GPIO_Qual_e qualification);
	//void GPIO_setQualificationPeriod(GPIO_Handle gpioHandle, const GPIO_Number_e gpioNumber, const uint16_t period);
	GPIO_setQualification(KEY_obj, KEY1, GPIO_Qual_ASync);

}
//
// (2.2) module function configure
//! \brief     KEY function configure
//! \param[in] None
//! \param[out] None
void KEY_functionConfigure(void)
{

}
//
// (2.3) module Event configure
//! \brief     KEY Event configure
//! \param[in] None
//! \param[out] None
void KEY_eventConfigure(void)
{
	//	void PIE_setExtIntPolarity(PIE_Handle pieHandle,
	//	                           const CPU_ExtIntNumber_e intNumber,
	//	                           const PIE_ExtIntPolarity_e polarity);
	PIE_setExtIntPolarity(myPie, CPU_ExtIntNumber_1, PIE_ExtIntPolarity_FallingEdge);

	//	void GPIO_setExtInt(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const CPU_ExtIntNumber_e intNumber);
	GPIO_setExtInt(KEY_obj, KEY1, CPU_ExtIntNumber_1);

}

                      
// **************************************************************************
// the API functions



// end of file
