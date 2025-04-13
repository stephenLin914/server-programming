// chapter:5-3
// ./test_listen 192.168.18.137 12345 5；192.168.18.137是本机IP。
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

static bool stop = false;

static void handle_term(int sig) {
    stop = true;
}

int main(int argc, char* args[]) {

    signal(SIGTERM, handle_term);

    if (argc != 4) {
        printf("usage: ip_address port_number backlog\n");
        return 1;
    }
    const char* ip = args[1];
    const int port = atoi(args[2]);
    const int backlog = atoi(args[3]);

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
    ret = listen(sock, backlog);
    assert(ret != -1);

    
    //循环等待连接，直到有SIGTERM信号将它中断。
    while (!stop) {
        sleep(1);
    }

    close(sock);

    return 0;
}