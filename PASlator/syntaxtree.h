#ifndef SYN_TREE
#define SYN_TREE

#include <stdbool.h>
#include "linked_list.h"
#include "parser.h"

typedef enum _type_id{
	TYPE_UNKNOWN = -1,
	TYPE_INTEGER = -2,
	TYPE_CHAR = -3,
	TYPE_BOOLEAN=-4,
	TYPE_NO = -5
} ExprType;

typedef struct  _expr{
	SList *tree;
	ExprType type;
} Expr;

typedef struct _vardeclaration{
	char *name;
	char *cname;
	long long type;
	int decl_level;
	SList *args;
	bool callable;
	bool constant;
	void *body;
} VarDeclaration;

typedef enum _syntype {
	SYN_DECL_VAR,
	SYN_VAR,
	SYN_INT,
	SYN_CHAR,
	SYN_BOOLEAN,
	SYN_CALL,
	SYN_NOT,
	SYN_IF,
	SYN_BINOP,
	SYN_NOOP,
	SYN_WHILE,
	SYN_REPEAT,
	SYN_FOR,
	SYN_WRITE,
	SYN_CRT,
	SYN_FLAG
} SynType;

typedef struct _syntax {
	SynType type;
	void *syn;
} Syntax;

static SList *var_decl(Parser *parser);
static SList *gram_var(Parser *parser);
static SList *gram_function(Parser *parser);
static int gram_const(Parser *parser);
static SList *gram_definition(Parser *parser);
static SList *gram_for(Parser *parser);
static SList *gram_st(Parser *parser);
static SList *gram_program(Parser *parser);
static Expr *gram_expr(Parser *parser);
static Expr *gram_call(Parser *parser);
static void free_syntax(Syntax *syn, void *userdata);
static VarDeclaration *get_var(Parser *parser, char *name);

typedef struct  _forloop{
	char *var;
	SList *start;
	SList *finish;
	SList *body;
	bool direct;
} ForLoop;

typedef struct  _int{
	int value;
} Int;

typedef struct  _call {
	char *cname;
	SList *args;
} Call;

typedef struct  _write {
	SList *args;
	bool newline;
} Write;

typedef struct  _char{
	char value;
} Char;

typedef struct  _var{
	char *value;
} Var;

typedef struct  _boolean{
	bool value;
} Boolean;

typedef struct  _not{
	SList *tree;
} Not;

typedef struct  _if {
	SList *cond;
	SList *then_body;
	SList *else_body;
} If;

typedef struct  _while {
	SList *cond;
	SList *body;
} While;

typedef enum _binoptype{
	OP_MULT,
	OP_DIV,
	OP_MOD,
	OP_SHL,
	OP_SHR,
	OP_ANDbool,
	OP_ANDint,
	OP_PLUS,
	OP_MINUS,
	OP_XOR,
	OP_ORint,
	OP_ORbool,
	OP_NOT_EQ,
	OP_LESS_EQ,
	OP_EQ,
	OP_LESS,
	OP_MORE,
	OP_MORE_EQ,
	OP_ASSIGN
} BinOpType;

typedef struct  _binop{
	SList *tree1;
	SList *tree2;
	BinOpType type;
} BinOp;

#endif