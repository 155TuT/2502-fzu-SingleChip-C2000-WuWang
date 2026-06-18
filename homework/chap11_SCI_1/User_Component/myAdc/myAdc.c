//#############################################################################
//
//! \file:  "User_Component/myAdc/myAdc.c"
//! \brief: myAdc
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
#include "User_Component/myAdc/myAdc.h"


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions
// (1) module Initial
//! \brief     myADC module initial
//! \param[in] None
//! \param[out] None
void myADC_initial(void)
{
	Temp = 0;
	TempC = 0;

}
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     myADC Pin configure
//! \param[in] None
//! \param[out] None
void myADC_pinConfigure(void)
{
    //1. temp sensor
	//void ADC_enableTempSensor(ADC_Handle adcHandle);
	ADC_enableTempSensor(myAdc);

}
//
// (2.2) module function configure
//! \brief     myADC function configure
//! \param[in] None
//! \param[out] None
void myADC_functionConfigure(void)
{
	//1. setup power
	//void ADC_powerUp(ADC_Handle adcHandle);
	//void ADC_enableBandGap(ADC_Handle adcHandle);
	//void ADC_enableRefBuffers(ADC_Handle adcHandle);
	ADC_powerUp(myAdc);
	ADC_enableBandGap(myAdc);
	ADC_enableRefBuffers(myAdc);
	//2. enable ADC
	//void ADC_enable(ADC_Handle adcHandle);
	ADC_enable(myAdc);
	//3. Volt ref
	ADC_setVoltRefSrc(myAdc, ADC_VoltageRefSrc_Int);
    //4. SOC
	//void ADC_setSocChanNumber(ADC_Handle adcHandle, const ADC_SocNumber_e socNumber, const ADC_SocChanNumber_e chanNumber);
	//void ADC_setSocSampleWindow(ADC_Handle adcHandle, const ADC_SocNumber_e socNumber, const ADC_SocSampleWindow_e sampleWindow);
	//void ADC_setSocTrigSrc(ADC_Handle adcHandle, const ADC_SocNumber_e socNumber, const ADC_SocTrigSrc_e trigSrc);
	ADC_setSocChanNumber(myAdc, ADC_SocNumber_0, ADC_SocChanNumber_A5);
	ADC_setSocSampleWindow(myAdc, ADC_SocNumber_0, ADC_SocSampleWindow_7_cycles);
//	ADC_setSocTrigSrc(myAdc, ADC_SocNumber_0, ADC_SocTrigSrc_CpuTimer_0);
	ADC_setSocTrigSrc(myAdc, ADC_SocNumber_0, ADC_SocTrigSrc_EPWM1_ADCSOCA);
}
//
// (2.3) module Event configure
//! \brief     myADC Event configure
//! \param[in] None
//! \param[out] None
void myADC_eventConfigure(void)
{
	ADC_setIntMode(myAdc, ADC_IntNumber_1, ADC_IntMode_ClearFlag);
	ADC_setIntPulseGenMode(myAdc, ADC_IntPulseGenMode_Prior);
	ADC_setIntSrc(myAdc, ADC_IntNumber_1, ADC_IntSrc_EOC0);
}

                      
// **************************************************************************
// the API functions



// end of file
