#include "generator.h"
#include "error_proc.h"
#include "syntaxtree.h"

static void gen_program(Generator *gen, SList *tree);
static void foreach_syn(Syntax *syn, Generator *gen);

Generator *gen_create(FILE *file) {
	Generator *result = smalloc(sizeof(Generator));
	result->f = file;
	result->sep = ';';
	return result;
}

static void printmain(FILE *f) {
	fprintf(f, "int main(){\n");
	fprintf(f, "    con_init();");
}

static void printneccstart(FILE *f) {
	fprintf(f, "#define _CRT_SECURE_NO_WARNINGS\n");
	fprintf(f, "#include <stdlib.h>\n");
	fprintf(f, "#include <stdio.h>\n");
	fprintf(f, "#include <stdbool.h>\n");
	fprintf(f, "#include \"console.h\"\n\n");
}

static void printneccfinish(FILE *f) {
	fprintf(f, "    con_deinit();\n");
	fprintf(f, "    return 0;\n");
	fprintf(f, "}");
}

static void printcrt(FILE *f) {
	//textcolor
	fprintf(f, "short PASlator_bg = CON_COLOR_BLACK;\n");
	fprintf(f, "short PASlator_fg = CON_COLOR_WHITE;\n");
	fprintf(f, "void textcolor(int fg){\n"
			   "    if (fg == 0) PASlator_fg = CON_COLOR_BLACK;\n"
			   "    if (fg == 1) PASlator_fg = CON_COLOR_BLUE;\n"
			   "    if (fg == 2) PASlator_fg = CON_COLOR_GREEN;\n"
			   "    if (fg == 4) PASlator_fg = CON_COLOR_RED;\n"
			   "    if (fg == 15) PASlator_fg = CON_COLOR_WHITE;\n"
			   "    con_initPair(0, PASlator_fg, PASlator_bg);\n"
			   "    con_setColor(0);\n"
			   "}\n\n");
	fprintf(f, "void textbackground(int bg){\n"
			"    if (bg == 0) PASlator_bg = CON_COLOR_BLACK;\n"
			"    if (bg == 1) PASlator_bg = CON_COLOR_BLUE;\n"
			"    if (bg == 2) PASlator_bg = CON_COLOR_GREEN;\n"
			"    if (bg == 4) PASlator_bg = CON_COLOR_RED;\n"
			"    if (bg == 15) PASlator_bg = CON_COLOR_WHITE;\n"
			"    con_initPair(0, PASlator_fg, PASlator_bg);\n"
			"    con_setColor(0);\n"
			"}\n\n");
	fprintf(f, "int ord(char c){\n"
			"    return c;\n"
			"}\n\n");
	fprintf(f, "char readkey(){\n"
			"    int result = con_getKey();\n"
			"    if ((result<0)||(result>255)) exit(1);\n"
			"    return (char) result;\n"
			"}\n\n");
	fprintf(f, "void clrscr(){\n"
			"    con_clearScr();\n"
			"}\n\n");
	fprintf(f, "void gotoxy(int x, int y){\n"
			"    con_gotoXY(x, y);\n"
			"}\n\n");
}

static void printdeclvar(Generator *gen, VarDeclaration *var) {
	switch (var->type) {
		case TYPE_INTEGER:
			fprintf(gen->f, "int ");
			break;
		case TYPE_BOOLEAN:
			fprintf(gen->f, "bool ");
			break;
		case TYPE_CHAR:
			fprintf(gen->f, "char ");
			break;
		case TYPE_NO:
			fprintf(gen->f, "void ");
			break;
		default:
			error_custom("Generator: unknown var type");
			break;
	}
	fprintf(gen->f, "%s", var->cname);
	if (var->callable) {
		fprintf(gen->f, "(");
		SList *args = slist_reverse(var->args);
		for (SList *i = args; i != NULL; i = i->next) {
			VarDeclaration *vd = ((Syntax *)(i->data))->syn;
			printdeclvar(gen, vd);
			if ((i->next) != NULL)
				fprintf(gen->f, ", ");
		}
		slist_free(args);
		fprintf(gen->f, ") {\n");
		gen_program(gen, var->body);
		fprintf(gen->f, "}\n\n");
	}

}

static void printfor(Generator *gen, ForLoop *loop) {
	fprintf(gen->f, "for (%s = ", loop->var);
	gen_program(gen, loop->start);
	if (loop->direct) {
		fprintf(gen->f, " %s <= ", loop->var);
		gen_program(gen, loop->finish);
		fprintf(gen->f, " %s ++) {\n", loop->var);
	}
	else {
		fprintf(gen->f, " %s >= ", loop->var);
		gen_program(gen, loop->finish);
		fprintf(gen->f, " %s --) {\n", loop->var);
	}
	gen_program(gen, loop->body);
	fprintf(gen->f, "}\n");

}

static void printcall(Generator *gen, Call *call) {
	fprintf(gen->f, "%s(", call->cname);

	char sep = gen->sep;
	gen->sep = ' ';

	SList *args = slist_reverse(call->args);
	for (SList *i = args; i != NULL; i = i->next) {
		Syntax *syn = (Syntax *)(i->data);
		foreach_syn(syn, gen);
		if ((i->next) != NULL)
			fprintf(gen->f, ", ");
	}
	slist_free(args);
	gen->sep = sep;
	fprintf(gen->f, ")", call->cname);

}

static void printwrite(Generator *gen, Write *write) {
	fprintf(gen->f, "con_outTxt(\"");
	SList *args = slist_reverse(write->args);
	for (SList *i = args; i != NULL; i = i->next) {
		Expr *expr = (Expr *)(i->data);
		switch (expr->type) {
			case TYPE_BOOLEAN:
				fprintf(gen->f, "%%s");
				break;
			case TYPE_INTEGER:
				fprintf(gen->f, "%%d");
				break;
			case TYPE_CHAR:
				fprintf(gen->f, "%%c");
				break;
		}
	}
	if (write->newline) {
		fprintf(gen->f, "\\n");
	}
	fprintf(gen->f, "\", ");
	char sep = gen->sep;
	gen->sep = ' ';
	for (SList *i = args; i != NULL; i = i->next) {
		Expr *expr = (Expr *)(i->data);
		switch (expr->type) {
			case TYPE_BOOLEAN:
				fprintf(gen->f, "(");
				gen_program(gen, expr->tree);
				fprintf(gen->f, ")?\"True\":\"False\"");
				break;
			case TYPE_INTEGER:
				gen_program(gen, expr->tree);
				break;
			case TYPE_CHAR:
				gen_program(gen, expr->tree);
				break;
		}
		if ((i->next) != NULL)
			fprintf(gen->f, ", ");
	}
	fprintf(gen->f, ")");
	slist_free(args);
	gen->sep = sep;
}

static void printbinop(Generator *gen, BinOp *binop) {
	char sep = gen->sep;
	gen->sep = ' ';

	fprintf(gen->f, "(");
	gen_program(gen, binop->tree1);
	fprintf(gen->f, ")");
	switch (binop->type) {
		case OP_ANDbool:
			fprintf(gen->f, "&&", gen->sep);
			break;
		case OP_ANDint:
			fprintf(gen->f, "&", gen->sep);
			break;
		case OP_ASSIGN:
			fprintf(gen->f, "=", gen->sep);
			break;
		case OP_DIV:
			fprintf(gen->f, "div", gen->sep);
			break;
		case OP_EQ:
			fprintf(gen->f, "==", gen->sep);
			break;
		case OP_LESS:
			fprintf(gen->f, "<", gen->sep);
			break;
		case OP_LESS_EQ:
			fprintf(gen->f, "<=", gen->sep);
			break;
		case OP_MINUS:
			fprintf(gen->f, "-", gen->sep);
			break;
		case OP_MOD:
			fprintf(gen->f, "mod", gen->sep);
			break;
		case OP_MORE:
			fprintf(gen->f, ">", gen->sep);
			break;
		case OP_MORE_EQ:
			fprintf(gen->f, ">=", gen->sep);
			break;
		case OP_MULT:
			fprintf(gen->f, "*", gen->sep);
			break;
		case OP_NOT_EQ:
			fprintf(gen->f, "!=", gen->sep);
			break;
		case OP_ORbool:
			fprintf(gen->f, "||", gen->sep);
			break;
		case OP_ORint:
			fprintf(gen->f, "|", gen->sep);
			break;
		case OP_PLUS:
			fprintf(gen->f, "+", gen->sep);
			break;
		case OP_SHL:
			fprintf(gen->f, ">>", gen->sep);
			break;
		case OP_SHR:
			fprintf(gen->f, "<<", gen->sep);
			break;
		default:
			error_custom("Generator: unknown binary operation");
			break;
	}

	fprintf(gen->f, "(");
	gen_program(gen, binop->tree2);
	fprintf(gen->f, ")");
	gen->sep = sep;
}



static void printrepeat(Generator *gen, While *loop) {
	fprintf(gen->f, "do{\n", gen->sep);
	gen_program(gen, loop->body);
	fprintf(gen->f, "} while (\n", gen->sep);
	char sep = gen->sep;
	gen->sep = ' ';
	gen_program(gen, loop->cond);
	gen->sep = sep;
	fprintf(gen->f, ");\n", gen->sep);
}

static void printwhile(Generator *gen, While *loop) {
	fprintf(gen->f, "while (", gen->sep);
	char sep = gen->sep;
	gen->sep = ' ';
	gen_program(gen, loop->cond);
	gen->sep = sep;
	fprintf(gen->f, "){\n", gen->sep);
	gen_program(gen, loop->body);
	fprintf(gen->f, "}\n", gen->sep);
}

static void printif(Generator *gen, If *ifthen) {
	fprintf(gen->f, "if (", gen->sep);
	char sep = gen->sep;
	gen->sep = ' ';
	gen_program(gen, ifthen->cond);
	gen->sep = sep;
	fprintf(gen->f, "){\n", gen->sep);
	gen_program(gen, ifthen->then_body);
	fprintf(gen->f, "}\n", gen->sep);
	if (ifthen->else_body != NULL) {
		fprintf(gen->f, " else {\n", gen->sep);
		gen_program(gen, ifthen->else_body);
		fprintf(gen->f, "}\n", gen->sep);
	}
}

static void foreach_syn(Syntax *syn, Generator *gen) {
	switch (syn->type) {
		case SYN_CRT:
			printcrt(gen->f);
			break;
		case SYN_FLAG:
			printmain(gen->f);
			break;
		case SYN_DECL_VAR:{
							  VarDeclaration *var = syn->syn;

							  printdeclvar(gen, var);
							  fprintf(gen->f, "%c\n", gen->sep);
							  break;
		}
		case SYN_NOOP:
			fprintf(gen->f, ";");
			break;
		case SYN_FOR:{
						 ForLoop *loop = syn->syn;
						 printfor(gen, loop);
						 fprintf(gen->f, "%c\n", gen->sep);
						 break;
		}
		case SYN_VAR:{
						 Var *var = syn->syn;
						 fprintf(gen->f, "%s%c\n", var->value, gen->sep);
						 break;
		}
		case SYN_CALL:{
						  Call *call = syn->syn;
						  printcall(gen, call);
						  fprintf(gen->f, "%c\n", gen->sep);
						  break;

		}
		case SYN_WRITE:{
						  Write *write = syn->syn;
						  printwrite(gen, write);
						  fprintf(gen->f, "%c\n", gen->sep);
						  break;

		}
		case SYN_CHAR:{
						 Char *var = syn->syn;
						 fprintf(gen->f, "'%c'%c\n", var->value, gen->sep);
						 break;
		}
		case SYN_INT:{
						 Int *var = syn->syn;
						 fprintf(gen->f, "%d%c\n", var->value, gen->sep);
						 break;
		}
		case SYN_BOOLEAN:{
						 Boolean *var = syn->syn;
						 fprintf(gen->f, "%s%c\n", (var->value)?true:false, gen->sep);
						 break;
		}
		case SYN_BINOP:{
						   BinOp *binop = syn->syn;
						   printbinop(gen, binop);
						   fprintf(gen->f, "%c\n", gen->sep);
						   break;;
		}
		case SYN_WHILE:{
						   While *loop = syn->syn;
						   printwhile(gen, loop);
						   fprintf(gen->f, "%c\n", gen->sep);
						   break;;
		}
		case SYN_REPEAT:{
							While *loop = syn->syn;
							printrepeat(gen, loop);
						   fprintf(gen->f, "%c\n", gen->sep);
						   break;;
		}
		case SYN_IF:{
							If *ifthen = syn->syn;
							printif(gen, ifthen);
							fprintf(gen->f, "%c\n", gen->sep);
							break;;
		}
		default:
			error_custom("\nGenerator: unknown syntax");
			break;
	}
}

static void gen_program(Generator *gen, SList *tree) {
	SList *queue = slist_reverse(tree);
	slist_foreach(queue, foreach_syn, gen);
	slist_free(queue);
}

void gen_process(Generator *gen, SList *tree) {
	printneccstart(gen->f);
	gen_program(gen, tree);
	printneccfinish(gen->f);
}

void gen_destroy(Generator *gen) {
	free(gen);
}