/*************************************************************************
	> File Name: CLFile.h
	> Author:qyj 
	> Mail:qyj_1994@126.com 
	> Created Time: 四 10/ 5 20:23:06 2017
 ************************************************************************/

#ifndef _CLFILE_H
#define _CLFILE_H

#include"CLStatus.h"
/**
*文件操作的封装类
*
* * */
class CLFile 
{
public:
    CLFile();
    ~CLFile();
    CLStatus file_open(const char *pathname,int flags,...);
    off_t file_lseek(off_t offset,int whence);
    ssize_t file_write(const char *buf,size_t count);
    ssize_t file_read(void *buf,size_t count);
    int file_close();
    CLStatus FlushWriteBuff();
    CLStatus FlushReadBuff();
private:
    int m_Fd;
    char *m_pread_buffer;
    char *m_pwrite_buffer;
    unsigned int m_nUsedBytesForWriteBuffer;
    unsigned int m_nUsedBytesForReadBuffer;
    unsigned int m_nSizeofReadBuffer;
    bool m_bAPPEND;
};


#endif
