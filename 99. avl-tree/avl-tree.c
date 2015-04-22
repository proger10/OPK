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
		queue_enqueue(q, node->left);
		queue_enqueue(q, node->right);
		free(node);
	}
	queue_destroy(q);
}

void avl_destroy(AVLTree *tree){
	avl_clear(tree);
	free(tree);
}

size_t avl_size(AVLTree *tree){
	int result = 0;
	Queue *q = malloc(sizeof(Queue));
	queue_create(q);
	queue_enqueue(q, tree->root);
	AVLTreeNode *node = NULL;
	while (node = queue_dequeue(q)){
		queue_enqueue(q, node->left);
		queue_enqueue(q, node->right);
		result++;
	}
	queue_destroy(q);
	return result;
}

Pointer avl_find(AVLTree *tree, Pointer data){
	Pointer result = NULL;
	AVLTreeNode *node = tree->root;
	while (node != NULL){
		//function like "return data-nodedata;"
		int cmp_res = tree->cmp_func(data, node->data);
		//item found
		if (cmp_res == 0)
				return node->data;
		//data less than nodedata. go to left
		if (cmp_res < 0)
			node = node->left;
		//data bigger than nodedata. go to rigth
		if (cmp_res > 0)
			node = node->right;
	}
	return result;
}

static AVLTree *avl_balance(AVLTree *tree){
	//todo balance
	return tree;
}

Pointer avl_insert(AVLTree *tree, Pointer data){
	AVLTreeNode **addto = &(tree->root);
	while (*addto != NULL){
		//function like "return data-nodedata;"
		int cmp_res = tree->cmp_func(data, (*addto)->data);
		//item found
		if (cmp_res == 0){
			Pointer result = (*addto)->data;
			(*addto)->data = data;
			return result;
		}
		//data less than nodedata. go to left
		if (cmp_res < 0)
			addto = &((*addto)->left);
		//data bigger than nodedata. go to rigth
		if (cmp_res > 0)
			addto = &((*addto)->right);
	}
	AVLTreeNode *node = malloc(sizeof(AVLTreeNode));
	node->data = data;
	node->left = node->right = NULL;
	node->parent = *addto;

	return avl_balance(tree);
}

Pointer avl_delete(AVLTree *tree, Pointer data){
	AVLTreeNode *node = avl_find(tree, data);
	if (node == NULL)
		return NULL;
	AVLTreeNode **remfrom = NULL;
	if (node->parent != NULL){
		if (node->parent->left == node)
			remfrom = &(node->parent->left);
		if (node->parent->right == node)
			remfrom = &(node->parent->right);
	}
	Pointer result = node->data;
	
	if ((node->left == NULL) && (node->right == NULL)){
		if (remfrom != NULL)
			*remfrom = NULL;
		free(node);

	}
	else
		if ((node->left == NULL) && (node->right != NULL)){

		}


	avl_balance(tree);
	return result;
}

void avl_foreach(AVLTree *tree, void(*foreach_func)(Pointer data, Pointer extra_data), Pointer extra_data){
	Queue *q = malloc(sizeof(Queue));
	queue_create(q);
	queue_enqueue(q, tree->root);
	AVLTreeNode *node = NULL;
	while (node = queue_dequeue(q)){
		queue_enqueue(q, node->left);
		queue_enqueue(q, node->right);
		foreach_func(node->data, extra_data);
	}
	queue_destroy(q);
}
