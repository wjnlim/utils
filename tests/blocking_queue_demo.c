#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep
#include <pthread.h>
#include "utils_ds/blocking_queue.h"

#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 3
#define ITEMS_TO_PRODUCE 20

Blocking_Q* queue;

void* producer(void* arg) {
    int producer_id = *(int*)arg;
    for (int i = 0; i < ITEMS_TO_PRODUCE / NUM_PRODUCERS; ++i) {
        int* item = malloc(sizeof(int));
        *item = producer_id * 1000 + i;
        printf("Producer %d: Pushing item %d\n", producer_id, *item);
        bq_push(queue, item);
    }
    return NULL;
}

void* consumer(void* arg) {
    int consumer_id = *(int*)arg;
    while (1) {
        int* item = (int*)bq_pop(queue);
        if (item == NULL) { 
            break; 
        }
        printf("Consumer %d: Popped item %d\n", consumer_id, *item);
    }
    printf("Consumer %d: Exiting\n", consumer_id);
    return NULL;
}

int main() {
    pthread_t prods[NUM_PRODUCERS], cons[NUM_CONSUMERS];
    int prod_ids[NUM_PRODUCERS], cons_ids[NUM_CONSUMERS];

    // bq_init(queue);
    queue = bq_create();

    // Create producers
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        prod_ids[i] = i + 1;
        pthread_create(&prods[i], NULL, producer, &prod_ids[i]);
    }

    // Create consumers
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        cons_ids[i] = i + 1;
        pthread_create(&cons[i], NULL, consumer, &cons_ids[i]);
    }

    // Wait for all producers to finish
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        pthread_join(prods[i], NULL);
    }

    // Push NULL to consumers to signal them to stop
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        bq_push(queue, NULL);
    }

    // Wait for all consumers to finish
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_join(cons[i], NULL);
    }

    bq_destroy(queue);
    printf("All threads finished. Queue destroyed.\n");

    return 0;
}