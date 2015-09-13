#include "lexer.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "d_str.h"
#include <ctype.h>
#include "error_proc.h"

Lexer *lex_create(Stream *in_stream){
	Lexer *result = smalloc(sizeof(Lexer));
	if (result == NULL)
		return result;
	result->inp = in_stream;
	result->state = ST_SEP;
	result->cur = NULL;
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
	if (str_compare(str, "not")){
		return LEX_NOT;
	}
	if (str_compare(str, "mod")){
		return LEX_MOD;
	}
	if (str_compare(str, "div")){
		return LEX_DIV;
	}
	if (str_compare(str, "and")){
		return LEX_AND;
	}
	if (str_compare(str, "for")){
		return LEX_FOR;
	}
	if (str_compare(str, "while")){
		return LEX_WHILE;
	}
	if (str_compare(str, "else")) {
		return LEX_ELSE;
	}
	if (str_compare(str, "do")){
		return LEX_DO;
	}
	if (str_compare(str, "to")){
		return LEX_TO;
	}
	if (str_compare(str, "if")) {
		return LEX_IF;
	}
	if (str_compare(str, "then")) {
		return LEX_THEN;
	}
	if (str_compare(str, "downto")) {
		return LEX_TO;
	}
	if (str_compare(str, "uses")) {
		return LEX_USES;
	}
	if (str_compare(str, "repeat")){
		return LEX_REPEAT;
	}
	if (str_compare(str, "until")){
		return LEX_UNTIL;
	}
	if (str_compare(str, "shl")){
		return LEX_SHL;
	}
	if (str_compare(str, "shr")){
		return LEX_SHR;
	}
	if (str_compare(str, "xor")){
		return LEX_XOR;
	}
	if (str_compare(str, "or")){
		return LEX_OR;
	}
	if (str_compare(str, "var")){
		return LEX_VAR;
	}
	if (str_compare(str, "const")){
		return LEX_CONST;
	}
	if (str_compare(str, "true")){
		return LEX_TRUE;
	}
	if (str_compare(str, "false")){
		return LEX_FALSE;
	}
	if (str_compare(str, "procedure")){
		return LEX_PROCEDURE;
	}
	if (str_compare(str, "function")){
		return LEX_FUNCTION;
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
			//+
			if (c == '+'){
				ret = 1;//it's a final stage
				stream_read(lexer->inp);
				lexer->state = ST_PLUS;
				break;
			}
			//-
			if (c == '-'){
				ret = 1;//it's a final stage
				stream_read(lexer->inp);
				lexer->state = ST_MINUS;
				break;
			}
			//*
			if (c == '*'){
				ret = 1;//it's a final stage
				stream_read(lexer->inp);
				lexer->state = ST_MULT;
				break;
			}
			//=
			if (c == '='){
				ret = 1;//it's a final stage
				stream_read(lexer->inp);
				lexer->state = ST_EQ;
				break;
			}
			//)
			if (c == ')'){
				ret = 1;//it's a final stage
				stream_read(lexer->inp);
				lexer->state = ST_RB;
				break;
			}
			//,
			if (c == ','){
				ret = 1;//it's a final stage
				stream_read(lexer->inp);
				lexer->state = ST_COMMA;
				break;
			}
			//char (string)
			if (c == '\''){
				ret = 1;//it's a final stage
				stream_read(lexer->inp);
				c = stream_next(lexer->inp);
				stream_read(lexer->inp);
				if (c == EOF){
					ret = 1;
					break;
				}
				while (c != '\''){
					str_add(str, c);
					c = stream_next(lexer->inp);
					stream_read(lexer->inp);
					if (c == EOF){
						ret = 1;
						break;
					}
				}
				if (str_size(str) != 1){
					//string not supported yet
					ret = 1;
					lexer->state = ST_SEP;
					break;
				}
				ret = 1;
				lexer->state = ST_CHAR;
				break;

			}
			//colon or assign
			if (c == ':'){
				stream_read(lexer->inp);
				lexer->state = ST_COLON;
				break;
			}
			if (c == '>'){
				stream_read(lexer->inp);
				lexer->state = ST_MORE;
				break;
			}
			if (c == '<'){
				stream_read(lexer->inp);
				lexer->state = ST_LESS;
				break;
			}
			//numbers. integer or real
			if (is_09(c)){
				str_add(str, c);
				stream_read(lexer->inp);
				lexer->state = ST_INT;
				break;
			}

			//Line commect
			if (c == '/'){
				stream_read(lexer->inp);
				c = stream_next(lexer->inp);
				if (c == EOF){
					ret = 1;
					break;
				}
				if (c == '/'){
					stream_read(lexer->inp);
					//line comment detected. waiting for \n
					while (c != '\n'){
						c = stream_next(lexer->inp);
						stream_read(lexer->inp);
						if (c == EOF){
							ret = 1;
							break;
						}
					}
					lexer->state = ST_SEP;
					break;
				}
				lexer->state = ST_ERROR;
				ret = 1;
				break;
			}
			//{} commect
			if (c == '{'){
				stream_read(lexer->inp);
				//{} comment detected. waiting for }
				while (c != '}'){
					c = stream_next(lexer->inp);
					stream_read(lexer->inp);
					if (c == EOF){
						ret = 1;
						break;
					}
				}
				lexer->state = ST_SEP;
				break;
			}
			//LB or (*
			if (c == '('){
				stream_read(lexer->inp);
				lexer->state = ST_LB;
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
		case ST_LESS:
			//decide it is colon or assignment
			if (c == '<'){
				//it's shl
				lexer->state = ST_SHL;
				stream_read(lexer->inp);
				ret = 1;
			}
			else if (c == '>'){
				//it's not-eq
				lexer->state = ST_NOT_EQ;
				stream_read(lexer->inp);
				ret = 1;
			}
			else if (c == '='){
				//it's less-eq
				lexer->state = ST_LESS_EQ;
				stream_read(lexer->inp);
				ret = 1;
			}
			else
			{
				//less
				ret = 1;
			}
			break;
		case ST_MORE:
			//decide it is colon or assignment
			if (c == '>'){
				//it's shr
				lexer->state = ST_SHR;
				stream_read(lexer->inp);
				ret = 1;
			}
			else if (c == '='){
				//it's more-eq
				lexer->state = ST_MORE_EQ;
				stream_read(lexer->inp);
				ret = 1;
			}
			else{
				//more
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

		case ST_LB:
			if (c == '*') {
				stream_read(lexer->inp);
				//(* comment
				while (c != ')'){
					while (c != '*'){
						c = stream_next(lexer->inp);
						stream_read(lexer->inp);
						if (c == EOF){
							ret = 1;
							lexer->state = ST_SEP;
							break;
						}
					}
					c = stream_next(lexer->inp);
					stream_read(lexer->inp);
					if (c == EOF){
						ret = 1;
						lexer->state = ST_SEP;
						break;
					}
				}
				lexer->state = ST_SEP;
			}
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
		result = smalloc(sizeof(Lex));
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
			case ST_LB:
				result->type = LEX_LB;
				break;
			case ST_LESS:
				result->type = LEX_LESS;
				break;
			case ST_MORE:
				result->type = LEX_MORE;
				break;
			case ST_SHL:
				result->type = LEX_SHL;
				break;
			case ST_SHR:
				result->type = LEX_SHR;
				break;
			case ST_NOT_EQ:
				result->type = LEX_NOT_EQ;
				break;
			case ST_LESS_EQ:
				result->type = LEX_LESS_EQ;
				break;
			case ST_MORE_EQ:
				result->type = LEX_MORE_EQ;
				break;
			case ST_RB:
				result->type = LEX_RB;
				break;
			case ST_PLUS:
				result->type = LEX_PLUS;
				break;
			case ST_MINUS:
				result->type = LEX_MINUS;
				break;
			case ST_MULT:
				result->type = LEX_MULT;
				break;
			case ST_EQ:
				result->type = LEX_EQ;
				break;
			case ST_COMMA:
				result->type = LEX_COMMA;
				break;
			case ST_INT:
				result->type = LEX_INT;
				result->value = str->str;
				break;
			case ST_CHAR:
				result->type = LEX_CHAR;
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
	lexer->cur = result;
	return result;
}

Lex *lex_cur(Lexer *lexer){
	if (lexer->cur == NULL)
		lex_next(lexer);

	return lexer->cur;
	
}

void lex_free_lex(Lex *lex){
	if (lex == NULL)
		return;
	free(lex->value);
	free(lex);
}

void lex_destroy_lex(Lex *lex){
	if (lex == NULL)
		return;
	free(lex);
}

void lex_destroy(Lexer *lexer){
	free(lexer);
}