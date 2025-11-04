#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils/my_err.h"

#define ERRBUFMAX 512

static void print_error(bool use_errno, int errn, const char* fmt, va_list ap) {
    char errbuf[ERRBUFMAX];

    vsnprintf(errbuf, ERRBUFMAX, fmt, ap);
    int n = strlen(errbuf);
    if (use_errno)
        snprintf(errbuf + n, ERRBUFMAX - n, ": %s", strerror(errn));
    strcat(errbuf, "\n");

    fputs(errbuf, stderr);

    return;
}

void err_msg(const char* format, ...) {
    va_list ap;

    va_start(ap, format);
    print_error(false, 0, format, ap);
    va_end(ap);
    return;
}

void err_msg_sys(const char* format, ...) {
    va_list ap;
    int errn = errno;

    va_start(ap, format);
    print_error(true, errn, format, ap);
    va_end(ap);
    return;
}

void err_exit(const char* format, ...) {
    va_list ap;

    va_start(ap, format);
    print_error(false, 0, format, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void err_exit_sys(const char* format, ...) {
    va_list ap;
    int errn = errno;

    va_start(ap, format);
    print_error(true, errn, format, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void err_exit_errn(int errnum, const char* format, ...) {
    va_list ap;

    va_start(ap, format);
    print_error(true, errnum, format, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

// void error_exit(const char* msg) {
//     // fprintf(stderr, msg);
//     // fprintf(stderr, "\n");
//     fprintf(stderr, "%s\n", msg);
//     exit(EXIT_FAILURE);
// }

// void perror_exit(const char* msg) {
//     perror(msg);
//     exit(EXIT_FAILURE);
// }

// void perror_exit_en(const char* msg, int err_num) {
//     errno = err_num;
//     perror(msg);
//     exit(EXIT_FAILURE);
// }

// int error_return(const char* msg) {
//     fprintf(stderr, "%s\n", msg);
//     return -1;
// }

// int perror_return(const char* msg) {
//     perror(msg);
//     return -1;
// }

// int perror_return_en(const char* msg, int err_num) {
//     errno = err_num;
//     perror(msg);
//     return -1;
// }