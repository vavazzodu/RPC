#include<stdio.h>

#define MAX_BUF_SIZE 100

typedef struct serialized_buffer {
    char * b;
    int size;
    int next;
}ser_buff_t;

/* Supporting APIs for (De)serialization */

/* API for initializing the serialized buffer.
 * buff: double pointer to ser_buff_t structure.
 */
void
Initialize_ser_buf( ser_buff_t **buff );

/* API for serializing the internal data structure.
 * buff: pointer to ser_buff_t structure.
 * data: pointer to the data which needs to be serialized.
 * data_len: total length of the data.
 */
void
Serialize_data ( ser_buff_t *buff, char *data, size_t data_len );

/* API for reading the serialized data (Deserialization)
 * dest: pointer to the buffer which needs to be filled.
 * buff: pointer to the serialized data.
 * size: size of serialized data.
 */
void
De_serialize_data ( char *dest, ser_buff_t *buff, int size);

/* API for skipping the data.
 * buff: pointer to the serialized data buffer.
 * skip_size: No. of bytes we want to skip. 
 *            -ve value will rewind the buffer.
 */
void
skip_serialize_data ( ser_buff_t *buff, int skip_size);

/* API for freeing the serialized buffer.
 * buff: pointer to the serialized buffer.
 */
void
free_serialize_buffer ( ser_buff_t *buff);

