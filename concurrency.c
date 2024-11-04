#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

// Structure to pass arguments to each thread
typedef struct {
    int thread_id;
    int number;  // The number for which the thread will calculate factorial
} ThreadData;

// Function to calculate factorial
long long factorial(int n) {
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Function to be executed by each thread
void *compute_factorial(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int thread_id = data->thread_id;
    int number = data->number;

    // Each thread calculates the factorial of its assigned number
    long long result = factorial(number);
    printf("Thread %d: Factorial of %d is %lld\n", thread_id, number, result);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int numbers[NUM_THREADS] = {5, 7, 10, 12, 15};  // Different numbers for each thread to compute factorial

    // Create threads, each calculating the factorial of a different number
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i + 1;
        thread_data[i].number = numbers[i];
        
        if (pthread_create(&threads[i], NULL, compute_factorial, &thread_data[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have completed their computations.\n");

    return 0;
}

