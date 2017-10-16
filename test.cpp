/*************************************************************************
	> File Name: test.cpp
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 四 10/ 5 22:54:02 2017
 ************************************************************************/

#include<iostream>
#include"CLFile.h"
#include<fcntl.h>
using namespace std;

int main()
{
    CLFile f;//构造一个文件类
    f.file_open("b.txt",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    char a = 'a';
    char b[10] = "uestc";
    char c[10]="";
    //顺序读
    //读的文件大于缓存大小
    f.file_read(c,5);
    cout<<c<<endl;  
    f.file_read(c,5);
    cout<<c<<endl;
    
    //写操作
    //写的数据大于缓存大小
    f.file_write(b,5);
    //写入的数据小于缓存大小
    f.file_write(&a,1); 
    
    //lseek操作
    //lseek到文件开始
    f.file_lseek(0,SEEK_SET);
    f.file_write(&a,1); 
    //读小于缓存大小的数据
    char c1;
    f.file_read(&c1,1);
    cout<<c1<<endl;
    //关闭文件
    f.file_close();
    return 0;
}

