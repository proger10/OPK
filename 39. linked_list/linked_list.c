#include "linked_list.h"
#include <stdlib.h>

SList *slist_append(SList *list, Pointer data){
	if (list == NULL){
		SList *result = (SList *)malloc(sizeof(SList));
		if (result == NULL){
			return list;
		}
		result->data = data;
		result->next = NULL;
		return result;
	}
	SList *ret = list;
	while (list->next != NULL)
		list = list->next;
	SList *result = (SList *)malloc(sizeof(SList));
	if (result == NULL){
		return list;
	}
	result->data = data;
	result->next = NULL;
	list->next = result;
	return ret;
}

SList *slist_prepend(SList *list, Pointer data){
	SList *result = (SList *)malloc(sizeof(SList));
	if (result == NULL){
		return list;
	}
	result->data = data;
	result->next = list;
	return result;
}


void slist_insert(SList *sibling, Pointer data){
	if (sibling == NULL){
		return;
	}
	SList *next = sibling->next;

	SList *result = (SList *)malloc(sizeof(SList));
	if (result == NULL){
		return ;
	}
	result->data = data;
	result->next = next;
	sibling->next = result;
}

SList *slist_remove(SList *list, Pointer data){
	SList *last = NULL;
	for (SList *i = list; i != NULL; i = i->next){
		if (i->data == data){
			if (last != NULL)
				last->next = i->next;
			if (i == list)
				list = list->next;
			free(i->data);
			free(i);
			break;
		}
		last = i;
	}
	return list;
}

SList *slist_remove_all(SList *list, Pointer data){
	SList *last = NULL;
	SList *next = NULL;
	for (SList *i = list; i != NULL; i = next){
		next = i->next;
		if (i->data == data){
			if (last != NULL)
				last->next = i->next;
			if (i == list)
				list = list->next;
			free(i->data);
			free(i);
		}
		last = i;
	}
	return list;
}

Pointer slist_remove_next(SList *sibling){
	if (sibling == NULL)
		return NULL;
	if (sibling->next == NULL)
		return NULL;
	//Pointer result = sibling->next->data;
	void *result = sibling->next->data;
	sibling->next = sibling->next->next;
	free(sibling->next);
	return result;
}

void slist_free(SList *list){
	SList *i = list;
	while(i != NULL){
		SList *next = i->next;
		free(i->data);
		free(i);
		i = next;
	}
}

unsigned slist_length(SList *list){
	unsigned size = 0;
	for (SList *i = list; i != NULL; i = i -> next){
		size++;
	}
	return size;
}

SList *slist_copy(SList *list){
	SList *result = NULL;
	SList *p = NULL;
	for (SList *i = list; i != NULL; i = i->next){
		if (p == NULL){
			result = slist_append(result, i->data);
			p = result;
		}
		else{
			p->next = slist_append(NULL, i->data);
			p = p->next;
		}

	}
	return result;
}

SList *slist_reverse(SList *list){
	SList *result = NULL;
	for (SList *i = list; i != NULL; i = i->next){
		result = slist_prepend(result, i->data);
	}
	return result;
}

SList *slist_concat(SList *list1, SList *list2){
	SList *result = NULL;
	SList *p = NULL;
	for (SList *i = list1; i != NULL; i = i->next){
		if (p == NULL){
			result = slist_append(result, i->data);
			p = result;
		}
		else{
			p->next = slist_append(NULL, i->data);
			p = p->next;
		}

	}
	for (SList *i = list2; i != NULL; i = i->next){
		if (p == NULL){
			result = slist_append(result, i->data);
			p = result;
		}
		else{
			p->next = slist_append(NULL, i->data);
			p = p->next;
		}

	}
	return result;
}

void slist_foreach(SList *list, void (*func)(Pointer data, Pointer user_data), Pointer user_data){
	for (SList *i = list; i != NULL; i = i->next){
		(*func) (i->data, user_data);
	}
}

SList *slist_last(SList *list){
	for (SList *i = list; i != NULL; i = i->next){
		if (i->next == NULL)
			return i;
	}
	return NULL;
}


SList *slist_nth(SList *list, int n){
	int size = slist_length(list);
	while (n < 0)
		n += size;
	int k=0; //?????????
	SList *i;
	for (i = list; i != NULL, k!=n; k++, i = i->next){
		if (i->next == NULL)
			return i;
	}
	return i;
}

SList *slist_find(SList *list, Pointer data){
	for (SList *i = list; i != NULL; i = i->next){
		if (i->data == data)
			return i;
	}
	return NULL;
}

SList *slist_find_custom(SList *list, Pointer data, int(*compare_func)(Pointer a, Pointer b)){
	for (SList *i = list; i != NULL; i = i->next){
		if ((*compare_func)(i->data, data) == 0)
			return i;
	}
	return NULL;
}

int slist_position(SList *list, SList *el){
	int k = 0; //?????????
	SList *i;
	for (i = list; i != NULL; k++, i = i->next){
		if (i == el)
			return k;
	}
	return -1;
}