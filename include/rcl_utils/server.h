#ifndef RCL_UTILS_SERVER_H
#define RCL_UTILS_SERVER_H

#include <rcl/rcl.h>
#include "rcl_utils/error.h"
#include <rosidl_runtime_c/service_type_support_struct.h>

typedef rcl_service_t srv_t;
typedef rcl_service_options_t srv_opts_t;
typedef rosidl_service_type_support_t srv_interface_t;
typedef rcl_node_t node_t;
typedef rcl_ret_t ret_t;

srv_t create_service(node_t* node, const char* service_name, const srv_interface_t* type_support);
int take_request(srv_t* service, rmw_request_id_t* req_header, void* request);
int send_response(srv_t* service, rmw_request_id_t* req_header, void* response);
int destroy_service(srv_t* service, node_t* node);

#endif
