# Prefix Scan

## Task

The task is to implement a work-efficient parallel prefix scan using the pthreads library. The Blelloch algorithm was used to compute the scan.

## Implementation

Prefix scan can be implemented using any operator that is associative and commutative. This project uses an operator that satisfies these criteria. This operator takes 2 numbers and an extra parameter(`-l`) that modifies the amount of times the operator loops.

The scan was computed in 2 stages : an up-stage followed by a down-stage. The complete binary tree that is used in this algorithm is maintained simply using an array. We operate under the assumption that the number of input values and threads are powers of 2 to simplify things. The work performed at each level of the tree is split up equally among all the threads, with a barrier that makes sure all threads complete their work before proceeding to the next level. If the number of elements at a certain level is less than the number of threads, some threads remain idle.

A custom barrier has also been implemented using semaphores. Either this, or the one offered by pthreads can be used.

## Requirements

- A C++ compiler that supports the C++17 standard
- Pthreads library

## Usage

The first line in the input file will contain a single integer *n* denoting the number of lines of input in the remainder of the input file. The following *n* lines will each contain a single integer. Some sample input files are present in the `inputs/` directory.

Running `make` will generate an executable named `prefix_scan` within the `bin` directory. This can be executed with the following command-line arguments:

- `-n <number of threads>`(int): The sequential version is called when 0 is specified. Note that using a single pthread (`-n 1`) is not the same, as it involves all the overheads of thread creation and teardown that would not be present in a truly sequential implementation.
- `-i <absolute path to input file>`(char*)
- `-o <absolute path to output file>`(char*)
- `-l <number of loops that the operator will do>`(int)
- `-s <use the custom semaphore barrier implementation, else default to pthreads barrier>`(optional)

The program also outputs the execution time in seconds to stdout.