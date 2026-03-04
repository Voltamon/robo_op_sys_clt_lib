#ifndef RCL_UTILS_SUB_H
#define RCL_UTILS_SUB_H

#include <rcl/rcl.h>
#include "rcl_utils/error.h"
#include <rosidl_runtime_c/message_type_support_struct.h>

rcl_subscription_t create_subscription(rcl_node_t* node, const char* topic, const rosidl_message_type_support_t* type_support);
int take_message(rcl_subscriber_t* subscriber, void* buffer);
int destroy_subscription(rcl_subscription_t* subscription);

#endif