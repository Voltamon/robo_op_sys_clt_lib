#include "rcl_utils/server.h"

srv_t create_service(node_t* node, const char* service_name, const srv_interface_t* type_support) {
    srv_t srv = rcl_get_zero_initialized_service();
    srv_opts_t srv_opts = rcl_service_get_default_options();

    ret_t ret = rcl_service_init(&srv, node, type_support, service_name, &srv_opts);
    if (check_rcl_ret(ret, "Service rcl_get_zero_initialized_service")) {
        return (srv_t) {0};
    }

    return srv;
}

int take_request(srv_t* service, rmw_request_id_t* req_header, void* request) {
    if (!rcl_service_is_valid(service)) {
        fprintf(stderr, "Service is invalid\n");
        return 1;
    }

    if (req_header == NULL || request == NULL) {
        fprintf(stderr, "Request is NULL\n");
        return 1;
    }

    ret_t ret = rcl_take_request(service, req_header, request);
    return check_rcl_ret(ret, "Taking request failed");
}

int send_response(srv_t* service, rmw_request_id_t* req_header, void* response) {
    if (!rcl_service_is_valid(service)) {
        fprintf(stderr, "Service is invalid\n");
        return 1;
    }

    if (req_header == NULL || response == NULL) {
        fprintf(stderr, "Response is NULL\n");
        return 1;
    }

    ret_t ret = rcl_send_response(service, req_header, response);
    return check_rcl_ret(ret, "Sending response failed");
}

int destroy_service(srv_t* service, node_t* node) {
    if (service == NULL) {
        fprintf(stderr, "Service is NULL\n");
        return 1;
    }

    if (node == NULL) {
        fprintf(stderr, "Node is NULL\n");
        return 1;
    }

    if (rcl_service_is_valid(service)) {
        ret_t ret = rcl_service_fini(service, node);
        return check_rcl_ret(ret, "Service cleanup failed");
    }

    return 0;
}
