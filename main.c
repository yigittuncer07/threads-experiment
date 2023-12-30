// Include libraries
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// This defines the max threads
#define MAX_THREADS 32

// Declear global variables
double global_sqrt_sum = 0;
long long int a, b;
int num_threads, method;
pthread_mutex_t mutex;

// Function to calculate the square root sum for a given range
void *calculate_sqrt_sum(void *arg)
{
    // Turn the input pointer into a long long int
    long long int start = *((long long int *)arg);

    // Calculate the end of the range by using the number of threads
    long long int end = start + (b - a) / num_threads;

    // Chooose method to operate with depending on method variable
    switch (method)
    {
    case 1:
        // Method 1: No synchronization
        // Here all threads access global_sqrt_sum without any synchronization.
        // This leads to false values being calculated because of race conditions
        // This is efficient but very unreliable
        for (long long int x = start; x <= end; x++)
        {
            global_sqrt_sum += sqrt(x);
        }
        break;

    case 2:
        // Method 2: Synchronize using a mutex (serial update)
        // Here all threads lock the variable before adding their calculated sqrt to
        // the variable. This prevents the race condition from happening but results in
        // very long system times, because global_sqrt_sum gets locked very frequently and
        // threads will wait for a long time
        // This is very ineficient but reliable
        for (long long int x = start; x <= end; x++)
        {
            // Lock the variable
            pthread_mutex_lock(&mutex);
            global_sqrt_sum += sqrt(x);
            // Unlock the variable
            pthread_mutex_unlock(&mutex);
        }
        break;

    case 3:
        // Method 3: Use local variables and synchronize using a mutex
        // Here we use best of both methods by using a local variable. Thanks
        // to this we dont have to lock the variable and wait each time we do a
        // root operation. Each thread can find their result and lock only once to
        // add to the sum.
        // This is both efficient and reliable

        // Declare the local variable
        double local_sqrt_sum = 0;

        // Calculate given range for thread
        for (long long int x = start; x <= end; x++)
        {
            local_sqrt_sum += sqrt(x);
        }

        // Lock the variable
        pthread_mutex_lock(&mutex);
        // Add calculated part to global sum
        global_sqrt_sum += local_sqrt_sum;
        // Unlock variable
        pthread_mutex_unlock(&mutex);
        break;

    // This exits if an int that is not 1 2 or 3 is entered for the method arg
    default:
        fprintf(stderr, "Invalid method\n");
        exit(EXIT_FAILURE);
    }

    // Free the start pointer
    free(arg);

    // Exit the thread with arg NULL
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // Checks if 5 args are given, if not prompts with info of correct usage
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <a> <b> <num_threads> <method>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // The input strings are turned into long long int types with the atoll method
    a = atoll(argv[1]);
    b = atoll(argv[2]);

    // Turns the string args into ints
    num_threads = atoi(argv[3]);
    method = atoi(argv[4]);

    // Checks if the max number of threads is breached
    if (num_threads > MAX_THREADS)
    {
        fprintf(stderr, "Number of threads exceeds the maximum allowed\n");
        exit(EXIT_FAILURE);
    }

    // Create an array to store all threads
    pthread_t threads[MAX_THREADS];
    pthread_mutex_init(&mutex, NULL);

    // This loop creates all the threads
    for (int i = 0; i < num_threads; i++)
    {
        // Creates a start index in memory. This has to be done otherwise some threads can overwrite the other start indices
        long long int *thread_start = malloc(sizeof(long long int));
        // Calculate the correct starting index depending on the number of threads
        *thread_start = a + i * ((b - a) / num_threads);
        // Create the thread on the calculate_sqrt_sum method and give thread_start as an argument
        pthread_create(&threads[i], NULL, calculate_sqrt_sum, (void *)thread_start);
    }

    // Joins all the threads. If this is not done the main method may terminate before all threads are complete.
    // This does not put the threads in an order but rather makes sure they are all done
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print the sum result
    printf("Sum: %.5e\n", global_sqrt_sum);

    // Destroy the mutex before exiting
    pthread_mutex_destroy(&mutex);

    return 0;
}
