//#############################################################################
//
//! \file:  "Application/isr.h"
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


#ifndef _ISR_H_
#define _ISR_H_


// **************************************************************************
// the includes
#include <stdint.h>

// driver
#include "User_Component/User_Device.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifndef TARGET_GLOBAL
   #define TARGET_EXT 	extern
#else
   #define TARGET_EXT
#endif


// isr API
//! \brief     isr
//! \param[in] None
//! \param[out] None
//TARGET_EXT interrupt void isr(void);

//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
TARGET_EXT interrupt void KEY_XINT1_isr(void);     //redefined in Isr.h

//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
TARGET_EXT interrupt void myTimer_CpuTimer0_isr(void);     //redefined in Isr.h

//! \brief     Interrupt Service Routine
//! \param[in] None
//! \param[out] None
TARGET_EXT interrupt void myCap_CAPINT_isr(void);     //redefined in Isr.h

#ifdef __cplusplus
}
#endif // extern "C"


#endif  // end of _EXAMPLE_H_ definition

