#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "avl-tree.h"
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define SIZE 256
#define LOAD 2740

int intcmp(void *a, void *b){
	return *((int *)a) - *((int *)b);
}

void foreach(void *data, void *table){
	assert(!((bool *)(table))[*((int *)data)]);
	((bool *)(table))[*((int *)data)] = true;
}

int **make_data(){
	int **result = (int **) malloc(SIZE*sizeof(int *));
	assert(result != NULL);
	for (int i = 0; i < SIZE; i++){
		result[i] = (int *) malloc(sizeof(int));
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

void test_all(){
	/*Create and destroy tree*/
	AVLTree *tree = avl_create(intcmp);
	assert(avl_size(tree) == 0);
	avl_destroy(tree);

	
	/*Create, clear and destroy tree*/
	tree = avl_create(intcmp);
	assert(avl_size(tree) == 0);
	avl_clear(tree);
	assert(avl_size(tree) == 0);
	avl_destroy(tree);


	/*Create tree, add 1-SIZE elements, find them all, try to find wrong data*/
	tree = avl_create(intcmp);
	int **test = make_data();
	for (int i = 0; i < SIZE; i++){
		int *res = (int *) avl_insert(tree, test[i]);
		assert(res == NULL);
		assert(avl_size(tree) == i + 1);
	}

	for (int i = 0; i < SIZE; i++){
		int *res = (int *)avl_find(tree, &i);
		assert(res == test[i]);
	}

	for (int i = 0; i < 100000; i++){
		int t = rand();
		int *res = (int *)avl_find(tree, &t);
		if ((t >= 0) && (t < SIZE))
			assert(res == test[t]);
		else
			assert(res == NULL);
	}

	//foreach test
	bool *foreachtest = (bool *) calloc(SIZE, sizeof(bool));
	assert(foreachtest != NULL);
	avl_foreach(tree, foreach, foreachtest);
	for (int i = 0; i < SIZE; i++){
		assert(foreachtest[i]);
	}
	free(foreachtest);
	

	//delete test
	for (int i = 0; i < SIZE; i++){
		int *res = (int *)avl_delete(tree, &i);
		assert(*res == i);
		assert(avl_size(tree) == SIZE-1-i);

	}

	//delete something
	for (int i = 0; i < SIZE; i++){
		int t = 1;// rand();
		int *res = (int *)avl_delete(tree, &t);
		assert(res == NULL);
	}
	avl_clear(tree);
	assert(avl_size(tree) == 0);
	avl_destroy(tree);
	free_data(test);
}

void foreachfree(void *data, void *freed){
	if (data != NULL){
		free(data);
		(*((int *)freed))++;
	}
}

void test_load(){
	AVLTree *tree = avl_create(intcmp);
	assert(avl_size(tree) == 0);
	int replaced = 0;
	int deleted = 0;
	int freed = 0;
	assert(tree != NULL);
	for (int i = 0; i < LOAD; i++){
		int *add = (int *) malloc(sizeof(int));
		*add =  rand();
		int *res;
		if (i==273)
			res = (int *)avl_insert(tree, add);
		else
			res = (int *)avl_insert(tree, add);
		if (res != NULL){
			free(res);
			replaced++;
		}
		int size = avl_size(tree);
		assert(size == i +1 - deleted - replaced);
	//	int del = 1;// rand();
	//	res = avl_delete(tree, &del);
	//	if (res != NULL){
	//		free(res);
	//		deleted++;
	//	}
	}
	avl_foreach(tree, foreachfree, &freed);

	assert(freed == avl_size(tree));
	assert(LOAD == replaced + deleted + freed);
	avl_destroy(tree);
}

void test_reinsert(){
	AVLTree *tree = avl_create(intcmp);
	assert(avl_size(tree) == 0);
	assert(tree != NULL);
	int *i1 = (int *)malloc(sizeof(int));
	int *i2 = (int *)malloc(sizeof(int));
	int *i3 = (int *)malloc(sizeof(int));
	*i1 = 1;
	*i2 = 1;
	*i3 = 1;
	int *res = (int *)avl_insert(tree, i1);
	assert(res == NULL);
	res = (int *)avl_insert(tree, i2);
	assert(res == i1);
	free(res);
	res = (int *)avl_delete(tree, i3);
	free(i3);
	assert(res == i2);
	free(res);

	avl_destroy(tree);
}

void test(){
	test_all();
	test_reinsert();
	test_load();
	_CrtDumpMemoryLeaks();
}