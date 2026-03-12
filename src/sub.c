#include "rcl_utils/sub.h"

sub_t create_subscription(node_t* node, const char* topic, const msg_type_t* type_support) {
    sub_t sub = rcl_get_zero_initialized_subscription();
    sub_opts_t sub_opts = rcl_subscription_get_default_options();

    ret_t ret = rcl_subscription_init(&sub, node, type_support, topic, &sub_opts);
    if (check_rcl_ret(ret, "Subscription initialization failed")) {
        destroy_subscription(&sub, node);
        return (sub_t) {0};
    }

    return sub;
}

int take_message(sub_t* subscriber, void* buffer) {
    if (!rcl_subscription_is_valid(subscriber)) {
        fprintf(stderr, "Subscription not valid\n");
        return 1;
    }

    if (buffer == NULL) {
        fprintf(stderr, "Buffer is null\n");
        return 1;
    }

    ret_t ret = rcl_take(subscriber, buffer, NULL, NULL);
    return check_rcl_ret(ret, "Taking message failed");
}

int destroy_subscription(sub_t* subscriber, node_t* node) {
    if (subscriber == NULL) {
        fprintf(stderr, "Subscription is null\n");
        return 1;
    }

    if (node == NULL) {
        fprintf(stderr, "Node is null\n");
        return 1;
    }

    if (rcl_subscription_is_valid(subscriber)) {
        ret_t ret = rcl_subscription_fini(subscriber, node);
        return check_rcl_ret(ret, "Subscription cleanup failed");
    }

    return 0;
}
