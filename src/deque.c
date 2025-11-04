#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "utils_ds/deque.h"

#define INITIAL_CAPACITY 64

struct Deque {
    // void** arr;
    Deque_iterator* arr;
    int front;
    int back;
    int size;
    int capacity;
};

Deque* deque_create() {
    Deque* deque = (Deque*)malloc(sizeof(*deque));
    if (deque == NULL) {
        fprintf(stderr, "[deque.c: deque_create()] malloc(deque) error(%s).\n", strerror(errno));
        return NULL;
    }
    // void** arr = malloc(INITIAL_CAPACITY * sizeof(void*));
    Deque_iterator* arr = malloc(INITIAL_CAPACITY * sizeof(Deque_iterator));
    if (arr == NULL) {
        fprintf(stderr, "[deque.c: deque_create()] calloc(arr) error(%s).\n", strerror(errno));
        free(deque);
        return NULL;
    }

    deque->arr = arr;
    deque->front = -1;
    deque->back = -1;
    deque->size = 0;
    deque->capacity = INITIAL_CAPACITY;

    return deque;
}

void deque_destroy(Deque* deque) {
    assert(deque != NULL);

    free(deque->arr);
    free(deque);
}

int deque_size(Deque* deque) {
    return deque->size;
}

int deque_capacity(Deque* deque) {
    return deque->capacity;
}

bool deque_empty(Deque* deque) {
    return deque->size == 0;
}

Deque_iterator* deque_front(Deque* deque) {
    if (deque_empty(deque))
        return NULL;

    return &deque->arr[deque->front];
}

Deque_iterator* deque_back(Deque* deque) {
    if (deque_empty(deque))
        return NULL;

    return &deque->arr[deque->back];
}

Deque_iterator* deque_next(Deque* deque, Deque_iterator* di) {
    int idx = di - deque->arr;
    if (idx == deque->back)
        return NULL;
    
    idx = (idx + 1) % (deque->capacity);
    return &deque->arr[idx];
}

Deque_iterator* deque_prev(Deque* deque, Deque_iterator* di) {
    int idx = di - deque->arr;
    if (idx == deque->front)
        return NULL;
    
    idx = (idx - 1 + deque->capacity) % (deque->capacity);
    return &deque->arr[idx];
}

// void* deque_front(Deque* deque) {
//     return deque->arr[deque->front];
// }

// void* deque_back(Deque* deque) {
//     return deque->arr[deque->back];
// }

static void double_capacity(Deque* deque) {
    int new_capa = 2 * deque->capacity;
    // void** new_arr = malloc(new_capa * sizeof(void*));
    Deque_iterator* new_arr = malloc(new_capa * sizeof(Deque_iterator));
    assert(new_arr != NULL);
    int i = 0;
    int j = deque->front;
    for (int s = 0; s < deque->size; s++) {
        // new_arr[i++] = deque->arr[j];
        new_arr[i++].elm = deque->arr[j].elm;
        j = (j + 1) % (deque->size);
    }

    deque->front = 0;
    deque->back = deque->size - 1;
    deque->capacity = new_capa;
    free(deque->arr);
    deque->arr = new_arr;
}

void deque_push_front(Deque* deque, void* elm) {
    if (deque->size == deque->capacity) {
        double_capacity(deque);
    }

    if (deque_empty(deque)) {
        deque->front = deque->back = 0;
        deque->arr[deque->front].elm = elm;
        deque->size++;
        return;
    }

    deque->front = (deque->front - 1 + deque->capacity) % (deque->capacity);
    deque->arr[deque->front].elm = elm;
    deque->size++;
    return;
}

void deque_push_back(Deque* deque, void* elm) {
    if (deque->size == deque->capacity) {
        double_capacity(deque);
    }

    if (deque_empty(deque)) {
        deque->front = deque->back = 0;
        deque->arr[deque->back].elm = elm;
        deque->size++;
        return;
    }

    deque->back = (deque->back + 1) % (deque->capacity);
    deque->arr[deque->back].elm = elm;
    deque->size++;
    return;
}

void deque_pop_front(Deque* deque) {
    if (deque_empty(deque)) {
        return;
    }

    if (deque->size == 1) {
        deque->front = deque->back = -1;
        deque->size--;
        return;
    }

    deque->front = (deque->front + 1) % (deque->capacity);
    deque->size--;
    return;
}

void deque_pop_back(Deque* deque) {
    if (deque_empty(deque)) {
        return;
    }

    if (deque->size == 1) {
        deque->front = deque->back = -1;
        deque->size--;
        return;
    }

    deque->back = (deque->back - 1 + deque->capacity) % (deque->capacity);
    deque->size--;
    return;
}