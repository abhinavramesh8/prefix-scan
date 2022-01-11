#include "prefix_scan.h"
#include "helpers.h"

using std::min;

void* compute_prefix_scan(void *a)
{
    /* Assumptions:
     * n_vals is a power of 2
     * n_threads is a power of 2 */
    
    prefix_scan_args_t *args = (prefix_scan_args_t *)a;
    
    int n_leaves = args->n_vals >> 1;
    
    // up-sweep
    int n_threads_reqd = min(args->n_threads, n_leaves);
    if (args->t_id < n_threads_reqd) {
        int n_partial_scans_per_thread = n_leaves / n_threads_reqd;
        int i_start = n_leaves + args->t_id * n_partial_scans_per_thread;
        int i_end = i_start + n_partial_scans_per_thread - 1;
        int j_start = 2 * (i_start - n_leaves);
        for (int i = i_start, j = j_start; i <= i_end; i++, j += 2) {
            args->tree[i] = args->op(args->input_vals[j], 
                                     args->input_vals[j + 1], 
                                     args->n_loops); 
        }
    }
    args->barrier->wait();
    
    for (int n_partial_scans = n_leaves >> 1; n_partial_scans >= 1; n_partial_scans >>= 1) {
        n_threads_reqd = min(n_partial_scans, args->n_threads);
        if (args->t_id < n_threads_reqd) {
            int n_partial_scans_per_thread = n_partial_scans / n_threads_reqd;
            int i_start = n_partial_scans + args->t_id * n_partial_scans_per_thread;
            int i_end = i_start + n_partial_scans_per_thread - 1;
            for (int i = i_start; i <= i_end; i++) {
                int lchild_idx = 2 * i, rchild_idx = lchild_idx + 1;
                args->tree[i] = args->op(args->tree[lchild_idx], 
                                         args->tree[rchild_idx], 
                                         args->n_loops);
            }
        }
        args->barrier->wait();
    }
    
    // down-sweep
    if (args->t_id == 0) {
        args->tree[1] = 0;
    }
    
    for (int n_prefix_scans = 1; n_prefix_scans < n_leaves; n_prefix_scans <<= 1) {
        n_threads_reqd = min(n_prefix_scans, args->n_threads);
        if (args->t_id < n_threads_reqd) {
            int n_prefix_scans_per_thread = n_prefix_scans / n_threads_reqd;
            int i_start = n_prefix_scans + args->t_id * n_prefix_scans_per_thread;
            int i_end = i_start + n_prefix_scans_per_thread - 1;
            for (int i = i_start; i <= i_end; i++) {
                int lchild_idx = 2 * i, rchild_idx = lchild_idx + 1;
                args->tree[rchild_idx] = args->op(args->tree[i], 
                                                  args->tree[lchild_idx], 
                                                  args->n_loops);
                args->tree[lchild_idx] = args->tree[i];
            }
        }
        args->barrier->wait();
    }
    
    n_threads_reqd = min(n_leaves, args->n_threads);
    if (args->t_id < n_threads_reqd) {
        int n_prefix_scans_per_thread = n_leaves / n_threads_reqd;
        int i_start = n_leaves + args->t_id * n_prefix_scans_per_thread;
        int i_end = i_start + n_prefix_scans_per_thread - 1;
        int j_start = 2 * (i_start - n_leaves);
        for (int i = i_start, j = j_start; i <= i_end; i++, j += 2) {
            args->output_vals[j] = args->op(args->input_vals[j], 
                                            args->tree[i], 
                                            args->n_loops);
            args->output_vals[j + 1] = args->op(args->input_vals[j + 1], 
                                                args->output_vals[j], 
                                                args->n_loops);
        }
    }
     
    return NULL;
}
