#ifndef D_STR_H
#define D_STR_H

#include <string.h>

typedef struct _d_str{
	char *str;
	size_t data_size;
	size_t size;
} String;

String *str_create();
int str_add(String *str, char a);
int str_compare(String *str, char *sample);
char *str_get(String *str);
void str_free(String *str);
void str_destroy(String *str);

#endif
