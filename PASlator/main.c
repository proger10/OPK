#define _CRT_SECURE_NO_WARNINGS
#include "stream.h"
#include "lexer.h"
#include <stdio.h>

int main(int argc, char **argv){
	int x = 023;
	printf("%d\n", x);
	if (argc == 2){
		FILE *f = fopen(argv[1], "r");
		Stream *stream = stream_create(SRC_FILE, f);
		Lexer *lexer = lex_create(stream);
		Lex *lex;
		while (lex = lex_next(lexer)){
			printf("%s\n", lex->value);
			lex_free(lex);
		}
		lex_destroy(lexer);
		stream_destroy(stream);
		fclose(f);
	}
	return 0;
}
