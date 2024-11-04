#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int shared_data = 0;                  // Shared data
int data_ready = 0;                   // Flag indicating data availability
pthread_mutex_t mutex;                // Mutex for synchronization
pthread_cond_t cond_var;              // Condition variable

// Producer thread function
void *producer(void *arg) {
    for (int i = 1; i <= 5; i++) {
        sleep(1);  // Simulate data production time

        pthread_mutex_lock(&mutex);
        shared_data = i * 10;         // Produce data
        data_ready = 1;               // Set flag to indicate data is ready
        printf("Producer: Produced data %d\n", shared_data);
        pthread_cond_signal(&cond_var); // Notify consumer that data is ready
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Consumer thread function
void *consumer(void *arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&mutex);
        while (data_ready == 0) {       // Wait until data is ready
            pthread_cond_wait(&cond_var, &mutex); // Wait for the signal
        }
        printf("Consumer: Consumed data %d\n", shared_data);
        data_ready = 0;                // Reset the flag
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for both threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    return 0;
}

