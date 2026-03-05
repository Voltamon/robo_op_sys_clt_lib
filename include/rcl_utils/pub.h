#ifndef RCL_UTILS_PUB_H
#define RCL_UTILS_PUB_H

#include <rcl/rcl.h>
#include "rcl_utils/error.h"
#include <rosidl_runtime_c/message_type_support_struct.h>

typedef rcl_publisher_t pub_t;
typedef rcl_publisher_options_t pub_opts_t;
typedef rosidl_message_type_support_t interface_t;
typedef rcl_node_t node_t;
typedef rcl_ret_t ret_t;

pub_t create_publisher(node_t* node, const char* topic, const interface_t* type_support);
int publish_message(pub_t* publisher, const void* message);
int destroy_publisher(pub_t* publisher, node_t* node);

#endif