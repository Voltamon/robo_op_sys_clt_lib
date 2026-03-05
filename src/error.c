#include "rcl_utils/error.h"
#include <stdio.h>

int check_rcl_ret(ret_t code, const char* msg) {
    if (code == RCL_RET_OK)
        return 0;

    if(rcl_error_is_set()) {
        fprintf(stderr, "%s\n", rcl_get_error_state()->message);
        rcl_reset_error();
    }

    fprintf(stderr, "%s\n", msg);
    return 1;
}