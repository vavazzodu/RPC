#include "./include/Serialize.h"
#include "./include/person.h"

int main()
{
    person_t p1;
    memset(&p1, 0, sizeof(person_t));
    /* Fill the structure */
    p1.vehical_no[0] = 1000;
    p1.vehical_no[1] = 1001;
    p1.vehical_no[2] = 1002;
    p1.vehical_no[3] = 1003;
    
    strcpy(p1.name, "Karan");
    p1.age = 30;

    p1.height = calloc(1, sizeof(unsigned int));
    *(p1.height) = 160;

    p1.last_salary_amount[0] = calloc(1, sizeof(unsigned int));
    *(p1.last_salary_amount[0]) = 20000;
    p1.last_salary_amount[1] = calloc(1, sizeof(unsigned int));
    *(p1.last_salary_amount[1]) = 18000;
    p1.last_salary_amount[2] = calloc(1, sizeof(unsigned int));
    *(p1.last_salary_amount[2]) = 16000;
    p1.last_salary_amount[3] = NULL;
    p1.last_salary_amount[4] = NULL;

    strcpy(p1.curr_company.company_name, "HPE");
    p1.curr_company.emp_no = 555;
    p1.curr_company.CEO = NULL;

    strcpy(p1.prev_company[0].company_name, "Aricent");
    p1.prev_company[0].emp_no = 100;
    p1.prev_company[0].CEO = NULL;

    p1.manager = NULL;
    
    /* print the structure before serializing it. */
    printf("Person structure before serializing it\n");
    print_person(&p1);
    
    /* Serialization begins */
    ser_buff_t *buf;
    buf = calloc(1, sizeof(ser_buff_t));
    Initialize_ser_buf(&buf);
    Serialize_person_t(&p1, buf);

    /* Always reset the buffer after serializing, otherwise
     * there is catastrophe waiting for you */
    reset_serialized_buffer(buf);
    
    /*Create a new object which will be filled
      via de-serialize method */
    person_t *p2 = NULL;
    
    p2 = Deserialize_person_t(buf);
    /* print the structure after de-serializing it */
    printf("Person structure after de-serializing it\n");
    print_person(p2);

    return 0;
}

