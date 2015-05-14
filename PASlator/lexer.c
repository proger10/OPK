#include "lexer.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "d_str.h"
#include <ctype.h>

Lexer *lex_create(Stream *in_stream){
	Lexer *result = malloc(sizeof(Lexer));
	if (result == NULL)
		return result;
	result->inp = in_stream;
	result->state = ST_SEP;
	return result;
}

static int lowercase(int a){
	if ((a >= 'A') && (a <= 'Z'))
		return a - 'A' + 'a';
	return a;
}

static int is_az(int a){
	return ((a >= 'a') && (a <= 'z'));
}

static int is_09(int a){
	return ((a >= '0') && (a <= '9'));
}

static int is_space(int a){
	return (isspace(a) || (a == EOF));
}


static Lex_type parse_word(String *str){
	//TODO REFACTOR IT!!!!!!!
	if (str_compare(str, "begin")){
		return LEX_BEGIN;
	}
	if (str_compare(str, "end")){
		return LEX_END;
	}
	if (str_compare(str, "for")){
		return LEX_FOR;
	}
	if (str_compare(str, "while")){
		return LEX_WHILE;
	}
	if (str_compare(str, "do")){
		return LEX_DO;
	}
	if (str_compare(str, "to")){
		return LEX_TO;
	}
	if (str_compare(str, "repeat")){
		return LEX_REPEAT;
	}
	if (str_compare(str, "until")){
		return LEX_UNTIL;
	}
	if (str_compare(str, "program")){
		return LEX_PROGRAM;
	}
	if (str_compare(str, "shl")){
		return LEX_SHL;
	}
	if (str_compare(str, "shr")){
		return LEX_SHR;
	}
	return LEX_ID;
}

Lex *lex_next(Lexer *lexer){
	String *str = str_create();
	int ret = 0;

	while (!ret) {
		int c = stream_next(lexer->inp);
		c = lowercase(c);

		if (c == EOF){
			ret = 1;
		}

		switch (lexer->state){

		//Starting
		case ST_SEP:
			//word case
			if (is_az(c)){
				lexer->state = ST_WORD;
				str_add(str, c);
				stream_read(lexer->inp);
				break;
			}
			//skip empty chars
			if (is_space(c)){
				stream_read(lexer->inp);
				break;
			}
			//operator separator
			if (c == ';'){
				ret = 1;//it's a final stage
				stream_read(lexer->inp);
				lexer->state = ST_OP_SEP;
				break;
			}
			//end of source
			if (c == '.'){
				ret = 1;//it's a final stage
				stream_read(lexer->inp);
				lexer->state = ST_DOT;
				break;
			}
			//colon or assign
			if (c == ':'){
				stream_read(lexer->inp);
				lexer->state = ST_COLON;
				break;
			}
			//numbers. integer or real
			if (is_09(c)){
				str_add(str, c);
				stream_read(lexer->inp);
				lexer->state = ST_INT;
				break;
			}
			//can't recognize lex. something unknown?
			lexer->state = ST_ERROR;
			str_add(str, c);
			ret = 1;
			break;

		//Word state
		case ST_WORD:
			//read part of word
			if (is_az(c) || is_09(c)){
				str_add(str, c);
				stream_read(lexer->inp);
				break;
			}
			//read something that can't be interpritated as word
			ret = 1;
			break;

		case ST_COLON:
			//decide it is colon or assignment
			if (c == '='){
				//it's assignment
				lexer->state = ST_ASSIGN;
				stream_read(lexer->inp);
				ret = 1;
			}
			else{
				//colon
				ret = 1;
			}
			break;

		case ST_INT:
			//read part of number
			if (is_09(c)){
				str_add(str, c);
				stream_read(lexer->inp);
				break;
			}
			//read something that can't be interpritated as decimal
			ret = 1;
			break;
			
		default:
			lexer->state = ST_ERROR;
			ret = 1;
			break;
		}
	}


	Lex *result = NULL;
	if (lexer->state != ST_SEP){
		result = malloc(sizeof(Lex));
		result->type = LEX_NO;
		result->value = NULL;
		if (result == NULL)
			return NULL;
		else{
			switch (lexer->state){
			case ST_WORD:
				//maybe it is some reserved word
				result->type = parse_word(str);
				if (result->type == LEX_ID){
					result->value = str->str;
				}
				else{
					//bad code. who cares? nobody cares.
					free(str->str);
				}
				break;
			case ST_OP_SEP:
				result->type = LEX_OP_SEP;
				break;
			case ST_ASSIGN:
				result->type = LEX_ASSIGN;
				break;
			case ST_COLON:
				result->type = LEX_COLON;
				break;
			case ST_DOT:
				result->type = LEX_DOT;
				break;
			case ST_INT:
				result->type = LEX_INT;
				result->value = str->str;
				break;
			default:
				result->type = LEX_UNKNOWN;
				result->value = str->str;
				break;
			}
		}
	}
	str_free(str);
	lexer->state = ST_SEP;
	return result;
}

void lex_free(Lex *lex){
	if (lex == NULL)
		return;
	free(lex->value);
	free(lex);
}

void lex_destroy(Lexer *lexer){
	free(lexer);
}
