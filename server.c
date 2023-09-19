#include "include/Serialize.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
multiply(int n1, int n2)
{
    return n1*n2;
}
int
addition(int n1, int n2)
{
    return n1+n2;
}
int
Unmarshal_received_data(ser_buff_t *received_buf)
{
    int num1, num2;
    IdentityHdr_t hdr;
    /* Buffer has to be reset first so that the
     * deserializing starts from beginning */
    reset_serialized_buffer(received_buf);
    /* Get the header data */
    De_serialize_data ( (char *)&hdr.OpId, received_buf, sizeof(int));
    De_serialize_data ( (char *)&hdr.PayloadSize, received_buf, sizeof(int));
    /* step 5 Deserialize the data and store it in local variable */
    De_serialize_data ( (char *)&num1, received_buf, sizeof(int));
    De_serialize_data ( (char *)&num2, received_buf, sizeof(int));
    printf("Received num1 = %d and num2 = %d from client\n",num1,num2);
    /* step 6 Invoke actual RPC at server */
    if(hdr.OpId == MULTIPLY)
    {
        printf("Operation: Multiplication\n");
        return multiply(num1,num2);
    }
    else if(hdr.OpId == ADDITION)
    {
        printf("Operation: Addition\n");
        return addition(num1,num2);
    }
    else
    {
        printf("Invalid operation %d\n",hdr.OpId);
        return 0;
    }
}
int main()
{
   int sock_fd;
   sock_fd = socket (AF_INET, SOCK_DGRAM, 0);
   if(sock_fd == -1)
   {
       printf("Error in creating a socket\n");
       exit(1);
   }
   int rc,result;
   struct sockaddr_in server_addr, client_addr;
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(1234);
   server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   unsigned int addrlen = sizeof(server_addr);
   if((bind(sock_fd, (struct sockaddr *)&server_addr, addrlen)) == -1)
   {
       printf("Error in binding at server\n");
       exit(1);
   }
   ser_buff_t *server_receive_buf,
              *server_send_buf;
   /* Initialize the buffers */
   Initialize_ser_buf(&server_receive_buf);
   Initialize_ser_buf(&server_send_buf);
   /* step 4 server process received the serialized data from client */
   rc = recvfrom(sock_fd, server_receive_buf->b , get_buf_size(server_receive_buf), 
                 0, (struct sockaddr *)&client_addr, &addrlen);
   printf("Server has received %d data\n",rc);
   /* step 5 Deserialize the data received */
   result = Unmarshal_received_data(server_receive_buf);
   /* step 7 Serialize the result */
   Serialize_data (server_send_buf, (char *)&result, sizeof(int) );
   /* step 8 Send the data back to client */
   rc = sendto(sock_fd, server_send_buf->b, get_buf_size(server_send_buf), 0,
               (struct sockaddr *)&client_addr, addrlen);
   printf("Server has send %d bytes to client\n",rc);
   free_serialize_buffer(server_receive_buf);
   free_serialize_buffer(server_send_buf);
   server_receive_buf = NULL;
   server_send_buf = NULL;
   return 0;
}
