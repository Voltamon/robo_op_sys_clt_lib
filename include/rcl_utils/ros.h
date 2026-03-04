#ifndef RCL_UTILS_ROS_H
#define RCL_UTILS_ROS_H

#include <rcl/rcl.h>
#include "rcl_utils/error.h"

typedef struct ros {
    rcl_context_t context;
    rcl_init_options_t init_options;
} ros_t;

ros_t ros_init(int argc, const char *const *argv);  
int ros_free(ros_t* ros);

#endif