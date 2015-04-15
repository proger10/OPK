#include "avl-tree.h"
#include "queue.h"
#include <stdlib.h>

AVLTree *avl_create(CmpFunc cmp_func){
	AVLTree *result = malloc(sizeof(AVLTree));
	result->cmp_func = cmp_func;
	result->root = NULL;
	return result;
}

void avl_clear(AVLTree *tree){
	Queue *q = malloc(sizeof(Queue));
	queue_create(q);
	queue_enqueue(q, tree->root);
	AVLTreeNode *node = NULL;
	while (node = queue_dequeue(q)){

	}
	queue_destroy(q);
}

void avl_destroy(AVLTree *tree){

}

size_t avl_size(AVLTree *tree){
	return 0;
}

Pointer avl_find(AVLTree *tree, Pointer data){
	return NULL;
}

Pointer avl_insert(AVLTree *tree, Pointer data){
	return NULL;
}

Pointer avl_delete(AVLTree *tree, Pointer data){
	return NULL;
}

void avl_foreach(AVLTree *tree, void(*foreach_func)(Pointer data, Pointer extra_data), Pointer extra_data){
	return;
}
