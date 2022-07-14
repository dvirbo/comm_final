#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 1337 

int main(int argc, char *argv[])
{
    int socket_fd_b, socket_fd_c, cc, fsize;
    struct sockaddr_in b, c, from;

    struct addrinfo *res;
    char *hostname, *hostaddr;
    struct sockaddr_in *saddr;

    char msg[100];
    double x = 0.3, random;

    if (argc != 2)
    {
        perror("didn't insert the name of the host\n");
        exit(1);
    }

    hostname = argv[1];

    if (0 != getaddrinfo(hostname, NULL, NULL, &res))
    { // get address information, if its not succeed print eror message.
        fprintf(stderr, "Error in resolving hostname %s\n", hostname);
        exit(1);
    }

    saddr = (struct sockaddr_in *)res->ai_addr; // setting the info from the strucy addrinfo res (the info provided by the function getadderinfo) into those variables and prints them.
    hostaddr = inet_ntoa(saddr->sin_addr);     




    random = (double)rand() / (double)RAND_MAX;
    printf("random= %lf, x= %lf\n", random, x);

    socket_fd_b = socket(AF_INET, SOCK_DGRAM, 0);
    socket_fd_c = socket(AF_INET, SOCK_DGRAM, 0);


    memset((char *)&b, 0, sizeof(b)); // change to whats was already..
    memset((char *)&c, 0, sizeof(c));

    b.sin_family = (short)AF_INET;
    b.sin_addr.s_addr = inet_addr(hostaddr);
    b.sin_port = htons(PORT + 1); // my port

    c.sin_family = (short)AF_INET;
    c.sin_addr.s_addr = htonl(INADDR_ANY);
    c.sin_port = htons(PORT); // another 



    bind(socket_fd_b, (struct sockaddr *)&b, sizeof(b));
    bind(socket_fd_c, (struct sockaddr *)&c, sizeof(c));


    while (1)
    {
        fsize = sizeof(from);
        printf("hey");
        cc = recvfrom(socket_fd_c, &msg, 100, 0, (struct sockaddr *)&from, &fsize);
        printf("I Got msg");
        fflush(stdout);

            if (random > x)
            { // if random bigger than X
                random = (double)rand() / (double)(RAND_MAX);
                printf(" from A:\n\"%s\"\n", msg);
                fsize = sizeof(c);
                sendto(socket_fd_b, &msg, 100, 0, (struct sockaddr *)&c, fsize);
                printf("\nI decided to send the msg to C\n");
                fflush(stdout);
            }
            else
            {
                printf("I decided drop it down\n");
            }

    }
    return 0;
}