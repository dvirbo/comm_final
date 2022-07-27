#include <sys/types.h>
#include <unistd.h>    
#include <strings.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#define SIM_LENGTH 10 
#define PORT 1337 


int main(int argc, char* argv[])
{ 
  struct addrinfo* res;
  struct sockaddr_in* serveur_addr;
  struct sockaddr_in cli_name; 
  char *url, *hostname;
  char* host_addr;
  int sock; 
  int count;
  int value; 
  


  if (argc != 2) {
    perror(" Error please use  hostnamelookup \n");
    exit(1);
  }
puts("enter URL");
  url = argv[1];
  int getinfoad=getaddrinfo(url, NULL, NULL, &res); // go to local dns by the url
  
  if (0 !=getinfoad) 
  {
    fprintf(stderr, "Error Resolving hostname %s \n", url);
    exit(1);
  }
  
  serveur_addr = (struct sockaddr_in*)res->ai_addr;
  host_addr = inet_ntoa(serveur_addr->sin_addr);

  printf("Address for %s is %s \n", url, host_addr);
  

  printf("Client is alive and establishing socket connection \n");
  
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
   for (count = 1; count <= SIM_LENGTH; count++)
    { read(sock, &value, 4);
      printf("Client has received %d from socket.\n", value);
    }
      

  bzero(&cli_name, sizeof(cli_name)); 
  cli_name.sin_family = AF_INET; 
  cli_name.sin_addr.s_addr = inet_addr(host_addr); 
  cli_name.sin_port = htons(PORT);


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }


  for (count = 1; count <= SIM_LENGTH; count++)
    { read(sock, &value, 4);
    printf("GET %c HTTP/1.0\n", url);  
   // printf("HOST: ", res->);
      printf("Client has received %d from socket  \n", value);
    }

  printf("Exiting now \n");

  close(sock); 
  exit(0); 

}  