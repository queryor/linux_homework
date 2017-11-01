/*************************************************************************
	> File Name: MyQueue.cpp
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 三 11/ 1 16:58:12 2017
 ************************************************************************/

#include<iostream>
#include"MyQueue.h"
using namespace std;

int main()
{
    MyQueue<int> q;
    for (int i = 0; i < 20; i++)
    {
        q.push(i);
    }
    cout << q.size() << endl;
    for (int i = 0; i < 20; i++)
    {
        if (!q.empty())
        {
            int t = q.pop();

            cout << t << "is dequeue" << endl;
        }
    }

    return 0;
}