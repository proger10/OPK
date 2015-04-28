#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Usage: %s <text file>\n", argv[0]);
		return 1;
	}

	FILE *f = fopen(argv[1], "r");
	if (f == NULL){
		printf("Cant open file %s\n", argv[1]);
		return 1;
	}

	char ch;
	int words = 0;
	int chars = 0;
	int lines = 1;
	char space = 1;
	while ((ch = getc(f)) != EOF){
		if ((!isspace(ch)) && (space)){
			words++;
		}
		space = isspace(ch);
		if (ch == '\n'){
			lines++;
		}
		else{
			chars++;
		}
	}
	printf("Chars: %d\n", chars);
	printf("Words: %d\n", words);
	printf("Lines: %d\n", lines);

	fclose(f);
}