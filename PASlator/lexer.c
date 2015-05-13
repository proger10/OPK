#include "lexer.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

Lexer *lex_create(Stream *in_stream){
	Lexer *result = malloc(sizeof(Lexer));
	if (result == NULL)
		return result;
	result->inp = in_stream;

	return result;
}

static char lowercase(char a){
	if ((a >= 'A') && (a <= 'Z'))
		return a - 'A' + 'a';
	return a;
}

Lex *lex_next(Lexer *lexer){
	bool correct = false;

	do {
		int c = stream_next(lexer->inp);
		if (c == EOF){
			break;
		}
		c = lowercase(c);
		//

		//

	} while (correct);

	if (!correct){
		return NULL;
	}
	return NULL;
}

void lex_destroy(Lexer *lexer){
	free(lexer);
}