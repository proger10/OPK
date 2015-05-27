#ifndef SYN_TREE
#define SYN_TREE

#include <stdbool.h>

typedef enum _syntype{
	SYN_PROGRAM,
	SYN_VAR,
	SYN_IF
} SynType;

typedef struct _syntax{
	SynType type;
	void *syn;
} Syntax;

typedef struct _vardeclaration{
	char *name;
	char *cname;
	long long type_id;
	long long var_id;
	int decl_level;
	SList *args;
	bool callable;
} VarDeclaration;

typedef struct  _typedeclaration{
	char *name;
	long long type_id;
} TypeDeclaration;

typedef struct  _forloop{
	long long var_id;
	SList *start;
	SList *end;
	SList *body;
} ForLoop;

typedef enum _type_id{
	TYPE_UNKNOWN = -1,
	TYPE_INTEGER = -2,
	TYPE_REAL = -3,
	TYPE_STRING = -4,
	TYPE_CHAR = -5,
	TYPE_NO = -6
};


/*
SYN_PROGRAM:
syn = char *name

SYN_VAR:
syn = VarDeclaration *var

*/

#endif