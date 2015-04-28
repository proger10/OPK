#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <assert.h>
#include "linked_list.h"

int find_int(int *a, int *b){
	return *a != *b;
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
	/*append, last, length, free, nth, position, find, custom_find*/
	SList *list = NULL;
	assert(NULL == slist_nth(list, 1));
	assert(NULL == slist_nth(list, 0));
	assert(NULL == slist_nth(list, -11));
	assert(NULL == slist_nth(list, 10));
	assert(slist_length(list) == 0);
	for (int i = 0; i < SIZE; i++){
		list = slist_append(list, test[i]);
		assert(slist_length(list) == i + 1);
		SList *last = slist_last(list);
		assert(last != NULL);
		assert(last->data != NULL);
		assert((int *)last->data == test[i]);
	}

	/*foreach*/
	int foreach_test = 0;
	slist_foreach(list, foreach, &foreach_test);
	assert(foreach_test == SIZE);

	for (int i = 0; i < 1000000; i++){
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
	for (int i = 0; i < SIZE; i++){
		free(test[i]);
	}
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
		list2 = slist_prepend(list2, test[i]);
		assert(list2 != NULL);
	}
	for (int i = 0; i < SIZE; i++){
		SList *nth1 = slist_nth(list1, i);
		SList *nth2 = slist_nth(list2, i);
		assert(nth1->data == nth2->data);
	}
	/*concat*/
	SList *list = slist_concat(list1, list2);
	slist_free(list1);
	slist_free(list2);
	for (int i = 0; i < 2 * SIZE; i++){
		SList *nth1 = slist_nth(list, i);
		assert(nth1->data == test[i%SIZE]);
	}

	SList *nulllist = slist_concat(NULL, list);
	for (int i = 0; i < 2 * SIZE; i++){
		SList *nth1 = slist_nth(nulllist, i);
		assert(nth1->data == test[i%SIZE]);
	}
	slist_free(nulllist);

	nulllist = slist_concat(list, NULL);
	for (int i = 0; i < 2 * SIZE; i++){
		SList *nth1 = slist_nth(nulllist, i);
		assert(nth1->data == test[i%SIZE]);
	}
	slist_free(nulllist);

	slist_free(list);
	for (int i = 0; i < SIZE; i++){
		free(test[i]);
	}
	free(test);
#undef SIZE
}

void test2(){
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
	/*copy*/
	SList *list2 = slist_copy(list1);
	assert(list2 != NULL);
	for (int i = 0; i < SIZE; i++){
		SList *nth1 = slist_nth(list1, i);
		SList *nth2 = slist_nth(list2, i);
		assert(nth1->data == nth2->data);
	}
	slist_free(list2);

	/*reverse*/
	list2 = slist_reverse(list1);
	for (int i = 0; i < SIZE; i++){
		SList *nth = slist_nth(list2, i);
		assert(nth->data == test[SIZE-i-1]);
	}
	slist_free(list2);
	list2 = slist_reverse(NULL);
	assert(list2 == NULL);

	slist_free(list1);
	for (int i = 0; i < SIZE; i++){
		free(test[i]);
	}
	free(test);
#undef SIZE
}

void test3(){
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
	/*insert*/
	SList *list2 = slist_prepend(NULL, test[0]);
	SList *last = list2;
	SList *ins_test = NULL;
#define INSTEST 123 % SIZE
	for (int i = 1; i < SIZE; i++){
		if (i == INSTEST){
			ins_test = last;
			continue;
		}
		slist_insert(last, test[i]);
		last = last->next;
		assert(last != NULL);
	}
	slist_insert(ins_test, test[INSTEST]);
#undef INSTEST
	assert(list2 != NULL);
	for (int i = 0; i < SIZE; i++){
		SList *nth = slist_nth(list1, i);
		assert(nth->data == test[i]);
	}

	/*remove*/
	assert(NULL == slist_remove(NULL, (Pointer)0xABAD1DEA));
	//remove first, last, random
	list2 = slist_remove(list2, test[0]);
	list2 = slist_remove(list2, test[SIZE-1]);
	int rnd = 0;
	while ((rnd == 0) || (rnd == SIZE -1))
		rnd = rand() % SIZE;
	list2 = slist_remove(list2, test[rnd]);
	assert(slist_length(list2) == SIZE - 3);
	for (int i = 0; i < SIZE; i++){
		SList *nth = slist_find(list2, test[i]);
		if ((i == 0) || (i == SIZE - 1) || (i == rnd))
			assert(nth == NULL);
		else
			assert(nth->data == test[i]);
	}
	//repair list2
	list2 = slist_prepend(list2, test[0]);
	list2 = slist_append(list2, test[SIZE-1]);
	slist_insert(slist_nth(list2, rnd-1), test[rnd]);
	//test repairing
	for (int i = 0; i < SIZE; i++){
		SList *nth = slist_nth(list1, i);
		assert(nth->data == test[i]);
	}

	
	/*remove_all*/
	//make trash
	int *trash = malloc(sizeof(int));
	assert(trash != NULL);
	*trash = SIZE;
	//add a lot of rubbish
#define TRASHSIZE1 1337
#define TRASHSIZE2 1337
	for (int i = 0; i < TRASHSIZE1; i++){
		list2 = slist_append(list2, trash);
	}
	for (int i = 0; i < TRASHSIZE2; i++){
		list2 = slist_prepend(list2, trash);
	}
	assert(slist_length(list2) == SIZE + TRASHSIZE1 + TRASHSIZE2);
#undef TRASHSIZE1
#undef TRASHSIZE2
	//remove trash and repair lsit2
	list2 = slist_remove_all(list2, trash);
	//test repairing
	for (int i = 0; i < SIZE; i++){
		SList *nth = slist_nth(list1, i);
		assert(nth->data == test[i]);
	}
	free(trash);

	/*remove next*/
	assert(NULL == slist_remove_next(NULL));
	int *next = NULL;
	int k = 2;
	while (next = slist_remove_next(list2->next)){
		assert(next == test[k]);
		k++;
	}
	assert(slist_length(list2) == 2);
	slist_free(list2);
	slist_free(list1);
	for (int i = 0; i < SIZE; i++){
		free(test[i]);
	}
	free(test);
#undef SIZE
}

int main(){
	test();
	test1();
	test2();
	test3();
	//Test memory
	_CrtDumpMemoryLeaks();
	return 0;
}