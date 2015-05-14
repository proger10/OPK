#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct _parser{
	Lexer *lexer;
} Parser;

Parser *parser_create(Lexer *lexer);
void parser_process(Parser *parser);
void parser_destroy(Parser *parser);

#endif
