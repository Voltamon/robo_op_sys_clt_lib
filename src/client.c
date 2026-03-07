#include "rcl_utils/client.h"

clt_t create_client(node_t* node, const char* service_name, const clt_interface_t* interface) {
    clt_t clt = rcl_get_zero_initialized_client();
    clt_opts_t clt_opts = rcl_client_get_default_options();

    ret_t ret = rcl_client_init(&clt, node, interface, service_name, &clt_opts);
    if (check_rcl_ret(ret, "Client initialization failed"))
        return (clt_t) {0};

    return clt;
}

int send_request(clt_t* client, const void* request, int64_t* sequence_number) {
    if (!rcl_client_is_valid(client)) {
        fprintf(stderr, "Client is not valid\n");
        return 1;
    }

    if (request == NULL || sequence_number == NULL) {
        fprintf(stderr, "Request is NULL\n");
        return 1;
    }

    ret_t ret = rcl_send_request(client, request, sequence_number);
    return check_rcl_ret(ret, "Request send failed");
}

int take_response(clt_t* client, rmw_request_id_t* request_header, void* response) {
    if (!rcl_client_is_valid(client)) {
        fprintf(stderr, "Client is not valid\n");
        return 1;
    }

    if (request_header == NULL || response == NULL) {
        fprintf(stderr, "Response buffer is NULL\n");
        return 1;
    }

    ret_t ret = rcl_take_response(client, request_header, response);
    return check_rcl_ret(ret, "Response take failed");
}

int destroy_client(clt_t* client, node_t* node) {
    if (client == NULL) {
        fprintf(stderr, "Client is NULL\n");
        return 1;
    }

    if (node == NULL) {
        fprintf(stderr, "Node is NULL\n");
        return 1;
    }

    ret_t ret = rcl_client_fini(client, node);
    return check_rcl_ret(ret, "Client cleanup failed");
}
