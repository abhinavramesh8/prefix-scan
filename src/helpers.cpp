#include "helpers.h"

prefix_scan_args_t* alloc_args(int n_threads) {
  return (prefix_scan_args_t*) malloc(n_threads * sizeof(prefix_scan_args_t));
}

int* alloc_tree(int n_vals) {
    return (int*) malloc(n_vals * sizeof(int));
}

Barrier* alloc_barrier(bool spin, int n_threads) {
    return (spin ? static_cast<Barrier*>(new SemaphoreBarrier(n_threads)) : 
            static_cast<Barrier*>(new PthreadBarrier(n_threads)));
}

int next_power_of_two(int x) {
    int pow = 1;
    while (pow < x) {
        pow *= 2;
    }
    return pow;
}

void fill_args(prefix_scan_args_t *args,
               int n_threads,
               int n_vals,
               int *inputs,
               int *outputs,
               int *tree,
               Barrier *barrier,
               int (*op)(int, int, int),
               int n_loops) {
    for (int i = 0; i < n_threads; ++i) {
        args[i] = {inputs, outputs, tree, barrier, n_vals,
                   n_threads, i, op, n_loops};
    }
}