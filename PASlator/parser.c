#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

Parser *parser_create(Lexer *lexer){
	Parser *result = malloc(sizeof(Parser));
	if (result == NULL)
		return NULL;
	result->lexer = lexer;
	return result;
}

static int parse_int(char *strint){
	//TODO hex numbers, etc...
	int result = 0;
	//TODO security check, etc...

	sscanf(strint, "%d", &result);
	return result;
}

static void EXPR(Parser *parser){
	Lex *lex = lex_next(parser->lexer);
	if (lex->type == LEX_INT){
		int num = parse_int(lex->value);
		printf("%d", num);
	}
	lex_free(lex);
}

void parser_process(Parser *parser){
	printf("IAM MEGA PAS PARSER!!!! \n");
}

void parser_destroy(Parser *parser){
	if (parser == NULL)
		return;
	free(parser);
}
