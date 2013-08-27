/*
 * Copyright (C) 2013 CN-LAB.  All rights reserved.
 * demo_tcpip_server.c
 * Original Author: linfushou@gmail.com, 2013-08-27
 * 基于TCP/IP协议的C/S系统 - 客户端程序
 * 说明: 
 * History
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define MAX_LINE 100

int main(int argc,char **argv)
{
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int s_fd;
    int port = 8000;
    char *str = "test string";
    char *sendstr = NULL;
    char *serverIP = "127.0.0.1";
    int n;
    if(argc > 1)
    {
        str = argv[1];
    }

    bzero(&sin , sizeof(sin));

    sin.sin_family = AF_INET;
    inet_pton(AF_INET,serverIP,(void *)&sin.sin_addr);
    sin.sin_port = htons(port);

    if((s_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("fail to create socket");
        exit(1);
    }
    if(connect(s_fd,(struct sockaddr *)&sin,sizeof(sin)) == -1)
    {
        perror("fail to create socket");
        exit(1);
    }

    /* 发送测试字符串 */
    sendstr = (argv[1] == NULL ? str : argv[1]);
    while (1) {
        memset(buf, 0, sizeof(buf));
        printf("CMD> ");
        scanf("%s", buf);
        sendstr = buf;
        n = send(s_fd, sendstr , strlen(sendstr) + 1, 0);
        if(n == -1)
        {
            perror("fail to send");
            exit(1);
        }

        memset(buf, 0, sizeof(buf));
        n = recv(s_fd, buf , MAX_LINE, 0);
        if(n == -1)
        {
            perror("fail to recv");
            exit(1);
        }
        printf("the length of str = %s\n" , buf);
    }
    if(close(s_fd) == -1)
    {
        perror("fail to close");
        exit(1);
    }    
    
    return 0;
}

