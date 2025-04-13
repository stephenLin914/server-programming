// chapter:5-5
// ./test_accept 192.168.18.137 54321；192.168.18.137是本机IP。
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h> // atoi();
#include <unistd.h> //sleep(); close();

int main(int argc, char* args[]) {

    if (argc != 3) {
        printf("usage: ip_address port_number\n");
        return 1;
    }
    const char* ip = args[1];
    const int port = atoi(args[2]);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    // 创建一个IPv4地址
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    // 命名socket
    int ret = bind(sock, (struct sockaddr*) &address, sizeof(address));
    assert(ret != -1);

    // 监听socket
    ret = listen(sock, 5);
    assert(ret != -1);

    sleep(20); //等待20秒

    // 接受连接
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock, (struct sockaddr*) &client, &client_addrlength);
    if (connfd < 0) {
        printf("err!");
    } else {
        char remote[INET_ADDRSTRLEN];
        printf("connected with ip: %s and port: %d\n", 
            inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN), 
            ntohs(client.sin_port));    //inet_ntop将网络字节序的整型转换为字符串形式的IPV4/6地址。
        close(connfd);
    }


    close(sock);

    return 0;
}