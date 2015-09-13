#ifndef _ERROR_PROC
#define _ERROR_PROC

#include <stdlib.h>
#include "lexer.h"

void *smalloc(size_t bytes);
void *srealloc(void *data, size_t bytes);
char *sstrdup(char *data);
void error_dupl_id(char *id);
void expect(Lex *lex, Lex_type expect);
void error_custom(char *error);

#endif