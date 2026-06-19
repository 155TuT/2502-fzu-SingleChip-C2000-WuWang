//#############################################################################
//
//! \file:  "User_Component/LED_Pwm/LED_Pwm.c"
//! \brief: PWM function configuration
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
#include "User_Component/LED_Pwm/LED_Pwm.h"


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions
// (1) module Initial
//! \brief     LED_PWM module initial
//! \param[in] None
//! \param[out] None
void LED_PWM_initial(void)
{
	myCmpA = 0;
	myCmpB = 100;
	PWM_setCmpA(myPwm1, myCmpA);

}
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     LED_PWM Pin configure
//! \param[in] None
//! \param[out] None
void LED_PWM_pinConfigure(void)
{
	// 1. set mode
	//void GPIO_setMode(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Mode_e mode);
	GPIO_setMode(myGpio, GPIO_Number_0, GPIO_0_Mode_EPWM1A);
	GPIO_setMode(myGpio, GPIO_Number_1, GPIO_1_Mode_EPWM1B);
	GPIO_setMode(myGpio, GPIO_Number_2, GPIO_2_Mode_EPWM2A);
	GPIO_setMode(myGpio, GPIO_Number_3, GPIO_3_Mode_EPWM2B);
	// 2. set pullup
	//void GPIO_setPullUp(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_PullUp_e pullUp);
	GPIO_setPullUp(myGpio, GPIO_Number_0, GPIO_PullUp_Disable);
	GPIO_setPullUp(myGpio, GPIO_Number_1, GPIO_PullUp_Disable);
	GPIO_setPullUp(myGpio, GPIO_Number_2, GPIO_PullUp_Disable);
	GPIO_setPullUp(myGpio, GPIO_Number_3, GPIO_PullUp_Disable);
	//3. set direction
	//void GPIO_setDirection(GPIO_Handle gpioHandle,const GPIO_Number_e gpioNumber,const GPIO_Direction_e direction);
	GPIO_setDirection(myGpio, GPIO_Number_0, GPIO_Direction_Output);
	GPIO_setDirection(myGpio, GPIO_Number_1, GPIO_Direction_Output);
	GPIO_setDirection(myGpio, GPIO_Number_2, GPIO_Direction_Output);
	GPIO_setDirection(myGpio, GPIO_Number_3, GPIO_Direction_Output);

}
//
// (2.2) module function configure
//! \brief     LED_PWM function configure
//! \param[in] None
//! \param[out] None
void LED_PWM_functionConfigure(void)
{
	//1. TBCLKSYNC = 0
	//void CLK_disableTbClockSync(CLK_Handle clkHandle);
	CLK_disableTbClockSync(myClk);
	//2. TBCLK
	//void PWM_setHighSpeedClkDiv(PWM_Handle pwmHandle,const PWM_HspClkDiv_e clkDiv);
	//void PWM_setClkDiv(PWM_Handle pwmHandle,const PWM_ClkDiv_e clkDiv);
	PWM_setHighSpeedClkDiv(myPwm1, PWM_HspClkDiv_by_1);
	PWM_setClkDiv(myPwm1, PWM_ClkDiv_by_1);

	PWM_setHighSpeedClkDiv(myPwm2, PWM_HspClkDiv_by_1);
	PWM_setClkDiv(myPwm2, PWM_ClkDiv_by_1);

	//3. TBCTR
	//void PWM_setCounterMode(PWM_Handle pwmHandle,const PWM_CounterMode_e counterMode);
	PWM_setCounterMode(myPwm1, PWM_CounterMode_Up);
	PWM_setCounterMode(myPwm2, PWM_CounterMode_Up);
	//4. TBPRD
	//void PWM_setPeriod(PWM_Handle pwmHandle,const uint16_t period);
	PWM_setPeriod(myPwm1, PWM_BREATH_PERIOD);
	PWM_setPeriod(myPwm2, 60000);
	//void PWM_setPeriodLoad(PWM_Handle pwmHandle,const PWM_PeriodLoad_e periodLoad);
	PWM_setPeriodLoad(myPwm1, PWM_PeriodLoad_Shadow);
	PWM_setPeriodLoad(myPwm2, PWM_PeriodLoad_Shadow);
	//5. CMPA
	//void PWM_setCmpA(PWM_Handle pwmHandle,const uint16_t pwmData)
//	PWM_setCmpA(myPwm1, 3000);
//	PWM_setCmpA(myPwm2, 60000);
	//void PWM_setShadowMode_CmpA(PWM_Handle pwmHandle,const PWM_ShadowMode_e shadowMode);
	PWM_setShadowMode_CmpA(myPwm1, PWM_ShadowMode_Shadow);
	PWM_setShadowMode_CmpA(myPwm2, PWM_ShadowMode_Shadow);
	//void PWM_setLoadMode_CmpA(PWM_Handle pwmHandle,const PWM_LoadMode_e loadMode);
	PWM_setLoadMode_CmpA(myPwm1, PWM_LoadMode_Zero);
	PWM_setLoadMode_CmpA(myPwm2, PWM_LoadMode_Zero);
	//6. CMPB
	//void PWM_setCmpB(PWM_Handle pwmHandle,const uint16_t pwmData)
	//PWM_setCmpB(myPwm1, 30000);
	//void PWM_setShadowMode_CmpB(PWM_Handle pwmHandle,const PWM_ShadowMode_e shadowMode);
	//PWM_setShadowMode_CmpB(myPwm1, PWM_ShadowMode_Shadow);
	//void PWM_setLoadMode_CmpB(PWM_Handle pwmHandle,const PWM_LoadMode_e loadMode);
	//PWM_setLoadMode_CmpB(myPwm1, PWM_LoadMode_Zero);
	//7. action
	PWM_setActionQual_CntUp_CmpA_PwmA(myPwm1, PWM_ActionQual_Set);
	PWM_setActionQual_Period_PwmA(myPwm1, PWM_ActionQual_Clear);

//	PWM_setActionQual_CntUp_CmpA_PwmB(myPwm1, PWM_ActionQual_Clear);
	PWM_setActionQual_Period_PwmB(myPwm1, PWM_ActionQual_Set);

	PWM_setActionQual_CntUp_CmpA_PwmA(myPwm2, PWM_ActionQual_Set);
//	PWM_setActionQual_Period_PwmA(myPwm2, PWM_ActionQual_Clear);
//	PWM_setActionQual_CntUp_CmpA_PwmB(myPwm2, PWM_ActionQual_Clear);
	PWM_setActionQual_Period_PwmB(myPwm2, PWM_ActionQual_Set);

	//8. TBCLKSYNC = 1
	//void CLK_enableTbClockSync(CLK_Handle clkHandle);
	CLK_enableTbClockSync(myClk);
}
//
// (2.3) module Event configure
//! \brief     LED_PWM Event configure
//! \param[in] None
//! \param[out] None
void LED_PWM_eventConfigure(void)
{
	//void PWM_setIntMode(PWM_Handle pwmHandle,const PWM_IntMode_e intMode);
	//void PWM_setIntPeriod(PWM_Handle pwmHandle,const PWM_IntPeriod_e intPeriod);
	PWM_setIntMode(myPwm1, PWM_IntMode_CounterEqualZero);
	PWM_setIntPeriod(myPwm1, PWM_IntPeriod_FirstEvent);
}

                      
// **************************************************************************
// the API functions



// end of file
