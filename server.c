#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8888

int main(void) {
    // set up socket and get file descriptor (sockfd)
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket error");
        exit(1);
    }

    // set any options/flags on sockfd
    int yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    // best effort for setsockopt() so no need for error checking;
    // continue on with default socket options

    // bind sockfd to an address
    struct sockaddr_in sa = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = htonl(INADDR_ANY),
    };
    int bind_res = bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    if (bind_res == -1) {
        perror("bind error");
        exit(1);
    }

    // call listen() to mark sockfd as passive
    int listen_res = listen(sockfd, SOMAXCONN);
    if (listen_res == -1) {
        perror("listen error");
        exit(1);
    }

    while (1) {
        int connfd = accept(sockfd, NULL, 0);
        if (connfd == -1) {
            perror("accept error");
            continue;
        }

        char read_buf[10];
        ssize_t bytes_read = read(connfd, read_buf, sizeof(read_buf) - 1);
        if (bytes_read == -1) {
            perror("read error");
            continue;
        }
        read_buf[bytes_read] = '\0';

        printf("read_buf = %s\n", read_buf);

        close(connfd);
    }

    return 0;
}
