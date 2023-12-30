# Multithreaded Sum Calculation Program

This C program demonstrates multithreading and synchronization using POSIX threads (pthread) to calculate the sum of square roots within a specified range.

## Program Details

The program calculates the sum of square roots of numbers in a user-defined range using multiple threads. Three different methods are implemented for updating a global sum variable (`global_sqrt_sum`):

1. **Method 1 (d = 1):** All threads update `global_sqrt_sum` concurrently without any critical sections. No mutex variables are used.

2. **Method 2 (d = 2):** All threads update `global_sqrt_sum` serially. A single shared mutex variable is used to protect the updates.

3. **Method 3 (d = 3):** Each thread uses a local variable (`local_sqrt_sum`) to calculate the sum of square roots. After local sums are calculated, each thread adds its local sum to `global_sqrt_sum`. A single shared mutex variable is used to protect the updates.

## Usage

```bash
gcc -pthread -o project3.out project3.c -lm
./project3.out <a> <b> <num_threads> <method>