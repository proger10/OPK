#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "hashtable.h"
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define SIZE 1024
#define LOAD 13370
#define HASH_SIZE 1023


void test();

int intcmp(int *a, int *b){
	return *a - *b;
}

void foreach(void *key, int *data, bool *foreachtest){
	assert(data == key);
	foreachtest[*data] = true;
}

int **make_data(){
	int **result = (int **)malloc(SIZE*sizeof(int *));
	assert(result != NULL);
	for (int i = 0; i < SIZE; i++){
		result[i] = (int *)malloc(sizeof(int));
		assert(result[i] != NULL);
		*result[i] = i;
	}
	return result;
}

void free_data(int **data){
	for (int i = 0; i < SIZE; i++)
		free(data[i]);
	free(data);
}

unsigned jenkin_hash(char *key) {
	unsigned hash = 0;

	for (int i = 0; i < sizeof(int); ++i) {
		hash += *key;
		hash += (hash << 10);
		hash ^= (hash >> 6);
		key++;
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

void test_all(){
	/*Create tree, add 1-SIZE elements, find them all, try to find wrong data*/
	HashTable *ht = ht_init(HASH_SIZE, jenkin_hash, intcmp);
	assert(ht != NULL);

	int **test = make_data();
	for (int i = 0; i < SIZE; i++){
		int *res = ht_set(ht, test[i], test[i]);
		assert(res == NULL);
	}

	for (int i = 0; i < SIZE; i++){
		int *res = ht_get(ht, test[i]);
		assert(res == test[i]);
	}

	for (int i = 0; i < 100000; i++){
		int t = rand();
		int *res = (int *)ht_get(ht, &t);
		if ((t >= 0) && (t < SIZE))
			assert(res == test[t]);
		else
			assert(res == NULL);
	}

	//foreach test
	bool *foreachtest = calloc(SIZE, sizeof(bool));
	assert(foreachtest != NULL);
	ht_foreach(ht, foreach, foreachtest);
	for (int i = 0; i < SIZE; i++){
		printf("%d\n", i);
		assert(foreachtest[i]);
	}
	free(foreachtest);

	//delete test
	for (int i = 0; i < SIZE; i++){
		int *res = ht_delete(ht, &i);
		assert(*res == i);
	}

	//delete something from cleared table
	for (int i = 0; i < SIZE; i++){
		int t = 1;// rand();
		int *res = ht_delete(ht, &t);
		assert(res == NULL);
	}
	ht_destroy(ht);
	free_data(test);
}

void foreachfree(void *key, int *data, int *freed){
	if (data != NULL){
		free(data);
		(*freed)++;
	}
}
void test_load(){
	HashTable *ht = ht_init(HASH_SIZE,jenkin_hash,intcmp);

	int replaced = 0;
	int deleted = 0;
	int freed = 0;
	assert(ht != NULL);
	for (int i = 0; i < LOAD; i++){
		int *add = malloc(sizeof(int));
		*add = rand();
		int *res = ht_set(ht, add, add);
		if (res != NULL){
			free(res);
			replaced++;
		}
		int size = ht_size(ht);
		assert(size == i + 1 - deleted - replaced);
		int del = rand();
		res = (int *)ht_delete(ht, &del);
		if (res != NULL){
			free(res);
			deleted++;
		}
		size = ht_size(ht);
		assert(size == i + 1 - deleted - replaced);

	}
	ht_foreach(ht, foreachfree, &freed);
	assert(freed == ht_size(ht));
	assert(LOAD == replaced + deleted + freed);
	ht_destroy(ht);
}

void test_reinsert(){
	HashTable *ht = ht_init(HASH_SIZE, jenkin_hash, intcmp);
	assert(ht != NULL);

	int *i1 = (int *)malloc(sizeof(int));
	int *i2 = (int *)malloc(sizeof(int));
	int *i3 = (int *)malloc(sizeof(int));
	*i1 = 1;
	*i2 = 1;
	*i3 = 1;

	int *res = ht_set(ht, i1, i1);
	assert(res == NULL);
	res = ht_set(ht, i2, i2);
	assert(res == i1);
	res = ht_delete(ht, i3);
	free(i3);
	assert(res == i2);
	free(i1);
	free(res);
	ht_destroy(ht);
}

void main(){
	test_all();
	test_reinsert();
	//test_load();
	_CrtDumpMemoryLeaks();
}
