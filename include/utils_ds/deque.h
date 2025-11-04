#ifndef DEQUE_H
#define DEQUE_H

#include <stdbool.h>

typedef struct Deque Deque;
typedef struct Deque_iterator {
    void* elm;
} Deque_iterator;

Deque* deque_create();
void deque_destroy(Deque* deque);

int deque_size(Deque* deque);
int deque_capacity(Deque* deque);
bool deque_empty(Deque* deque);

Deque_iterator* deque_front(Deque* deque);
Deque_iterator* deque_back(Deque* deque);
Deque_iterator* deque_next(Deque* deque, Deque_iterator* di);
Deque_iterator* deque_prev(Deque* deque, Deque_iterator* di);
// void* deque_front(Deque* deque);
// void* deque_back(Deque* deque);

void deque_push_front(Deque* deque, void* elm);
void deque_push_back(Deque* deque, void* elm);

void deque_pop_front(Deque* deque);
void deque_pop_back(Deque* deque);

#endif