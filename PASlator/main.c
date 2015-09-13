#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "stream.h"
#include "lexer.h"
#include "parser.h"
#include "generator.h"
#include <stdio.h>
#include <string.h>



int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s <input_file> <output_file>\n", argv[0]);
		printf("<input_file> can be assigned to stdin\n");
		printf("<output_file> can be assigned to stdout\n");
		return 1;
	}
	FILE *fin;
	FILE *fout;
	if (strcmp(argv[1], "stdin") == 0)
		fin = stdin;
	else
		fin = fopen(argv[1], "r");
	if (strcmp(argv[2], "stdout") == 0)
		fout = stdout;
	else
		fout = fopen(argv[2], "w");
	Stream *stream = stream_create(SRC_FILE, fin);
	Lexer *lexer = lex_create(stream);
	Parser *parser = parser_create(lexer);

	SList *tree = parser_tree(parser);
	
	Generator *gen = gen_create(fout);

	gen_process(gen, tree);

	parser_tree_destroy(tree);
	gen_destroy(gen);
	parser_destroy(parser);
	lex_destroy(lexer);
	stream_destroy(stream);

	fclose(fin);
	fclose(fout);

	_CrtDumpMemoryLeaks();
	return 0;
}