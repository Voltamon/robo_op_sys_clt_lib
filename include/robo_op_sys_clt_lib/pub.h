#ifndef ROBO_OP_SYS_CLT_LIB_PUB_H
#define ROBO_OP_SYS_CLT_LIB_PUB_H

#include <rcl/rcl.h>
#include "robo_op_sys_clt_lib/error.h"
#include <rosidl_runtime_c/message_type_support_struct.h>

typedef rcl_publisher_t pub_t;
typedef rcl_publisher_options_t pub_opts_t;
typedef rosidl_message_type_support_t msg_type_t;
typedef rcl_node_t node_t;
typedef rcl_ret_t ret_t;

pub_t create_publisher(node_t* node, const char* topic, const msg_type_t* type_support);
int publish_message(pub_t* publisher, const void* message);
int destroy_publisher(pub_t* publisher, node_t* node);

#endif
