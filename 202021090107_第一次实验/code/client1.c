#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv){
    if(argc<2)
        printf("usage: %s <server address>", argv[0]);

    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    if((inet_pton(AF_INET, argv[1], &server_addr.sin_addr)) < 0)
        perror("invalid IP address");

    if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        perror("can't connect to server");
    close(sockfd);
    return 0;
}
