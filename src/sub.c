#include "rcl_utils/sub.h"

typedef rosidl_message_type_support_t interface;

rcl_subscription_t create_subscription(rcl_node_t* node, const char* topic, const interface* type_support) {
    rcl_subscription_t sub = rcl_get_zero_initialized_subscription();
    rcl_subscription_options_t sub_opts = rcl_subscription_get_default_options();

    rcl_ret_t ret = rcl_subscription_init(&sub, node, type_support, topic, &sub_opts);
    if (check_rcl_ret(ret, "Subscription initialization failed")) {
        destroy_subscription(&sub);
        return (rcl_subscription_t) {0};
    }

    return sub;
}

int take_message(rcl_subscription_t* subscriber, void* buffer) {
    if (!rcl_subscription_is_valid(subscriber)) {
        fprintf(stderr, "Subscription not valid\n");
        return 1;
    }

    if (buffer == NULL) {
        fprintf(stderr, "Buffer is null\n");
        return 1;
    }

    rcl_ret_t ret = rcl_take(subscriber, buffer, NULL, NULL);
    return check_rcl_ret(ret, "Taking message failed");
}

int destroy_subscription(rcl_subscription_t* subscription, rcl_node_t* node) {
    if (subscription == NULL) {
        fprintf(stderr, "Subscription is null\n");
        return 1;
    }

    if (node == NULL) {
        fprintf(stderr, "Node is null\n");
        return 1;
    }

    if (rcl_subscription_is_valid(subscription)) {
        rcl_ret_t ret = rcl_subscription_fini(subscription, node);
        return check_rcl_ret(ret, "Subscription cleanup failed");
    }

    return 0;
}
