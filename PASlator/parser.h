#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "linked_list.h"
#include <stdbool.h>

typedef struct _parser{
	Lexer *lexer;
	SList *defined_vars;
	//Consts? lol @ in vars @ and fucntions in vars @ and procedures in vars @ ALL IN VARS
	//WHY @ because you can declare function/var/const named  same as some type @
	//and cant declare function/var/const with same name as previous function/var/const
	//except function with defferent arguments. it can be named same.
	int decl_level;
	bool flag;
} Parser;


Parser *parser_create(Lexer *lexer);
SList *parser_tree(Parser *parser);
void parser_tree_destroy(SList *tree);
void parser_destroy(Parser *parser);

#endif