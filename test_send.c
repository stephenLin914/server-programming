// chapter:5-5
// ./test_send 192.168.18.137 54321；192.168.18.137是服务器IP。
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
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);

    // 发起连接
    if (connect(sock, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        printf("conection failed!\n");
    } else {
        // 发送数据
        const char* oob_data = "abc";
        const char* normal_data = "123";
        send(sock, normal_data, strlen(normal_data), 0);
        send(sock, oob_data, strlen(oob_data), MSG_OOB);
        send(sock, normal_data, strlen(normal_data), 0);
    }

    close(sock);

    return 0;
}