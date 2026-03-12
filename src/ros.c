#include "robo_op_sys_clt_lib/ros.h"

ros_t ros_init(int argc, const char *const *argv) {
    ros_t ros;
    ros.context = rcl_get_zero_initialized_context();
    ros.init_options = rcl_get_zero_initialized_init_options();

    ret_t ret;
    alloc_t alloc = rcl_get_default_allocator();

    ret = rcl_init_options_init(&ros.init_options, alloc);
    if (check_rcl_ret(ret, "RCL initialization failed")) {
        ros_free(&ros);
        return (ros_t) {0};
    }

    ret = rcl_init(argc, argv, &ros.init_options, &ros.context);
    if (check_rcl_ret(ret, "RCL initialization failed")) {
        ros_free(&ros);
        return (ros_t) {0};
    }

    return ros;
}

int ros_free(ros_t* ros) {
    if (ros == NULL) {
        fprintf(stderr, "ROS is null\n");
        return 1;
    }

    ret_t ret;
    uint8_t error = 0;

    ret = rcl_shutdown(&ros->context);
    if (check_rcl_ret(ret, "RCL shutdown failed"))
        error = 1;

    ret = rcl_init_options_fini(&ros->init_options);
    if (check_rcl_ret(ret, "Init_Options cleanup failed"))
        error = 1;

    ret = rcl_context_fini(&ros->context);
    if (check_rcl_ret(ret, "Context cleanup failed"))
        error = 1;

    return error;
}
