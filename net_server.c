#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#define SIM_LENGTH 10
#define PORT 1337

int main(void)
{
  int sock;
  int connect_sock;
  struct sockaddr_in serv_name;
  socklen_t len;
  int count;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&serv_name, sizeof(serv_name));
  serv_name.sin_family = AF_INET;
  serv_name.sin_port = htons(PORT);

  bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name));

  listen(sock, 1);

  len = sizeof(serv_name);

  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len);

  for (count = 1; count <= SIM_LENGTH; count++)
  {
    write(connect_sock, &count, 4);
    printf("Server has written %d to socket.\n", count);
  }

  close(connect_sock);
  close(sock);
}


/*
a. after compile net_server.c & net_client.c 'as is' i got seg fault
b. after change the defenition of the ip & recompile net_client.c & run both as above 
i got another seg fault.. but when i change the len type from size_t to socklen_t -->
i got 10 received massege from Client & 10 written massege from Server
probably the compiler didn't implicitly cast it


*/