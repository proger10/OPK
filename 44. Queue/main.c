#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "queue.h"

void add_int(int a, Queue *pqueue){
	int *data = malloc(sizeof(int));
	assert(data!=NULL);
	*data = a;
	int res = queue_enqueue(pqueue, data);
	assert(res);
}

void print_queue(Queue *pqueue){
	int *a;
	while (a = queue_dequeue(pqueue)){
		printf("%d ", *a);
		free(a);
	}
	printf("\n");
}

bool test_queue(int test, Queue *pqueue){
	int *a = queue_dequeue(pqueue);
	bool result = (*a == test);
	free(a);
	return result;
}

void test(){
	Queue q;
	queue_create(&q);
	queue_tune(&q, 1, 2);

	add_int(1, &q);
	add_int(2, &q);
	add_int(3, &q);
	print_queue(&q);

	add_int(1, &q);
	add_int(2, &q);
	add_int(3, &q);

	assert(test_queue(1, &q));
	assert(test_queue(2, &q));
	assert(test_queue(3, &q));


	add_int(1, &q);
	add_int(2, &q);
	add_int(3, &q);

	queue_destroy(&q);
}

void test2() {
	int *v;
	Queue q;
	queue_create(&q);
	queue_tune(&q, 5, 2);
	add_int(1, &q);
	add_int(2, &q);
	add_int(3, &q);
	add_int(4, &q);
	
	assert(4 == queue_size(&q));
	assert(1 == *(int*)queue_peek(&q));
	v = queue_dequeue(&q);
	assert(*v == 1);
	free(v);
	assert(3 == queue_size(&q));

	queue_dequeue(&q);

	add_int(5, &q);
	add_int(6, &q);

	assert(4 == queue_size(&q));
	queue_destroy(&q);
}

void test3() {
	Queue q;
	queue_create(&q);
	queue_tune(&q, 2, 2);
	queue_enqueue(&q, 1);
	queue_enqueue(&q, 2);
	for (unsigned i = 2; i < 4000000000; i++) {
		unsigned v = queue_dequeue(&q);
		printf("%d %u\n", i-1, v);
		assert(v == i-1);
		bool res = queue_enqueue(&q, i+1);
		if (!res)
			break;
	}
}

int main(){
	test();
	test2();
	test3();
	_CrtDumpMemoryLeaks();
	return 0;
}
