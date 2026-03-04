#include "rcl_utils/ros.h"
#include "rcl_utils/node.h"

int main(int argc, const char *const *argv) {
    ros_t ros = ros_init(argc, argv);
    if (ros.context.impl == NULL) {
        return 1;
    }

    rcl_node_t node = create_node("base_node_impl", &ros.context);
    if (node.impl == NULL) {
        ros_free(&ros);
        return 1;
    }

    while(rcl_context_is_valid(&ros.context))
        sleep(1);

    destroy_node(&node);
    ros_free(&ros);
    return 0;
}