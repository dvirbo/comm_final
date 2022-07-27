#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#define PORT 1337 // P

int main(int argc, char *argv[])
{
    int socket_fd, check;
    struct sockaddr_in dest;
    struct hostent *hostptr;
    char *hostname, *hostaddr;
    u_long num = 1;

    struct
    {
        u_long body;
    } msgbuf;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0); // Create a new socket
    bzero((char *)&dest, sizeof(dest));         // To keep the same size with struct sockaddr
    hostptr = gethostbyname(argv[1]);           // get the dest hostname that recieved as an arg, Return entry from host data base for host with NAME.
    dest.sin_family = (short)AF_INET;

    dest.sin_port = htons((u_short)0x3333);                            // define destanation port
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