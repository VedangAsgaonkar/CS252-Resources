#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3490"
#define MAXDATASIZE 100

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *serverinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &serverinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = serverinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            fprintf(stderr, "client: socket\n");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            fprintf(stderr, "client: connect\n");
            continue;
        }
        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("Connected to server on %s\n", s);

    freeaddrinfo(serverinfo);

    while (1)
    {
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1)
        {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';
        printf("client: recieved %s\n", buf);
        memset(buf, '\0', MAXDATASIZE);
    }
    close(sockfd);
    return 0;
}