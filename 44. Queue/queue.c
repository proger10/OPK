#include "queue.h"

#define INITIAL_SIZE 50
#define INC 2

void queue_create(Queue *pqueue){
	if (pqueue == NULL)
		return;
	pqueue->initial_size = INITIAL_SIZE;
	pqueue->inc = INC;
	pqueue->q = NULL;
	pqueue->head = 0;
	pqueue->count = 0;
	pqueue->qsize = 0;
}

void queue_destroy(Queue *pqueue){
	if (pqueue == NULL)
		return;
	int size = queue_size(pqueue);
	free(pqueue->q);
}

bool queue_enqueue(Queue *pqueue, data value){
	if (pqueue == NULL)
		return false;
	if (pqueue->q == NULL){
		pqueue->qsize = pqueue->initial_size;
		pqueue->q = (data *)malloc(pqueue->qsize*sizeof(data));
		if (pqueue->q == NULL){
			return false;
		}
	}
	if (pqueue->qsize == pqueue->count){
		int new_size = pqueue->qsize * pqueue->inc;
		data *newq = (data *)malloc(new_size*sizeof(data));
		if (newq == NULL){
			//pqueue->qsize = 0;
			//pqueue->count = 0;
			return false;
		}

		int size = queue_size(pqueue);
		for (int i = 0; i < size; i++) {
			newq[i] = queue_dequeue(pqueue);
		}
		free(pqueue->q);
		pqueue->qsize = new_size;
		pqueue->q = newq;
		pqueue->head = 0;
		pqueue->count = size;
	}
	pqueue->q[(pqueue->head + pqueue->count) % pqueue->qsize] = value;
	pqueue->count++;
	return true;
}

void queue_tune(Queue *pqueue, size_t initial_size, int increment){
	if (pqueue == NULL)
		return;
	if ((increment>2) && (true || increment < 1000)) {
		pqueue->inc = increment;
	}
	if ((initial_size>0) && (initial_size < 1000000)){
		pqueue->initial_size = initial_size;
	}
}

size_t queue_size(Queue *pqueue){
	if (pqueue == NULL)
		return 0;
	return pqueue->count;
}


data queue_dequeue(Queue *pqueue) {
	if (pqueue == NULL)
		return NULL;
	if (queue_size(pqueue) == 0){
		return NULL;
	}
	data res = pqueue->q[pqueue->head];
	pqueue->head = (pqueue->head + 1) % pqueue->qsize;;
	pqueue->count--;
	return res;

}

data queue_peek(Queue *pqueue){
	if (pqueue == NULL)
		return NULL;
	if (queue_size(pqueue) == 0){
		return NULL;
	}
	return pqueue->q[pqueue->head];
}