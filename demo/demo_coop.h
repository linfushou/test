/*
 * Copyright (C) 2013 CN-LAB.  All rights reserved.
 * demo_coop.h
 * Original Author: linfushou@gmail.com, 2013-08-23
 * C语言面向对象编程示例
 * 说明: 这是person结构体的声明
 * History
 */

#define DEMO_STR_LEN 20

typedef struct demo_person_s {
    char name[DEMO_STR_LEN];
    int age;
    char city[DEMO_STR_LEN];

    /* 结构体中的函数指针，定义结构体变量后进行初始化时将回调函数挂上 */
    void (* demo_set_name)(struct demo_person_s *this, char *name);
    void (* demo_set_age)(struct demo_person_s *this, int age);
    void (* demo_set_city)(struct demo_person_s *this, char *city);
    /* 打印所有成员名称 */
    void (* demo_print_info)(struct demo_person_s *this);
} demo_person_t;

void demo_set_name(demo_person_t *this, char *name);
void demo_set_age(demo_person_t *this, int age);
void demo_set_city(demo_person_t *this, char *name);  
void demo_init_person_strc(demo_person_t *person);

