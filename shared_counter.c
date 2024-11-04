#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10
#define MAX 100000 //consider the range to be 100 up to 100000

int counter = 0;  // Shared variable

// Function to increment the counter
void *increment_counter(void *arg) {
    for (int i = 0; i < MAX; i++) {
        counter++;  
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Create threads to increment the counter
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Expected result is NUM_THREADS * INCREMENTS, but due to race conditions, it will likely be less
    printf("Final counter value: %d (Expected: %d)\n", counter, NUM_THREADS * MAX);

    return 0;
}

