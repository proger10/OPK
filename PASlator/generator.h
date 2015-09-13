#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include "linked_list.h"

typedef struct _generator {
	FILE *f;
	char sep;
} Generator;

Generator *gen_create(FILE *file);
void gen_process(Generator *gen, SList *tree);
void gen_destroy(Generator *gen);

#endif