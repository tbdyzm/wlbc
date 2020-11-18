#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
#include <malloc.h>

#include <pthread.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void process_cli(int connfd, struct sockaddr_in client_addr);
static void *start_routine(void *arg);
void saveData(char *recvs, int len, char *save);

struct ARG{
    int connfd;
    struct sockaddr_in client_addr;
};

int main(void){
    int listenfd, connfd;
    char buff[1024], recvs[1024];
    memset(buff, 0, sizeof(buff));
    memset(recvs, 0, sizeof(recvs));

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(9999);

    pthread_t tid;
    struct ARG *arg;
    socklen_t clilen;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("create socket error\n");
        exit(-1);
    }

    int opt = SO_REUSEADDR;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if((bind(listenfd, (struct sockaddr*) &server_addr, sizeof(server_addr))) == -1){
        perror("bind error\n");
        exit(-1);
    }

    if((listen(listenfd, 10)) == -1){
        perror("listen error\n");
        exit(-1);
    }

    clilen = sizeof(client_addr);

    while (1)
    {
        if((connfd = accept(listenfd, (struct sockaddr*) &client_addr, &clilen)) == -1){
            perror("accept error\n");
            exit(-1);
        }

        arg = (struct ARG*)malloc(sizeof(struct ARG));
        arg->connfd = connfd;
        memcpy((void*) &arg->client_addr, &client_addr, sizeof(client_addr));
        if(pthread_create(&tid, NULL, &start_routine, (void*)arg) > 0){
            perror("create thead error\n");
            free(arg);
            continue;
        }
    }

    close(listenfd);
    return(0);    
}

void process_cli(int connfd, struct sockaddr_in client_addr){
    int num, i;
    num = i = 0;
    char client_data[1024], recvs[1024], sends[1024], client_name[1024];
    memset(client_data, 0, sizeof(client_data));
    memset(recvs, 0, sizeof(recvs));
    memset(sends, 0, sizeof(sends));
    memset(client_name, 0, sizeof(client_name));
    printf("client address: %s, client port: %d\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
    num = recv(connfd, client_name, sizeof(client_name), 0);
    if(num == 0){
        close(connfd);
        printf("client broken\n");
        return;
    }
    printf("client name is %s\n", client_name);
    while (num = recv(connfd, recvs, sizeof(recvs), 0))
    {
        saveData(recvs, num, client_data);
        printf("Received client (%s) message: %s\n",client_name, recvs);
        int len = strlen(recvs);
        for(i = 0; i < len/2; i++){
            char tmp = recvs[i];
            recvs[i] = recvs[len-1 -i];
            recvs[len-1 - i] = tmp;
        }
        strcpy(sends, recvs);
        send(connfd, sends, strlen(sends), 0);
    }
    printf("all of data from client (%s:%d): %s\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), client_data);
    close(connfd);
}

static void *start_routine(void* arg){
        struct ARG *info;
        info = (struct ARG*) arg;
        process_cli(info->connfd, info->client_addr);
        free(arg);
        pthread_exit(NULL);
}

void saveData(char *recvs, int len, char *save){
    int start = strlen(save);
    for(int i = 0; i < len; i++){
            save[i+start] = recvs[i];
    }
}