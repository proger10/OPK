#include "avl-tree.h"
#include "queue.h"
#include <stdlib.h>

AVLTree *avl_create(CmpFunc cmp_func){
	AVLTree *result = (AVLTree *)malloc(sizeof(AVLTree));
	result->cmp_func = cmp_func;
	result->root = NULL;
	return result;
}

void avl_clear(AVLTree *tree){
	Queue *q = (Queue *)malloc(sizeof(Queue));
	queue_create(q);
	queue_enqueue(q, tree->root);
	AVLTreeNode *node = NULL;
	while (node = (AVLTreeNode *)queue_dequeue(q)){
		if (node->left != NULL)
			queue_enqueue(q, node->left);
		if (node->right != NULL)
			queue_enqueue(q, node->right);
		free(node);
	}
	queue_destroy(q);
	free(q);
	tree->root = NULL;
}

void avl_destroy(AVLTree *tree){
	avl_clear(tree);
	free(tree);
}

size_t avl_size(AVLTree *tree){
	int result = 0;
	Queue *q = (Queue *)malloc(sizeof(Queue));
	queue_create(q);
	queue_enqueue(q, tree->root);
	AVLTreeNode *node = NULL;
	while (node = (AVLTreeNode *) queue_dequeue(q)){
		if (node->left != NULL)
			queue_enqueue(q, node->left);
		if (node->right != NULL)
			queue_enqueue(q, node->right);
		result++;
	}
	queue_destroy(q);
	free(q);
	return result;
}

static AVLTreeNode *avl_find_node(AVLTree *tree, Pointer data){
	AVLTreeNode *node = tree->root;
	while (node != NULL){
		//function like "return data-nodedata;"
		int cmp_res = tree->cmp_func(data, node->data);
		//item found
		if (cmp_res == 0)
			return node;
		//data less than nodedata. go to left
		if (cmp_res < 0)
			node = node->left;
		//data bigger than nodedata. go to rigth
		if (cmp_res > 0)
			node = node->right;
	}
	return NULL;
}

Pointer avl_find(AVLTree *tree, Pointer data){
	AVLTreeNode *node = avl_find_node(tree, data);
	if (node == NULL)
		return NULL;
	return node->data;
}

static AVLTree *avl_balance(AVLTree *tree){
	//todo balance
	return tree;
}

Pointer avl_insert(AVLTree *tree, Pointer data){
	AVLTreeNode **addto = &(tree->root);
	AVLTreeNode *parent = NULL;
	while (*addto != NULL){
		//function like "return data-nodedata;"
		int cmp_res = tree->cmp_func(data, (*addto)->data);
		//item found
		if (cmp_res == 0){
			Pointer result = (*addto)->data;
			(*addto)->data = data;
			return result;
		}
		parent = *addto;
		//data less than nodedata. go to left
		if (cmp_res < 0)
			addto = &((*addto)->left);
		//data bigger than nodedata. go to rigth
		if (cmp_res > 0)
			addto = &((*addto)->right);
	}
	AVLTreeNode *node = (AVLTreeNode *)malloc(sizeof(AVLTreeNode));
	node->data = data;
 	node->left = node->right = NULL;
	node->parent = parent;
	*addto = node;

	avl_balance(tree);

	return NULL;
		
}

Pointer avl_delete(AVLTree *tree, Pointer data){
	AVLTreeNode *node = avl_find_node(tree, data);
	AVLTreeNode **remfrom = NULL;
	if (node == NULL)
		return NULL;
	//AVLTreeNode **remfrom = &(tree->root); ??????????????????????????????????????????????????????
	remfrom = &(tree->root);
	if (node->parent != NULL){
		if (node->parent->left == node)
			remfrom = &(node->parent->left);
		if (node->parent->right == node)
			remfrom = &(node->parent->right);
	}
	Pointer result = node->data;

	if ((node->left == NULL) && (node->right == NULL)){
		*remfrom = NULL;
		free(node);
	}
	else
	if ((node->left == NULL) && (node->right != NULL)){
		AVLTreeNode *delnode = node->right;
		node->data = delnode->data;
		node->left = delnode->left;
		node->right = delnode->right;
		free(delnode);
	}
	else
	if ((node->left != NULL) && (node->right == NULL)){
		AVLTreeNode *delnode = node->left;
		node->data = delnode->data;
		node->left = delnode->left;
		node->right = delnode->right;
		free(delnode);
	}
	else{
		AVLTreeNode *delnode=node;
		//left subtree less than right subtree
		if (delnode->balance < 0){
			delnode = node->left;
			//find most right node in left subtree
			while (delnode->right != NULL)
				delnode = delnode->right;

		}else{
			//right subtree greater or equal than left subtree
			delnode = node->right;
			//find most left node in right subtree
			while (delnode->left != NULL)
				delnode = delnode->left;
			
		}
		node->data = delnode->data;
		node->left = delnode->left;
		node->right = delnode->right;
		free(delnode);
	}
	avl_balance(tree);
	return result;
}

void avl_foreach(AVLTree *tree, void(*foreach_func)(Pointer data, Pointer extra_data), Pointer extra_data){
	Queue *q = (Queue *)malloc(sizeof(Queue));
	queue_create(q);
	queue_enqueue(q, tree->root);
	AVLTreeNode *node = NULL;
	while (node = (AVLTreeNode *) queue_dequeue(q)){
		if (node->left != NULL)
			queue_enqueue(q, node->left);
		if (node->right != NULL)
			queue_enqueue(q, node->right);
		foreach_func(node->data, extra_data);
	}
	queue_destroy(q);
	free(q);
}