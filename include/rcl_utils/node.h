#ifndef RCL_UTILS_NODE_H
#define RCL_UTILS_NODE_H

#include <rcl/rcl.h>
#include "rcl_utils/error.h"

#define NODE_NAMESPACE ""

rcl_node_t create_node(const char* name, rcl_context_t* context);
int destroy_node(rcl_node_t* node);

#endif