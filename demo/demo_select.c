/*
 * Copyright (C) 2013 CN-LAB.  All rights reserved.
 * demo_select.c
 * Original Author: linfushou@gmail.com, 2013-08-27
 * select机制使用示例
 * 说明: 
 * History
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    /* Wait up to five seconds. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(1, &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

    if (retval == -1)
        perror("select()");
    else if (retval)
        printf("Data is available now.\n");
        /* FD_ISSET(0, &rfds) will be true. */
        if (FD_ISSET(0, &rfds)) {
            printf("Data from stdin (fd 0).\n");
        }
    else
        printf("No data within five seconds.\n");

    exit(EXIT_SUCCESS);
}

