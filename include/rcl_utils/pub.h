#ifndef RCL_UTILS_PUB_H
#define RCL_UTILS_PUB_H

#include <rcl/rcl.h>
#include "rcl_utils/error.h"
#include <rosidl_runtime_c/message_type_support_struct.h>

rcl_publisher_t create_publisher(rcl_node_t* node, const char* topic, const rosidl_message_type_support_t* type_support);
int publish_message(rcl_publisher_t* publisher, const void* message);
int destroy_publisher(rcl_publisher_t* publisher);

#endif