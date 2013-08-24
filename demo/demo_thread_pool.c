/*
 * Copyright (C) 2013 CN-LAB.  All rights reserved.
 * demo_thread.c
 * Original Author: linfushou@gmail.com, 2013-08-23
 * �̳߳�ʹ��ʾ��
 * ˵��: 
 * History
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>

#define FAIL -1
#define SUCC 0
/*
*�̳߳����������к͵ȴ���������һ��CThread_worker
*�������������������������һ������ṹ
*/
typedef struct worker
{
    /*�ص���������������ʱ����ô˺�����ע��Ҳ��������������ʽ*/
    void *(*process) (void *arg);
    void *arg;/*�ص������Ĳ���*/
    struct worker *next;

} CThread_worker;


/*�̳߳ؽṹ*/
typedef struct
{
     pthread_mutex_t queue_lock;
     pthread_cond_t queue_ready;

    /*����ṹ���̳߳������еȴ�����*/
     CThread_worker *queue_head;

    /*�Ƿ������̳߳�*/
    int shutdown;
    pthread_t *threadid;
    /*�̳߳�������Ļ�߳���Ŀ*/
    int max_thread_num;
    /*��ǰ�ȴ����е�������Ŀ*/
    int cur_queue_size;

} CThread_pool;

int pool_add_worker (void *(*process) (void *arg), void *arg);
void *thread_routine (void *arg);

static CThread_pool *pool = NULL;

/* ��ʼ���̳߳أ�����������߳����� */
void pool_init (int max_thread_num)
{
    int i;

    pool = (CThread_pool *) malloc (sizeof (CThread_pool));
    if (pool == NULL) {
        return;
    }

    pthread_mutex_init (&(pool->queue_lock), NULL);
    pthread_cond_init (&(pool->queue_ready), NULL);

    pool->queue_head = NULL;

    pool->max_thread_num = max_thread_num;
    pool->cur_queue_size = 0;

    pool->shutdown = 0;

    pool->threadid = (pthread_t *) malloc (max_thread_num * sizeof (pthread_t));
    if (pool->threadid == NULL) {
        return;
    }

    /* ����������������߳� */
    for (i = 0; i < max_thread_num; i++) {
        pthread_create (&(pool->threadid[i]), NULL, thread_routine, NULL);
    }
}


/*���̳߳��м�������*/
int pool_add_worker (void *(*process) (void *arg), void *arg)
{
    CThread_worker *member;

    /*����һ��������*/
    CThread_worker *newworker = (CThread_worker *) malloc (sizeof (CThread_worker));
    if (newworker == NULL) {
        return FAIL;
    }
    newworker->process = process;
    newworker->arg = arg;
    newworker->next = NULL;/*�����ÿ�*/

    pthread_mutex_lock (&(pool->queue_lock));
    /*��������뵽�ȴ�������*/
    member = pool->queue_head;
    if (member != NULL) {
        while (member->next != NULL) {
             member = member->next;
        }
        /* �ҵ��ȴ�����������һ��λ�� */
        member->next = newworker;
    } else {
        pool->queue_head = newworker;
    }

    assert (pool->queue_head != NULL);

    pool->cur_queue_size++;
    pthread_mutex_unlock (&(pool->queue_lock));
    /*���ˣ��ȴ��������������ˣ�����һ���ȴ��̣߳�
    ע����������̶߳���æµ�����û���κ�����*/
    pthread_cond_signal(&(pool->queue_ready));
    
    return SUCC;
}


/*�����̳߳أ��ȴ������е����񲻻��ٱ�ִ�У������������е��̻߳�һֱ
����������������˳�*/
int pool_destroy ()
{
    if (pool->shutdown)
        return -1;/*��ֹ���ε���*/
     pool->shutdown = 1;

    /*�������еȴ��̣߳��̳߳�Ҫ������*/
     pthread_cond_broadcast (&(pool->queue_ready));

    /*�����ȴ��߳��˳�������ͳɽ�ʬ��*/
    int i;
    for (i = 0; i < pool->max_thread_num; i++)
         pthread_join (pool->threadid[i], NULL);
     free (pool->threadid);

    /*���ٵȴ�����*/
     CThread_worker *head = NULL;
    while (pool->queue_head != NULL)
     {
         head = pool->queue_head;
         pool->queue_head = pool->queue_head->next;
         free (head);
     }
    /*���������ͻ�����Ҳ����������*/
     pthread_mutex_destroy(&(pool->queue_lock));
     pthread_cond_destroy(&(pool->queue_ready));
    
     free (pool);
    /*���ٺ�ָ���ÿ��Ǹ���ϰ��*/
     pool=NULL;
    return 0;
}


void *thread_routine(void *arg)
{
    /* ��ʼ���������߳� */
    printf ("Starting thread %u\n", (uint)pthread_self());
    while (1) {
        pthread_mutex_lock (&(pool->queue_lock));
        /*����ȴ�����Ϊ0���Ҳ������̳߳أ���������״̬; ע��
        pthread_cond_wait��һ��ԭ�Ӳ������ȴ�ǰ����������Ѻ�����*/
        while (pool->cur_queue_size == 0 && !pool->shutdown)
        {
            printf("Thread %u is waiting for task\n", (uint)pthread_self ());
            pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));
        }

        /*�̳߳�Ҫ������*/
        if (pool->shutdown)
        {
            /*����break,continue,return����ת��䣬ǧ��Ҫ�����Ƚ���*/
            pthread_mutex_unlock (&(pool->queue_lock));
            printf ("Thread %u will exit\n", (uint)pthread_self ());
            pthread_exit (NULL);
        }

        printf ("Thread %u is starting to work\n", (uint)pthread_self ());

        /*assert�ǵ��Եĺð���*/
        assert (pool->cur_queue_size != 0);
        assert (pool->queue_head != NULL);

        /*�ȴ����г��ȼ�ȥ1����ȡ�������е�ͷԪ��*/
        pool->cur_queue_size--;
        printf("Reeeeeeeeeeeeeeemain task num = %d\n", pool->cur_queue_size);
        CThread_worker *worker = pool->queue_head;
        pool->queue_head = worker->next;
        pthread_mutex_unlock (&(pool->queue_lock));

        /* XXX ���ûص�������ִ������ */
        (*(worker->process)) (worker->arg);
        
        free (worker);
        worker = NULL;
    }
    /*��һ��Ӧ���ǲ��ɴ��*/
    pthread_exit (NULL);
}

void *myprocess(void *arg)
{
    int taskid;

    taskid = *(int *)arg;
    printf ("XXX Thread ID is %u, working on task %d\n", (uint)pthread_self (),*(int *) arg);
    switch (taskid) {
    case 0:
    case 1:
    case 2:
    case 3:
        printf("AAA\n");
        break;
    case 4:
        printf("BBB\n");
        break;
    default:
        printf("CCC\n");
        break;
    }
    
    sleep (6);/*��Ϣһ�룬�ӳ������ִ��ʱ��*/
    printf ("XXX Thread ID is %u, end of task %d\n", (uint)pthread_self (),*(int *) arg);
    return NULL;
}

int main (int argc, char **argv)
{
    int *workingnum, i;

    pool_init (3);/*�̳߳������������߳�*/
    /*���������Ͷ��10������*/
    workingnum = (int *) malloc (sizeof (int) * 10);
    
    for (i = 0; i < 10; i++)
    {
        workingnum[i] = i;
        /* ���̳߳���Ͷ��������д����˴���������ͬ��Ϊmyprocess */
        pool_add_worker(myprocess, &workingnum[i]);
    }
    
    /*�ȴ������������*/
    sleep (1);
    /*�����̳߳�*/
    pool_destroy ();

    free (workingnum);
    return 0;
}

