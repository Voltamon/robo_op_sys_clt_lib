#ifndef ROBO_OP_SYS_CLT_LIB_CLIENT_H
#define ROBO_OP_SYS_CLT_LIB_CLIENT_H

#include <rcl/rcl.h>
#include "robo_op_sys_clt_lib/error.h"
#include <rosidl_runtime_c/service_type_support_struct.h>

typedef rcl_client_t clt_t;
typedef rcl_client_options_t clt_opts_t;
typedef rosidl_service_type_support_t clt_interface_t;
typedef rcl_node_t node_t;
typedef rcl_ret_t ret_t;

clt_t create_client(node_t* node, const char* service_name, const clt_interface_t* interface);
int send_request(clt_t* client, const void* request, int64_t* sequence_number);
int take_response(clt_t* client, rmw_request_id_t* request_header, void* response);
int destroy_client(clt_t* client, node_t* node);

#endif
