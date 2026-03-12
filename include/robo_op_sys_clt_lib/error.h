#ifndef ROBO_OP_SYS_CLT_LIB_ERROR_H
#define ROBO_OP_SYS_CLT_LIB_ERROR_H

#include <stdio.h>
#include <unistd.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>

typedef rcl_ret_t ret_t;

int check_rcl_ret(ret_t code, const char* msg);

#endif
