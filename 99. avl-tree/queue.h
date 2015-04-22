#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <queue> 
using namespace std;
typedef void *data;
typedef data Pointer;

typedef struct _Queue {
	queue<Pointer> *q;
} Queue;

void queue_create(Queue *pqueue);
void queue_destroy(Queue *pqueue);
bool queue_enqueue(Queue *pqueue, data value);
data queue_dequeue(Queue *pqueue);

#endif