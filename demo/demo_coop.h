/*
 * Copyright (C) 2013 CN-LAB.  All rights reserved.
 * demo_coop.h
 * Original Author: linfushou@gmail.com, 2013-08-23
 * C�������������ʾ��
 * ˵��: ����person�ṹ�������
 * History
 */

#define DEMO_STR_LEN 20

typedef struct demo_person_s {
    char name[DEMO_STR_LEN];
    int age;
    char city[DEMO_STR_LEN];

    /* �ṹ���еĺ���ָ�룬����ṹ���������г�ʼ��ʱ���ص��������� */
    void (* demo_set_name)(struct demo_person_s *this, char *name);
    void (* demo_set_age)(struct demo_person_s *this, int age);
    void (* demo_set_city)(struct demo_person_s *this, char *city);
    /* ��ӡ���г�Ա���� */
    void (* demo_print_info)(struct demo_person_s *this);
} demo_person_t;

void demo_set_name(demo_person_t *this, char *name);
void demo_set_age(demo_person_t *this, int age);
void demo_set_city(demo_person_t *this, char *name);  
void demo_init_person_strc(demo_person_t *person);

