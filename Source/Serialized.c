#include "../include/Serialized.h"

void
Initialize_ser_buf( ser_buff_t **buff )
{
    buff = calloc(1, sizeof(ser_buff_t));
    if(buff == NULL)
        {
	printf("Error while allocating the memory\n");
	return;
        }
    (*buff)->b = calloc(1, MAX_BUF_SIZE);
    if((*buff)->b == NULL)
        {
        printf("Error while allocating the memory for b\n");
        return;
        }
    (*buff)->size = MAX_BUF_SIZE;
    (*buff)->next = 0;
    return;
}
void
Serialize_data ( ser_buff_t *buff, char *data, size_t data_len )
{
    size_t lvavailable_size = (buff->size) - (buff->next);
    int lvallocated = 0;
    while(lvavailable_size < data_len)
        {
        lvallocated = 1;
        (buff->size) = (buff->size)*2;
	lvavailable_size = (buff->size) - (buff->next);
        }
    if(lvallocated == 0)
        {
        memcpy((char *)buff->b + buff->next, data, data_len);
        buf->next = buf->next + data_len;
	return;
        }
    buf->b = realloc(buff->b, buff->size);
    memcpy((char *)buff->b + buff->next, data, data_len);
    buf->next = buf->next + data_len;
    return;
}
void
De_serialize_data ( char *dest, ser_buff_t *buff, int size)
{
    if(!buff || !buff->b)
        {
        printf("Buffer is NULL, in De_serialize_data()\n");
	return;
        }
    if(!size)
        {
        printf("size is 0, in De_serialize_data()\n");
	return;
        }
    if((buf->size - buf->next) < size)
        {
        printf("Invalid data copy, in De_serialize_data()\n");
        return;
        }
    memcpy(dest, buff->c + buf->next, size);
    return;

}
void
skip_serialize_data ( ser_buff_t *buff, int skip_size)
{
    size_t lvavailable = buf->size - buf->next;
    if(!buff || !buff->b)
        {
        printf("Buffer is NULL, in De_serialize_data()\n");
        return;
        }
    while(lvavailable < skip_size)
        {
        buff->size = buff->size*2;
	lvavailable = buff->size - buff->next;
        }
    buff->b = realloc(buff->b, buff->size);
    buff->next = buff->next + skip_size;
    return;
}
void
free_serialize_buffer ( ser_buff_t *buff)
{
    if(buff != NULL || buff->b != NULL)
	{
        free(buff->b);
        free(buff);
        }
    return;
}

