/*
 * Copyright (C) 2013 CN-LAB.  All rights reserved.
 * demo_coop.c
 * Original Author: linfushou@gmail.com, 2013-08-23
 * C语言面向对象编程示例
 * 说明: 将person结构体类化
 * 作为面向对象设计，全局数据也可以用类来表达，本例中人员管理器可以单独成为一个类
 * 类数据有: 人员链表头，人数统计等
 * 类方法有: 初始化链表，接收新人员，删除新人员，打印所有人员信息，释放所有人员信息等
 * 类名称  : demo_person_master_t
 * History
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "demo_coop_person.h" /* person结构体声明 */

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
            /* 接收新person */
            /* 输入新person信息 */
            demo_recv_person();
            /* 多余\n字符的处理 */
            goto next_input;
        case 's':
            /* 显示所有人信息 */
            /* 打印所有person的信息 */
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

