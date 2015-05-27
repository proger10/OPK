#define _CRT_SECURE_NO_WARNINGS
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syntaxtree.h"
#include "linked_list.h"

Parser *parser_create(Lexer *lexer){
	Parser *result = malloc(sizeof(Parser));
	if (result == NULL)
		return NULL;
	result->lexer = lexer;
	result->decl_level = 0;
	result->defined_types = NULL;
	result->defined_vars = NULL;
	return result;
}

static int parse_int(char *strint){
	//TODO hex numbers, etc...
	int result = 0;
	//TODO security check, etc...

	sscanf(strint, "%d", &result);
	return result;
}


static int var_compare(VarDeclaration *data, VarDeclaration *needle){
	if (data->decl_level != needle->decl_level)
		return -1; // stop search
	if (strcmp(needle->name, data->name) == 0){
		//found
		return 0;
	}
	return 1;
}


static int var_c_compare(VarDeclaration *data, VarDeclaration *needle){
	if (data->decl_level != needle->decl_level)
		return -1; // stop search
	if (strcmp(needle->cname, data->cname) == 0){
		//found
		return 0;
	}
	return 1;
}

//return 1 if str is c-reserved name
static int reserved_name(char *str){
	//int, double, float, char, static, const, struct, enum, switch, return
	if (strcmp(str, "int") == 0)
		return 1;
	if (strcmp(str, "double") == 0)
		return 1;
	if (strcmp(str, "float") == 0)
		return 1;
	if (strcmp(str, "char") == 0)
		return 1;
	if (strcmp(str, "static") == 0)
		return 1;
	if (strcmp(str, "const") == 0)
		return 1;
	if (strcmp(str, "struct") == 0)
		return 1;
	if (strcmp(str, "enum") == 0)
		return 1;
	if (strcmp(str, "switch") == 0)
		return 1;
	if (strcmp(str, "return") == 0)
		return 1;
	if (strcmp(str, "typedef") == 0)
		return 1;

	return 0;
}

static int type_compare(TypeDeclaration *data, char *name){
	if (strcmp(name, data->name) == 0){
		//found
		return 0;
	}
	return 1;
}

static long long get_type_id(Parser *parser, char *type){
	SList *list_type = slist_find_custom(parser->defined_types, type, type_compare);
	if (list_type != NULL){
		return ((TypeDeclaration *)(list_type->data))->type_id;
	}

	if (strcmp(type, "integer") == 0){
		return TYPE_INTEGER;
	}
	if (strcmp(type, "real") == 0){
		return TYPE_REAL;
	}
	if (strcmp(type, "char") == 0){
		return TYPE_CHAR;
	}
	if (strcmp(type, "string") == 0){
		return TYPE_STRING;
	}
	return TYPE_UNKNOWN;
}

//parse ONE var declaration
static SList *var_decl(Parser *parser){//TODO or NOTTODO var enumumiration declaration
	Lex *lex = lex_cur(parser->lexer);
	if (lex->type == LEX_ID) {
		char *var_name = lex->value;
		lex_destroy_lex(lex);
		lex = lex_next(parser->lexer);

		VarDeclaration *var = malloc(sizeof(VarDeclaration));
		if (var == NULL){
			//TODO raise error. memory allocation error
		}
		var->decl_level = parser->decl_level;
		var->name = var_name;
		var->cname = _strdup(var_name);
		if (var->cname == NULL){
			//TODO. raise error. memory allocation failure
		}
		var->args = NULL;


		//find for duplicate id
		SList *duplicated_var_list = slist_find_custom(parser->defined_vars, var, var_compare);
		if (duplicated_var_list != NULL){
			VarDeclaration *duplicated_var = (VarDeclaration *)duplicated_var_list->data;
			if (duplicated_var->args != NULL){
				//TODO raise error. duplicate id
			}
			//have to assign new cname.
			int varname_len = strlen(var->cname);
			while (slist_find_custom(parser->defined_vars, var, var_c_compare) != NULL){
				var->cname = realloc(var->cname, sizeof(char)*(varname_len + 2));
				if (var->cname == NULL){
					//TODO. raise error. memory allocation failure
				}
				varname_len++;
				var->cname[varname_len] = '_';
				var->cname[varname_len + 1] = '\0';
			}
		}
		//check if it is c-reserved name
		if (reserved_name(var->cname)){
			//add _ (=
			int varname_len = strlen(var->cname);
			var->cname = realloc(var->cname, (strlen(var->cname) + 2)*sizeof(char));
			if (var->cname == NULL){
				//TODO. raise error. memory allocation failure
			}
			var->cname[varname_len + 1] = '_';
			var->cname[varname_len + 2] = '\0';
		}


		//Should be :
		if (lex->type != LEX_COLON){
			//TODO. Raise error
		}
		lex_free_lex(lex);

		lex = lex_next(parser->lexer);
		//should be id
		if (lex->type != LEX_ID){
			//TODO. raise error
		}
		var->type_id = get_type_id(parser, lex->value);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		if (var->type_id == TYPE_UNKNOWN){
			//TODO. raise error
		}

		//Should be ;
		if (lex->type != LEX_OP_SEP){
			//TODO. Raise error
		}
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);

		//assign var_id and set var as non-callable
		var->type_id = parser->var_num;
		(parser->var_num)++;
		var->callable = false;
		//add created var declaration to declaration list
		parser->defined_vars = slist_prepend(parser->defined_vars, var);
		//creating syntax element 
		Syntax *syn = malloc(sizeof(Syntax));
		if (syn == NULL){
			//TUDO. memory allocation error
		}
		syn->type = SYN_VAR;
		syn->syn = var;
		return slist_prepend(NULL, syn);
	}
	return NULL;
}

//var
static SList *gram_var(Parser *parser){
	Lex *lex = lex_cur(parser->lexer);
	SList *result = NULL;
	//should be var
	if (lex->type != LEX_VAR){
		return NULL;
	}
	lex_destroy_lex(lex);
	lex = lex_next(parser->lexer);

	SList *syn = NULL;
	while ((syn = var_decl(parser)) != NULL){
		result = slist_contact(syn, result);
	}
	return result;
}

//type
static SList *gram_type(Parser *parser){

	return NULL;
}

//function
static SList *gram_function(Parser *parser){

	return NULL;
}

//procedure
static SList *gram_procedure(Parser *parser){

	return NULL;
}


//const
static SList *gram_const(Parser *parser){

	return NULL;
}

//definitions
static SList *gram_definition(Parser *parser){
	SList *result = NULL; // syntax list
	Lex *lex = lex_cur(parser->lexer);
	SList *syn = NULL;

	while ((syn = gram_var(parser)) != NULL){
		result = slist_contact(syn, result);
	}

	while ((syn = gram_type(parser)) != NULL){
		result = slist_contact(syn, result);
	}

	while ((syn = gram_procedure(parser)) != NULL){
		result = slist_contact(syn, result);
	}

	while ((syn = gram_function(parser)) != NULL){
		result = slist_contact(syn, result);
	}

	while ((syn = gram_const(parser)) != NULL){
		result = slist_contact(syn, result);
	}

	return result;
}

//for
static SList *gram_for(Parser *parser){//TODO me
	Lex *lex = lex_cur(parser->lexer);
	SList *result = NULL;
	//should be for
	if (lex->type == LEX_FOR) {
		lex_destroy_lex(lex);
		lex = lex_next(parser->lexer);
		//should be id
		if (lex->type != LEX_ID){
			//TODO. raise error
		}

	}

	return result;
}

static int var_name_compare(VarDeclaration *data, char *needle){\
	if (strcmp(needle, data->name) == 0){
		//found
		return 0;
	}
	return 1;
}


//statement
static SList *st(Parser *parser){
	SList *result = NULL; // syntax list
	Lex *lex = lex_cur(parser->lexer);
	SList *syn = NULL;

	if ((syn = gram_for(parser)) != NULL){
		return syn;
	}
	/*other st like if-then-else, while-do, repeat-until*/
	//TODO
	/*function calls*/
	//TODO
	//write/writeln call. writeln is not function in my realization. too strange argument policy
	//TODO

	//assignment
	if (lex->type == LEX_ID){
		//get var id
		VarDeclaration *var = NULL;
		SList *var_list = slist_find_custom(parser->defined_vars, lex->value, var_name_compare);
		if (var_list != NULL)
			var = var_list->data;
	}


	//TODO raise error. unknown construction
	return NULL;
}

static SList *program(Parser *parser){
	SList *result = NULL; // syntax list


	Lex *lex = lex_cur(parser->lexer);
	if (lex->type = LEX_PROGRAM){
		//program detected.
		//now waiting for program_name and ;
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		if (lex->type != LEX_ID){
			//TODO. raise error. how? i dont know....
			//parser->error = 
		}
		//making new syn
		Syntax *syn = malloc(sizeof(Syntax));
		if (syn == NULL){
			//TUDO. raise error
		}
		syn->type = SYN_PROGRAM;
		syn->syn = lex->value;
		result = slist_prepend(result, syn);
		lex_destroy_lex(lex);
		lex = lex_next(parser->lexer);
		if (lex->type != LEX_OP_SEP){
			//TODO. raise error

		}
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
	}


	//Reading definitions (type, var, function, etc).
	SList *define = NULL;
	while ((define = gram_definition(parser))){
		result = slist_contact(define, result);
	}

	/*Main block*/

	//waiting for begin and (optional) ;
	lex = lex_cur(parser->lexer);
	if (lex->type != LEX_BEGIN){
		//TODO. raise error
	}
	lex_free_lex(lex);
	lex = lex_next(parser->lexer);
	if (lex->type == LEX_OP_SEP){
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
	}

	//Reading staments (for, while, assigment, if, call procedure, etc);
	SList *stament = NULL;
	while ((define = st(parser))){
		result = slist_contact(stament, result);
	}

	//waiting for end and .
	lex = lex_cur(parser->lexer);
	if (lex->type != LEX_END){
		//TODO. raise error
	}
	lex_free_lex(lex);
	lex = lex_next(parser->lexer);
	if (lex->type != LEX_DOT){
		//TODO. raise error
	}
	lex_free_lex(lex);
	//Ignore anything else.


	//TODO DESTROY DECLARATION LISTS!!!!
	slist_free(parser->defined_types);
	slist_free(parser->defined_vars);
	return result;
}

static void EXPR(Parser *parser){
	Lex *lex = lex_next(parser->lexer);
	if (lex->type == LEX_INT){
		int num = parse_int(lex->value);
		printf("%d", num);
	}
	lex_free_lex(lex);
}

SList *parser_tree(Parser *parser){
	printf("IAM MEGA PAS PARSER!!!! \n");
	return program(parser);
}

void parser_destroy(Parser *parser){
	if (parser == NULL)
		return;
	free(parser);
}


static void free_syntax(Syntax *syn, void *userdata){
	VarDeclaration *var;//studio bug =(
	switch (syn->type){
	case SYN_PROGRAM:
		free(syn->syn);
		break;
	case SYN_VAR:
		var = syn->syn;
		//TODO destroy args
		free(var->cname);
		free(var->name);
		free(var);
		break;
	default:
		break;
	}
	free(syn);
}

void parser_tree_destroy(SList *tree){
	slist_foreach(tree, free_syntax, NULL);
	slist_free(tree);
}