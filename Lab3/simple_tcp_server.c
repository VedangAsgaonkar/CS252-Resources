#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#define MY_PORT "3490"
#define BACKLOG 10

int main(int argc, char *argv[])
{
    struct socketaddr_storage;
    struct addrinfo hints, *res;
    int status;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; //Use ipv4 or ipv6 whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; //fill my IP. Can also manually fill the IP you want to use

    //NULL signifies take IP from hints.
    if ((status = getaddrinfo(NULL, MY_PORT, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    bind(sockfd, res->ai_addr, res->ai_addrlen);

    /*
    Old way of coding the same thing :
    int sockfd;
    struct sockaddr_in my_addr;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MY_PORT); // htons converts to short data type in network byte order
    my_addr.sin_addr.s_addr = inet_addr("10.12.110.57"); 
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

    bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr);
    */

    listen(sockfd, BACKLOG);

    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    addr_size = sizeof their_addr;
    int new_fd = accept(sockfd, (struct socketaddr *)&their_addr, &addr_size);

    char *msg = "Hello Client";
    int len, bytes_sent;
    len = strlen(msg);
    bytes_sent = send(new_fd, msg, len, 0);

    close(sockfd);
}