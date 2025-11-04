#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils/socket_helper.h"

int sock_connect(const char *ip, int port) {
    int sockfd;
    struct sockaddr_in serveraddr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    // fill server address info to connect
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons((unsigned short)port);
    inet_pton(AF_INET, ip, &serveraddr.sin_addr);

    if (connect(sockfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int sock_bind(int port) {
    int sockfd, optval = 1;
    struct sockaddr_in serveraddr;

    // create a socket fd
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    // eliminates "address already in use" error from bind
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,
                   sizeof(int)) < 0) {
        close(sockfd);
        return -1;
    }

    // setup server address
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);

    // bind socket to server address
    if (bind(sockfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int sock_bind_listen(int port) {
    int listenfd, optval = 1;
    struct sockaddr_in serveraddr;

    // create a socket fd
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    // eliminates "address already in use" error from bind
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,
                   sizeof(int)) < 0) {
        close(listenfd);
        return -1;
    }

    // setup server address
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);

    // bind socket to server address
    if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0) {
        close(listenfd);
        return -1;
    }

    // listen
    if (listen(listenfd, LISTENQ) < 0) {
        close(listenfd);
        return -1;
    }

    return listenfd;
}

bool setnonblock(int fd) {
    if (fd < 0)
        return false;
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        return false;
    return (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0);
}

ssize_t readn(int fd, void *buffer, size_t n) {
    ssize_t num_read; // # of bytes from last read()
    size_t tot_read;  // Total bytes read so far
    char *buf;        // for pointer arithmetic

    buf = buffer;
    for (tot_read = 0; tot_read < n;) {
        num_read = read(fd, buf, n - tot_read);

        if (num_read == 0)
            return tot_read; // EOF
        if (num_read == -1) {
            if (errno == EINTR)
                continue;
            else
                return -1; // error return
        }
        tot_read += num_read;
        buf += num_read;
    }
    return tot_read;
}

ssize_t writen(int fd, const void *buffer, size_t n) {
    ssize_t num_written; // # of bytes written by last write()
    size_t tot_written;  // total # of bytes written so far
    const char *buf;     //  buf pointer to constant character

    buf = buffer;
    for (tot_written = 0; tot_written < n;) {
        num_written = write(fd, buf, n - tot_written);

        if (num_written <= 0) {
            if (num_written == -1 && errno == EINTR)
                continue;
            else
                return -1; // error return
        }
        tot_written += num_written;
        buf += num_written;
    }
    return tot_written;
}


