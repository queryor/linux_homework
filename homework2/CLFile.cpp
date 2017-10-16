/*************************************************************************
	> File Name: CLFile.cpp
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 四 10/ 5 21:31:37 2017
 ************************************************************************/

#include<iostream>
using namespace std;
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<stdio.h>
#include<cstdarg>
#include"CLFile.h"

#define BUFFER_SIZE_FILE_READ 2 
#define BUFFER_SIZE_FILE_WRITE 4

CLFile::CLFile()
{
    m_pread_buffer = new char[BUFFER_SIZE_FILE_READ];
    m_pwrite_buffer = new char[BUFFER_SIZE_FILE_WRITE];
    m_nUsedBytesForReadBuffer = 0;
    m_nUsedBytesForWriteBuffer = 0;
    m_Fd = 0;
}

CLFile::~CLFile()
{
    FlushWriteBuff();
    FlushReadBuff();
    delete m_pwrite_buffer;
    delete m_pread_buffer;
}

CLStatus CLFile::file_open(const char *pathname,int flags,...)
{
    va_list arg_ptr;
    va_start(arg_ptr,flags);
    int den = va_arg(arg_ptr,int);
    va_end(arg_ptr);
    m_bAPPEND = flags&O_APPEND;
    m_Fd = open(pathname,flags,den);
    if(-1 == m_Fd){
        return CLStatus(-1,errno);
    }
    m_nSizeofReadBuffer = read(m_Fd,m_pread_buffer,BUFFER_SIZE_FILE_READ);
    return CLStatus(0,0);
}
int CLFile::file_close()
{
    if(m_Fd!=-1){
        FlushWriteBuff();
        FlushReadBuff();
        close(m_Fd);
    }
    return 0;
}
off_t CLFile::file_lseek(off_t offset,int whence)
{
   if(m_bAPPEND != 1)
    {
        FlushWriteBuff();
    }
    FlushReadBuff();
    off_t r = lseek(m_Fd,offset,whence);
    m_nSizeofReadBuffer = read(m_Fd,m_pread_buffer,BUFFER_SIZE_FILE_READ);
    return r;
}
ssize_t CLFile::file_write(const char *buf,size_t count)
{
    if(buf == 0)
        return -1;
    if(count == 0)
        return -1;
    if(m_pwrite_buffer == 0)
        return -1;
    if(strlen(buf) == 0)
        return -1;
    unsigned int nleftroom = BUFFER_SIZE_FILE_WRITE - m_nUsedBytesForWriteBuffer;
    unsigned int len_buf = count;
    if(len_buf > BUFFER_SIZE_FILE_WRITE)
    {
        if(m_Fd == -1)
            return -1;
        lseek(m_Fd,(int)m_nUsedBytesForReadBuffer-(int)m_nSizeofReadBuffer,SEEK_CUR);
        FlushWriteBuff();
        ssize_t r = write(m_Fd,buf,count);
        return r;
    }
    if(len_buf > nleftroom)
    {     
        lseek(m_Fd,(int)m_nUsedBytesForReadBuffer-(int)m_nSizeofReadBuffer,SEEK_CUR);
        CLStatus s = FlushWriteBuff();
        if(s.IsSuccess())
            return -1;
    }
    memcpy(m_pwrite_buffer + m_nUsedBytesForWriteBuffer,buf,count);
    m_nUsedBytesForWriteBuffer += count;
    return count;
}

ssize_t CLFile::file_read(void *buf,size_t count)
{

    if(m_nUsedBytesForWriteBuffer != 0)
    {
        lseek(m_Fd,(int)m_nUsedBytesForReadBuffer-(int)m_nSizeofReadBuffer,SEEK_CUR);
        FlushWriteBuff();
        FlushReadBuff();
        m_nSizeofReadBuffer = read(m_Fd,m_pread_buffer,BUFFER_SIZE_FILE_READ);
    }
    signed int nleftroom = m_nSizeofReadBuffer - m_nUsedBytesForReadBuffer;
    if(count <= nleftroom){
        memcpy(buf,m_pread_buffer+m_nUsedBytesForReadBuffer,count);
        m_nUsedBytesForReadBuffer += count;
        return count;
    }
    lseek(m_Fd,-(int)nleftroom,SEEK_CUR);
    ssize_t r = read(m_Fd,buf,count);
    m_nSizeofReadBuffer = read(m_Fd,m_pread_buffer,BUFFER_SIZE_FILE_READ);
    m_nUsedBytesForReadBuffer = 0;
    return r;    
}

CLStatus CLFile::FlushWriteBuff()
{
    if(m_Fd == -1)
        return CLStatus(-1,0);
    
    if(m_pwrite_buffer == 0)
        return CLStatus(-1,0);
    if(m_nUsedBytesForWriteBuffer == 0)
        return CLStatus(0,0);
    ssize_t r = write(m_Fd,m_pwrite_buffer,m_nUsedBytesForWriteBuffer);
    if(r == -1)
        return CLStatus(-1,errno);
    m_nUsedBytesForWriteBuffer = 0;

    return CLStatus(0,0);
}

CLStatus CLFile::FlushReadBuff()
{
    m_nUsedBytesForReadBuffer = 0;
    return CLStatus(0,0);
}
