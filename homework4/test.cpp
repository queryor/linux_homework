#include <iostream>
#include <string>
#include <unistd.h>
#include "CLThread.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLMessageObserver.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLExecutiveNameServer.h"
#include "CLExecutiveCommunication.h"
#include "CLThreadForMsgLoop.h"
#include "CLNonThreadForMsgLoop.h"
#include <time.h>

using namespace std;

#define ADD_MSG 0
#define QUIT_MSG 1
#define ADD_RESULT 2
#define NumCompThread 20

string ComputerName[]={"Cp0","Cp1","Cp2","Cp3","Cp4","Cp5",
"Cp6","Cp7","Cp8","Cp9","Cp10","Cp11","Cp12","Cp13","Cp14",
"Cp15","Cp16","Cp17","Cp18","Cp19","Cp20"
};
class CLAddMessage : public CLMessage
{
public: 
	friend class CLComputeObserver;
	friend class CLManagerObserver;
	CLAddMessage(int Op1,int Op2):CLMessage(ADD_MSG)
	{
		m_Op1 = Op1;
		m_Op2 = Op2;
	}
	virtual ~CLAddMessage()
	{

	}
	CLAddMessage(const CLAddMessage& C):CLMessage(ADD_MSG)
	{
		m_Op1 = C.m_Op1;
		m_Op2 = C.m_Op2;
	}
	CLAddMessage&operator=(const CLAddMessage &C)
	{
		if(this==&C)
		{
			return *this;
		}
		this->m_Op1 = C.m_Op1;
		this->m_Op2 = C.m_Op2;
		return *this;
	}
private:
	int m_Op1;
	int m_Op2;
};
class CLAddResultMessage : public CLMessage
{
private:
	int m_Result;
	int m_Op1;
	int m_Op2;
public:
	friend class CLClientObserver;
	CLAddResultMessage(int result,int Op1,int Op2):CLMessage(ADD_RESULT)
	{
		m_Result = result;
		m_Op1 = Op1;
		m_Op2 = Op2;
	}
	
	virtual ~CLAddResultMessage()
	{
		 
	}
};



class CLComputeObserver : public CLMessageObserver
{
public:
	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop,void* pContext)
	{
		//cout<<"计算类初始化函数"<<endl;
		pMessageLoop->Register(ADD_MSG,(CallBackForMessageLoop)(&CLComputeObserver::On_AddMsg));
		pMessageLoop->Register(QUIT_MSG,(CallBackForMessageLoop)(&CLComputeObserver::On_QuitMsg));
		//CLExecutiveNameServer::PostExecutiveMessage("Manager", new CLAddMessage(1,2));
		return CLStatus(0,0);
	}
	CLStatus On_AddMsg(CLMessage *pM)
	{
		//cout<<"Copmute::ON_AddMsg"<<endl;
		CLAddMessage* pAddMsg = (CLAddMessage *)pM;
		int temp =  pAddMsg->m_Op1 + pAddMsg->m_Op2;
		//new CLAddResultMessage r(temp);
		//cout<<temp<<endl;
		CLExecutiveNameServer::PostExecutiveMessage("Client",new CLAddResultMessage(temp,pAddMsg->m_Op1,pAddMsg->m_Op2));

		return CLStatus(0,0);
	}
	CLStatus On_QuitMsg()
	{
		return CLStatus(QUIT_MESSAGE_LOOP,0);	
	}
};

class CLManagerObserver : public CLMessageObserver
{
public:
	
	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop,void* pContext)
	{
		pMessageLoop->Register(ADD_MSG,(CallBackForMessageLoop)(&CLManagerObserver::On_AddMsg));
		pMessageLoop->Register(QUIT_MSG,(CallBackForMessageLoop)(&CLManagerObserver::On_QuitMsg));
		m_iter=0;
        //cout<<"进入管理类"<<endl
		//创建计算线程
		for(int i=0;i<NumCompThread;i++)
		{
			m_List[i] = (char*)ComputerName[i].data();
			m_pTcomp = new CLThreadForMsgLoop(new CLComputeObserver,m_List[i],true);
			m_pTcomp->Run(0);
		}
		

		return CLStatus(0,0);
	}
	CLStatus On_AddMsg(CLMessage *pM)
	{	
		CLAddMessage* pAddMsg = (CLAddMessage*)pM;
		CLAddMessage* pN = new CLAddMessage(*pAddMsg);
		CLExecutiveNameServer::PostExecutiveMessage(m_List[0],pN);
		m_iter = (m_iter + 1)/NumCompThread;
		return CLStatus(0,0);
	}
	CLStatus On_QuitMsg(CLMessage *pM)
	{
		for(int i=0;i<NumCompThread;i++)
			CLExecutiveNameServer::PostExecutiveMessage(m_List[i],new CLMessage(QUIT_MSG));
		return CLStatus(QUIT_MESSAGE_LOOP,0);
	}
private:
	CLThreadForMsgLoop* m_pTcomp;
	char* m_List[NumCompThread];
	int m_iter;
};

class CLClientObserver : public CLMessageObserver
{
private:
	clock_t m_start;
public:
	virtual CLStatus Initialize(CLMessageLoopManager* pMessageLoop,void* pContext)
	{
		m_start = clock();
		pMessageLoop->Register(ADD_RESULT,(CallBackForMessageLoop)(&CLClientObserver::On_AddResult));
		for(int i =0; i<2000;i++)
		{
			//cout<<i<<":"<<endl;
			CLExecutiveNameServer::PostExecutiveMessage("Manager",new CLAddMessage(1,i));
		}
				return CLStatus(0,0);
	}
	CLStatus On_AddResult(CLMessage *pM)
	{
		CLAddResultMessage* pAddResult = (CLAddResultMessage *)pM;
		cout<<"Client:"<<pAddResult->m_Op1<<"+"<<pAddResult->m_Op2<<"="<<pAddResult->m_Result<<endl;
		if(pAddResult->m_Op2 == 1999)
			{
				CLExecutiveNameServer::PostExecutiveMessage("Manager",new CLMessage(QUIT_MSG));
				clock_t t = clock();
				double totaltime = (double)(t-m_start)/CLOCKS_PER_SEC;
				cout<<"totaltime:"<<totaltime<<endl;
				return CLStatus(QUIT_MESSAGE_LOOP,0);
			}
		return CLStatus(0,0);

	}
};



int main()
{
	//创建线程池管理线程
	CLThreadForMsgLoop m(new CLManagerObserver, "Manager",true);
	m.Run(0);

	//创建线程池客户线程
	CLThreadForMsgLoop client(new CLClientObserver,"Client",true);
	client.Run(0);
	return 0;
}
