#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <assert.h>
#include "linked_list.h"

int find_int(int *a, int *b){
	if (*a == *b)
		return 0;
	return 1;
}

void foreach(int *data, int *user_data){
	assert(*data == *user_data);
	(*user_data)++;
}

void test(){
#define SIZE 16
	int **test = (int **)malloc(sizeof(int *)*SIZE);
	assert(test != NULL);
	for (int i = 0; i < SIZE; i++){
		test[i] = malloc(sizeof(int));
		assert(test[i] != NULL);
		*test[i] = i;
	}
	/*append, last, size, free, nth, position, find, custom_find*/
	SList *list = NULL;
	assert(slist_length(list) == 0);
	for (int i = 0; i < SIZE; i++){
		list = slist_append(list, test[i]);
		assert(slist_length(list) == i + 1);
		SList *last = slist_last(list);
		assert(last != NULL);
		assert(last->data != NULL);
		assert((int *)last->data == test[i]);
	}
	int foreach_test = 0;
	slist_foreach(list, foreach, &foreach_test);
	assert(foreach_test == SIZE);
	for (int i = 0; i < 10000000; i++){
		int r = rand() % SIZE;
		SList *nth = slist_nth(list, r);
		assert(nth != NULL);
		assert(nth->data != NULL);
		assert((int *)nth->data == test[r]);
		assert(slist_position(list, nth) == r);
		SList *find = slist_find(list, test[r]);
		assert(find == nth);
		int find_me = *test[r];
		SList* cfind = slist_find_custom(list, &find_me, find_int);
		assert(find == cfind);
	}
	for (int i = 0; i < 1000000; i++){
		int r = rand() - rand();
		int rr = r;
		while (rr < 0){
			rr += SIZE;
		}
		SList *nth1 = slist_nth(list, r);
		SList *nth2 = slist_nth(list, rr);
		assert(nth1 == nth2);
	}
	slist_free(list);
	free(test);
#undef SIZE
}

void test1(){
#define SIZE 16
	int **test = (int **)malloc(sizeof(int *)*SIZE);
	assert(test != NULL);
	for (int i = 0; i < SIZE; i++){
		test[i] = malloc(sizeof(int));
		assert(test[i] != NULL);
		*test[i] = i;
	}
	SList *list1 = NULL;
	for (int i = 0; i < SIZE; i++){
		list1 = slist_append(list1, test[i]);
		assert(list1 != NULL);
	}
	/*prepand*/
	SList *list2 = NULL;
	for (int i = SIZE-1; i >= 0; i--){
		list2 = slist_prepend(list1, test[i]);
		assert(list2 != NULL);
	}
	for (int i = 0; i < SIZE; i++){
		SList *nth1 = slist_nth(list1, i);
		SList *nth2 = slist_nth(list2, i);
		assert(nth1->data == nth2->data);
	}
	/*concat*/
	//SList *list = slist_concat(list1, list2);

}

int main(){
	test();
	test1();
	//Test memory
	_CrtDumpMemoryLeaks();
	return 0;
}
