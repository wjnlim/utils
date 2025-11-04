#include <stdio.h>

#include "utils_ds/deque.h"

int main (int argc, char** argv) {
    Deque* dq = deque_create();

    for (int i = 1; i < 10; i++) {
        if (i % 2 == 0)
            deque_push_back(dq, (void*)i);
        else
            deque_push_front(dq, (void*)i);
    }

    printf("Current capacity: %d\n", deque_capacity(dq));
    printf("Current size: %d\n", deque_size(dq));
    printf("Front element: %d\n", (int)(deque_front(dq)->elm));
    printf("Back element: %d\n\n", (int)(deque_back(dq)->elm));

    printf("Pop an element from front\n");
    deque_pop_front(dq);
    printf("Pop an element from back\n\n");
    deque_pop_back(dq);

    printf("Current capacity: %d\n", deque_capacity(dq));
    printf("Current size: %d\n", deque_size(dq));
    printf("Front element: %d\n", (int)(deque_front(dq)->elm));
    printf("Back element: %d\n\n", (int)(deque_back(dq)->elm));
}