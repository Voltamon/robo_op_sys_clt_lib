#include "rcl_utils/node.h"

rcl_node_t create_node(const char* name, rcl_context_t* context) {
    rcl_node_t node = rcl_get_zero_initialized_node();
    rcl_node_options_t node_opts = rcl_node_get_default_options();

    rcl_ret_t ret = rcl_node_init(&node, name, NODE_NAMESPACE, context, &node_opts);
    if (check_rcl_ret(ret, "Node initialization failed")) {
        destroy_node(&node);
        return (rcl_node_t) {0};
    }

    return node;
}

int destroy_node(rcl_node_t* node) {
    if (node == NULL) {
        fprintf(stderr, "Node is null\n");
        return 1;
    }

    if (rcl_node_is_valid(node)) {
        rcl_ret_t ret = rcl_node_fini(node);
        return check_rcl_ret(ret, "Node cleanup failed");
    }

    return 0;
}