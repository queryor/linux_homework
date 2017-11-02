/*************************************************************************
	> File Name: MyQueue.h
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 三 11/ 1 17:23:13 2017
 ************************************************************************/

#ifndef ARRAY_QUEUE_HXX
#define ARRAY_QUEUE_HXX

#include <iostream>
#include "CLMutex.h"
#include "CLEvent.h"
#include "CLLogger.h"
#include "CLCriticalSection.h"
using namespace std;
#define QUEUESIZE 50
template<class T> class MyQueue{
    public:
        MyQueue();
        ~MyQueue();

        CLStatus push(T t);
        T front();
        T pop();
        int size();
        bool is_empty();
        bool is_Full();

    private:
        T array[QUEUESIZE];
        int m_nfront;
        int m_nrear;
        CLMutex m_Mutex;
        
};

template<class T>
MyQueue<T>::MyQueue() 
{
    m_nrear = 0;
    m_nfront = 0;
}

template<class T>
MyQueue<T>::~MyQueue() 
{
    
}

//入队
template<class T>
CLStatus MyQueue<T>::push(T t) 
{
    if(!is_Full())
    {
        CLCriticalSection cs(&m_Mutex);
        array[m_nrear] = t;
        m_nrear = (m_nrear + 1)%QUEUESIZE;
        return CLStatus(0,0);
    }
    else
    {
        cerr<<"queue is full"<<endl;
        CLLogger::WriteLogMsg("In MyQueue::Push",0);
        return CLStatus(-1,0);        
    }
}

//返回”队列“首元素
template<class T>
T MyQueue<T>::front() 
{
    CLCriticalSection cs(&m_Mutex);
    return array[m_nfront];
}

//弹出“队列“首元素，出队
template<class T>
T MyQueue<T>::pop() 
{
    if(!is_empty())
    {
        CLCriticalSection cs(&m_Mutex);
        int tmp = m_nfront;
        m_nfront = (m_nfront + 1)%QUEUESIZE;
        return array[tmp];
    }
    else
    {
        cerr << "queue is empty"<<endl;
        return (T)-1;
    }
}

// 返回“队列”的大小
template<class T>
int MyQueue<T>::size() 
{
    return (m_nrear - m_nfront + QUEUESIZE) % QUEUESIZE;
}

// 返回“队列”是否为空
template<class T>
bool MyQueue<T>::is_empty()
{
    return m_nrear == m_nfront;
}

//返回“队列”是否满了
template<class T>
bool MyQueue<T>::is_Full()
{
    return (m_nrear + 1) % QUEUESIZE == m_nfront;
}

#endif