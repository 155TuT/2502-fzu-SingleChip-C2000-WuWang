
//#############################################################################
//
//! \file    User_Component/mySci/mySci.h
//! \brief   SCI
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


#ifndef _MYSCI_H_
#define _MYSCI_H_


// **************************************************************************
// the includes
#include <stdint.h>

// driver
#include "F2802x_Component/F2802x_Device.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef TARGET_GLOBAL
   #define TARGET_EXT 	extern
#else
   #define TARGET_EXT
#endif


/*------- hardware description of the example module   -------------*/
// For example
// The module derived from GPIO
//#define   EXAMPLE_obj   myGpio          //here myGpio is defined in
                                          //F2802x_Component/F2802x_Device.h
//#define   EXAMPLE_PIN   GPIO_Number_??  //pin
//
// the function prototypes
//
// (1) module Initial
//! \brief     mySCI module initial
//! \param[in] None
//! \param[out] None
TARGET_EXT void mySCI_initial(void);
//
// (2) module Configure
// (2.1) module Pin configure
//! \brief     mySCI Pin configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void mySCI_pinConfigure(void);
//
// (2.2) module function configure
//! \brief     EXAMPLE function configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void mySCI_functionConfigure(void);
//
// (2.3) module Event configure
//! \brief     mySCI Event configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void mySCI_eventConfigure(void);
//

// (3) module Initial
//! \brief     mySCIFIFO module initial
//! \param[in] None
//! \param[out] None
TARGET_EXT void mySCIFIFO_initial(void);
//
// (4) SCIFIFO Configure
// (4.1) module Pin configure
//! \brief     mySCIFIFO Pin configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void mySCIFIFO_pinConfigure(void);
//
// (4.2) module function configure
//! \brief     mySCIFIFO function configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void mySCIFIFO_functionConfigure(void);
//
// (4.3) module Event configure
//! \brief     mySCIFIFO Event configure
//! \param[in] None
//! \param[out] None
TARGET_EXT void mySCIFIFO_eventConfigure(void);
//
/*******************************************/

/*------- API description of the example module   -------------*/
// EXAMPLE API
// the function prototypes
//! \brief     EXAMPLE API
//! \param[in] None
//! \param[out] None
//TARGET_EXT void EXAMPLE_API(void);

//! \brief     Transmit a char from the SCI
//! \param[in] string
//! \param[out] None
TARGET_EXT void inline mySCI_sendCharBlocking(uint16_t ch)
{
	SCI_putDataBlocking(mySci, ch);
}

//! \brief     Transmit a string from the SCI
//! \param[in] string
//! \param[out] None
TARGET_EXT void mySCI_sendMessageBlocking(uint16_t * msg);

//! \brief     Receive a char from the SCI
//! \param[in] None
//! \param[out] receive data
TARGET_EXT uint16_t inline mySCI_receiveCharBlocking(void)
{
	return SCI_getDataBlocking(mySci);
}

//! \brief     Receive a char from the SCI
//! \param[in] None
//! \param[out] SciReceiveBuf: receive data; <50 data
TARGET_EXT uint16_t* mySCI_receiveMessageBlocking(void);


/*-------------SCI  FIFO API -----------------*/
//! \brief     Transmit a char from the SCI(FIFO)
//! \param[in] string
//! \param[out] None
TARGET_EXT void inline mySCI_Fifo_sendCharBlocking(uint16_t ch)
{
	while(SCI_getTxFifoStatus(mySci) == SCI_FifoStatus_4_Words) {}
	SCI_putData(mySci, ch);
}

//! \brief     Receive a char from the SCI(FIFO)
//! \param[in] None
//! \param[out] receive data
TARGET_EXT uint16_t inline mySCI_Fifo_receiveCharBlocking(void)
{
	while(SCI_getRxFifoStatus(mySci) == SCI_FifoStatus_Empty) {}
	return SCI_getData(mySci);
}

//! \brief     Transmit a string from the SCI(FIFO)
//! \param[in] string
//! \param[out] None
TARGET_EXT void mySCI_Fifo_sendMessageBlocking(uint16_t * msg);

//! \brief     Receive a char from the SCI(FIFO)
//! \param[in] None
//! \param[out] SciReceiveBuf: receive data; <10 data
TARGET_EXT uint16_t* mySCI_Fifo_receiveMessageBlocking(void);


/*-------end of API description -------------*/


/*------- Globals of the example module   -------------*/
// the globals

//! \brief Defines the Variable
//TARGET_EXT uint16_t  Example;
TARGET_EXT uint16_t SciSendBuf[50];
TARGET_EXT uint16_t SciSendPoint;
TARGET_EXT uint16_t SciReceiveBuf[50];
TARGET_EXT uint16_t SciReceivePoint;
TARGET_EXT uint16_t SciSentOk;
TARGET_EXT uint16_t SciReceivedOk;

/*------- end of globals  -------------*/


/*------- Defines of the example module   -------------*/
// the defines

//! \brief Defines the constant
//#define EXAMPLE_DEF

/*------- end of defines  -------------*/



#ifdef __cplusplus
}
#endif // extern "C"


#endif  // end of _MYSCI_H_ definition

