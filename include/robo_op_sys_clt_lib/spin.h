#ifndef ROBO_OP_SYS_CLT_LIB_SPIN_H
#define ROBO_OP_SYS_CLT_LIB_SPIN_H

#include "robo_op_sys_clt_lib/error.h"

#include <stdio.h>
#include <signal.h>

typedef rcl_wait_set_t spinner_t;
typedef rcl_ret_t ret_t;

typedef rcl_subscription_t sub_t;
typedef rcl_client_t clt_t;
typedef rcl_service_t srv_t;

typedef rcl_allocator_t alloc_t;
typedef rcl_context_t ctx_t;
typedef rcl_node_t node_t;

typedef rcl_guard_condition_t guard_t;
typedef rcl_guard_condition_options_t guard_opts_t;

typedef struct spin {
    spinner_t spinner;
    void*** entities;
    size_t* capacities;

    rcl_clock_t* clock;
    rcl_timer_t* timer;
    guard_t* guard;
} spin_t;

int is_running();

spin_t create_spinner(node_t *node, void*** entities, size_t* capacities);
int spin_node(spin_t* spinner);
int destroy_spinner(spin_t* spinner);

#endif
