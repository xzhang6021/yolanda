//
// Created by shengym on 2019-07-12.
//
#include "lib/common.h"

#define MAXLINE 4096

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        error(1, 0, "usage: udpclient1 <IPaddress>");
    }

    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    socklen_t server_len = sizeof(server_addr);

    if (connect(socket_fd, (struct sockaddr*)&server_addr, server_len))
    {
        error(1, errno, "connect failed");
    }

    struct sockaddr* reply_addr;
    reply_addr = malloc(server_len);

    char      send_line[MAXLINE], recv_line[MAXLINE + 1];
    socklen_t len;
    int       n;

    while (fgets(send_line, MAXLINE, stdin) != NULL)
    {
        int i = strlen(send_line);
        if (send_line[i - 1] == '\n')
        {
            send_line[i - 1] = 0;
        }

        printf("now sending %s\n", send_line);
        size_t rt = sendto(socket_fd, send_line, strlen(send_line), 0, (struct sockaddr*)&server_addr, server_len);
        if (rt < 0)
        {
            error(1, errno, "sendto failed");
        }
        printf("send bytes: %zu \n", rt);

        len = 0;
        recv_line[0] = 0;
        n = recvfrom(socket_fd, recv_line, MAXLINE, 0, reply_addr, &len);
        if (n < 0)
            error(1, errno, "recvfrom failed");
        recv_line[n] = 0;
        fputs(recv_line, stdout);
        fputs("\n", stdout);
    }

    exit(0);
}
