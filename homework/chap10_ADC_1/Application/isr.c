//#############################################################################
//
//! \file:  "Application/isr.c"
//! \brief: Interrupt program
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
#include "Application/isr.h"


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions

//interrupt void isr(void)
//{
//
//    // Acknowledge this interrupt to get more from group 1
//    PIE_clearInt(myPie, PIE_GroupNumber_1);
//
//}

//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
interrupt void KEY_XINT1_isr(void)     //redefined in Isr.h
{
	LED_toggle(LED1);

	PIE_clearInt(myPie, PIE_GroupNumber_1);

}

//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
interrupt void myTimer_CpuTimer0_isr(void)     //redefined in Isr.h
{
	myCmpA += 1000;
	if(myCmpA>60000) myCmpA = 0;
	PWM_setCmpA(myPwm1, myCmpA);

	PIE_clearInt(myPie, PIE_GroupNumber_1);

}


//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
interrupt void LED_EPWM1_isr(void)     //redefined in Isr.h
{
	myCmpB += 1;
	if(myCmpB >= 60000 ) myCmpB = 0;
	PWM_setCmpA(myPwm1, myCmpB);

	//void PWM_clearIntFlag(PWM_Handle pwmHandle)
	PWM_clearIntFlag(myPwm1);

	// Acknowledge this interrupt to get more from group 1
	PIE_clearInt(myPie, PIE_GroupNumber_3);

}

//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
interrupt void myCap_CAPINT_isr(void)
{
	myCapVal1 = CAP_getCap1(myCap);
	myCapVal2 = CAP_getCap2(myCap);

	//void CAP_clearInt(CAP_Handle capHandle, const CAP_Int_Type_e intType)
	CAP_clearInt(myCap, CAP_Int_Type_CEVT2);
    CAP_clearInt(myCap, CAP_Int_Type_Global);

	// Acknowledge this interrupt to get more from group 1
	PIE_clearInt(myPie, PIE_GroupNumber_4);

}

//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
interrupt void myAdc_ADCINT_isr(void)     //redefined in Isr.h
{
	Temp = ADC_readResult(myAdc, ADC_ResultNumber_0);
	TempC = ADC_getTemperatureC(myAdc, Temp);

	//void ADC_clearIntFlag(ADC_Handle adcHandle, const ADC_IntNumber_e intNumber)
	ADC_clearIntFlag(myAdc, ADC_IntNumber_1);


	// Acknowledge this interrupt to get more from group 1
	PIE_clearInt(myPie, PIE_GroupNumber_10);

}


// end of file
