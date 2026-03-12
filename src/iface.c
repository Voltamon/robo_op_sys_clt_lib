#include "robo_op_sys_clt_lib/iface.h"

interface_type_t create_interface(size_t struct_size, field_map_t* field_map, size_t field_count) {
    interface_type_t interface_type;

    interface_type.struct_size = struct_size;
    interface_type.field_map = field_map;
    interface_type.field_count = field_count;

    return interface_type;
}

interface_t serialize_interface(void* gen_struct, interface_type_t* interface_type) {
    interface_t msg;
    interface_init(&msg);

    if (!gen_struct || !interface_type || interface_type->field_count == 0) {
        fprintf(stderr, "Interface type is invalid\n");
        return msg;
    }

    cjson* json = cjson_create_object();
    if (!json) {
        fprintf(stderr, "JSON creation failed for interface\n");
        return msg;
    }

    const char* raw_mem = (const char*)gen_struct;
    for (size_t i = 0; i < interface_type->field_count; i++) {
        const void* var_ptr = (const void*)(raw_mem + interface_type->field_map[i].offset);

        switch (interface_type->field_map[i].type) {
            case NUM:
                cjson_add_number_to_object(json, interface_type->field_map[i].name, *(const double*)var_ptr);
                break;
            case BOOL:
                cjson_add_bool_to_object(json, interface_type->field_map[i].name, *(const int*)var_ptr);
                break;
            case STR:
                cjson_add_string_to_object(json, interface_type->field_map[i].name, *(const char**)var_ptr);
                break;
        }
    }

    char* json_str = cjson_print_unformatted(json);
    if (json_str) {
        rosidl_runtime_c__String__assign(&msg.data, json_str);
        free(json_str);
    }

    cjson_delete(json);
    return msg;
}

void* deserialize_interface(interface_t* msg, interface_type_t* iface_type) {
    if (!msg || !iface_type || !msg->data.data) {
        fprintf(stderr, "Interface is null\n");
        return NULL;
    }

    void* out_struct = calloc(1, iface_type->struct_size);
    if (!out_struct) {
        fprintf(stderr, "Memory allocation failed for interface\n");
        return NULL;
    }

    cjson* json = cjson_parse(msg->data.data);
    if (!json) {
        fprintf(stderr, "JSON parse failed for interface\n");

        free(out_struct);
        return NULL;
    }

    char* raw_mem = (char*)out_struct;
    for (size_t i = 0; i < iface_type->field_count; i++) {
        cjson* item = cjson_get_object_item_case_sensitive(json, iface_type->field_map[i].name);
        if (!item)
            continue;

        void* var_ptr = (void*)(raw_mem + iface_type->field_map[i].offset);
        switch (iface_type->field_map[i].type) {
            case NUM:
                *(double*)var_ptr = cjson_get_number_value(item);
                break;
            case BOOL:
                if (cjson_is_bool(item))
                    *(int*)var_ptr = cjson_is_true(item);
                break;
            case STR:
                *(char**)var_ptr = strdup(cjson_get_string_value(item));
                break;
        }

    }

    cjson_delete(json);
    return out_struct;
}

int destroy_interface(interface_type_t* interface_type) {
    if (!interface_type)
        return 1;

    // free(interface_type->field_map);
    return 0;
}

int success_response(response_t* response, char* message) {
    if (!response) {
        fprintf(stderr, "Response is null\n");
        return 1;
    }

    response->success = true;
    rosidl_runtime_c__String__assign(&response->message, message);
    return 0;
}

int error_response(response_t* response, char* message) {
    if (!response) {
        fprintf(stderr, "Response is null\n");
        return 1;
    }

    response->success = false;
    rosidl_runtime_c__String__assign(&response->message, message);
    return 0;
}

msg_type_t* get_msg_type_support() {
    return ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String);
}

srv_type_t* get_srv_type_support() {
    return ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs, srv, Trigger);
}
