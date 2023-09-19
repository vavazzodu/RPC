#include "include/Serialize.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void rpc_send_recv(ser_buff_t *in_buf, ser_buff_t *res_buf)
{
   int sock_fd;
   sock_fd = socket (AF_INET, SOCK_DGRAM, 0);
   if(sock_fd == -1)
   {
       printf("Error in creating a socket\n");
       return;
   }
   int rc;
   struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_port = htons(1234);
   addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   unsigned int addrlen = sizeof(addr);
   rc = sendto(sock_fd, in_buf->b, get_buf_size(in_buf), 0, (struct sockaddr *)&addr, addrlen);
   printf("client has send %d bytes to server\n",rc);
   /* step 4,5,6,7 & 8 will be at server side, so check server.c */
   /* Following will be the blocking system call untill
    * the client process receives data from server */
   rc = recvfrom(sock_fd, res_buf->b, get_buf_size(res_buf), 0, (struct sockaddr *)&addr , &addrlen);
   /* at this moment the res_buf contains the data from server */
   printf("client has received %d bytes from server\n",rc);
   return;
}
int rpc_multiply(int n1, int n2)
{
    /* initialize the serial buffers */
    ser_buff_t *client_send_buf, 
               *client_receive_buf;
    IdentityHdr_t hdr;
    int result;
    /* Fill the header */
    hdr.OpId = ADDITION;
    hdr.PayloadSize = 0; //We dont know yet

    /* Initialize the buffers */
    Initialize_ser_buf(&client_send_buf);
    Initialize_ser_buf(&client_receive_buf);
    
    /* Reserve the header space in buffer */
    skip_serialized_buffer(client_send_buf, sizeof(hdr));
    /* step 2, Fill the buffer with data */
    Serialize_data(client_send_buf, (char *)&n1, sizeof(int));
    Serialize_data(client_send_buf, (char *)&n2, sizeof(int));
    /* now the data will be serialized and look something like
        -------------------------
        *           |           *
        *    n1     |    n2     *
        *           |           *
        -------------------------          */
                                           
    /* Now that we know the payload size */
    hdr.PayloadSize = get_serialize_buffer_data_size(client_send_buf) - sizeof(hdr);
    /* Now copy the header into serialized buffer but at the beginning */
    copy_in_serialized_buffer_by_offset(client_send_buf, (char *)&hdr.OpId, sizeof(hdr.OpId), 0); 
    copy_in_serialized_buffer_by_offset(client_send_buf, (char *)&hdr.PayloadSize, 
                                           sizeof(hdr.PayloadSize), sizeof(hdr.OpId));
    /* step 3 Send the data over the NW to server */
    rpc_send_recv(client_send_buf, client_receive_buf);
    /* step 9 Deserialize the receive buffer and get the result */
    /* reset the buffer so that reading start from beginning */
    reset_serialized_buffer(client_receive_buf);
    De_serialize_data((char *)&result, client_receive_buf, sizeof(int));
    free_serialize_buffer(client_send_buf);
    free_serialize_buffer(client_receive_buf);
    client_send_buf = NULL;
    client_receive_buf = NULL;
    return result;
}

int main()
{
    int a, b;
    printf("Please enter the value of a:");
    scanf("%d",&a);
    printf("\n");
    printf("Please enter the value of b:");
    scanf("%d",&b);
    printf("\n");
    /* Calling RPC function for multiplication */
    int res;
    /* step 1 */
    res = rpc_multiply(a,b);
    printf("Multiplication of a and b is: %d\n",res);
    return 0;
}
