/*************************************************************************
	> File Name: CLMessageQueueByMYqueue.h
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 三 11/ 1 20:05:23 2017
 ************************************************************************/

#ifndef _CLMESSAGEQUEUEBYMYQUEUE_H
#define _CLMESSAGEQUEUEBYMYQUEUE_H

#include "CLStatus.h"
#include "CLMutex.h"
#include "CLEvent.h"
#include "MyQueue.h"
#include "CLMessage.h"

class CLMessageQueueByMyqueue
{
public:
	CLMessageQueueByMyqueue();
	virtual ~CLMessageQueueByMyqueue();
	CLStatus PushMessage(CLMessage *pMessage);
	CLMessage *GetMessage();
private:
	MyQueue<CLMessage* >m_MessageQueue;
	CLEvent m_Event;
};
#endif
