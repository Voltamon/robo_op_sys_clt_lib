#ifndef ROBO_OP_SYS_CLT_LIB_ROS_H
#define ROBO_OP_SYS_CLT_LIB_ROS_H

#include <rcl/rcl.h>
#include "robo_op_sys_clt_lib/error.h"

typedef rcl_context_t ctx_t;
typedef rcl_init_options_t init_opts_t;
typedef rcl_allocator_t alloc_t;
typedef rcl_ret_t ret_t;

typedef struct ros {
    ctx_t context;
    init_opts_t init_options;
} ros_t;

ros_t ros_init(int argc, const char *const *argv);
int ros_free(ros_t* ros);

#endif
