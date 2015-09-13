#define _CRT_SECURE_NO_WARNINGS
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syntaxtree.h"
#include "linked_list.h"
#include "error_proc.h"

Parser *parser_create(Lexer *lexer) {
	Parser *result = smalloc(sizeof(Parser));
	if (result == NULL)
		return NULL;
	result->lexer = lexer;
	result->decl_level = 0;
	result->defined_vars = NULL;
	result->flag = true;
	return result;
}

static int parse_int(char *string) {
	//TODO hex numbers, etc... AHAHAHAHA anything else you'd like?
	int result = 0;
	if (sscanf(string, "%d", &result) != 1) {
		printf("%s\n", string);
		error_custom("Can't parse integer");
	}
	return result;
}


static int var_compare(VarDeclaration *data, VarDeclaration *needle) {
	if (data->decl_level != needle->decl_level)
		return -1; // stop search
	if (strcmp(needle->name, data->name) == 0) {
		//found
		return 0;
	}
	return 1;
}


static int var_c_compare(VarDeclaration *data, VarDeclaration *needle) {
	if (data->decl_level != needle->decl_level)
		return -1; // stop search
	if (strcmp(needle->cname, data->cname) == 0) {
		//found
		return 0;
	}
	return 1;
}

//return 1 if str is c-reserved name
static int reserved_name(char *str) {
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

static long long get_type_id(char *type) {
	if (strcmp(type, "integer") == 0) {
		return TYPE_INTEGER;
	}
	if (strcmp(type, "char") == 0) {
		return TYPE_CHAR;
	}
	if (strcmp(type, "boolean") == 0) {
		return TYPE_BOOLEAN;
	}
	return TYPE_UNKNOWN;
}

//parse ONE var declaration
static SList *var_decl(Parser *parser) {//TODO or NOTTODO var enumumiration declaration
	Lex *lex = lex_cur(parser->lexer);
	if (lex->type == LEX_ID) {
		char *var_name = lex->value;
		lex_destroy_lex(lex);
		lex = lex_next(parser->lexer);

		VarDeclaration *var = smalloc(sizeof(VarDeclaration));

		var->decl_level = parser->decl_level;
		var->name = var_name;
		var->cname = sstrdup(var_name);
		var->args = NULL;



		//find for duplicate id
		SList *duplicated_var_list = slist_find_custom(parser->defined_vars, var, var_compare);
		if (duplicated_var_list != NULL) {
			error_dupl_id(var->name);
		}
		//check if it is c-reserved name
		int varname_len = strlen(var->cname);
		if (reserved_name(var->cname)) {
			//add _ (=
			var->cname = srealloc(var->cname, (strlen(var->cname) + 2)*sizeof(char));
			varname_len++;
			var->cname[varname_len] = '_';
			var->cname[varname_len + 1] = '\0';
		}
		//maybe there are conflicts with vars like "c-reserved_"
		while (slist_find_custom(parser->defined_vars, var, var_c_compare) != NULL) {
			var->cname = srealloc(var->cname, sizeof(char)*(varname_len + 2));
			varname_len++;
			var->cname[varname_len] = '_';
			var->cname[varname_len + 1] = '\0';
		}

		//Should be :
		//var a: integer;
		expect(lex, LEX_COLON);
		lex_free_lex(lex);

		lex = lex_next(parser->lexer);
		//should be id


		expect(lex, LEX_ID);
		var->type = get_type_id(lex->value);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		if (var->type == TYPE_UNKNOWN) {
			error_custom("Unknown type in var declaration");
		}

		//assign var_id and set var as non-callable
		var->callable = false;
		var->constant = false;
		var->body = NULL;
		//add created var declaration to declaration list
		parser->defined_vars = slist_prepend(parser->defined_vars, var);
		//creating syntax element 
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_DECL_VAR;
		syn->syn = var;
		return slist_prepend(NULL, syn);
	}
	return NULL;
}

//var
static SList *gram_var(Parser *parser) {
	Lex *lex = lex_cur(parser->lexer);
	SList *result = NULL;
	//should be var
	if (lex->type != LEX_VAR) {
		return NULL;
	}
	lex_destroy_lex(lex);
	lex = lex_next(parser->lexer);

	SList *syn = NULL;
	while ((syn = var_decl(parser)) != NULL) {
		result = slist_contact(syn, result);
		//Should be ;
		lex = lex_cur(parser->lexer);
		expect(lex, LEX_OP_SEP);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
	}
	return result;
}

int level_cmp(VarDeclaration *listdata, Parser *parser) {
	if (listdata->decl_level == parser->decl_level) {
		if (listdata->constant) {
			parser_tree_destroy(listdata->args);
			if (listdata->constant) {
				free(listdata->body);
			} else
				parser_tree_destroy(listdata->body);
			free(listdata->cname);
			free(listdata->name);
			free(listdata);
		}
		return 1;
	}
	return 0;
}

//function
static SList *gram_function(Parser *parser) {
	Lex *lex = lex_cur(parser->lexer);
	if ((lex->type == LEX_FUNCTION) || (lex->type == LEX_PROCEDURE)) {
		bool func = (lex->type == LEX_FUNCTION);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		expect(lex, LEX_ID);
		VarDeclaration *var = smalloc(sizeof(VarDeclaration));
		var->name = lex->value;
		lex_destroy_lex(lex);
		lex = lex_next(parser->lexer);
		//raise declaration level
		(parser->decl_level)++;
		//argument list or no args
		if (lex->type == LEX_LB) {
			lex_free_lex(lex);
			lex = lex_next(parser->lexer);
			//argment list
			var->args = NULL;
			SList *syn;
			while ((syn = var_decl(parser)) != NULL) {
				var->args = slist_contact(syn, var->args);
				lex = lex_cur(parser->lexer);
				if (lex->type != LEX_OP_SEP) {
					break;
				}
				lex_free_lex(lex);
				lex = lex_next(parser->lexer);
			}
			lex = lex_cur(parser->lexer);
			expect(lex, LEX_RB);
			lex_free_lex(lex);
			lex = lex_next(parser->lexer);
		}

		//Check for duplicated ids
		if (slist_find_custom(parser->defined_vars, var, var_compare) != NULL) {
			error_dupl_id(var->name);
		}
		//looks like everything is ok. rename func
		var->cname = _strdup(var->name);
		//check if it is c-reserved name
		int varname_len = strlen(var->cname);
		if (reserved_name(var->cname)) {
			//add _ (=
			var->cname = srealloc(var->cname, (strlen(var->cname) + 2)*sizeof(char));
			varname_len++;
			var->cname[varname_len] = '_';
			var->cname[varname_len + 1] = '\0';
		}
		//maybe there are conflicts with vars like "c-reserved_"
		while (slist_find_custom(parser->defined_vars, var, var_c_compare) != NULL) {
			var->cname = srealloc(var->cname, sizeof(char)*(varname_len + 2));
			varname_len++;
			var->cname[varname_len] = '_';
			var->cname[varname_len + 1] = '\0';
		}

		var->type = TYPE_NO;
		if (func) {
			//cname fixed. waiting for type
			expect(lex, LEX_COLON);

			lex_free_lex(lex);
			lex = lex_next(parser->lexer);
			expect(lex, LEX_ID);
			var->type = get_type_id(lex->value);
			if (var->type == TYPE_UNKNOWN) {
				error_custom("Unknown type in function declaration");
			}
			//add result var
			VarDeclaration *func_result = smalloc(sizeof(VarDeclaration));
			func_result->args = NULL;
			func_result->body = NULL;
			func_result->callable = false;
			func_result->constant = false;
			func_result->cname = _strdup("result");
			func_result->decl_level = parser->decl_level;
			func_result->name = _strdup("result");
			func_result->type = var->type;
			if (slist_find_custom(parser->defined_vars, var, var_c_compare) != NULL) {
				error_dupl_id("result");
			}
			parser->defined_vars = slist_prepend(parser->defined_vars, func_result);
			lex_free_lex(lex);
			lex = lex_next(parser->lexer);
		}
		expect(lex, LEX_OP_SEP);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);

		var->callable = true;
		var->constant = false;
		(parser->decl_level)--;
		var->decl_level = parser->decl_level;
		Syntax *syn = smalloc(sizeof(Syntax));
		var->body = gram_program(parser);
		Lex *lex = lex_cur(parser->lexer);
		expect(lex, LEX_OP_SEP);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		syn->type = SYN_DECL_VAR;
		syn->syn = var;
		parser->defined_vars = slist_prepend(parser->defined_vars, var);

		//dec decl_level
		return slist_prepend(NULL, syn);
	}
	return NULL;
}

static int gram_const_decl(Parser *parser) {
	Lex *lex = lex_cur(parser->lexer);
	if (lex->type != LEX_ID) {
		return 0;
	}
	VarDeclaration *var = var = smalloc(sizeof(VarDeclaration));
	var->name = lex->value;
	var->decl_level = parser->decl_level;
	lex_destroy_lex(lex);
	lex = lex_next(parser->lexer);
	SList *duplicated_var_list = slist_find_custom(parser->defined_vars, var, var_compare);
	if (duplicated_var_list != NULL) {
		error_dupl_id(var->name);
	}
	var->args = NULL;
	var->body = NULL;
	var->callable = false;
	var->constant = true;
	var->cname = _strdup(var->name);

	size_t varname_len = strlen(var->name);
	if (reserved_name(var->cname)) {
		//add _ (=
		var->cname = srealloc(var->cname, (strlen(var->cname) + 2)*sizeof(char));
		varname_len++;
		var->cname[varname_len] = '_';
		var->cname[varname_len + 1] = '\0';
	}
	//maybe there are conflicts with vars like "c-reserved_"
	while (slist_find_custom(parser->defined_vars, var, var_c_compare) != NULL) {
		var->cname = srealloc(var->cname, sizeof(char)*(varname_len + 2));
		varname_len++;
		var->cname[varname_len] = '_';
		var->cname[varname_len + 1] = '\0';
	}

	expect(lex, LEX_EQ);
	lex_free_lex(lex);
	lex = lex_next(parser->lexer);
	//constants. VARNAME = EXPR
	if (lex->type == LEX_INT) {//int
		int num = parse_int(lex->value);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Int *int_syn = smalloc(sizeof(Int));
		int_syn->value = num;
		var->body = int_syn;
		var->type = TYPE_INTEGER;
		parser->defined_vars = slist_prepend(parser->defined_vars, var);
		return 1;
	}
	if (lex->type == LEX_CHAR) {//char
		char c = ((char *)lex->value)[0];
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Char *char_syn = smalloc(sizeof(Char));
		char_syn->value = c;
		var->body = char_syn;
		var->type = TYPE_CHAR;
		parser->defined_vars = slist_prepend(parser->defined_vars, var);
		return 1;
	}
	if ((lex->type == LEX_TRUE) || (lex->type == LEX_FALSE)) {//boolean
		bool boolean = (lex->type == LEX_TRUE);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Boolean *bool_syn = smalloc(sizeof(Boolean));
		bool_syn->value = boolean;
		var->body = bool_syn;
		var->type = TYPE_BOOLEAN;
		parser->defined_vars = slist_prepend(parser->defined_vars, var);
		return 1;
	}
	error_custom("Unsupported const type");
	return 0;
}

//const
static int gram_const(Parser *parser) {
	Lex *lex = lex_cur(parser->lexer);
	if (lex->type != LEX_CONST) {
		return 0;
	}
	lex_destroy_lex(lex);
	lex = lex_next(parser->lexer);

	while (gram_const_decl(parser)) {
		lex = lex_cur(parser->lexer);
		expect(lex, LEX_OP_SEP);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
	}
	return 1;
}

//definitions
static SList *gram_definition(Parser *parser) {
	SList *result = NULL; // syntax list
	SList *syn = NULL;

	while ((syn = gram_var(parser)) != NULL) {
		result = slist_contact(syn, result);
	}

	while ((syn = gram_function(parser)) != NULL) {
		result = slist_contact(syn, result);
	}

	while (gram_const(parser)) {
		result = slist_contact(syn, result);
	}

	return result;
}

//for
static SList *gram_for(Parser *parser) {
	Lex *lex = lex_cur(parser->lexer);
	//should be for
	if (lex->type == LEX_FOR) {
		lex_destroy_lex(lex);
		lex = lex_next(parser->lexer);
		//should be id
		if (lex->type != LEX_ID) {
			//TODO. raise error
		}
		VarDeclaration *var = get_var(parser, lex->value);
		if ((var->callable) || (var->constant)) {
			error_custom("For loop iterator should be variable");
			return NULL;
		}
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		expect(lex, LEX_ASSIGN);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Expr *start = gram_expr(parser);
		if (start->type != var->type) {
			error_custom("For loop start expression should be same type as iterator");
			return NULL;
		}
		lex = lex_cur(parser->lexer);
		bool direct = true;
		if (lex->type == LEX_DOWNTO) {
			direct = false;
		} else {
			expect(lex, LEX_TO);
		}
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Expr *finish = gram_expr(parser);
		if (finish->type != var->type) {
			error_custom("For loop finish expression should be same type as iterator");
			return NULL;
		}
		lex = lex_cur(parser->lexer);
		expect(lex, LEX_DO);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		SList *body = gram_st(parser);
		ForLoop *loop = smalloc(sizeof(ForLoop));
		loop->body = body;
		loop->finish = finish->tree;
		loop->direct = direct;
		free(finish);
		loop->start = start->tree;
		free(start);
		loop->var = var->cname;
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_FOR;
		syn->syn = loop;
		return slist_prepend(NULL, syn);
	}
	return NULL;
}

//if
static SList *gram_if(Parser *parser) {
	Lex *lex = lex_cur(parser->lexer);
	//should be if
	if (lex->type == LEX_IF) {
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		//read expression
		Expr *cond = gram_expr(parser);
		if (cond->type != TYPE_BOOLEAN) {
			error_custom("If-condition should be a boolean expression");
			return NULL;
		}
		lex = lex_cur(parser->lexer);
		expect(lex, LEX_THEN);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		SList *then_body = gram_st(parser);
		SList *else_body = NULL;
		lex = lex_cur(parser->lexer);
		if (lex->type == LEX_ELSE) {
			lex_free_lex(lex);
			lex = lex_next(parser->lexer);
			else_body = gram_st(parser);
		}
		If *ifelse = smalloc(sizeof(If));
		ifelse->cond = cond->tree;
		free(cond);
		ifelse->then_body = then_body;
		ifelse->else_body = else_body;
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_IF;
		syn->syn = ifelse;
		return slist_prepend(NULL, syn);
	}
	return NULL;
}

//while
static SList *gram_while(Parser *parser) {
	Lex *lex = lex_cur(parser->lexer);
	//should be while
	if (lex->type == LEX_WHILE) {
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		//read expression
		Expr *cond = gram_expr(parser);
		if (cond->type != TYPE_BOOLEAN) {
			error_custom("While-condition should be a boolean expression");
			return NULL;
		}
		lex = lex_cur(parser->lexer);
		expect(lex, LEX_DO);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		SList *body = gram_st(parser);
		While *loop = smalloc(sizeof(While));
		loop->cond = cond->tree;
		free(cond);
		loop->body = body;
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_WHILE;
		syn->syn = loop;
		return slist_prepend(NULL, syn);
	}
	return NULL;
}

//repeat
static SList *gram_repeat(Parser *parser) {
	Lex *lex = lex_cur(parser->lexer);
	//should be repeat
	if (lex->type == LEX_REPEAT) {
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		//body
		SList *stament = NULL;
		SList *body = NULL;
		while ((stament = gram_st(parser)) != NULL) {
			body = slist_contact(stament, body);
		}
		//read condition. waiting for until
		lex = lex_cur(parser->lexer);
		expect(lex, LEX_UNTIL);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		//read expression
		Expr *cond = gram_expr(parser);
		lex = lex_cur(parser->lexer);
		expect(lex, LEX_OP_SEP);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		if (cond->type != TYPE_BOOLEAN) {
			error_custom("Repeat-condition should be a boolean expression");
			return NULL;
		}
		While *loop = smalloc(sizeof(While));
		loop->cond = cond->tree;
		free(cond);
		loop->body = body;
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_REPEAT;
		syn->syn = loop;
		return slist_prepend(NULL, syn);
	}
	return NULL;
}

static int var_name_compare(VarDeclaration *data, char *needle) {

	if (strcmp(needle, data->name) == 0) {
		//found
		return 0;
	}
	return 1;
}


//statement
static SList *gram_st(Parser *parser) {
	SList *result = NULL;
	if (parser->flag) {
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->syn = NULL;
		syn->type = SYN_FLAG;
		parser->flag = false;
		result = slist_prepend(NULL, syn);
	}
	Lex *lex = lex_cur(parser->lexer);
	SList *syn = NULL;
	//begin:end block
	if (lex->type == LEX_BEGIN) {
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		SList *stament = NULL;
		while ((stament = gram_st(parser)) != NULL) {
			result = slist_contact(stament, result);
		}

		lex = lex_cur(parser->lexer);
		expect(lex, LEX_END);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		return result;
	}
	//NOOP
	if (lex->type == LEX_OP_SEP) {
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_NOOP;
		syn->syn = NULL;
		return slist_prepend(result, syn);
	}

	/*other st like if-then-else, while-do, repeat-until*/
	if ((syn = gram_for(parser)) != NULL) {
		return slist_contact(syn, result);
	}
	if ((syn = gram_if(parser)) != NULL) {
		return slist_contact(syn, result);
	}
	if ((syn = gram_while(parser)) != NULL) {
		return slist_contact(syn, result);
	}
	if ((syn = gram_repeat(parser)) != NULL) {
		return slist_contact(syn, result);;
	}

	//assignment and call
	if (lex->type == LEX_ID) {
		//write/writeln call. writeln is not function in my realization. too strange argument policy
		if ((strcmp(lex->value, "write") == 0) || (strcmp(lex->value, "writeln") == 0)) {
			bool newline = (strcmp(lex->value, "writeln") == 0);
			lex_free_lex(lex);
			lex = lex_next(parser->lexer);
			SList *args = NULL;
			if (lex->type == LEX_LB) {
				do {
					lex_free_lex(lex);
					lex = lex_next(parser->lexer);
					Expr *expr = gram_expr(parser);
					args = slist_prepend(args, expr);
					lex = lex_cur(parser->lexer);

				} while (lex->type == LEX_COMMA);
				expect(lex, LEX_RB);
				lex_free_lex(lex);
				lex = lex_next(parser->lexer);
			}
			Write *write = smalloc(sizeof(Write));
			write->args = args;
			write->newline = newline;
			Syntax *syn = smalloc(sizeof(Syntax));
			syn->type = SYN_WRITE;
			syn->syn = write;
			return slist_prepend(result, syn);
		}
		//get var id
		VarDeclaration *var = get_var(parser, lex->value);
		if (var == NULL) {
			printf("%s", lex->value);
			error_custom("Non-declarated variable");
		}


		if (var->constant) {
			error_custom("Error in expression");
			return NULL;
		}

		if (var->callable) {
			Expr *expr = gram_call(parser);
			result = slist_contact(expr->tree, result);
			free(expr);
			lex = lex_cur(parser->lexer);
			expect(lex, LEX_OP_SEP);
			lex_free_lex(lex);
			lex = lex_next(parser->lexer);
			return result;
		}

		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		if (lex->type != LEX_ASSIGN) {
			error_custom("Error in expression");
		}
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Expr *expr = gram_expr(parser);
		if (var->type != expr->type) {
			error_custom("Variable can't be assigned to a expression of different type");
		}
		BinOp *assign = smalloc(sizeof(BinOp));
		Var *assign_var = smalloc(sizeof(Var));
		assign_var->value = var->cname;
		Syntax *assign_var_syn = smalloc(sizeof(Syntax));
		assign_var_syn->type = SYN_VAR;
		assign_var_syn->syn = assign_var;
		assign->tree1 = slist_prepend(NULL, assign_var_syn);
		assign->tree2 = expr->tree;
		assign->type = OP_ASSIGN;
		free(expr);
		Syntax *assign_syn = smalloc(sizeof(Syntax));
		assign_syn->type = SYN_BINOP;
		assign_syn->syn = assign;
		lex = lex_cur(parser->lexer);
		expect(lex, LEX_OP_SEP);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		return slist_prepend(result, assign_syn);
	}

	return result;
}

static SList *gram_program(Parser *parser) {
	bool flag = parser->flag;
	parser->flag = false;
	SList *result = NULL; // syntax list

	//Reading definitions (type, var, function, etc).
	SList *define = NULL;
	while ((define = gram_definition(parser)) != NULL) {
		result = slist_contact(define, result);
	}

	/*Main block*/

	//Reading staments (for, while, assigment, if, call procedure, etc);
	parser->flag = flag;
	result = result = slist_contact(gram_st(parser), result);

	//Ignore anything else.


	//remove all declared vars on current level
	parser->defined_vars = slist_remove_if(parser->defined_vars, parser, level_cmp);


	return result;
}

int id_finder(VarDeclaration *var, char *name) {
	return (strcmp(var->name, name) == 0) ? 0 : 1;
}

static VarDeclaration *get_var(Parser *parser, char *name) {
	SList *list_var = slist_find_custom(parser->defined_vars, name, id_finder);
	if (list_var == NULL)
		return NULL;
	return list_var->data;
}

static Expr *gram_call(Parser *parser) {
	Lex *lex = lex_cur(parser->lexer);
	if (lex->type != LEX_ID) {
		error_custom("Something goes wrong");
		return NULL;
	}
	VarDeclaration *var = get_var(parser, lex->value);
	if ((var == NULL) || (!(var->callable))) {
		error_custom("Something goes wrong");
		return NULL;
	}
	lex_free_lex(lex);
	lex = lex_next(parser->lexer);
	SList *args = NULL;
	if (lex->type == LEX_LB) {
		SList *args_exp = var->args;
		do {
			lex_free_lex(lex);
			lex = lex_next(parser->lexer);
			if (args_exp == NULL) {
				error_custom("Too many arguments");
				return NULL;
			}
			if (((Syntax *)(args_exp->data))->type != SYN_DECL_VAR) {
				error_custom("Something goes wrong");
				return NULL;
			}
			VarDeclaration *arg = (VarDeclaration *)(((Syntax *)(args_exp->data))->syn);
			Expr *expr = gram_expr(parser);
			if (arg->type != expr->type) {
				printf("%s\n", var->name);
				error_custom("Wrong argument type");
				return NULL;
			}
			args = slist_contact(expr->tree, args);
			free(expr);
			lex = lex_cur(parser->lexer);

		} while (lex->type == LEX_COMMA);
		expect(lex, LEX_RB);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
	}
	Call *call = smalloc(sizeof(Call));
	call->args = args;
	call->cname = var->cname;
	Syntax *syn = smalloc(sizeof(Syntax));
	syn->type = SYN_CALL;
	syn->syn = call;
	Expr *expr = smalloc(sizeof(Expr));
	expr->type = var->type;
	expr->tree = slist_prepend(NULL, syn);
	return expr;
}

static Expr *gram_expr_lvl0(Parser *parser) {
	//LEVEL 0.
	//(), int/char/bool, const, var or int-function call...
	Lex *lex = lex_cur(parser->lexer);
	if (lex->type == LEX_LB) {//()
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Expr *result = gram_expr(parser);
		Lex *lex = lex_cur(parser->lexer);
		expect(lex, LEX_RB);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		return result;
	}
	if (lex->type == LEX_INT) {//int
		int num = parse_int(lex->value);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Int *int_syn = smalloc(sizeof(Int));
		int_syn->value = num;
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_INT;
		syn->syn = int_syn;
		Expr *result = smalloc(sizeof(Expr));
		result->tree = slist_prepend(NULL, syn);
		result->type = TYPE_INTEGER;
		return result;
	}
	if (lex->type == LEX_CHAR) {//char
		char c = ((char *)lex->value)[0];
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Char *char_syn = smalloc(sizeof(Char));
		char_syn->value = c;
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_CHAR;
		syn->syn = char_syn;
		Expr *result = smalloc(sizeof(Expr));
		result->tree = slist_prepend(NULL, syn);
		result->type = TYPE_CHAR;
		return result;
	}
	if ((lex->type == LEX_TRUE) || (lex->type == LEX_FALSE)) {//boolean
		bool boolean = (lex->type == LEX_TRUE);
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Char *bool_syn = smalloc(sizeof(Boolean));
		bool_syn->value = boolean;
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_BOOLEAN;
		syn->syn = bool_syn;
		Expr *result = smalloc(sizeof(Expr));
		result->tree = slist_prepend(NULL, syn);
		result->type = TYPE_BOOLEAN;
		return result;
	}
	if (lex->type == LEX_ID) {//var/const/call
		//find id declaration
		VarDeclaration *var = get_var(parser, lex->value);
		if (var == NULL) {
			printf("%s\n", lex->value);
			error_custom("Non-declarated variable");
			return NULL;
		}
		if (var->constant) {
			lex_free_lex(lex);
			lex = lex_next(parser->lexer);
			if (var->type == TYPE_INTEGER) {
				int num = ((Int *)var->body)->value;
				Int *int_syn = smalloc(sizeof(Int));
				int_syn->value = num;
				Syntax *syn = smalloc(sizeof(Syntax));
				syn->type = SYN_INT;
				syn->syn = int_syn;
				Expr *result = smalloc(sizeof(Expr));
				result->tree = slist_prepend(NULL, syn);
				result->type = TYPE_INTEGER;
				return result;
			}
			if (var->type == TYPE_CHAR) {//char
				char c = ((Char *)var->body)->value;
				Char *char_syn = smalloc(sizeof(Char));
				char_syn->value = c;
				Syntax *syn = smalloc(sizeof(Syntax));
				syn->type = SYN_CHAR;
				syn->syn = char_syn;
				Expr *result = smalloc(sizeof(Expr));
				result->tree = slist_prepend(NULL, syn);
				result->type = TYPE_CHAR;
				return result;
			}
			if (var->type == TYPE_BOOLEAN) {//boolean
				bool boolean = ((Boolean *)var->body)->value;
				Boolean *bool_syn = smalloc(sizeof(Boolean));
				bool_syn->value = boolean;
				Syntax *syn = smalloc(sizeof(Syntax));
				syn->type = SYN_BOOLEAN;
				syn->syn = bool_syn;
				Expr *result = smalloc(sizeof(Expr));
				result->tree = slist_prepend(NULL, syn);
				result->type = TYPE_BOOLEAN;
				return result;
			}
		}
		if (var->callable) {
			if (var->type == TYPE_NO) {
				error_custom("Procedure not allowed in expressions");
				return NULL;
			}
			return gram_call(parser);
		}
		//var_id
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Var *var_syn = smalloc(sizeof(Var));
		var_syn->value = var->cname;
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_VAR;
		syn->syn = var_syn;
		Expr *result = smalloc(sizeof(Expr));
		result->tree = slist_prepend(NULL, syn);
		result->type = var->type;
		return result;
	}
	error_custom("Can't recongnize expression");
	return NULL;
}
static Expr *gram_expr_lvl1(Parser *parser) {
	//LEVEL 1. Unary operators: not
	Lex *lex = lex_cur(parser->lexer);

	Lex_type lextype = lex->type;
	if (lex->type != LEX_NOT) {
		return gram_expr_lvl0(parser);
		lex = lex_cur(parser->lexer);
	}
	lex_free_lex(lex);
	lex = lex_next(parser->lexer);
	Expr *expr = gram_expr_lvl0(parser);
	lex = lex_cur(parser->lexer);
	if ((expr->type != TYPE_INTEGER) && (expr->type != TYPE_BOOLEAN)) {
		error_custom("Not operator allows only integer or boolean operand");
		return NULL;
	}
	Not *not = smalloc(sizeof(Not));
	not->tree = expr->tree;
	Syntax *syn = smalloc(sizeof(Syntax));
	syn->type = SYN_NOT;
	syn->syn = not;
	expr->tree = slist_prepend(NULL, syn);
	return expr;
}

static Expr *gram_expr_lvl2(Parser *parser) {
	//LEVEL 2. Multiplying operators: * div mod shl shr and
	Expr *expr1 = gram_expr_lvl1(parser);
	Lex *lex = lex_cur(parser->lexer);
	Lex_type lextype = lex->type;
	if ((lex->type == LEX_MULT) || (lex->type == LEX_DIV) || (lex->type == LEX_MOD) || (lex->type == LEX_SHL) || (lex->type == LEX_SHR) || (lex->type == LEX_AND)) {
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Expr *expr2 = gram_expr_lvl1(parser);
		lex = lex_cur(parser->lexer);
		if (expr1->type != expr2->type) {
			error_custom("Operands should be same type");
		}
		if (lextype == LEX_AND) {
			if ((expr1->type != TYPE_BOOLEAN) && (expr1->type != TYPE_INTEGER)) {
				error_custom("AND operation allows only boolean or integer operands");
			}
		} else if (expr1->type != TYPE_INTEGER) {
			error_custom("Operation allows only integer operands");
		}
		BinOp *op = smalloc(sizeof(BinOp));
		op->tree1 = expr1->tree;
		op->tree2 = expr2->tree;
		switch (lextype) {
			case LEX_MULT:
				op->type = OP_MULT;
				break;
			case LEX_DIV:
				op->type = OP_DIV;
				break;
			case LEX_MOD:
				op->type = OP_MOD;
				break;
			case LEX_SHL:
				op->type = OP_SHL;
				break;
			case LEX_SHR:
				op->type = OP_SHR;
				break;
			case LEX_AND:
				if (expr1->type == TYPE_INTEGER) {
					op->type = OP_ANDint;
				} else {
					op->type = OP_ANDbool;
				}
				break;
		}
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_BINOP;
		syn->syn = op;
		free(expr2);
		expr1->tree = slist_prepend(NULL, syn);
		return expr1;
	}
	return expr1;

}

static Expr *gram_expr_lvl3(Parser *parser) {
	//LEVEL 3. Adding operators: + - or xor
	Expr *expr1 = gram_expr_lvl2(parser);
	Lex *lex = lex_cur(parser->lexer);
	Lex_type lextype = lex->type;
	if ((lex->type == LEX_PLUS) || (lex->type == LEX_MINUS) || (lex->type == LEX_OR) || (lex->type == LEX_XOR)) {
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Expr *expr2 = gram_expr_lvl2(parser);
		lex = lex_cur(parser->lexer);
		if (expr1->type != expr2->type) {
			error_custom("Operands should be same type");
		}
		if ((lex->type == LEX_OR) || (lex->type == LEX_XOR)) {
			if ((expr1->type != TYPE_BOOLEAN) && (expr1->type != TYPE_INTEGER)) {
				error_custom("Operation allows only boolean or integer operands");
			}
		} else if (expr1->type != TYPE_INTEGER) {
			error_custom("Operation allows only integer operands");
		}
		BinOp *op = smalloc(sizeof(BinOp));
		op->tree1 = expr1->tree;
		op->tree2 = expr2->tree;
		switch (lextype) {
			case LEX_PLUS:
				op->type = OP_PLUS;
				break;
			case LEX_MINUS:
				op->type = OP_MINUS;
				break;
			case LEX_OR:
				if (expr1->type == TYPE_INTEGER) {
					op->type = OP_ORint;
				} else {
					op->type = OP_ORbool;
				}
				break;
			case LEX_XOR:
				if (expr1->type == TYPE_INTEGER) {
					op->type = OP_XOR;
				} else {
					op->type = OP_NOT_EQ;
				}
				break;
		}
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_BINOP;
		syn->syn = op;
		free(expr2);
		expr1->tree = slist_prepend(NULL, syn);
		return expr1;
	}
	return expr1;
}
static Expr *gram_expr(Parser *parser) {
	//LEVEL 4. Relational operators: = <> < > <= >=
	Expr *expr1 = gram_expr_lvl3(parser);
	Lex *lex = lex_cur(parser->lexer);
	Lex_type lextype = lex->type;
	if ((lextype == LEX_EQ) || (lextype == LEX_NOT_EQ) || (lextype == LEX_MORE) || (lextype == LEX_LESS) || (lextype == LEX_LESS_EQ) || (lextype == LEX_MORE_EQ)) {
		lex_free_lex(lex);
		lex = lex_next(parser->lexer);
		Expr *expr2 = gram_expr_lvl3(parser);
		lex = lex_cur(parser->lexer);
		if (expr1->type != expr2->type) {
			error_custom("Operands should be same type");
		}
		BinOp *op = smalloc(sizeof(BinOp));
		op->tree1 = expr1->tree;
		op->tree2 = expr2->tree;
		switch (lextype) {
			case LEX_EQ:
				op->type = OP_EQ;
				break;
			case LEX_NOT_EQ:
				op->type = OP_NOT_EQ;
				break;
			case LEX_MORE:
				op->type = OP_MORE;
				break;
			case LEX_LESS:
				op->type = OP_LESS;
				break;
			case LEX_MORE_EQ:
				op->type = OP_MORE_EQ;
				break;
			case LEX_LESS_EQ:
				op->type = OP_LESS_EQ;
				break;
		}
		Syntax *syn = smalloc(sizeof(Syntax));
		syn->type = SYN_BINOP;
		syn->syn = op;
		free(expr2);
		expr1->tree = slist_prepend(NULL, syn);
		expr1->type = TYPE_BOOLEAN;
		return expr1;
	}
	return expr1;
}

VarDeclaration *make_int_const(char *name, int value, int decl_level) {
	VarDeclaration *var = smalloc(sizeof(VarDeclaration));
	var->args = NULL;
	Int *val = smalloc(sizeof(Int));
	val->value = value;
	var->body = val;
	var->callable = false;
	var->cname = _strdup(name);
	var->name = _strdup(name);
	var->constant = true;
	var->decl_level = decl_level;
	var->type = TYPE_INTEGER;
	return var;
}

SList *make_arg(ExprType type) {
	VarDeclaration *arg = smalloc(sizeof(VarDeclaration));
	arg->args = NULL;
	arg->body = NULL;
	arg->callable = false;
	arg->cname = _strdup("arg");
	arg->name = _strdup("arg");
	arg->constant = false;
	arg->decl_level = 0;
	arg->type = type;
	Syntax *syn = smalloc(sizeof(Syntax));
	syn->type = SYN_DECL_VAR;
	syn->syn = arg;
	return slist_prepend(NULL, syn);
}

VarDeclaration *make_func_int_arg(char *name, ExprType type, int decl_level) {
	VarDeclaration *var = smalloc(sizeof(VarDeclaration));
	var->args = make_arg(TYPE_INTEGER);
	var->body = NULL;
	var->callable = true;
	var->cname = _strdup(name);
	var->name = _strdup(name);
	var->constant = false;
	var->decl_level = decl_level;
	var->type = type;
	return var;
}

VarDeclaration *make_func_int_int_arg(char *name, ExprType type, int decl_level) {
	VarDeclaration *var = smalloc(sizeof(VarDeclaration));
	var->args = slist_contact(make_arg(TYPE_INTEGER), make_arg(TYPE_INTEGER));
	var->body = NULL;
	var->callable = true;
	var->cname = _strdup(name);
	var->name = _strdup(name);
	var->constant = false;
	var->decl_level = decl_level;
	var->type = type;
	return var;
}

VarDeclaration *make_func_no_arg(char *name, ExprType type, int decl_level) {
	VarDeclaration *var = smalloc(sizeof(VarDeclaration));
	var->args = NULL;
	var->body = NULL;
	var->callable = true;
	var->cname = _strdup(name);
	var->name = _strdup(name);
	var->constant = false;
	var->decl_level = decl_level;
	var->type = type;
	return var;
}

VarDeclaration *make_func_char_arg(char *name, ExprType type, int decl_level) {
	VarDeclaration *var = smalloc(sizeof(VarDeclaration));
	var->args = make_arg(TYPE_CHAR);
	var->body = NULL;
	var->callable = true;
	var->cname = _strdup(name);
	var->name = _strdup(name);
	var->constant = false;
	var->decl_level = decl_level;
	var->type = type;
	return var;
}

SList *gram_uses(Parser *parser) {
	Lex *lex = lex_cur(parser->lexer);
	if (lex->type != LEX_USES) {
		return NULL;
	}
	lex_free_lex(lex);
	lex = lex_next(parser->lexer);
	if (strcmp(lex->value, "crt") != 0) {
		error_custom("Unsupported module");
		return NULL;
	}
	lex_free_lex(lex);
	lex = lex_next(parser->lexer);
	expect(lex, LEX_OP_SEP);
	lex_free_lex(lex);
	lex = lex_next(parser->lexer);
	//const
	parser->defined_vars = slist_prepend(parser->defined_vars, make_int_const("black", 0, parser->decl_level));
	parser->defined_vars = slist_prepend(parser->defined_vars, make_int_const("blue", 1, parser->decl_level));
	parser->defined_vars = slist_prepend(parser->defined_vars, make_int_const("green", 2, parser->decl_level));
	parser->defined_vars = slist_prepend(parser->defined_vars, make_int_const("red", 4, parser->decl_level));
	parser->defined_vars = slist_prepend(parser->defined_vars, make_int_const("white", 15, parser->decl_level));
	//func
	parser->defined_vars = slist_prepend(parser->defined_vars, make_func_int_arg("textcolor", TYPE_NO, parser->decl_level));
	parser->defined_vars = slist_prepend(parser->defined_vars, make_func_char_arg("ord", TYPE_INTEGER, parser->decl_level));
	parser->defined_vars = slist_prepend(parser->defined_vars, make_func_int_arg("textbackground", TYPE_NO, parser->decl_level));
	parser->defined_vars = slist_prepend(parser->defined_vars, make_func_no_arg("readkey", TYPE_CHAR, parser->decl_level));
	parser->defined_vars = slist_prepend(parser->defined_vars, make_func_no_arg("clrscr", TYPE_NO, parser->decl_level));
	parser->defined_vars = slist_prepend(parser->defined_vars, make_func_int_int_arg("gotoxy", TYPE_NO, parser->decl_level));


	Syntax *syn = smalloc(sizeof(Syntax));
	syn->type = SYN_CRT;
	syn->syn = NULL;
	return slist_prepend(NULL, syn);
}

int level_cmp_free(VarDeclaration *listdata, Parser *parser) {
	if (listdata->decl_level == parser->decl_level) {
		parser_tree_destroy(listdata->args);
		if (listdata->constant) {
			free(listdata->body);
		} else
			parser_tree_destroy(listdata->body);
		free(listdata->cname);
		free(listdata->name);
		free(listdata);
		return 1;
	}
	return 0;
}


SList *parser_tree(Parser *parser) {
	(parser->decl_level)--;
	SList *result = gram_uses(parser);
	(parser->decl_level)++;
	result = slist_contact(gram_program(parser), result);
	Lex *lex = lex_cur(parser->lexer);
	expect(lex, LEX_DOT);
	lex_free_lex(lex);

	return result;
}

void parser_destroy(Parser *parser) {
	if (parser == NULL)
		return;
	(parser->decl_level)--;
	parser->defined_vars = slist_remove_if(parser->defined_vars, parser, level_cmp_free);
	slist_free(parser->defined_vars);
	free(parser);
}

static void foreach_free_expr(Expr *expr, void *null) {
	free(null);//warning bugfix
	parser_tree_destroy(expr->tree);
	free(expr);
}

static void free_syntax(Syntax *syn, void *userdata) {
	free(userdata);
	switch (syn->type) {
		case SYN_DECL_VAR:{
							  VarDeclaration *var = syn->syn;

							  parser_tree_destroy(var->args);
							  if (var->constant) {
								  free(var->body);
							  } else
								 parser_tree_destroy(var->body);
							  free(var->cname);
							  free(var->name);
							  free(var);
							  break;
		}
		case SYN_VAR:
		case SYN_CHAR:
		case SYN_INT:
		case SYN_BOOLEAN:
			free(syn->syn);
			break;
		case SYN_CALL:{
						  Call *call = syn->syn;
						  parser_tree_destroy(call->args);//should work fine
						  free(call);
						  break;
		}
		case SYN_WRITE:{
						   Write *write = syn->syn;
						   slist_foreach(write->args, foreach_free_expr, NULL);
						   slist_free(write->args);
						   free(write);
						   break;
		}
		case SYN_NOT:
		{
						Not *not = syn->syn;
						parser_tree_destroy(not->tree);
						free(not);
						break;
		};
		case SYN_BINOP:
		{
						  BinOp *bin = syn->syn;
						  parser_tree_destroy(bin->tree1);
						  parser_tree_destroy(bin->tree2);
						  free(bin);
						  break;
		}
		case SYN_FOR:
		{
						ForLoop *loop = syn->syn;
						parser_tree_destroy(loop->body);
						parser_tree_destroy(loop->finish);
						parser_tree_destroy(loop->start);
						free(loop);
						break;
		}
		case SYN_IF:
		{
					   If *ifthen = syn->syn;
					   parser_tree_destroy(ifthen->else_body);
					   parser_tree_destroy(ifthen->then_body);
					   parser_tree_destroy(ifthen->cond);
					   free(ifthen);
					   break;
		}
		case SYN_WHILE:
		case SYN_REPEAT:
		{
						   While *loop = syn->syn;
						   parser_tree_destroy(loop->body);
						   parser_tree_destroy(loop->cond);
						   free(loop);
						   break;
		}

		default:
			break;
	}
	free(syn);
}

void parser_tree_destroy(SList *tree) {
	slist_foreach(tree, free_syntax, NULL);
	slist_free(tree);
}
