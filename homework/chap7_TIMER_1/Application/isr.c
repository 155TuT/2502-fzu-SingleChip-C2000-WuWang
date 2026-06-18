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
    LED_off(LED[LED_Count]);
    LED_Count++;
     if(LED_Count >=4)
        {
           LED_Count=0;
        }
     LED_on(LED[LED_Count]);

	PIE_clearInt(myPie, PIE_GroupNumber_1);

}


interrupt void myTimer_CpuTimer0_isr1(void)     //流水灯
{

	    LED_off(LED[LED_Count]);
	    LED_Count++;
	     if(LED_Count >=4)
	    	{
	    	   LED_Count=0;
	    	}
	     LED_on(LED[LED_Count]);

	PIE_clearInt(myPie, PIE_GroupNumber_1);
}
// end of file
