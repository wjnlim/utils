#ifndef SOCK_HELPER_H
#define SOCK_HELPER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>

typedef struct sockaddr SA;
#define LISTENQ 1024

int sock_connect(const char *, int);
int sock_bind(int port);
int sock_bind_listen(int);
bool setnonblock(int fd);

ssize_t readn(int fd, void *buffer, size_t n);
ssize_t writen(int fd, const void *buffer, size_t n);

#endif