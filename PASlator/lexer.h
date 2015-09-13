#ifndef LEXER_H
#define LEXER_H

#include "stream.h"


typedef enum _lex_type{
	LEX_ID,
	LEX_PLUS,			// +
	LEX_MINUS,			// -
	LEX_MULT,			// *
	LEX_EQ,				// =
	LEX_LESS,			// <
	LEX_MORE,			// >
	LEX_DOT,			// .
	LEX_COMMA,			// ,
	LEX_LB,				// (
	LEX_RB,				// )
	LEX_COLON,			// :
	LEX_OP_SEP,			// ;
	LEX_SHL,			// << or shl
	LEX_SHR,			// >> or shr
	LEX_INT,
	LEX_NOT_EQ,			// <>
	LEX_LESS_EQ,		// <=
	LEX_MORE_EQ,		// >=
	LEX_ASSIGN,			// :=
	LEX_BEGIN,
	LEX_END,
	LEX_IF,
	LEX_NOT,
	LEX_DIV,
	LEX_MOD,
	LEX_AND,
	LEX_XOR,
	LEX_OR,
	LEX_VAR,
	LEX_CONST,
	LEX_FOR,
	LEX_TRUE,
	LEX_FALSE,
	LEX_THEN,
	LEX_ELSE,
	LEX_WHILE,
	LEX_REPEAT,
	LEX_UNTIL,
	LEX_PROCEDURE,
	LEX_FUNCTION,
	LEX_TO,
	LEX_DOWNTO,
	LEX_USES,
	LEX_DO,
	LEX_CHAR,
	LEX_UNKNOWN,
	LEX_NO
} Lex_type;

typedef enum _state{
	ST_SEP,
	ST_ERROR,
	ST_WORD,
	ST_OP_SEP,
	ST_COLON,
	ST_ASSIGN,
	ST_INT,
	ST_LB,
	ST_RB,
	ST_PLUS,
	ST_MINUS,
	ST_MULT,
	ST_EQ,
	ST_COMMA,
	ST_LESS,
	ST_MORE,
	ST_CHAR,
	ST_NOT_EQ,
	ST_LESS_EQ,
	ST_SHL,
	ST_SHR,
	ST_MORE_EQ,
	ST_DOT
} SM_state;

typedef struct _lex{
	Lex_type type;
	void *value;
} Lex;

typedef struct _lexer{
	Stream *inp;
	SM_state state;
	Lex *cur;
}Lexer;

/*Create lexer from stream*/
Lexer *lex_create(Stream *in_stream);

/*Get next lex or NULL on failure*/
Lex *lex_next(Lexer *lexer);

/*Get cur lex or NULL on failure*/
Lex *lex_cur(Lexer *lexer);

/*Free memmory for lex*/
void lex_free_lex(Lex *lex);

/*Free memmory for lex, but lex string data*/
void lex_destroy_lex(Lex *lex);

/*Destroy lexer*/
void lex_destroy(Lexer *lexer);


#endif