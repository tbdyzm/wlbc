#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in server_addr, client_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    int opt = SO_REUSEADDR;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(9999);

    if(bind(listenfd, (struct sockaddr*)& server_addr,sizeof(server_addr)))
        perror("bind error\n");

    if(listen(listenfd, 10) < 0)
        perror("listen error\n");

    char buff[1024];

    while(1){
        clilen = sizeof(client_addr);

        if(connfd = accept(listenfd, (struct sockaddr*) &client_addr, &clilen) < 0)
            perror("accept error\n");
        printf("client address: %s, client port: %d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
        close(connfd);
    }

    close(listenfd);
    return 0;
}