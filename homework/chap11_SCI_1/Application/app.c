//#############################################################################
//
//! \file:  "Application/app.c"
//! \brief: Application program
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
#include "Application/app.h"


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions

//! \brief     LED Control
//! \param[in] None
//! \param[out] None
void LED_Control(void)
{
	LED_on(LED1);
    Delay(1000000L);
    LED_off(LED1);
    Delay(1000000L);
}

//! \brief     LED Control by KEY
//! \param[in] None
//! \param[out] None
void KEY_LED_Control(void)
{
	if( KEYPRESSED == GetKeyStatus(KEY1))
	{
		LED_on(LED1);
	}
	else
	{
	    LED_off(LED1);
	}
}

//! \brief     SCI Test
//! \param[in] None
//! \param[out] None
void SCI_Test(void)
{   uint16_t ch;
    uint16_t * msg;
    msg =  (uint16_t *)"\r\n Enter a character: \0";
	mySCI_sendMessageBlocking(msg);
	ch = mySCI_receiveCharBlocking(); //waiting for a char
//	ch= 0x30;
	mySCI_sendCharBlocking(ch); //send the received char
}

//! \brief     SCI Test1
//! \param[in] None
//! \param[out] None
void SCI_Test1(void)
{   uint16_t ch;
    while(1)
    {
    	ch = mySCI_receiveCharBlocking(); //waiting for a char
    	mySCI_sendCharBlocking(ch); //send the received char
    }
}

//! \brief     SCI Test
//! \param[in] None
//! \param[out] None
void SCI_ExchageData(void)
{
	uint16_t ch;
	if(true == SciReceivedOk && false == SciSentOk)
	{   SciReceivePoint = 0;
		do
	 	{   ch = SciReceiveBuf[SciReceivePoint];
			SciSendBuf[SciReceivePoint] = ch;
			SciReceivePoint ++;
	 	}while(ch != '\0');

		SciReceivePoint = 0;
		SciReceivedOk = false;
		SciSentOk = true;
		SciSendPoint = 0;
		SCI_putData(mySci, SciSendBuf[0]);
    }

}



// end of file
