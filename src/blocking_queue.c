#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

// #include "include/utils_ds/deque.h"
#include "utils_ds/deque.h"
#include "utils_ds/blocking_queue.h"

struct Blocking_Q {
    Deque* dq;

    // int n_elm;
    int n_empty_waiters;

    pthread_mutex_t lock;
    pthread_cond_t empty_cond;
};

Blocking_Q* bq_create() {
    Blocking_Q* bq = malloc(sizeof(*bq));
    if (bq == NULL) {
        fprintf(stderr, "[blocking_queue.c: bq_create()] malloc error(%s)\n", strerror(errno));
        return NULL;
    }
    
    Deque* dq = deque_create();
    if (dq == NULL) {
        fprintf(stderr, "[blocking_queue.c: bq_create()] deque_create() error\n");
        free(bq);
        return NULL;
    }

    bq->dq = dq;
    // bq->n_elm = 0;
    bq->n_empty_waiters = 0;

    pthread_mutex_init(&bq->lock, NULL);
    pthread_cond_init(&bq->empty_cond, NULL);

    return bq;
}

void bq_destroy(Blocking_Q* bq) {
    assert(bq != NULL);
    pthread_mutex_destroy(&bq->lock);
    pthread_cond_destroy(&bq->empty_cond);
    deque_destroy(bq->dq);
    free(bq);
}

void bq_push(Blocking_Q* bq, void* elm) {
    pthread_mutex_lock(&bq->lock);
    
    deque_push_back(bq->dq, elm);
    // bq->n_elm++;
    if (bq->n_empty_waiters > 0) {
        pthread_cond_signal(&bq->empty_cond);
    }

    pthread_mutex_unlock(&bq->lock);
}

void* bq_pop(Blocking_Q* bq) {
    pthread_mutex_lock(&bq->lock);

    while (deque_empty(bq->dq)) {
        bq->n_empty_waiters++;
        pthread_cond_wait(&bq->empty_cond, &bq->lock);
        bq->n_empty_waiters--;
    }

    void* elm = deque_front(bq->dq)->elm;
    deque_pop_front(bq->dq);
    // bq->n_elm--;

    pthread_mutex_unlock(&bq->lock);
    return elm;
}

int bq_size(Blocking_Q* bq) {
    pthread_mutex_lock(&bq->lock);

    int size = deque_size(bq->dq);

    pthread_mutex_unlock(&bq->lock);

    return size;
}

int bq_empty(Blocking_Q* bq) {
    pthread_mutex_lock(&bq->lock);

    bool empty = deque_empty(bq->dq);

    pthread_mutex_unlock(&bq->lock);

    return empty;
}

