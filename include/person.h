#ifndef __PERSON__
#define __PERSON__

#include "Serialize.h"

typedef struct _person_ person_t;

typedef struct company {
    char company_name[30];
    int emp_no;
    person_t *CEO;
}company_t;

struct _person_ {
    unsigned int vehical_no[4];
    char name[30];
    int age;
    unsigned int *height;
    unsigned int *last_salary_amount[5];
    company_t curr_company;
    company_t prev_company[3];
    person_t *manager;
};

void print_company(company_t *);
void print_person(person_t *);
void Serialize_company_t(company_t *obj, ser_buff_t *buffer);
void Serialize_person_t(person_t *obj, ser_buff_t *buffer);
company_t* Deserialize_company_t(ser_buff_t *buffer);
person_t* Deserialize_person_t(ser_buff_t *buffer);
#endif
