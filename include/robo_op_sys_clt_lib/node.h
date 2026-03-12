#ifndef ROBO_OP_SYS_CLT_LIB_NODE_H
#define ROBO_OP_SYS_CLT_LIB_NODE_H

#include <rcl/rcl.h>
#include "robo_op_sys_clt_lib/error.h"

#define NODE_NAMESPACE ""

typedef rcl_node_t node_t;
typedef rcl_node_options_t node_opts_t;
typedef rcl_context_t ctx_t;
typedef rcl_ret_t ret_t;

node_t create_node(const char* name, ctx_t* context);
int destroy_node(node_t* node);

#endif
