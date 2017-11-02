/*************************************************************************
	> File Name: CLMsgLoopManagerForMyqueue.cpp
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 三 11/ 1 21:21:34 2017
 ************************************************************************/

#include "CLMsgLoopManagerForMyqueue.h"
#include "CLMessageQueueByMyqueue.h"
#include <string.h>
#include "CLExecutiveNameServer.h"
#include "CLLogger.h"
#include "CLThreadCommunicationByMyqueue.h"

CLMsgLoopManagerForMyqueue::CLMsgLoopManagerForMyqueue(CLMessageObserver *pMsgObserver,const char *pstrThreadName):CLMessageLoopManager(pMsgObserver)
{
	if((pstrThreadName == 0) || (strlen(pstrThreadName) == 0))
	throw "In CLMsgLoopManagerForSTLqueue::CLMsgLoopManagerForSTLqueue(), pstrThreadName error";

m_strThreadName = pstrThreadName;

m_pMsgQueue = new CLMessageQueueByMyqueue;
}
CLMsgLoopManagerForMyqueue::~CLMsgLoopManagerForMyqueue()
{
	
}

CLStatus CLMsgLoopManagerForMyqueue::Initialize()
{
	CLExecutiveNameServer *pNameServer = CLExecutiveNameServer::GetInstance();
	if(pNameServer == 0)
	{
		delete m_pMsgQueue;
		m_pMsgQueue = 0;
		CLLogger::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Initialize(), CLExecutiveNameServer::GetInstance error", 0);
		return CLStatus(-1, 0);
	}

	CLStatus s = pNameServer->Register(m_strThreadName.c_str(), new CLThreadCommunicationByMyqueue(m_pMsgQueue));
	if(!s.IsSuccess())
	{
		delete m_pMsgQueue;
		m_pMsgQueue = 0;
		CLLogger::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Initialize(), pNameServer->Register error", 0);
		return CLStatus(-1, 0);
	}
	else
	{
		return CLStatus(0, 0);
	}
}
CLStatus CLMsgLoopManagerForMyqueue::Uninitialize()
{
	CLExecutiveNameServer *pNameServer = CLExecutiveNameServer::GetInstance();
	if(pNameServer == 0)
	{
		CLLogger::WriteLogMsg("In CLMsgLoopManagerForMsgQueue::Uninitialize(), CLExecutiveNameServer::GetInstance error", 0);
		return CLStatus(-1, 0);
	}
	return pNameServer->ReleaseCommunicationPtr(m_strThreadName.c_str());
}
CLMessage* CLMsgLoopManagerForMyqueue::WaitForMessage()
{
	return m_pMsgQueue->GetMessage();
}