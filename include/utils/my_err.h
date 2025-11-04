#ifndef ERR_H
#define ERR_H

#include <stdarg.h>

/*
    error unrelated to sys call or library call.
    errno is not set.
    print error msg.
*/
void err_msg(const char* format, ...);
/*
    error related to sys call or library call.
    errno is set.
    print error msg.
*/
void err_msg_sys(const char* format, ...);
/*
    error unrelated to sys call or library call.
    errno is not set.
    print error msg and exit.
*/
void err_exit(const char* format, ...);
/*
    error related to sys call or library call.
    errno is set.
    print error msg and exit.
*/
void err_exit_sys(const char* format, ...);
/*
    error related to sys call or library call.
    It takes errno as a parameter.
    print error msg and exit.
*/
void err_exit_errn(int errnum, const char* format, ...);

// void error_exit(const char* msg);
// void perror_exit(const char* msg);
// void perror_exit_en(const char* msg, int err_num);

// int error_return(const char* msg);
// int perror_return(const char* msg);
// int perror_return_en(const char* msg, int err_num);

#endif
