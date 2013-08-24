/*
 * Copyright (C) 2013 CN-LAB.  All rights reserved.
 * demo_coop.c
 * Original Author: linfushou@gmail.com, 2013-08-23
 * C�������������ʾ��
 * ˵��: ��person�ṹ���໯
 * ��Ϊ���������ƣ�ȫ������Ҳ��������������������Ա���������Ե�����Ϊһ����
 * ��������: ��Ա����ͷ������ͳ�Ƶ�
 * �෽����: ��ʼ��������������Ա��ɾ������Ա����ӡ������Ա��Ϣ���ͷ�������Ա��Ϣ��
 * ������  : demo_person_master_t
 * History
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "demo_coop_person.h" /* person�ṹ������ */

struct list_head head;

static void demo_init_global_data(void)
{
    INIT_LIST_HEAD(&head);
}

static void demo_print_all_person(void)
{
    demo_person_t *pos;

    printf("All persons' information:\n");
    list_for_each_entry(pos, &head, list) {
        printf("---------------------\n");
        pos->demo_print_info(pos);
    }
}

static void demo_recv_person(void)
{
    demo_person_t *person;
    char buffer[DEMO_STR_LEN];
    
    person = (demo_person_t *)malloc(sizeof(demo_person_t));
    if (person == NULL) {
        printf("Malloc failed\n");
        return;
    }

    demo_init_person_strc(person);
    
    printf("Input new person information (name, age, city), name/city <= 20 characters\n");
    printf("name : ");
    memset(buffer, 0, DEMO_STR_LEN);
    scanf("%s", buffer);
    person->demo_set_name(person, buffer);
    printf("age  : ");
    scanf("%d", &person->age);
    printf("city : ");
    memset(buffer, 0, DEMO_STR_LEN);
    scanf("%s", buffer);    
    person->demo_set_city(person, buffer);

    person->demo_add_person(person);
}

static void demo_free_all(void)
{
    demo_person_t *pos, *n;

    list_for_each_entry_safe(pos, n, &head, list) {
        list_del(&pos->list);
        free(pos);
    }
}

int main()
{
    char c;

    demo_init_global_data();
    
    do {
        printf("CMD> ");
next_input: 
        scanf("%c", &c); 
        switch (c) {
        case 'a':
            /* ������person */
            /* ������person��Ϣ */
            demo_recv_person();
            /* ����\n�ַ��Ĵ��� */
            goto next_input;
        case 's':
            /* ��ʾ��������Ϣ */
            /* ��ӡ����person����Ϣ */
            demo_print_all_person();
            goto next_input;
        case '\n':
        case 'q':
            break;
        default:
            printf("Unknown command\n");
            break;
        }
    } while (c != 'q');
    
    return 0;
}

