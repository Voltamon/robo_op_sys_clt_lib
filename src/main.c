#include "rcl_utils/ros.h"
#include "rcl_utils/node.h"
#include "rcl_utils/pub.h"
#include "rcl_utils/sub.h"
#include "rcl_utils/spin.h"

#include <stdio.h>
#include <std_msgs/msg/string.h>
#include <rosidl_runtime_c/string_functions.h>

int main(int argc, const char *const *argv) {
    ros_t ros = ros_init(argc, argv);
    if (ros.context.impl == NULL)
        return 1;

    rcl_node_t node = create_node("base_node_impl", &ros.context);
    if (node.impl == NULL) {
        ros_free(&ros);
        return 1;
    }

    const rosidl_message_type_support_t* type = ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);

    pub_t pub = create_publisher(&node, "/test", type);
    if (pub.impl == NULL) {
        destroy_node(&node);
        ros_free(&ros);
        return 1;
    }

    sub_t sub = create_subscription(&node, "/test", type);
    if (sub.impl == NULL) {
        destroy_publisher(&pub, &node);
        destroy_node(&node);
        ros_free(&ros);
        return 1;
    }

    sub_t* subs[] = {&sub};
    void** entities[3] = {(void**) subs, NULL, NULL};
    size_t capacities[3] = {1, 0, 0};

    spin_t spinner = create_spinner(&node, entities, capacities);
    if (spinner.timer == NULL) {
        destroy_publisher(&pub, &node);
        destroy_subscription(&sub, &node);
        destroy_node(&node);
        ros_free(&ros);
        return 1;
    }

    std_msgs__msg__String pub_msg, sub_msg;
    std_msgs__msg__String__init(&pub_msg);
    std_msgs__msg__String__init(&sub_msg);

    int count = 0;

    while(rcl_context_is_valid(&ros.context) && is_running()) {
        int status = spin_node(&spinner);

        if (status == 0) {
            if (spinner.spinner.timers[0] != NULL) {
                rcl_timer_call(spinner.timer);

                char buffer[50];
                snprintf(buffer, sizeof(buffer), "%d", count++);
                rosidl_runtime_c__String__assign(&pub_msg.data, buffer);

                publish_message(&pub, &pub_msg);
                printf("[Published]: %s\n", pub_msg.data.data);
                fflush(stdout);
            }

            if (spinner.spinner.subscriptions[0] != NULL) {
                if (take_message(&sub, &sub_msg) == 0) {
                    printf("[Received]: %s\n\n", sub_msg.data.data);
                    fflush(stdout);
                }
            }
        }
    }

    std_msgs__msg__String__fini(&pub_msg);
    std_msgs__msg__String__fini(&sub_msg);

    destroy_spinner(&spinner);
    destroy_publisher(&pub, &node);
    destroy_subscription(&sub, &node);
    destroy_node(&node);
    ros_free(&ros);
    return 0;
}
