#define _CRT_SECURE_NO_WARNINGS
#include "error_proc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void error_allocation(){
	printf("Error while memory allocation! PASlator will be closed.\n");
	exit(EXIT_FAILURE);
}

void *smalloc(size_t bytes){
	void *result = malloc(bytes);
	if (result == NULL)
		error_allocation();
	return result;
}

void *srealloc(void *data, size_t bytes){
	void *result = realloc(data, bytes);
	if (bytes != 0){
		if (result == NULL)
			error_allocation();
	}
	return result;
}

char *sstrdup(char *data){
	void *result = _strdup(data);
	if (data != NULL){
		if (result == NULL)
			error_allocation();
	}
	return result;
}


static char *lex_type_to_str(Lex_type type){
	switch (type){
	case LEX_ID:
		return "identifier";
		break;
	case LEX_PLUS:
		return "+";
		break;
	case LEX_ELSE:
		return "else";
		break;
	case LEX_MINUS:
		return "-";
		break;
	case LEX_MULT:
		return "*";
		break;
	case LEX_EQ:
		return "=";
		break;
	case LEX_LESS:
		return "<";
		break;
	case LEX_MORE:
		return ">";
		break;
	case LEX_DOT:
		return ".";
		break;
	case LEX_COMMA:
		return ",";
		break;
	case LEX_LB:
		return "(";
		break;
	case LEX_NOT:
		return "not";
		break;
	case LEX_RB:
		return ")";
		break;
	case LEX_COLON:
		return ":";
		break;
	case LEX_OP_SEP:
		return ";";
		break;
	case LEX_SHL:
		return "shl";
		break;
	case LEX_SHR:
		return "shr";
		break;
	case LEX_INT:
		return "integer";
		break;
	case LEX_CHAR:
		return "char";
		break;
	case LEX_NOT_EQ:
		return "<>";
		break;
	case LEX_LESS_EQ:
		return "<=";
		break;
	case LEX_MORE_EQ:
		return ">=";
		break;
	case LEX_ASSIGN:
		return ":=";
		break;
	case LEX_BEGIN:
		return "begin";
		break;
	case LEX_MOD:
		return "mod";
		break;
	case LEX_DIV:
		return "div";
		break;
	case LEX_XOR:
		return "xor";
		break;
	case LEX_OR:
		return "or";
		break;
	case LEX_END:
		return "end";
		break;
	case LEX_IF:
		return "if";
		break;
	case LEX_VAR:
		return "var";
		break;
	case LEX_CONST:
		return "const";
		break;
	case LEX_FOR:
		return "for";
		break;
	case LEX_THEN:
		return "then";
		break;
	case LEX_WHILE:
		return "while";
		break;
	case LEX_REPEAT:
		return "repeat";
		break;
	case LEX_UNTIL:
		return "until";
		break;
	case LEX_PROCEDURE:
		return "procedure";
		break;
	case LEX_FUNCTION:
		return "function";
		break;
	case LEX_TO:
		return "to";
		break;
	case LEX_DOWNTO:
		return "downto";
		break;
	case LEX_DO:
		return "do";
		break;
	case LEX_USES:
		return "uses";
		break;
	case LEX_TRUE:
		return "true";
		break;
	case LEX_FALSE:
		return "false";
		break;
	default:
		return "";
		break;
	}
}

static char *lex_to_str(Lex *lex){
	switch (lex->type)
	{
	case LEX_ID:
	case LEX_INT:
	case LEX_CHAR:{
					  char *result = smalloc(strlen(lex->value) + 3);
					  sprintf(result, "'%s'", lex->value);
					  return result;
					  break;
	}
	default:
		return lex_type_to_str(lex->type);
		break;
	}
}

void expect(Lex *lex, Lex_type expect){
	if (lex->type == expect){
		return;
	}
	//unexpected lex
	printf("Unexpected lex! %s expected, but %s got. PASlator will be closed.\n", lex_type_to_str(expect), lex_to_str(lex));
	exit(EXIT_FAILURE);
}

void error_dupl_id(char *id){
	printf("Duplicated id: %s. PASlator will be closed.\n", id);
	exit(EXIT_FAILURE);
}

void error_custom(char *error){
	printf("%s. PASlator will be closed.\n", error);
	exit(EXIT_FAILURE);
}