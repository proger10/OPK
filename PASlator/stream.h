#ifndef STREAM_H
#define STREAM_H

typedef enum _source {
	SRC_FILE,
	SRC_STRING
} Source;

typedef struct _stream {
	Source scr_type;
	int pointer;
	void *src;
} Stream;

/*Create stream*/
Stream *stream_create(Source src_type, void *source);

/*get next char*/
int stream_next(Stream *str);

/*get next char and delete if from buffer*/
int stream_read(Stream *str);

/*Destroy stream*/
void stream_destroy(Stream *str);

#endif