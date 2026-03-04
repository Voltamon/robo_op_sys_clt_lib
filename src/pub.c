#include "rcl_utils/pub.h"

typedef rosidl_message_type_support_t interface;

rcl_publisher_t create_publisher(rcl_node_t* node, const char* topic, const interface* type_support) {
    rcl_publisher_t pub = rcl_get_zero_initialized_publisher();
    rcl_publisher_options_t pub_opts = rcl_publisher_get_default_options();

    rcl_ret_t ret = rcl_publisher_init(&pub, node, type_support, topic, &pub_opts);
    if (check_rcl_ret(ret, "Publisher initiailization failed")) {
        destroy_publisher(&pub, node);
        return (rcl_publisher_t) {0};
    }

    return pub;
}

int publish_message(rcl_publisher_t *publisher, const void *message) {
    if (!rcl_publisher_is_valid(publisher)) {
        fprintf(stderr, "Publisher not valid\n");
        return 1;
    }

    rcl_ret_t ret = rcl_publish(publisher, message, NULL);
    return check_rcl_ret(ret, "Publishing failed");
}

int destroy_publisher(rcl_publisher_t* publisher, rcl_node_t* node) {
    if (publisher == NULL) {
        fprintf(stderr, "Publisher is null\n");
        return 1;
    }

    if (node == NULL) {
        fprintf(stderr, "Node is null\n");
        return 1;
    }

    if (rcl_publisher_is_valid(publisher)) {
        rcl_ret_t ret = rcl_publisher_fini(publisher, node);
        return check_rcl_ret(ret, "Publisher cleanup failed");
    }

    return 0;
}