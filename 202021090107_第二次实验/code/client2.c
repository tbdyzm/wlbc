#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv){
    if(argc<2){
        printf("usage: %s <server address>\n", argv[0]);
        return -1;
    }

    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    if((inet_pton(AF_INET, argv[1], &server_addr.sin_addr)) < 0)
        perror("invalid IP address\n");

    if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        perror("can't connect to server\n");
    
    char sends[1024], recvs[1024];
    memset(sends, 0, sizeof(sends));
    memset(recvs, 0, sizeof(recvs));
    while (1)
    {
        printf("Please enter a string that you want to send to the server: ");
        fgets(sends, sizeof(sends), stdin);
        if(send(sockfd, sends, strlen(sends)-1, 0) == -1)
            perror("send error\n");
        if(strcmp(sends, "quit\n") == 0 || strcmp(sends, "exit\n") == 0){
            close(sockfd);
            break;
        }
        if(recv(sockfd, recvs, sizeof(recvs), 0) == -1)
            perror("recv error\n");
        fputs(recvs, stdout);
        memset(sends, 0, sizeof(sends));
        memset(recvs, 0, sizeof(recvs));
        printf("\n");
    }

    close(sockfd);
    return 0;
}