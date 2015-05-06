#include "linked_list.h"
#include <stdlib.h>

SList *slist_append(SList *list, Pointer data){
	if (list == NULL){
		return slist_prepend(NULL, data);
	}
	else {
		slist_insert(slist_last(list), data);
		return list;
	}
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
		return;
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
			free(i);
		}
		else{
			last = i;
		}
	}
	return list;
}

Pointer slist_remove_next(SList *sibling){
	if (sibling == NULL)
		return NULL;
	if (sibling->next == NULL)
		return NULL;
	Pointer result = sibling->next->data;
	SList *next = sibling->next->next;
	free(sibling->next);
	sibling->next = next;
	return result;
}

void slist_free(SList *list){
	SList *i = list;
	while (i != NULL){
		SList *next = i->next;
		free(i);
		i = next;
	}
}

unsigned slist_length(SList *list){
	unsigned size = 0;
	for (SList *i = list; i != NULL; i = i->next){
		size++;
	}
	return size;
}

SList *slist_copy(SList *list){
	return slist_concat(list, NULL);
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
	SList *last = NULL;
	for (SList *i = list1; i != NULL; i = i->next){
		if (last == NULL){
			result = slist_prepend(NULL, i->data);
			last = result;
		}
		else{
			last->next = slist_prepend(NULL, i->data);
			last = last->next;
		}
	}
	for (SList *i = list2; i != NULL; i = i->next){
		if (last == NULL){
			result = slist_prepend(NULL, i->data);
			last = result;
		}
		else{
			last->next = slist_prepend(NULL, i->data);
			last = last->next;
		}

	}
	return result;
}

void slist_foreach(SList *list, void(*func)(Pointer data, Pointer user_data), Pointer user_data){
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
	if (list == NULL)
		return NULL;

	if (n < 0){
		int size = slist_length(list);
		while (n < 0)
			n += size;
	}
	
	SList *i = list;
	for (int k = 0; k != n; k++, i = i->next){
		if (i == NULL){
			i = list;
		}
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

SList *slist_find_custom(SList *list, Pointer data, int(*compare_func)(Pointer, Pointer)){
	for (SList *i = list; i != NULL; i = i->next){
		if ((*compare_func)(i->data, data) == 0)
			return i;
	}
	return NULL;
}

SList *slist_find_custom_param(SList *list, Pointer needle, int(*compare_func)(Pointer a, Pointer b, Pointer param), Pointer param){
	for (SList *i = list; i != NULL; i = i->next){
		if (compare_func(i->data, needle, param) == 0)
			return i;
	}
	return NULL;
}

int slist_position(SList *list, SList *el){
	if (list == NULL)
		return -1;
	int k = 0;
	SList *i = list;
	for (i = list; i != NULL; k++, i = i->next){
		if (i == el)
			return k;
	}
	return -1;
}
