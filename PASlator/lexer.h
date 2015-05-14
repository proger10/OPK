#ifndef LEXER_H
#define LEXER_H

#include "stream.h"


typedef enum _lex_type{
	LEX_ID,
	LEX_PLUS,			// +
	LEX_MINUS,			// -
	LEX_MULT,			// *
	LEX_R_DIV,			// /
	LEX_EQ,				// =
	LEX_LESS,			// <
	LEX_MORE,			// >
	LEX_SQ_BR_OP,		// [
	LEX_SQ_BR_CL,		// ]
	LEX_DOT,			// .
	LEX_COMMA,			// ,
	LEX_LB,				// (
	LEX_RB,				// )
	LEX_COLON,			// :
	LEX_OP_SEP,			// ;
	LEX_PTR,			// ^
	LEX_SOBAKA,//rename // @
	LEX_BRACE_OP,		// {
	LEX_BRACE_CL,		// }
	LEX_DOLLAR,			// $
	LEX_SHARP,			// #
	LEX_AMP,			// &
	LEX_PERC,			// %
	LEX_SHL,			// << or shl
	LEX_SHR,			// >> or shr
	LEX_INT,
	LEX_REAL,
	LEX_NOT_EQ,			// <>
	LEX_LESS_EQ,		// <=
	LEX_MORE_EQ,		// >=
	LEX_ASSIGN,			// :=
	LEX_COMMENT_OP,		// (*
	LEX_COMMENT_CL,		// *)
	LEX_LINE_COMMENT,	// //
	LEX_PROGRAM,
	LEX_BEGIN,
	LEX_END,
	LEX_IF,
	LEX_FOR,
	LEX_THEN,
	LEX_WHILE,
	LEX_REPEAT,
	LEX_UNTIL,
	LEX_TO,
	LEX_DO,
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