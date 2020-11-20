#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 9999
#define MAXDATASIZE 100
int main(int argc, char *argv[])
{

    int i = 1, fd, numbytes;
    char buf[MAXDATASIZE];
    struct sockaddr_in server_addr;

    if (argc != 2)
    {
        printf("Usage:%s <IP address>\n", argv[0]);
        exit(1);
    }

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Create socket failed\n");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if((inet_pton(AF_INET, argv[1], &server_addr.sin_addr)) < 0)
        perror("invalid IP address\n");
    if (i = connect(fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect error\n");
        exit(1);
    }
    printf("Please input your name:");
    scanf("%s", buf);
    if ( ((numbytes = send(fd, buf, strlen(buf), 0)) == -1))
    {
        perror("send error\n");
        exit(1);
    }

    while (1)
    {
        printf("Please input a string:");
        scanf("%s", buf);
        if ((!strcmp(buf, "exit")) || (!strcmp(buf, "quit")))
        {
            close(fd);
            break;
        }
        buf[strlen(buf)] = '\0';

        if ((numbytes = send(fd, buf, MAXDATASIZE, 0)) == -1)
        {
            perror("send error\n");
            exit(1);
        }
        if ((numbytes = recv(fd, buf, MAXDATASIZE, 0)) == -1)
        {
            perror("recv error\n");
            exit(1);
        }
        buf[numbytes] = '\0';
        printf("Server reversed message:%s\n", buf);
    }

    close(fd);
    return 0;
}
