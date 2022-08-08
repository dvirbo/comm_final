#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        perror("didn't insert hostname & port number\n");
        exit(1);
    }

    int socket_fd, check;
    struct sockaddr_in dest; // host internet address (binary)
    struct hostent *hostptr; // server host name information
    char *hostname, *hostaddr;
    unsigned short port; // port number sent as parameter

    u_long num = 1;

    struct
    {
        u_long body;
    } msgbuf;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0); // Create a new socket
    bzero((char *)&dest, sizeof(dest));         // To keep the same size with struct sockaddr
    hostptr = gethostbyname(argv[1]);           // get the dest hostname that recieved as an arg, Return entry from host data base for host with NAME.
    dest.sin_family = (short)AF_INET;

    port = (unsigned short)atoi(argv[2]); // The port is the second argument
    fprintf(stdout, "port = %d\n", port);
    dest.sin_port = htons(port);
    bcopy(hostptr->h_addr, (char *)&dest.sin_addr, hostptr->h_length); // Copy N bytes of hostptr to sin_addr

    // msgbuf.body = htonl(NUM); // convert between host and network byte order.
    puts("connect..");
    while (1)
    {
        msgbuf.body = num;
        check = sendto(socket_fd, &msgbuf, sizeof(msgbuf), 0, (struct sockaddr *)&dest,
                       sizeof(dest)); // Send N bytes of BUF on socket FD to peer at address ADDR , return nom of bits that sent
        if (check == -1)
        {
            puts("err");
        }
        ++num;
        sleep(1);
    }
    return 0;
}