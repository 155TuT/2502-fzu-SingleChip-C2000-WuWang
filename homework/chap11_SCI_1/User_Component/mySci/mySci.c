//#############################################################################
//
//! \file    User_Component/mySci/mySci.c
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

// **************************************************************************
// the includes
#include "User_Component/mySci/mySci.h"


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions


// **************************************************************************
// the API functions

//! \brief     mySCI module initial
//! \param[in] None
//! \param[out] None
void mySCI_initial(void)
{
	SciSendPoint = 0;
	SciReceivePoint = 0;
	SciSentOk = false;
	SciReceivedOk =false;
	//SciReceivedOk =true;
}
//
//! \brief     mySCI Pin configure
//! \param[in] None
//! \param[out] None
void mySCI_pinConfigure(void)
{
	// 1. set mode
	GPIO_setMode(myGpio, GPIO_Number_28, GPIO_28_Mode_SCIRXDA);
	GPIO_setMode(myGpio, GPIO_Number_29, GPIO_29_Mode_SCITXDA);
	// 2. set pullup
	GPIO_setPullUp(myGpio, GPIO_Number_28, GPIO_PullUp_Enable);
	GPIO_setPullUp(myGpio, GPIO_Number_29, GPIO_PullUp_Disable);
	//3. set direction
	GPIO_setDirection(myGpio, GPIO_Number_28, GPIO_Direction_Input);
	GPIO_setDirection(myGpio, GPIO_Number_29, GPIO_Direction_Output);
	//4 input qualification
	GPIO_setQualification(myGpio, GPIO_Number_28, GPIO_Qual_ASync);
}
//
//! \brief     EXAMPLE function configure
//! \param[in] None
//! \param[out] None
void mySCI_functionConfigure(void)
{
    //1. SCI BRR = LSPCLK/(SCI BAUDx8) - 1
    SCI_setBaudRate(mySci, SCI_BaudRate_9_6_kBaud);
    //2. configure package(1 stop bit,  No loopback, No parity,8 char bits, async mode, idle-line protocol)
    SCI_disableParity(mySci);
    SCI_setNumStopBits(mySci, SCI_NumStopBits_One);
    SCI_setCharLength(mySci, SCI_CharLength_8_Bits);
    //3. enable SCI TX&RX
    SCI_enableTx(mySci);
    SCI_enableRx(mySci);
    //4. enable SCI module
    SCI_enable(mySci);
}

//! \brief     mySCI Event configure
//! \param[in] None
//! \param[out] None
void mySCI_eventConfigure(void)
{

}

//! \brief     mySCIFIFO module initial
//! \param[in] None
//! \param[out] None
void mySCIFIFO_initial(void)
{

}
//! \brief     mySCIFIFO Pin configure
//! \param[in] None
//! \param[out] None
void mySCIFIFO_pinConfigure(void)
{
}
//
//! \brief     mySCIFIFO function configure
//! \param[in] None
//! \param[out] None
void mySCIFIFO_functionConfigure(void)
{
    //1. configure the SCI TX&RX FIFO
    //1.1 enable FIFO
    SCI_enableFifoEnh(mySci);
    //1.2 reset TX&RX
    SCI_resetChannels(mySci);
    //1.3 configure TX FIFO
    SCI_resetTxFifo(mySci);
    //1.4 configure RX FIFO
    SCI_resetRxFifo(mySci);
    //1.5 delay
    SCI_setTxDelay(mySci, 10);
}

//! \brief     mySCIFIFO Event configure
//! \param[in] None
//! \param[out] None
void mySCIFIFO_eventConfigure(void)
{
	//1. TX FIFO INT
	//void SCI_setTxFifoIntLevel(SCI_Handle sciHandle,const SCI_FifoLevel_e fifoLevel);
	SCI_setTxFifoIntLevel(mySci, SCI_FifoLevel_Empty);
	//2. RX FIFO INT
	//void SCI_setRxFifoIntLevel(SCI_Handle sciHandle,const SCI_FifoLevel_e fifoLevel);
	SCI_setRxFifoIntLevel(mySci, SCI_FifoLevel_1_Word);
}

//! \brief     Transmit a string from the SCI
//! \param[in] string
//! \param[out] None
void mySCI_sendMessageBlocking(uint16_t * msg)
{
    uint16_t i = 0;

    while('\0' != msg[i])
    {
    	mySCI_sendCharBlocking(msg[i]);
        i++;
    }
}

//! \brief     Receive a char from the SCI
//! \param[in] None
//! \param[out] SciReceiveBuf: receive data; <10 data
uint16_t* mySCI_receiveMessageBlocking(void)
{
	SciReceivePoint = 0;
	while( SciReceivePoint <10 )
	{
		SciReceiveBuf[SciReceivePoint]=mySCI_receiveCharBlocking();
		if('\0' == SciReceiveBuf[SciReceivePoint]) break;
		SciReceivePoint += 1;
	}
	return SciReceiveBuf;
}

//! \brief      Transmit a string from the SCI(FIFO)
//! \param[in]  string
//! \param[out] None
void mySCI_Fifo_sendMessageBlocking(uint16_t * msg)
{
    uint16_t i = 0;

    while('\0' != msg[i])
    {
    	mySCI_Fifo_sendCharBlocking(msg[i]);
        i++;
    }
}

//! \brief      Receive a char from the SCI(FIFO)
//! \param[in]  None
//! \param[out] SciReceiveBuf: receive data; <50 data
uint16_t* mySCI_Fifo_receiveMessageBlocking(void)
{   uint16_t ch;
	SciReceivePoint = 0;
	while( SciReceivePoint <50 )
	{   ch = mySCI_Fifo_receiveCharBlocking();
		SciReceiveBuf[SciReceivePoint] = ch;
		if('\0' == ch) break;
		SciReceivePoint += 1;
	}
	return SciReceiveBuf;
}

// end of file
