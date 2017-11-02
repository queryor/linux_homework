#include "CLThreadCommunicationByMyqueue.h"
#include "CLMessageQueueByMyqueue.h"
#include "CLMessage.h"

CLThreadCommunicationByMyqueue::CLThreadCommunicationByMyqueue(CLMessageQueueByMyqueue*pMsgQueue)
{
	if(pMsgQueue == 0)
		throw "In CLThreadCommunicationByMyqueue::CLThreadCommunicationByMyqueue(), pMsgQueue error";
	
	m_pMsgQueue = pMsgQueue;
}

CLThreadCommunicationByMyqueue::~CLThreadCommunicationByMyqueue()
{
	delete m_pMsgQueue;
}

CLStatus CLThreadCommunicationByMyqueue::PostExecutiveMessage(CLMessage *pMessage)
{
	if(pMessage == NULL)
		return CLStatus(-1, 0);
	
	return m_pMsgQueue->PushMessage(pMessage);
}