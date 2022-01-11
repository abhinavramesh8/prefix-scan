#pragma once

#include "operators.h"
#include <stdlib.h>
#include <pthread.h>
#include <barrier.h>

struct prefix_scan_args_t {
  int*               input_vals;
  int*               output_vals;
  int*               tree;
  Barrier*           barrier;
  int                n_vals;
  int                n_threads;
  int                t_id;
  int (*op)(int, int, int);
  int n_loops;
};

prefix_scan_args_t* alloc_args(int n_threads);

int* alloc_tree(int n_vals);

Barrier* alloc_barrier(bool spin, int n_threads);

int next_power_of_two(int x);

void fill_args(prefix_scan_args_t *args,
               int n_threads,
               int n_vals,
               int *inputs,
               int *outputs,
               int *tree,
               Barrier *barrier,
               int (*op)(int, int, int),
               int n_loops);
