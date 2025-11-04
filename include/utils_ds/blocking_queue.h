#ifndef BLOCKING_Q_H
#define BLOCKING_Q_H

typedef struct Blocking_Q Blocking_Q;

Blocking_Q* bq_create();
void bq_destroy(Blocking_Q* bq);
void bq_push(Blocking_Q* bq, void* elm);
void* bq_pop(Blocking_Q* bq);
int bq_size(Blocking_Q* bq);
int bq_empty(Blocking_Q* bq);

#endif