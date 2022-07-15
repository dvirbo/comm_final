#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
int NUM = 1;

int main(int argc, char *argv[])
{
    int socket_fd;
    struct sockaddr_in dest;
    struct hostent *hostptr;
        char *hostname, *hostaddr;

    struct
    {
        char head;
        u_long body;
        char tail;
    } msgbuf;

 
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0); // Create a new socket
    bzero((char *)&dest, sizeof(dest));         // To keep the same size with struct sockaddr
    hostptr = gethostbyname(argv[1]);           // get the dest hostname that recieved as an arg, Return entry from host data base for host with NAME.
    dest.sin_family = (short)AF_INET;
    bcopy(hostptr->h_addr, (char *)&dest.sin_addr, hostptr->h_length); // Copy N bytes of hostptr to sin_addr

    dest.sin_port = htons((u_short)0x3333); // define destanation port

    msgbuf.head = '<';
    msgbuf.body = htonl(NUM); // convert between host and network byte order.
    msgbuf.tail = '>';
    while (1)
    {
        sendto(socket_fd, &msgbuf, sizeof(msgbuf), 0, (struct sockaddr *)&dest,
               sizeof(dest)); // Send N bytes of BUF on socket FD to peer at address ADDR , return nom of bits that sent
        // need to inc something here:
        msgbuf.body = htonl(++NUM);
        sleep(1);

        return 0;
    }
