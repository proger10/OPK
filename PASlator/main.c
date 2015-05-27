#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "stream.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>

int main(int argc, char **argv){
	if (argc == 2){
		FILE *f = fopen(argv[1], "r");
		Stream *stream = stream_create(SRC_FILE, f);
		Lexer *lexer = lex_create(stream);
		Parser *parser = parser_create(lexer);

		SList *tree = parser_tree(parser);

		parser_tree_destroy(tree);

		parser_destroy(parser);
		lex_destroy(lexer);
		stream_destroy(stream);
		fclose(f);
	}

	_CrtDumpMemoryLeaks();
	return 0;
}