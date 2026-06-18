//#############################################################################
//
//! \file  Application\Isr.c
//!
//! \brief  Interrupt Service Routine
//
//  Author:
//  Group:          C2000
//  Target Device:  TMS320F2802x
//
//  (C) Copyright 2014, Texas Instruments, Inc.
//#############################################################################
// $TI Release: f2802x Support Library v210 $
// $Release Date: Mon Sep 17 09:13:31 CDT 2012 $
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

interrupt void myCap_CAPINT_isr(void)
{

    myCapVal1 = CAP_getCap2(myCap);
    myCapVal2=  CAP_getCap3(myCap);
    myCapVal3= myCapVal2 -myCapVal1;


    CAP_clearInt(myCap,CAP_Int_Type_CEVT3);

    CAP_clearInt(myCap, CAP_Int_Type_Global);

    // Acknowledge this interrupt to get more from group 1
    PIE_clearInt(myPie, PIE_GroupNumber_4);
}




interrupt void KEY_XINT1_isr(void)
{


    if(key_counter>=3)  key_counter=0;
    key_counter++;
    key_status=key_counter;
    // Acknowledge this interrupt to get more from group 1
    PIE_clearInt(myPie, PIE_GroupNumber_1);

}


interrupt void myTimer_CpuTimer0_isr(void)     //Á÷Ë®µÆ
{


    LED_TIME_DELAY_COUNTER++;
    key_two_msecond++;
//        LED_off(LED[LED_Count]);
//        LED_Count++;
//         if(LED_Count >=4)
//            {
//               LED_Count=0;
//            }
//         LED_on(LED[LED_Count]);

    PIE_clearInt(myPie, PIE_GroupNumber_1);
}







// end of file
