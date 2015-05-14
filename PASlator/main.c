#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#include "stream.h"
#include "lexer.h"
#include <stdio.h>

struct _loh{
	int i;
	int j;
	char a, b;
};

int main(int argc, char **argv){
	printf("%d\n", sizeof(struct _loh));
	if (argc == 2){
		FILE *f = fopen(argv[1], "r");
		Stream *stream = stream_create(SRC_FILE, f);
		Lexer *lexer = lex_create(stream);
		Lex *lex;
		while ((lex = lex_next(lexer)) && (lex->type != LEX_UNKNOWN)){
			switch (lex->type){
			case LEX_WORD:
				printf("LEX_WORD: %s\n", lex->value);
				break;
			case LEX_OP:
				printf("LEX_OP\n");
				break;
			}
			lex_free(lex);
		}
		if ((lex) && (lex->type == LEX_UNKNOWN)){
			printf("Unknown lex: %s\n", lex->value);
		}
		lex_free(lex);
		lex_destroy(lexer);
		stream_destroy(stream);
		fclose(f);
	}

	_CrtDumpMemoryLeaks();
	return 0;
}