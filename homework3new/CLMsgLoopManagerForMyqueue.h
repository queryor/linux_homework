/*************************************************************************
	> File Name: CLMsgLoopManagerForMyqueue.h
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 三 11/ 1 21:17:17 2017
 ************************************************************************/

#ifndef _CLMSGLOOPMANAGERFORMYQUEUE_H
#define _CLMSGLOOPMANAGERFORMYQUEUE_H

#include<string>
#include"CLMessageLoopManager.h"

class CLMessageQueueByMyqueue;

class CLMsgLoopManagerForMyqueue:public CLMessageLoopManager
{
public:
	CLMsgLoopManagerForMyqueue(CLMessageObserver *pMsgObserver,const char* pstrThreadName);
	virtual ~CLMsgLoopManagerForMyqueue();
protected:
	virtual CLStatus Initialize();
	virtual CLStatus Uninitialize();

	virtual CLMessage* WaitForMessage();
private:
	CLMsgLoopManagerForMyqueue(const CLMsgLoopManagerForMyqueue&);
	CLMsgLoopManagerForMyqueue& operator=(const CLMsgLoopManagerForMyqueue&);
private:
	CLMessageQueueByMyqueue *m_pMsgQueue;
	std::string m_strThreadName;
};
#endif
