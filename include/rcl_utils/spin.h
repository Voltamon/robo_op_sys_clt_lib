#ifndef RCL_UTILS_SPIN_H
#define RCL_UTILS_SPIN_H

typedef rcl_wait_set_t spinner_t;
typedef rcl_subscriber_t sub_t;

typedef struct spin {
    spinner_t spinner;
    void*** entities;
    size_t* capacities;
} spin_t;

spin_t create_spinner(node_t *node, void*** entities, size_t* capacities);
int spin_node(spin_t* spinner);
int destroy_spinner(spin_t* spinner);

#endif