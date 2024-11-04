#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5  // Size of the buffer (circular queue)
#define NUM_ITEMS 10   // Number of items to produce and consume

int buffer[BUFFER_SIZE];  // Circular buffer
int count = 0;            // Number of items in the buffer
int in = 0;               // Index for the producer to add items
int out = 0;              // Index for the consumer to remove items

pthread_mutex_t mutex;             // Mutex for buffer synchronization
pthread_cond_t not_full;           // Condition variable for "not full"
pthread_cond_t not_empty;          // Condition variable for "not empty"

// Producer thread function
void *producer(void *arg) {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        // Simulate production time
        sleep(1);  // Producer takes 1 second to produce

        pthread_mutex_lock(&mutex);

        // Wait if the buffer is full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &mutex);
        }

        // Produce an item and add it to the buffer
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("Producer: %d\n", i);

        // Signal that buffer is not empty
        pthread_cond_signal(&not_empty);

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Consumer thread function
void *consumer(void *arg) {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        pthread_mutex_lock(&mutex);

        // Wait if the buffer is empty
        while (count == 0) {
            pthread_cond_wait(&not_empty, &mutex);
        }

        // Consume an item from the buffer
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("Consumer: %d\n", item);

        // Signal that buffer is not full
        pthread_cond_signal(&not_full);

        pthread_mutex_unlock(&mutex);

        // Simulate consumption time
        sleep(2);  // Consumer takes 2 seconds to consume
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for both threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}

