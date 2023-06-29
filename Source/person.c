#include "../include/person.h"
#include "../include/sentinel.h"

void 
print_company(company_t *temp)
{
    if(temp == NULL)
        return NULL;
    printf("Company name: %s\n",temp->company_name);
    printf("Employee number: %d\n",temp->emp_no);
    if(temp->CEO == NULL)
        printf("Company CEO: Nil\n");
    return;
}
void
print_person(person_t *temp)
{
    int i;
    for(i=0;i<4;i++)
        printf("vehical_no %d= %d\n",i+1, temp->vehical_no[i]);
    printf("Name: %s\n",temp->name);
    printf("Age: %d\n",temp->age);
    printf("Height: %d\n",*(temp->height));
    
    printf("salaries of last five months: ");
    for(i=0; i<5; i++)
    {
        if(temp->last_salary_amount[i] != NULL)
            printf("%u ",*(temp->last_salary_amount[i]));
        else
            printf("Nil ");
    }
    printf("\n");
    printf("================\n");
    printf("Current company details:\n");
    print_company(&(temp->curr_company));
    printf("================\n");
    printf("Previous company details:\n");
    for(i=0; i<3; i++)
    {
        print_company(&(temp->prev_company[i]));

    }
    printf("================\n");
    if(temp->manager == NULL)
        printf("Manager: Nil\n");
    return;
}
void
Serialize_company_t(company_t *obj, ser_buff_t *buffer)
{
    SENTINEL_INSERTION_CODE(obj, buffer);
    int i;
    unsigned int sentinel = 0xFFFFFFFF;
    
    Serialize_data(buffer, (char *)obj->company_name, sizeof(char)*30);
    Serialize_data(buffer, (char *)&obj->emp_no, sizeof(int));
    /* Check if CEO field is NULL
     * if it is then insert the sentinel code */
    if(obj->CEO == NULL)
    {
        Serialize_data(buffer, (char *)&sentinel, sizeof(unsigned int));
    }
    else
        Serialize_person_t(obj->CEO, buffer);
    
    return;
}
void
Serialize_person_t(person_t *obj, ser_buff_t *buffer)
{
    /* Check If the passed arg obj is NULL
     * if it is then insert the sentinel code */
    SENTINEL_INSERTION_CODE (obj, buffer);
    int i;
    unsigned int sentinel = 0xFFFFFFFF;
    for (i=0; i<4; i++)
        Serialize_data(buffer,(char *)&obj->vehical_no[i],sizeof(unsigned int));

    Serialize_data(buffer, (char *)obj->name, sizeof(char)*30);
    Serialize_data(buffer, (char *)&obj->age, sizeof(int));
    /* If height is NULL, insert the sentinel number */
    if(obj->height == NULL)
        Serialize_data(buffer, (char *)&sentinel, sizeof(unsigned int));
    else
        Serialize_data(buffer, (char *)&obj->height, sizeof(unsigned int));
    /* check If last_salary_amount is NULL
     * if it is then insert the sentinel code */
    if(obj->last_salary_amount == NULL)
        Serialize_data(buffer, (char *)&sentinel, sizeof(unsigned int));
    else
        for(i=0; i<5; i++)
            Serialize_data(buffer, (char *)&obj->last_salary_amount[i], sizeof(unsigned int));
    
    Serialize_company_t(&obj->curr_company,buffer);
    
    for(i=0;i<3;i++)
        Serialize_company_t(&obj->prev_company[i], buffer);
    if(obj->manager == NULL)
        Serialize_data(buffer, (char *)&sentinel, sizeof(person_t));
    else
        Serialize_person_t(obj->manager, buffer);
    
    printf("Data serialized successfully\n");
    return;
}
company_t*
Deserialize_company_t(ser_buff_t *buffer)
{
    /* check If first 4 bytes are sentinel code
     * if it is then return NULL; otherwise
     * rewind the next pointer to 4 bytes */
    printf("Deserialize company\n");
    SENTINEL_DETECTION_CODE(buffer);
    company_t *obj;
    unsigned int sentinel = 0;
    obj = calloc(1,sizeof(company_t));
    De_serialize_data((char *)obj->company_name, buffer, 30*sizeof(char));
    De_serialize_data((char *)&obj->emp_no, buffer, sizeof(int));
    printf("Deserializing CEO\n");
    De_serialize_data((char *)&sentinel, buffer, sizeof(unsigned int));
    printf("In deserialize company sentinel %x\n",sentinel);
    if(sentinel == 0xFFFFFFFF)
    {
        printf("CEO is sentinel\n");
        obj->CEO = NULL;
    }
    else
    {
        skip_serialize_data(buffer, -1*sizeof(unsigned int));
        obj->CEO = calloc(1, sizeof(person_t));
        obj->CEO = Deserialize_person_t(buffer);
    }
    return obj;
}
person_t*
Deserialize_person_t(ser_buff_t *buffer)
{
    printf("Deserialize person\n");
    SENTINEL_DETECTION_CODE(buffer);
    person_t *obj;
    obj = calloc(1, sizeof(person_t));
    int i;
    unsigned int sentinel=0;
    for(i=0;i<4;i++)
    {
        De_serialize_data((char *)&obj->vehical_no[i], buffer, sizeof(unsigned int));
    }
    De_serialize_data((char *)obj->name, buffer, 30*sizeof(char));
    printf("Name: %s\n",obj->name);
    De_serialize_data((char *)&obj->age, buffer, sizeof(int));
    printf("Age: %d\n",obj->age);
    /* check if the next read is sentinel, 
     * If it is then height is NULL, assign it.
     * other wise allocate memory and re-read */
    De_serialize_data((char *)&sentinel, buffer, sizeof(unsigned int));
    if(sentinel == 0xFFFFFFFF)
        obj->height = NULL;
    else
    {
        skip_serialize_data(buffer, -1*sizeof(unsigned int));
        obj->height = calloc(1, sizeof(unsigned int));
        De_serialize_data((char *)obj->height, buffer,sizeof(unsigned int));
    }
    printf("Height: %d\n",obj->height);
    printf("Deserialize last salary\n");
    for(i=0;i<5;i++)
    {
        De_serialize_data((char *)&sentinel, buffer, sizeof(unsigned int));
        if(sentinel == 0xFFFFFFFF)
            obj->last_salary_amount[i] = NULL;
        else
        {
            skip_serialize_data(buffer, -1*sizeof(unsigned int));
            obj->last_salary_amount[i] = calloc(1, sizeof(unsigned int));
            De_serialize_data((char *)obj->last_salary_amount[i], buffer,sizeof(unsigned int));
        }
    }
    company_t *temp;
    printf("Deserialize curr company\n");
    temp = Deserialize_company_t(buffer);
    obj->curr_company = *temp; /* Shallow copy from temp */
    free(temp); /* shallow free the temp */
    printf("Deserialize prev_company\n");

    for(i=0;i<3;i++)
    {
        temp = Deserialize_company_t(buffer);
        obj->prev_company[i] = *temp; /* shallow copy */
        free(temp); /*shallow free */
    }
    
    De_serialize_data((char *)&sentinel, buffer, sizeof(unsigned int));
    if(sentinel == 0xFFFFFFFF)
        obj->manager = NULL;
    else
    {
        skip_serialize_data(buffer, -1*sizeof(unsigned int));
        obj->manager = Deserialize_person_t(buffer);
    }
    return obj;
}
