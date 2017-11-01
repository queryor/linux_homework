/*************************************************************************
	> File Name: CLMsgLoopManagerForMyqueue.cpp
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 三 11/ 1 21:21:34 2017
 ************************************************************************/

#include "CLMsgLoopManagerForMyqueue.h"
#include "CLMessageQueueByMyqueue.h"

CLMsgLoopManagerForMyqueue::CLMsgLoopManagerForMyqueue(CLMessageObserver *pMsgObserver,CLMessageQueueByMyqueue *pMsgQueue):CLMessageLoopManager(pMsgObserver)
{
	if(pMsgQueue == 0)
		throw "In CLMsgLoopManagerForMyqueue::CLMsgloopManagerForMyqueue(),pMsgQueue error";
	m_pMsgQueue = pMsgQueue;
}
CLMsgLoopManagerForMyqueue::~CLMsgLoopManagerForMyqueue()
{
	delete m_pMsgQueue;
}

CLStatus CLMsgLoopManagerForMyqueue::Initialize()
{
	return CLStatus(0,0);
}
CLStatus CLMsgLoopManagerForMyqueue::Uninitialize()
{
	return CLStatus(0,0);
}
CLMessage* CLMsgLoopManagerForMyqueue::WaitForMessage()
{
	return m_pMsgQueue->GetMessage();
}