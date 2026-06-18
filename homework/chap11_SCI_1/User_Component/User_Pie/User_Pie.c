//#############################################################################
//
//! \file:  "User_Component/User_Pie/User_Pie.c"
//! \brief: PIE function configuration
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
#include "User_Component/User_Pie/User_Pie.h"


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions

// (1) module Initial
//! \brief     User_Pie module initial
//! \param[in] None
//! \param[out] None
void User_Pie_initial(void)
{

}
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     User_Pie Pin configure
//! \param[in] None
//! \param[out] None
void User_Pie_pinConfigure(void)
{

}
//
// (2.2) module function configure
//! \brief     User_Pie function configure
//! \param[in] None
//! \param[out] None
void User_Pie_functionConfigure(void)
{
	//1. register PIE vector
//	void PIE_registerPieIntHandler(PIE_Handle pieHandle,
//	                           const PIE_GroupNumber_e groupNumber,
//	                           const PIE_SubGroupNumber_e subGroupNumber,
//	                           const intVec_t vector);
	//1.1 TIMER0
	//PIE_registerPieIntHandler(myPie, PIE_GroupNumber_1, PIE_SubGroupNumber_7, (intVec_t) &myTimer_CpuTimer0_isr);
	//1.2 XINT1
	//PIE_registerPieIntHandler(myPie, PIE_GroupNumber_1, PIE_SubGroupNumber_4, (intVec_t) &KEY_XINT1_isr);
    //1.3 EPWM1INT
	//PIE_registerPieIntHandler(myPie, PIE_GroupNumber_3, PIE_SubGroupNumber_1, (intVec_t) &LED_EPWM1_isr);
	//1.4 ECAPINT
	//PIE_registerPieIntHandler(myPie, PIE_GroupNumber_4, PIE_SubGroupNumber_1, (intVec_t) & myCap_CAPINT_isr);
	//1.5 ADCINT
	PIE_registerPieIntHandler(myPie, PIE_GroupNumber_10, PIE_SubGroupNumber_1, (intVec_t) &myAdc_ADCINT_isr);
    //1.6 SCI TXINT
	PIE_registerPieIntHandler(myPie, PIE_GroupNumber_9, PIE_SubGroupNumber_2, (intVec_t) &mySCI_TXINT_isr);
    //1.7 SCI RXINT
	PIE_registerPieIntHandler(myPie, PIE_GroupNumber_9, PIE_SubGroupNumber_1, (intVec_t) &mySCI_RXINT_isr);
}
//
// (2.3) module Event configure
//! \brief     User_Pie Event configure
//! \param[in] None
//! \param[out] None
void User_Pie_eventConfigure(void)
{
 	//1. module IE
	//1.1 TIMER0
	//void TIMER_enableInt(TIMER_Handle timerHandle);
	//TIMER_enableInt(myTimer0);
	//1.2 XINT1
	//	void PIE_enableExtInt(PIE_Handle pieHandle,const CPU_ExtIntNumber_e intNumber);
	//PIE_enableExtInt(myPie, CPU_ExtIntNumber_1);
	//1.3 EPWM1INT
	//PWM_enableInt(myPwm1);
	//1.4 ECAPINT
	//void CAP_enableInt(CAP_Handle capHandle, const CAP_Int_Type_e intType);
	//CAP_enableInt(myCap, CAP_Int_Type_CEVT2);
	//1.5 ADCINT
	ADC_enableInt(myAdc, ADC_IntNumber_1);
	//1.6 SCI TXINT
	//void SCI_enableTxInt(SCI_Handle sciHandle);
	SCI_enableTxInt(mySci);
	//1.7 SCI RXINT
	//void SCI_enableRxInt(SCI_Handle sciHandle);
	SCI_enableRxInt(mySci);

	//2. PIE PIEIERx.y
	//void PIE_enableInt(PIE_Handle pieHandle, const PIE_GroupNumber_e group, const PIE_InterruptSource_e intSource);
	//2.1 TIMER0
	//PIE_enableInt(myPie, PIE_GroupNumber_1, PIE_InterruptSource_TIMER_0);
    //2.2 XINT1
	//PIE_enableInt(myPie, PIE_GroupNumber_1, PIE_InterruptSource_XINT_1);
	//2.3 EPWM1INT
	//PIE_enableInt(myPie, PIE_GroupNumber_3, PIE_InterruptSource_EPWM1);
    //2.4 CAPINT
	//PIE_enableInt(myPie, PIE_GroupNumber_4, PIE_InterruptSource_ECAP1);
    //2.5 ADCINT
	PIE_enableInt(myPie, PIE_GroupNumber_10, PIE_InterruptSource_ADCINT_10_1);
    //2.6 SCI TXINT
	//PIE_enableInt(myPie, PIE_GroupNumber_9, PIE_InterruptSource_SCIATX);
    //2.7 SCI RXINT
	//PIE_enableInt(myPie, PIE_GroupNumber_9, PIE_InterruptSource_SCIARX);

	//3. CPU IERx
	//void CPU_enableInt(CPU_Handle cpuHandle,const CPU_IntNumber_e intNumber);
	//CPU_enableInt(myCpu, CPU_IntNumber_1);
	//CPU_enableInt(myCpu, CPU_IntNumber_3);
	//CPU_enableInt(myCpu, CPU_IntNumber_4);
	CPU_enableInt(myCpu, CPU_IntNumber_10);
	CPU_enableInt(myCpu, CPU_IntNumber_9);

}


// **************************************************************************
// the API functions




// end of file
