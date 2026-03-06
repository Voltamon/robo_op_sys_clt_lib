#ifndef RCL_UTILS_SUB_H
#define RCL_UTILS_SUB_H

#include <rcl/rcl.h>
#include "rcl_utils/error.h"
#include <rosidl_runtime_c/message_type_support_struct.h>

typedef rcl_subscription_t sub_t;
typedef rcl_subscription_options_t sub_opts_t;
typedef rosidl_message_type_support_t interface_t;
typedef rcl_node_t node_t;
typedef rcl_ret_t ret_t;

sub_t create_subscription(node_t* node, const char* topic, const interface_t* type_support);
int take_message(sub_t* subscriber, void* buffer);
int destroy_subscription(sub_t* subscriber, node_t* node);

#endif
