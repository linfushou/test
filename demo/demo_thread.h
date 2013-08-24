/*
 * Copyright (C) 2013 CN-LAB.  All rights reserved.
 * demo_thread.c
 * Original Author: linfushou@gmail.com, 2013-08-23
 * 多线程程序头文件
 * History
 */

#ifndef _DEMO_THREAD_H_
#define _DEMO_THREAD_H_

#include <sys/syscall.h>  

#define gettid()        syscall(__NR_gettid)
#define DEMO_SLEEP_TIME 0
#define DEMO_LOOP_CNT   3

#endif /* _DEMO_THREAD_H_ */

