#include "d_str.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>


String *str_create(){
	String *result = malloc(sizeof(String));
	if (result == NULL)
		return NULL;
	result->data_size = 0;
	result->size = 0;
	result->str = NULL;
	return result;
}

int str_add(String *str, char a){
	if (str == NULL)
		return 0;
	if (str->size <= str->data_size + 1){
		char *realloc_str = realloc(str->str, str->data_size + 10);
		if (realloc_str == NULL)
			return 0;
		realloc_str[str->data_size] = '\0';
		str->size += 10;
		str->str = realloc_str;
	}
	assert(str->str[str->data_size] == '\0');
	str->str[str->data_size] = a;
	str->str[str->data_size + 1] = '\0';
	str->data_size++;
	return 1;
}

char *str_get(String *str){
	if (str == NULL)
		return NULL;
	return str->str;
}


int str_compare(String *str, char *sample){
	if ((str == NULL) || (sample == NULL)){
		return 0;
	}
	//TODO Think about it. Really strange one.
	if (strlen(str->str) == strlen(sample)){
		return strcmp(str->str, sample) == 0;
	}
	return 0;
}

void str_free(String *str){
	if (str == NULL)
		return;
	free(str);
}

void str_destroy(String *str){
	if (str == NULL)
		return;
	free(str->str);
	free(str);
}