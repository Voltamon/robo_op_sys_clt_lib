#include "rcl_utils/spin.h"

static volatile int running = 1;
static guard_t* term_g = NULL;

static void signal_handler(int sig) {
    running = 0;
    if(term_g != NULL)
        rcl_trigger_guard_condition(term_g);
}

int is_running() {
    return running;
}

spin_t create_spinner(node_t* node, void*** entities, size_t* capacities) {
    ret_t ret;
    spin_t spinner = {0};

    ctx_t* ctx = node->context;
    alloc_t alloc = rcl_get_default_allocator();

    if (node == NULL || !rcl_node_is_valid(node)) {
        fprintf(stderr, "Node invalid in create_spinner\n");
        return (spin_t) {0};
    }

    spinner.spinner = rcl_get_zero_initialized_wait_set();
    spinner.entities = entities;
    spinner.capacities = capacities;

    ret = rcl_wait_set_init(&spinner.spinner, capacities[0], 1, 1, capacities[1], capacities[2], 0, ctx, alloc);
    if (check_rcl_ret(ret, "Wait set initialization failed")) {
        destroy_spinner(&spinner);
        return (spin_t) {0};
    }

    spinner.clock = alloc.allocate(sizeof(rcl_clock_t), alloc.state);
    spinner.timer = alloc.allocate(sizeof(rcl_timer_t), alloc.state);
    spinner.guard = alloc.allocate(sizeof(guard_t), alloc.state);

    ret = rcl_clock_init(RCL_STEADY_TIME, spinner.clock, &alloc);
    if (check_rcl_ret(ret, "Clock initialization failed")) {
        destroy_spinner(&spinner);
        return (spin_t) {0};
    }

    *spinner.timer = rcl_get_zero_initialized_timer();
    ret = rcl_timer_init(spinner.timer, spinner.clock, ctx, RCL_MS_TO_NS(1000), NULL, alloc);
    if (check_rcl_ret(ret, "Timer initialization failed")) {
        destroy_spinner(&spinner);
        return (spin_t) {0};
    }

    *spinner.guard = rcl_get_zero_initialized_guard_condition();
    guard_opts_t guard_opts = rcl_guard_condition_get_default_options();
    ret = rcl_guard_condition_init(spinner.guard, ctx, guard_opts);
    if (check_rcl_ret(ret, "Guard condition initialization failed")) {
        destroy_spinner(&spinner);
        return (spin_t) {0};
    }

    running = 1;
    term_g = spinner.guard;
    signal(SIGINT, signal_handler);

    return spinner;
}

int spin_node(spin_t* spinner) {
    if (spinner == NULL) {
        fprintf(stderr, "Spinner is null\n");
        return 1;
    }

    rcl_wait_set_clear(&spinner->spinner);
    rcl_wait_set_add_timer(&spinner->spinner, spinner->timer, NULL);
    rcl_wait_set_add_guard_condition(&spinner->spinner, spinner->guard, NULL);

    if (spinner->capacities[0] > 0 && spinner->entities[0] != NULL) {
        sub_t** subs = (sub_t**) spinner->entities[0];
        for(size_t i = 0; i < spinner->capacities[0]; i++)
            rcl_wait_set_add_subscription(&spinner->spinner, subs[i], NULL);
    }

    if (spinner->capacities[1] > 0 && spinner->entities[1] != NULL) {
        clt_t** clients = (clt_t**) spinner->entities[1];
        for(size_t i = 0; i < spinner->capacities[1]; i++)
            rcl_wait_set_add_client(&spinner->spinner, clients[i], NULL);
    }

    if (spinner->capacities[2] > 0 && spinner->entities[2] != NULL) {
        srv_t** servers = (srv_t**) spinner->entities[2];
        for(size_t i = 0; i < spinner->capacities[2]; i++)
            rcl_wait_set_add_service(&spinner->spinner, servers[i], NULL);
    }

    ret_t ret = rcl_wait(&spinner->spinner, -1);
    if (check_rcl_ret(ret, "Wait_set failed"))
        return 1;

    return 0;
}

int destroy_spinner(spin_t* spinner) {
    ret_t ret;
    uint8_t sig = 0;
    alloc_t alloc = rcl_get_default_allocator();

    if (spinner == NULL) {
        fprintf(stderr, "Spinner is NULL\n");
        return 1;
    }

    if (spinner->clock) {
        ret = rcl_clock_fini(spinner->clock);
        if (check_rcl_ret(ret, "Clock cleanup failed"))
            sig = 1;
        alloc.deallocate(spinner->clock, alloc.state);
    }

    if (spinner->timer) {
        ret = rcl_timer_fini(spinner->timer);
        if (check_rcl_ret(ret, "Timer cleanup failed"))
            sig = 1;
        alloc.deallocate(spinner->timer, alloc.state);
    }

    if (spinner->guard) {
        ret = rcl_guard_condition_fini(spinner->guard);
        if (check_rcl_ret(ret, "Guard cleanup failed"))
            sig = 1;
        alloc.deallocate(spinner->guard, alloc.state);
    }

    ret = rcl_wait_set_fini(&spinner->spinner);
    if (check_rcl_ret(ret, "Wait set cleanup failed"))
        sig = 1;

    return sig;
}
