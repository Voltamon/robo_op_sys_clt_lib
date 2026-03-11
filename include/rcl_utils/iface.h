#ifndef RCL_UTILS_IFACE_H
#define RCL_UTILS_IFACE_H

#include <stdlib.h>
#include <string.h>

#include <rcl/rcl.h>
#include "rcl_utils/cjson.h"

#include <std_msgs/msg/string.h>
#include <rosidl_runtime_c/string_functions.h>

#define interface_init(msg_ptr) std_msgs__msg__String__init(msg_ptr)
#define interface_fini(msg_ptr) std_msgs__msg__String__fini(msg_ptr)

typedef std_msgs__msg__String interface_t;

typedef enum {
    NUM,
    BOOL,
    STR,
} type_t;

typedef struct field_map {
    const char* name;
    type_t type;
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

#endif
