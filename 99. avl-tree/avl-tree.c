#include "avl-tree.h"
#include "queue.h"
#include <stdlib.h>
#include <assert.h>

AVLTree *avl_create(CmpFunc cmp_func){
	AVLTree *result = (AVLTree *)malloc(sizeof(AVLTree));
	if (result == NULL)
		return NULL;
	result->cmp_func = cmp_func;
	result->root = NULL;
	return result;
}

void avl_clear(AVLTree *tree){
	Queue q;
	queue_create(&q);
	queue_enqueue(&q, tree->root);
	AVLTreeNode *node = NULL;
	while (node = (AVLTreeNode *)queue_dequeue(&q)){
		if (node->left != NULL)
			queue_enqueue(&q, node->left);
		if (node->right != NULL)
			queue_enqueue(&q, node->right);
		free(node);
	}
	queue_destroy(&q);
	tree->root = NULL;
}

void avl_destroy(AVLTree *tree){
	avl_clear(tree);
	free(tree);
}

size_t avl_size(AVLTree *tree){
	int result = 0;
	Queue q;
	queue_create(&q);
	bool res = queue_enqueue(&q, tree->root);
	assert(res);
	AVLTreeNode *node = NULL;
	while (node = (AVLTreeNode *)queue_dequeue(&q)){
		result++;
		if (node->left != NULL) {
			bool res = queue_enqueue(&q, node->left);
			assert(res);
		}
		if (node->right != NULL) {
			bool res = queue_enqueue(&q, node->right);
			assert(res);
		}
	}
	queue_destroy(&q);
	return result;
}

static AVLTreeNode *avl_find_node(AVLTree *tree, Pointer data){
	AVLTreeNode *node = tree->root;
	while (node != NULL){
		//function like "return data-nodedata;"
		int cmp_res = tree->cmp_func(data, node->data);
		if (cmp_res == 0){
			//item found
			return node;
		}
		else if (cmp_res < 0){
			//data less than nodedata. go to left
			node = node->left;
		}
		else {
			assert(cmp_res > 0);
			//data bigger than nodedata. go to rigth
			node = node->right;
		}
	}
	return NULL;
}

Pointer avl_find(AVLTree *tree, Pointer data){
	AVLTreeNode *node = avl_find_node(tree, data);
	if (node == NULL)
		return NULL;
	return node->data;
}

static void avl_left_right_case_rotation(AVLTree *tree, AVLTreeNode *pivot){
	int balance_case = pivot->left->right->balance;
	assert(abs(balance_case) <= 1);


	AVLTreeNode *subtreeA = pivot->left->left;
	AVLTreeNode *subtreeB = pivot->left->right->left;
	AVLTreeNode *subtreeC = pivot->left->right->right;
	AVLTreeNode *subtreeD = pivot->left;

	AVLTreeNode *a_node = pivot;
	AVLTreeNode *b_node = pivot->left;
	AVLTreeNode *c_node = pivot->left->right;

	Pointer a_data = a_node->data;
	Pointer b_data = b_node->data;
	Pointer c_data = c_node->data;
	
	//Fix new root (c node)
	a_node->data = c_data;
	a_node->left = b_node;
	a_node->right = c_node;
	a_node->balance = 0;

	//Fix b node
	b_node->left = subtreeA;
	b_node->right = subtreeB;
	b_node->parent = a_node;
	if (balance_case == -1)
		b_node->balance = 1;
	else
		b_node->balance = 0;

	//Fix c node
	c_node->left = subtreeC;
	c_node->right = subtreeD;
	c_node->data = a_data;
	if (balance_case == 1)
		c_node->balance = -1;
	else
		c_node->balance = 0;
}

static void avl_right_left_case_rotation(AVLTree *tree, AVLTreeNode *pivot){
	int balance_case = pivot->right->left->balance;
	assert(abs(balance_case) <= 1);


	AVLTreeNode *subtreeA = pivot->left;
	AVLTreeNode *subtreeB = pivot->right->left->left;
	AVLTreeNode *subtreeC = pivot->right->left->right;
	AVLTreeNode *subtreeD = pivot->right->right;

	AVLTreeNode *a_node = pivot;
	AVLTreeNode *b_node = pivot->right;
	AVLTreeNode *c_node = pivot->right->left;

	Pointer a_data = a_node->data;
	Pointer b_data = b_node->data;
	Pointer c_data = c_node->data;

	//Fix new root (c node)
	a_node->data = c_data;
	a_node->left = c_node;
	a_node->right = b_node;
	a_node->balance = 0;

	//Fix b node
	b_node->left = subtreeC;
	b_node->right = subtreeD;
	b_node->parent = a_node;
	if (balance_case == 1)
		b_node->balance = -1;
	else
		b_node->balance = 0;

	//Fix c node
	c_node->left = subtreeA;
	c_node->right = subtreeB;
	c_node->data = a_data;
	if (balance_case == -1)
		c_node->balance = -1;
	else
		c_node->balance = 0;
}

static void avl_left_left_case_rotation(AVLTree *tree, AVLTreeNode *pivot){
	int balance_case_x = pivot->left->left->balance;
	int balance_case = pivot->left->balance;
	assert((balance_case == 1) || (balance_case == 0));
	assert(abs(balance_case_x) <= 1);


	AVLTreeNode *a_node = pivot;
	AVLTreeNode *b_node = pivot->left;
	AVLTreeNode *c_node = pivot->left->left;

	AVLTreeNode *subtreeA = c_node->left;
	AVLTreeNode *subtreeB = c_node->right;
	AVLTreeNode *subtreeC = b_node->right;
	AVLTreeNode *subtreeD = a_node->right;

	Pointer a_data = a_node->data;
	Pointer b_data = b_node->data;
	Pointer c_data = c_node->data;

	//Fix new root (c node)
	a_node->data = b_data;
	a_node->left = b_node;
	a_node->right = c_node;
	if (balance_case == 1)
		a_node->balance = 0;
	else
		a_node->balance = -1;

	//Fix b node
	b_node->data = c_data;
	b_node->left = subtreeA;
	b_node->right = subtreeB;
	b_node->parent = a_node;
	b_node->balance = balance_case_x;

	//Fix c node
	c_node->data = a_data;
	c_node->left = subtreeC;
	c_node->right = subtreeD;
	c_node->parent = a_node;
	if (balance_case == 1)
		a_node->balance = 0;
	else
		a_node->balance = 1;
}

static void avl_right_right_case_rotation(AVLTree *tree, AVLTreeNode *pivot){
	int balance_case_x = pivot->left->left->balance;
	int balance_case = pivot->left->balance;
	assert((balance_case == 1) || (balance_case == 0));
	assert(abs(balance_case_x) <= 1);


	AVLTreeNode *a_node = pivot;
	AVLTreeNode *b_node = pivot->left;
	AVLTreeNode *c_node = pivot->left->left;

	AVLTreeNode *subtreeA = c_node->left;
	AVLTreeNode *subtreeB = c_node->right;
	AVLTreeNode *subtreeC = b_node->right;
	AVLTreeNode *subtreeD = a_node->right;

	Pointer a_data = a_node->data;
	Pointer b_data = b_node->data;
	Pointer c_data = c_node->data;

	//Fix new root (c node)
	a_node->data = b_data;
	a_node->left = b_node;
	a_node->right = c_node;
	if (balance_case == 1)
		a_node->balance = 0;
	else
		a_node->balance = -1;

	//Fix b node
	b_node->data = c_data;
	b_node->left = subtreeA;
	b_node->right = subtreeB;
	b_node->parent = a_node;
	b_node->balance = balance_case_x;

	//Fix c node
	c_node->data = a_data;
	c_node->left = subtreeC;
	c_node->right = subtreeD;
	c_node->parent = a_node;
	if (balance_case == 1)
		a_node->balance = 0;
	else
		a_node->balance = 1;
}

static void avl_balance(AVLTree *tree){
	//todo balance
	return;
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
		//data bigger than nodedata. go to right
		if (cmp_res > 0)
			addto = &((*addto)->right);
	}
	AVLTreeNode *node = (AVLTreeNode *)malloc(sizeof(AVLTreeNode));
	if (node == NULL)
	{
		return NULL;
	}
	node->data = data;
	node->left = node->right = NULL;
	node->parent = parent;
	node->balance = 0;
	*addto = node;

	avl_balance(tree);

	return NULL;

}

Pointer avl_delete(AVLTree *tree, Pointer data){
	AVLTreeNode *node = avl_find_node(tree, data);
	//AVLTreeNode **remfrom = NULL;
	if (node == NULL)
		return NULL;
	AVLTreeNode **remfrom = &(tree->root);// ??????????????????????????????????????????????????????
	//remfrom = &(tree->root);
	if (node->parent != NULL){
		if (node->parent->left == node)
			remfrom = &(node->parent->left);
		if (node->parent->right == node)
			remfrom = &(node->parent->right);
	}
	Pointer result = node->data;

	//Node without children
	if ((node->left == NULL) && (node->right == NULL)){
		//Unkink node and parent node
		*remfrom = NULL;
		//Free node
		free(node);
	}
	else{
		//Node with one (right) child
		if ((node->left == NULL) && (node->right != NULL)){
			AVLTreeNode *replnode = node->right;
			node->data = replnode->data;
			node->left = replnode->left;
			node->right = replnode->right;
			//Fix parent for replnode children
			if (node->left){
				node->left->parent = node;
			}
			if (node->right){
				node->right->parent = node;
			}
			//Free replacing node
			free(replnode);
		}
		else{
			//Node with one (left) child
			if ((node->left != NULL) && (node->right == NULL)){
				AVLTreeNode *replnode = node->left;
				node->data = replnode->data;
				node->left = replnode->left;
				node->right = replnode->right;
				//Fix parent for replnode children
				if (node->left){
					node->left->parent = node;
				}
				if (node->right){
					node->right->parent = node;
				}
				//Free replacing node
				free(replnode);
			}
			else{
				//Node with both children
				AVLTreeNode *replnode;// node to replace
				AVLTreeNode *child; // child node

				if (node->balance < 0){
					//left subtree less than right subtree
					replnode = node->left;
					//find most right node in left subtree
					while (replnode->right != NULL)
						replnode = replnode->right;
					child = replnode->left;
				}
				else{
					//right subtree greater than or equal to left subtree
					replnode = node->right;
					//find most left node in right subtree
					while (replnode->left != NULL)
						replnode = replnode->left;
					child = replnode->right;

				}
				//Replace data for deleting node
				node->data = replnode->data;
				if (child == NULL){
					//if repacing node have no child
					//unlink parent
					if (replnode->parent->left == replnode)
						replnode->parent->left = NULL;
					if (replnode->parent->right == replnode)
						replnode->parent->right = NULL;
					//free replacing node
					free(replnode);
				}
				else{
					//replacing node have a child
					//move child to replacing node
					replnode->data = child->data;
					replnode->left = child->left;
					replnode->right = child->right;
					//Fix parent for child children
					if (child->left){
						child->left->parent = replnode;
					}
					if (child->right){
						child->right->parent = replnode;
					}
					//free child
					free(child);
				}
			}
		}
	}

	avl_balance(tree);
	return result;
}

void avl_foreach(AVLTree *tree, void(*foreach_func)(Pointer data, Pointer extra_data), Pointer extra_data){
	Queue q;
	queue_create(&q);
	queue_enqueue(&q, tree->root);
	AVLTreeNode *node = NULL;
	while (node = (AVLTreeNode *)queue_dequeue(&q)){
		if (node->left != NULL)
			queue_enqueue(&q, node->left);
		if (node->right != NULL)
			queue_enqueue(&q, node->right);
		foreach_func(node->data, extra_data);
	}
	queue_destroy(&q);
}
