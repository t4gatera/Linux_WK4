#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10
#define LIMIT 1000000

// Each thread will store its partial sum in this array
long partial_sums[NUM_THREADS];

// Structure to pass arguments to each thread
typedef struct {
    int start;
    int end;
    int thread_id;
} ThreadData;

// Function to be executed by each thread
void *partial_sum(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int start = data->start;
    int end = data->end;
    int thread_id = data->thread_id;

    // Calculate partial sum for this thread's range
    partial_sums[thread_id] = 0;
    for (int i = start; i <= end; i++) {
        partial_sums[thread_id] += i;
    }
    
    printf("Here is the thread %d:%ld\n", thread_id,partial_sums[thread_id]);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int range_per_thread = LIMIT / NUM_THREADS;

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * range_per_thread + 1;
        thread_data[i].end = (i + 1) * range_per_thread;
        thread_data[i].thread_id = i;
        
        if (pthread_create(&threads[i], NULL, partial_sum, &thread_data[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Combine results from all threads
    long total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total_sum += partial_sums[i];
    }

    printf("The sum of the first %d numbers is: %ld\n", LIMIT, total_sum);

    return 0;
}

