#include "include/Serialize.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{
   int sock_fd;
   sock_fd = socket (AF_INET, SOCK_DGRAM, 0);
   if(sock_fd == -1)
   {
       printf("Error in creating a socket\n");
       exit(1);
   }
   int rc;
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
   Initialize_ser_buf(&server_receive_buf);
   Initialize_ser_buf(&server_send_buf);

   rc = recvfrom(sock_fd, server_receive_buf->b , get_buf_size(server_receive_buf), 
                 0, (struct sockaddr *)&client_addr, &addrlen);
   printf("Server has received %d data\n",rc);
   return 0;
}
