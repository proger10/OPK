#ifndef LEXER_H
#define LEXER_H

#include "stream.h"


typedef enum _lex_type{
	LEX_WORD,
	LEX_OP,
	LEX_LB,
	LEX_RB,
	LEX_INT,
	LEX_REAL,
	LEX_UNKNOWN,
	LEX_NO
} Lex_type;

typedef enum _state{
	ST_SEP,
	ST_ERROR,
	ST_WORD
} SM_state;

typedef struct _lex{
	Lex_type type;
	void *value;
} Lex;

typedef struct _lexer{
	Stream *inp;
	SM_state state;
}Lexer;

/*Create lexer from stream*/
Lexer *lex_create(Stream *in_stream);

/*Get next lex or NULL on failure*/
Lex *lex_next(Lexer *lexer);

/*Free memmory for lex*/
void lex_free(Lex *lex);

/*Destroy lexer*/
void lex_destroy(Lexer *lexer);


#endif
