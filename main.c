#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


#define MAX_THREADS 32

// Global variables
double global_sqrt_sum = 0;
long long int a, b;
int num_threads, method;
pthread_mutex_t mutex;

// Function to calculate the square root sum for a given range
void *calculate_sqrt_sum(void *arg)
{
    long long int start = *((long long int *)arg);
    long long int end = start + (b - a) / num_threads;

    double local_sqrt_sum = 0;

    // Update global_sqrt_sum based on the chosen method
    switch (method)
    {
    case 1:
        // Method 1: No synchronization
        for (long long int x = start; x <= end; x++)
        {
            global_sqrt_sum += sqrt(x);
        }
        break;

    case 2:
        // Method 2: Synchronize using a mutex (serial update)
        for (long long int x = start; x <= end; x++)
        {
            pthread_mutex_lock(&mutex);
            global_sqrt_sum += sqrt(x);
            pthread_mutex_unlock(&mutex);
        }
        break;

    case 3:
        // Method 3: Use local variables and synchronize using a mutex
        for (long long int x = start; x <= end; x++)
        {
            local_sqrt_sum += sqrt(x);
        }
        pthread_mutex_lock(&mutex);
        global_sqrt_sum += local_sqrt_sum;
        pthread_mutex_unlock(&mutex);
        break;

    default:
        fprintf(stderr, "Invalid method\n");
        exit(EXIT_FAILURE);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <a> <b> <num_threads> <method>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    a = atoll(argv[1]);
    b = atoll(argv[2]);
    num_threads = atoi(argv[3]);
    method = atoi(argv[4]);

    if (num_threads > MAX_THREADS)
    {
        fprintf(stderr, "Number of threads exceeds the maximum allowed\n");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[MAX_THREADS];
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < num_threads; i++)
    {
        long long int start = a + i * ((b - a) / num_threads);
        pthread_create(&threads[i], NULL, calculate_sqrt_sum, (void *)&start);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print results
    printf("Sum: %.5e\n", global_sqrt_sum);

    // Clean up
    pthread_mutex_destroy(&mutex);

    return 0;
}
