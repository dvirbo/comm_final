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
#include <string.h>
#include <net/if.h>

/*
This function will print the string indicated by pname ended with a line break,
 then it will print the string indicated by msg,
 then the string "ip= " follow by the IP address of the host associated with this socket address structure in dotted-decimal notation, a comma,
 and finally the string "port= " followed by the port number associated with this socket address structure.
*/
void printsin(struct sockaddr_in *s, char *str1, char *str2)
{
    printf("%s\n", str1);
    printf("%s ", str2);
    printf("ip= %s port= %d\n", inet_ntoa(s->sin_addr), s->sin_port);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("didn't insert port number\n");
        exit(1);
    }
    int socket_fd, cc, fsize;
    struct sockaddr_in s_in, from;
    unsigned short port; // port number sent as parameter
    struct
    {
        u_long body;
    } msg;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0); // create a socket for UDP

    bzero((char *)&s_in, sizeof(s_in)); // To keep the same size with struct sockaddr

    s_in.sin_family = (short)AF_INET;         // insert IPv4 to s_in
    s_in.sin_addr.s_addr = htonl(INADDR_ANY); // cast IPv4 (~INADDER_ANY) to net form

    port = (unsigned short)atoi(argv[1]); // The port is the second argument
    fprintf(stdout, "port = %d\n", port);
    s_in.sin_port = htons(port);

    printsin(&s_in, "RECV_UDP", "Local socket is:");
    fflush(stdout); // Flush STREAM --> clear the buffer

    bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in)); // connect the socket with the addr of s_in & port

    for (;;)
    {
        fsize = sizeof(from);
        cc = recvfrom(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *)&from, &fsize); // Read N bytes into BUF through socket FD & insert cc size of the message
        if (cc == -1)                                                                     // check the number of bytes read or -1 for errors
        {
            perror("got nothing");
        }
        else
        {
            printsin(&from, "recv_udp: ", "Packet from:");
            printf("Got data ::%d\n", (int)ntohl(msg.body)); // Write formatted output to stdout

            fflush(stdout); // Flush STREAM --> clear the buffer
        }
    }
    return 0;
}
  /*
   run:
        ./sink ubuntu 3637
  */