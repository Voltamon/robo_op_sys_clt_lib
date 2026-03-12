#include "robo_op_sys_clt_lib/pub.h"

pub_t create_publisher(node_t* node, const char* topic, const msg_type_t* type_support) {
    pub_t pub = rcl_get_zero_initialized_publisher();
    pub_opts_t pub_opts = rcl_publisher_get_default_options();

    ret_t ret = rcl_publisher_init(&pub, node, type_support, topic, &pub_opts);
    if (check_rcl_ret(ret, "Publisher initiailization failed")) {
        destroy_publisher(&pub, node);
        return (pub_t) {0};
    }

    return pub;
}

int publish_message(pub_t* publisher, const void* message) {
    if (!rcl_publisher_is_valid(publisher)) {
        fprintf(stderr, "Publisher not valid\n");
        return 1;
    }

    ret_t ret = rcl_publish(publisher, message, NULL);
    return check_rcl_ret(ret, "Publishing failed");
}

int destroy_publisher(pub_t* publisher, node_t* node) {
    if (publisher == NULL) {
        fprintf(stderr, "Publisher is null\n");
        return 1;
    }

    if (node == NULL) {
        fprintf(stderr, "Node is null\n");
        return 1;
    }

    if (rcl_publisher_is_valid(publisher)) {
        ret_t ret = rcl_publisher_fini(publisher, node);
        return check_rcl_ret(ret, "Publisher cleanup failed");
    }

    return 0;
}
