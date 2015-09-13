#include "stream.h"
#include <stdio.h>
#include <stdlib.h>
#include "error_proc.h"

Stream *stream_create(Source src_type, void *source){
	Stream *result = smalloc(sizeof(Stream));
	if (result == NULL)
		return NULL;
	result->pointer = 0;
	result->scr_type = src_type;
	result->src = source;
	return result;
}

int stream_next(Stream *str){
	int result = EOF;
	switch (str->scr_type)
	{
	case SRC_FILE:
		result = fgetc(str->src);
		if (result != EOF){
			ungetc(result, str->src);
		}
		break;
	case SRC_STRING:
		if (str->pointer == '\0'){
			result = EOF;
		}
		else {
			result = ((char *)str->src)[str->pointer];
		}
		break;
	default:
		break;
	}

	return result;
}

int stream_read(Stream *str){
	int result = EOF;
	switch (str->scr_type)
	{
	case SRC_FILE:
		result = fgetc(str->src);
		break;
	case SRC_STRING:
		if (str->pointer == '\0'){
			result = EOF;
		}
		else {
			result = ((char *)str->src)[str->pointer];
			str->pointer++;
		}
		break;
	default:
		break;
	}

	return result;
}

void stream_destroy(Stream *str){
	free(str);
}