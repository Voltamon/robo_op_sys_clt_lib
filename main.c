#include "rcl_utils/ros.h"
#include "rcl_utils/node.h"

#include "rcl_utils/pub.h"
#include "rcl_utils/sub.h"

#include "rcl_utils/server.h"
#include "rcl_utils/client.h"

#include "rcl_utils/spin.h"
#include "rcl_utils/iface.h"

#include <stdio.h>
#include <std_msgs/msg/string.h>
#include <std_srvs/srv/trigger.h>
#include <rosidl_runtime_c/string_functions.h>

int main(int argc, const char *const *argv) {
    ros_t ros = ros_init(argc, argv);
    if (ros.context.impl == NULL)
        return 1;

    rcl_node_t node = create_node("base_node_impl", &ros.context);
    if (node.impl == NULL) {
        ros_free(&ros);
        return 1;
    }

    const type_t* msg_type = ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);
    const rosidl_service_type_support_t* srv_type = ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs, srv, Trigger);

    pub_t pub = create_publisher(&node, "/count", msg_type);
    if (pub.impl == NULL) {
        destroy_node(&node);
        ros_free(&ros);
        return 1;
    }

    sub_t sub = create_subscription(&node, "/count", msg_type);
    if (sub.impl == NULL) {
        destroy_publisher(&pub, &node);

        destroy_node(&node);
        ros_free(&ros);
        return 1;
    }

    srv_t srv = create_service(&node, "/reset", srv_type);
    if (srv.impl == NULL) {
        destroy_publisher(&pub, &node);
        destroy_subscription(&sub, &node);

        destroy_node(&node);
        ros_free(&ros);
        return 1;
    }

    clt_t clt = create_client(&node, "/reset", srv_type);
    if (clt.impl == NULL) {
        destroy_publisher(&pub, &node);
        destroy_subscription(&sub, &node);

        destroy_service(&srv, &node);

        destroy_node(&node);
        ros_free(&ros);
        return 1;
    }

    sub_t* subs[] = {&sub};
    srv_t* srvs[] = {&srv};
    clt_t* clts[] = {&clt};

    void** entities[3] = {(void**) subs, (void**) clts, (void**) srvs};
    size_t capacities[3] = {1, 1, 1};

    spin_t spinner = create_spinner(&node, entities, capacities);
    if (spinner.timer == NULL) {
        destroy_publisher(&pub, &node);
        destroy_subscription(&sub, &node);

        destroy_service(&srv, &node);
        destroy_client(&clt, &node);

        destroy_node(&node);
        ros_free(&ros);
        return 1;
    }

    typedef struct {
      double count;
    } msg_t;

    field_map_t fields[] = {
      { "count", NUM, offsetof(msg_t, count), sizeof(double) },
    };

    int count = 0;
    size_t num_fields = 2;

    interface_t pub_msg, sub_msg;
    interface_type_t iface_type = create_interface(sizeof(msg_t), fields, num_fields);
    interface_init(&pub_msg);
    interface_init(&sub_msg);

    std_srvs__srv__Trigger_Request srv_req;
    std_srvs__srv__Trigger_Response srv_res;
    std_srvs__srv__Trigger_Request__init(&srv_req);
    std_srvs__srv__Trigger_Response__init(&srv_res);

    std_srvs__srv__Trigger_Request clt_req;
    std_srvs__srv__Trigger_Response clt_res;
    std_srvs__srv__Trigger_Request__init(&clt_req);
    std_srvs__srv__Trigger_Response__init(&clt_res);


    while(rcl_context_is_valid(&ros.context) && is_running()) {
        int status = spin_node(&spinner);

        if (status != 0)
            continue;

        if (spinner.spinner.timers[0]) {
            rcl_timer_call(spinner.timer);
            count++;

            if (count == 10) {
                int64_t seq_num;
                send_request(&clt, &clt_req, &seq_num);
            }

            msg_t* msg = (msg_t*)malloc(sizeof(msg_t));
            msg->count = count;
            pub_msg = serialize_interface(msg, &iface_type);

            publish_message(&pub, &pub_msg);
            fprintf(stdout, "\n-> %d\n", (int)msg->count);

            fflush(stdout);
            free(msg);
        }

        if (spinner.spinner.subscriptions[0]) {
            if (take_message(&sub, &sub_msg) == 0) {
              msg_t* msg = (msg_t*)deserialize_interface(&sub_msg, &iface_type);
              fprintf(stdout, "\n<- %d\n", (int)msg->count);

              fflush(stdout);
              free(msg);
            }
        }

        if (spinner.spinner.services[0]) {
            rmw_request_id_t req_id;

            if (!take_request(&srv, &req_id, &srv_req)) {
                count = 0;
                srv_res.success = true;

                rosidl_runtime_c__String__assign(&srv_res.message, "\n< - >");
                send_response(&srv, &req_id, &srv_res);
            }
        }

        if (spinner.spinner.clients[0]) {
            rmw_request_id_t req_id;

            if (!take_response(&clt, &req_id, &clt_res)) {
                fprintf(clt_res.success ? stdout : stderr, "%s\n", clt_res.message.data);
                fflush(stdout);
            }
        }
    }

    interface_fini(&pub_msg);
    interface_fini(&sub_msg);

    std_srvs__srv__Trigger_Request__fini(&srv_req);
    std_srvs__srv__Trigger_Response__fini(&srv_res);

    std_srvs__srv__Trigger_Request__fini(&clt_req);
    std_srvs__srv__Trigger_Response__fini(&clt_res);

    destroy_spinner(&spinner);
    destroy_publisher(&pub, &node);
    destroy_subscription(&sub, &node);

    destroy_service(&srv, &node);
    destroy_client(&clt, &node);

    destroy_node(&node);
    ros_free(&ros);
    return 0;
}
