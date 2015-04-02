#include "queue.h"

void queue_create(Queue *pqueue){
	pqueue->initial_size = 50;
	pqueue->inc = 2;
	pqueue->q = NULL;
	pqueue->head = 0;
	pqueue->tail = 0;
	pqueue->qsize = 0;
}

void queue_destroy(Queue *pqueue){
	if (pqueue == NULL)
		return;
}

bool queue_enqueue(Queue *pqueue, data value){
	if (pqueue->qsize <= pqueue->tail){
		//
	}
	pqueue->q[pqueue->tail] = value;
}

void queue_tune(Queue *pqueue, size_t initial_size, int increment){
	if (pqueue == NULL)
		return;
	if ((increment>2) && (increment<1000))
		pqueue->inc = increment;
	if ((initial_size>0) && (initial_size < 1000000)){
		pqueue->initial_size;
	}
}

size_t queue_size(Queue *pqueue){
	return pqueue->tail - pqueue->head;
}


data queue_dequeue(Queue *pqueue) {
	if (queue_size(pqueue) == 0){
		return NULL;
	}
	data res = pqueue->q[pqueue->head];

	res = 

}

data queue_peek(Queue *pqueue){
	if (queue_size(pqueue) == 0){
		return NULL;
	}
	return pqueue->q[pqueue->head];
}