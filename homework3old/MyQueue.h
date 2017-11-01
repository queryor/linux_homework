/*************************************************************************
	> File Name: MyQueue.h
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 三 11/ 1 17:12:02 2017
 ************************************************************************/

#ifndef _MYQUEUE_H
#define _MYQUEUE_H

#include<iostream>
using namespace std;
#define QUEUESIZE 20 //实际队中元素个数是 QUEUESIZE-1
template <typename T>
class MyQueue{
private:
    int front;//队头
    int rear;//队尾
    T array[QUEUESIZE];
public:
    MyQueue()
    {
        rear = 0;
        front = 0;
    }
    bool empty(){ return rear == front; }
    bool isFull(){ return (rear + 1) % QUEUESIZE == front; }
    int size(){ return (rear - front + QUEUESIZE) % QUEUESIZE; }
    void push(T&t);//入队
    T pop();//出队
};

template <typename T>
void MyQueue<T>::push(T&t)
{
    if (!isFull())
    {
        cout << t << "is enqueue" << endl;
        array[rear]=t;
        rear = (rear + 1) % QUEUESIZE;
    }
    else
    {
        cerr << "queue is full" << endl;
    }
}
template <typename T>
T MyQueue<T>::pop()
{
    if (!empty())
    {
        int tmp = front;
        front = (front + 1) % QUEUESIZE;
        return array[tmp];
    }
    else
    {
        cerr << "queue is empty" << endl;
        return -1;
    }
}


#endif
