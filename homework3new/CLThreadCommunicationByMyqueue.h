#ifndef CLThreadCommunicationByMyqueue_H
#define CLThreadCommunicationByMyqueue_H

#include "CLExecutiveCommunication.h"
#include "CLStatus.h"

class CLMessage;
class CLMessageQueueByMyqueue;

class CLThreadCommunicationByMyqueue : public CLExecutiveCommunication
{
public:
	/*
	pMsgQueue����Ӷ��з��䣬�Ҳ�����ʾ����delete
	*/
	CLThreadCommunicationByMyqueue(CLMessageQueueByMyqueue *pMsgQueue);
	virtual ~CLThreadCommunicationByMyqueue();

	virtual CLStatus PostExecutiveMessage(CLMessage *pMessage);

private:
	CLThreadCommunicationByMyqueue(const CLThreadCommunicationByMyqueue&);
	CLThreadCommunicationByMyqueue& operator=(const CLThreadCommunicationByMyqueue&);

private:
	CLMessageQueueByMyqueue* m_pMsgQueue;
};

#endif