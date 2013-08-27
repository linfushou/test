/*
 * Copyright (C) 2013 CN-LAB.  All rights reserved.
 * demo_tcpip_server.c
 * Original Author: linfushou@gmail.com, 2013-08-27
 * 基于TCP/IP协议的C/S系统 - 服务端程序
 * 说明: 
 * History
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <sys/syscall.h>  

#define gettid()        syscall(__NR_gettid)

#define INET_ADDR_STR_LEN   1024
#define MAX_LINE 10000

static int demo_thread_num;

typedef struct demo_thread_arg_s {
    struct sockaddr_in cin;
    int c_fd;
} demo_thread_arg_t;

static void demo_hd_clientmsg(void *arg)
{
    int n, c_fd;
    char buf[MAX_LINE];
    char addr_p[INET_ADDR_STR_LEN];
    demo_thread_arg_t *targ;
    struct sockaddr_in cin;

    printf("Create new thread, tid = %d, total thread num = %d.\n", (int)gettid(), demo_thread_num);

    targ = (demo_thread_arg_t *)arg;
    c_fd = targ->c_fd;
    cin = targ->cin;
    /* 接收消息 */
    while (1) {
        memset(buf, 0, sizeof(buf));
        n = recv(c_fd , buf, MAX_LINE, 0);
        if(n == -1)
        {
            perror("fail to recv");
            //exit(1);
            break;
        }
        else if(n == 0)
        {
            printf("the connect has been closed\n");
            break;
        }
        inet_ntop(AF_INET,&cin.sin_addr,addr_p,sizeof(addr_p));
        printf("Thread(%d) recv content %s\n", (int)gettid(), buf);
        
        n = strlen(buf);
        sprintf(buf, "%d", n);
        
        /* 处理完返回字符串长度信息 */
        n = send(c_fd , buf, strlen(buf) + 1 , 0);
        if( n == -1)
        {
            perror("fail to send");
            exit(1);
        }
    }
    /* 关闭socket */
    if(close(c_fd) == -1)
    {
        perror("fail to close");
        exit(1);
    }

    demo_thread_num--;
    printf("Destory thread, tid = %d, total thread num = %d.\n", (int)gettid(), demo_thread_num);
    /* 调用该函数回收线程相关的资源 */
    pthread_detach(pthread_self());
}

int main(int argc,char **argv)
{
    struct sockaddr_in sin;
    struct sockaddr_in cin;
    int l_fd, c_fd;
    socklen_t len;    
    int port = 8000;
    int n, ret;
    pthread_t tid;
    demo_thread_arg_t targ;
    
    bzero(&sin , sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    printf("Server process with PID = %d.\n", (int)gettid());

    /* 创建socket */
    if((l_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("fail to create socket");
        exit(1);
    }
    /* 将socket描述符绑定到本机地址上 */
    if(bind(l_fd,(struct sockaddr *)&sin ,sizeof(sin) ) == -1) 
    {
        perror("fail to bind");
        exit(1);
    }

    /* 开启侦听服务 */
    if(listen(l_fd,10) == -1)
    {
        perror("fail to listen");
        exit(1);
    }
    printf("waiting.....\n");
    while(1)
    {
        /* 接受客户端连接 c_fd是针对接受的客户端的描述符 */
        if((c_fd = accept(l_fd,(struct sockaddr *)&cin, &len)) == -1)
        {
            perror("fail to accept");
            exit(1);
        }

        memset(&targ, 0, sizeof(demo_thread_arg_t));
        targ.cin = cin;
        targ.c_fd = c_fd;
        /* 创建一个新线程 */
        ret = pthread_create(&tid, NULL, (void *)demo_hd_clientmsg, &targ);
        if(ret != 0){
            printf("Create pthread error!\n");
            return -1;
        }
        demo_thread_num++;
    }
    if(close(l_fd) == -1)
    {
        perror("fail to close");
        exit(1);
    }
    return 0;
}

