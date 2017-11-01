/*************************************************************************
	> File Name: CLMessageQueueByMyqueue.cpp
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 三 11/ 1 20:15:11 2017
 ************************************************************************/

#include "CLMessageQueueByMyqueue.h"
#include "CLCriticalSection.h"
#include "CLMessage.h"
#include "CLLogger.h"

CLMessageQueueByMyqueue::CLMessageQueueByMyqueue():m_Event(true)
{

}

CLMessageQueueByMyqueue::~CLMessageQueueByMyqueue()
{

}
CLStatus CLMessageQueueByMyqueue::PushMessage(CLMessage* pMessage)
{
	if(pMessage == NULL)
		return CLStatus(-1,0);
	CLStatus s = m_MessageQueue.push(pMessage);
	if(!s.IsSuccess())
	{
		CLLogger::WriteLogMsg("In CLMessageQueue::PushMessage(),Push error",0);
		return CLStatus(-1,0);
	}
	CLStatus s2 = m_Event.Set();
	if(!s2.IsSuccess())
	{
		CLLogger::WriteLogMsg("In CLMessageQueue::PushMessage(),Push error",0);
		return CLStatus(-1,0);
	}
	return CLStatus(0,0);
}

CLMessage* CLMessageQueueByMyqueue::GetMessage()
{
	CLStatus s = m_Event.Wait();
	return m_MessageQueue.pop();
}