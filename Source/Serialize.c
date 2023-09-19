#include "../include/Serialize.h"

void
Initialize_ser_buf( ser_buff_t **buff )
{
    //Allocating memory for the serialized buffer//
    (*buff) = calloc(1, sizeof(ser_buff_t));
    if(buff == NULL)
        {
	    printf("Error while allocating the memory\n");
        return;
        }
    //Allocating memory for the DATA buffer//
    (*buff)->b = calloc(1, MAX_BUF_SIZE);
    if((*buff)->b == NULL)
        {
        printf("Error while allocating the memory for b\n");
        return;
        }
    //Initializing DATA buffer size and next pointer//
    (*buff)->size = MAX_BUF_SIZE;
    (*buff)->next = 0;
    return;
}
void
Serialize_data ( ser_buff_t *buff, char *data, size_t data_len )
{
    //check available size in DATA buffer
    size_t lvavailable_size = (buff->size) - (buff->next);
    int lvallocated = 0;
    // Check if we have enough space to accomodate the data
    // If we dont have then double the buffer size, till it
    // can accomodate the data.
    while(lvavailable_size < data_len)
        {
        lvallocated = 1;
        (buff->size) = (buff->size)*2;
        //re-calculate the available size in DATA buffer
        lvavailable_size = (buff->size) - (buff->next);
        }
    //If there was enogh space to accomodate new data
    if(lvallocated == 0)
        {
        memcpy((char *)buff->b + buff->next, data, data_len);
        buff->next = buff->next + data_len;
        return;
        }
    //Expand the buffer to its double size.
    buff->b = realloc(buff->b, buff->size);
    memcpy((char *)buff->b + buff->next, data, data_len);
    buff->next = buff->next + data_len;
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
    //If there is no size provided, like howmuch data we want
    //to deserialized
    if(!size)
        {
        printf("size is 0, in De_serialize_data()\n");
	    return;
        }
    //If we are requesting more data to deserialized than
    //what ever is available in DATA buffer
    if((buff->size - buff->next) < size)
        {
        printf("Invalid data copy, in De_serialize_data()\n");
        return;
        }
    //For the first time deserialization the next will be 0
    //so that the data will be read from beginning
    //that is the reason for resetting the next pointer after 
    //serialization.
    memcpy(dest, buff->b + buff->next, size);
    buff->next = buff->next + size;

}
void
skip_serialize_data ( ser_buff_t *buff, int skip_size)
{
    int lvavailable = buff->size - buff->next;
    int lvrealloc = 0;
    if(!buff || !buff->b)
        {
        printf("Buffer is NULL, in skip_serialized_data\n");
        return;
        }
    //If we dont have much space left in DATA buffer
    //to skip the data, double the buffer and 
    while(lvavailable < skip_size)
        {
        lvrealloc = 1;
        buff->size = buff->size*2;
        lvavailable = buff->size - buff->next;
        }
    if(lvrealloc == 1)
        buff->b = realloc(buff->b, buff->size);
    //Move the next pointer
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
//After serializing the data we need to reset the next pointer
//so that while de-serializing we can de-serialize from the 
//beginning of the DATA buffer.
void
reset_serialized_buffer ( ser_buff_t *buff )
{
    buff->next = 0;
}
int
get_buf_size(ser_buff_t *buff)
{
    if(buff == NULL)
        return 0;
    else
    {
        return buff->size;
    }
}
/*
 *       |<-----------size---------------->|
 *        ---------------------------------
 *       |            |                    |
 *       |            |                    |
 *        ---------------------------------
 *       b            next
*/ 
void
copy_in_serialized_buffer_by_offset( ser_buff_t *buff, char *value, int size, int offset)
{
    // If buffer size is less than offset
    if(buff->size < offset)
    {
        printf("Error: Attempt to write outside buffer boundries\n");
        return;
    }
    memcpy(buff->b + offset, value, size); 
}
void
skip_serialized_buffer( ser_buff_t *buff, int size)
{
    int lv_availableSize;
    lv_availableSize = buff->size - buff->next;
    if(lv_availableSize >= size)
    {
        buff->next = buff->next + size;
        return;
    }
    while(lv_availableSize < size)
    {
        buff->size = buff->size*2;
        lv_availableSize = buff->size - buff->next;
    }
    buff->b = realloc(buff->b, buff->size);
    buff->next = buff->next + size;
    return;
}
int  
get_serialize_buffer_data_size(ser_buff_t *b){
        return b->next;
}
