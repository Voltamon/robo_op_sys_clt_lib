#ifndef ROBO_OP_SYS_CLT_LIB_SUB_H
#define ROBO_OP_SYS_CLT_LIB_SUB_H

#include <rcl/rcl.h>
#include "robo_op_sys_clt_lib/error.h"
#include <rosidl_runtime_c/message_type_support_struct.h>

typedef rcl_subscription_t sub_t;
typedef rcl_subscription_options_t sub_opts_t;
typedef rosidl_message_type_support_t msg_type_t;
typedef rcl_node_t node_t;
typedef rcl_ret_t ret_t;

sub_t create_subscription(node_t* node, const char* topic, const msg_type_t* type_support);
int take_message(sub_t* subscriber, void* buffer);
int destroy_subscription(sub_t* subscriber, node_t* node);

#endif
