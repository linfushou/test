/*
 * Copyright (C) 2013 CN-LAB.  All rights reserved.
 * demo_coop_person.c
 * Original Author: linfushou@gmail.com, 2013-08-23
 * C语言面向对象编程示例
 * 说明: 这是person结构体的方法定义
 * History
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "demo_coop_person.h"

void demo_set_name(demo_person_t *this, char *name)
{
    if (!this) {
        return;
    }
    strcpy(this->name, name);
}

void demo_set_age(demo_person_t *this, int age)
{
    if (!this) {
        return;
    }
    this->age = age;
}

void demo_set_city(demo_person_t *this, char *name)
{
    if (!this) {
        return;
    }
    strcpy(this->city, name);
}

void demo_print_info(demo_person_t *this)
{
    if (!this) {
        printf("This is NULL\n");
    }

    printf("name: %s\n", this->name);
    printf("age : %d\n", this->age);
    printf("city: %s\n", this->city);    
}

void demo_add_person(demo_person_t *this)
{
    demo_person_t *pos;
    bool is_max;
    
    if (!this) {
        printf("This is NULL\n");
    }

    is_max = true;
    list_for_each_entry(pos, &head, list) {
        if (this->age <= pos->age) {
            /* 加在pos前面 */
            list_add_tail(&this->list, &pos->list);
            is_max = false;
            break;
        }
    }

    /* 不是最大age直接返回，因为前面已加入链表 */
    if (is_max == false) {
        return;
    }
  
    list_add_tail(&this->list, &head);
}

void demo_init_person_strc(demo_person_t *person)
{
    if (!person) {
        return;
    }

    memset(person, 0, sizeof(demo_person_t));
    INIT_LIST_HEAD(&person->list);
    person->demo_print_info = demo_print_info;
    person->demo_set_name = demo_set_name;    
    person->demo_set_age = demo_set_age;
    person->demo_set_city = demo_set_city;
    person->demo_add_person = demo_add_person;
}

