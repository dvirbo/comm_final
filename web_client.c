
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/*
In this part of the lab, we will build a simple analog to wget.
Our program will take a URL as a command-line argument and write the response from the web server to STDOUT.
*/
#define MAX_SIZE 1024
int main(int argc, char *argv[])
{
    int sock, value, count = 0, port = 0 , i;
    char send[MAX_SIZE], recv[MAX_SIZE], slash = '/', dots = ':', eof = '\0';

    struct addrinfo *res;
    struct sockaddr_in cli_name;
    bzero(&cli_name, sizeof(cli_name));

    printf("Client is alive and establishing socket connection.\n");

    // check if the URL isnert in CMD
    if (argc != 2)
    {
        perror("didn't insert URL\n");
        exit(1);
    }

    char *url = argv[1]; // command-line argument should be a URL
    int s_host = 7, startPath = 0;

    // The method illustrates a simple parser for URLs of this form:

    for (i = 0; i < strlen(url); i++) // find the start path:
    {
        if (url[i] == slash)
        {
            startPath = i;
        }
    }

    while (url[s_host] != dots && url[s_host] != slash) // check length of hostname:
    {
        ++s_host;
        ++count;
    }
    // init path & hostName:
    int pathcounter = 0, curr = startPath;

    // check the length of the path:
    while (curr + 1 < strlen(url))
    {
        curr++;
        pathcounter++;
    }
    // alocate sapce to the path:
    char *charPath = (char *)malloc((pathcounter + 1) * sizeof(char));
    for (i = 0; i < pathcounter; ++i)
    {
        charPath[i] = url[startPath + i + 1];
    }
    charPath[startPath + pathcounter] = eof;
    // alocate space to host name:
    char *hostname = (char *)malloc((count + 1) * sizeof(char));

    for (i = 0; i < count; ++i)
    {
        hostname[i] = url[i + 7];
    }
    hostname[count + 7] = eof;

    // init port:
    if (url[count + 7] != dots) // if there is a port in the URL: we got to the ':'
    {
        port = 80; // for HTTP
    }
    else
    {
        int cPort = 0; // count port length
        int sPort = 8 + count;
        int curr;
        while (url[sPort] != slash) // find the last slash
        {
            ++sPort;
            ++cPort;
        }
        curr = sPort - cPort;
        // alocate sapce to the port:
        char *portChar = (char *)malloc((cPort + 1) * sizeof(char));
        for (int i = 0; i < cPort; ++i)
        {
            portChar[i] = url[curr + i];
        }
        portChar[cPort + 7] = eof;
        //Convert a string (port) to a long integer:
        port = strtol(portChar, NULL, 10); 
        free(portChar);
    }

    // getting the IP address by the host
    struct hostent *host = gethostbyname(hostname);
    if (getaddrinfo(host->h_name, 0, 0, &res) != 0)
    {
        printf("error");
        exit(1);
    }
    struct sockaddr_in *saddr = (struct sockaddr_in *)res->ai_addr;
    char *ip_address = inet_ntoa(saddr->sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, 0); // create and check if error occur
    if (sock < 0) // check if -1 for errors
    {
        perror("Error opening channel");
        close(sock);
        exit(1);
    }

    cli_name.sin_family = AF_INET;
    cli_name.sin_addr.s_addr = inet_addr(ip_address);
    cli_name.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) == -1)
    {
        perror("connect");
        close(sock);
        exit(1);
    }
    // simple parser for URL:
    sprintf(send, "GET /%s HTTP/1.0\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n", charPath, hostname); 
    free(charPath);
    free(hostname);

    if (write(sock, send, strlen(send)) >= 0)
    {
        size_t n;
        while ((n = read(sock, recv, MAX_SIZE)) > 0) // Read NBYTES into BUF from FD
        {
            recv[n] = eof;
            // You will know all of the data has been read when the return value from the read(...) syscall is zero (indicating 0 bytes read):
            if (fputs(recv, stdout) == EOF)
            {
                printf("error while fputs");
            }
        }
    }
    printf("\nExiting now.\n");
    // When all of the data has been read, close the socket
    close(sock);
    exit(0);
}

/*
if the URL is http://www.yahoo.com :
then the data begin with: Content-Security-Policy-Report-Only

else, if the URL is :http://www.yahoo.com/does-not-exist:
then the data begin with: "Content-Security-Policy"
*/