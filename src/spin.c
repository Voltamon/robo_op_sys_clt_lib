#include "rcl_utils/spin.h"

spin_t create_spinner(node_t* node, void*** entities, size_t* capacities) {
    spin_t spinner;
    spinner.spinner = rcl_get_zero_initialized_wait_set();
    spinner.entities = entities;
    spinner.capacities = capacities;

    ctx_t* ctx = rcl_node_get_context(node);
    ret_t ret = rcl_wait_set_init(&spinner.spinner, capacities[0], 1, 1, capacities[1], capacities[2], 0, ctx);
    if (check_rcl_ret(ret, "Wait set initialization failed")) {
        // destroy_spinner(&spinner);
        return (spin_t) {0};
    }

    return spinner;
}

int spin_node(spin_t* spinner) {
    if (spinner == NULL) {
        fprintf(stderr, "Spinner is null\n");
        return 1;
    }
    
    rcl_wait_set_clear(&spinner->spinner);
    
    if (spinner->capacities[0] > 0 && spinner->entities[0] != NULL) {
        sub_t** subs = (sub_t**) spinner->entities[0];
        for(size_t i = 0; i < spinner->capacities[0]; i++)
            rcl_wait_set_add_subscription(&spinner->spinner, subs[i], NULL);
    }

    return 0;
}