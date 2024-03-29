#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    struct
    {
        u_long num;
    } msg;
    if (argc != 3)
    {
        perror("didn't insert hostname & port number\n");
        exit(1);
    }
    unsigned short port; // port number sent as parameter
    struct hostent *hostnet = gethostbyname(argv[1]);
    int sock_send, sock_recv, from_size;   // define sock param
    struct sockaddr_in listen, recv, dest; // Structure describing an Internet socket address.

    port = (unsigned short)atoi(argv[2]); // The port is the second argument
    fprintf(stdout, "Clnt: port = %d\n", port);

    sock_recv = socket(AF_INET, SOCK_DGRAM, 0); // sock to recv
    bzero((char *)&listen, sizeof(listen));
    listen.sin_family = AF_INET;
    listen.sin_port = htons(port);              // recv port
    listen.sin_addr.s_addr = htonl(INADDR_ANY); // recives from every internal interface

    sock_send = socket(AF_INET, SOCK_DGRAM, 0); // sock to send
    bzero((char *)&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port + 1);

    bcopy(hostnet->h_addr, (char *)&dest.sin_addr, hostnet->h_length);

    srandom(1222); // Seed the random number generator with the given number.

    bind(sock_recv, (struct sockaddr *)&listen, sizeof(listen)); // bind socket & address for listen
    puts("init gateway\n");
    while (1)
    {
        from_size = sizeof(recv);
        int cc = recvfrom(sock_recv, &msg, sizeof(msg), 0, (struct sockaddr *)&recv, &from_size); // recvive from source sock
        float rand = ((float)random()) / ((float)RAND_MAX);                                       // generate random number
        if (rand > 0.5)
        {
            printf("sending %ld\n", msg.num);
            sendto(sock_send, &msg, sizeof(msg), 0, (struct sockaddr *)&dest, sizeof(dest)); // send to sink
        }
        fflush(stdout); // clear the buffer
    }
    return 0;
}