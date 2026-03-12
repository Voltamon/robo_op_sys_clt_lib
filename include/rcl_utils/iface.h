#ifndef RCL_UTILS_IFACE_H
#define RCL_UTILS_IFACE_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <rcl/rcl.h>
#include "rcl_utils/cjson.h"
#include "rcl_utils/error.h"

#include <std_msgs/msg/string.h>
#include <std_srvs/srv/trigger.h>
#include <rosidl_runtime_c/string_functions.h>

#define interface_init(msg_ptr) std_msgs__msg__String__init(msg_ptr)
#define interface_fini(msg_ptr) std_msgs__msg__String__fini(msg_ptr)

#define request_init(msg_ptr) std_srvs__srv__Trigger_Request__init(msg_ptr)
#define request_fini(msg_ptr) std_srvs__srv__Trigger_Request__fini(msg_ptr)

#define response_init(msg_ptr) std_srvs__srv__Trigger_Response__init(msg_ptr)
#define response_fini(msg_ptr) std_srvs__srv__Trigger_Response__fini(msg_ptr)

typedef std_msgs__msg__String interface_t;
typedef std_srvs__srv__Trigger_Request request_t;
typedef std_srvs__srv__Trigger_Response response_t;

typedef rosidl_message_type_support_t msg_type_t;
typedef rosidl_service_type_support_t srv_type_t;

typedef enum {
    NUM,
    BOOL,
    STR,
} field_type_t;

typedef struct field_map {
    const char* name;
    field_type_t type;
    size_t offset;
    size_t size;
} field_map_t;

typedef struct interface_type {
    size_t struct_size;
    field_map_t* field_map;
    size_t field_count;
} interface_type_t;

interface_type_t create_interface(size_t struct_size, field_map_t* field_map, size_t field_count);
interface_t serialize_interface(void* gen_struct, interface_type_t* interface_type);
void* deserialize_interface(interface_t* interface, interface_type_t* interface_type);
int destroy_interface(interface_type_t* interface_type);

int success_response(response_t* response, char* message);
int error_response(response_t* response, char* message);

msg_type_t* get_msg_type_support();
srv_type_t* get_srv_type_support();

#endif
