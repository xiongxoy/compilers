#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../include/haneda_util.h"


#ifndef HANE_LEX
#define HANE_LEX

#define issingle(c) (c)=='(' || (c)==')' || (c)==';' || (c)==',' \
					|| (c)=='{' || (c)=='}' || (c)=='[' || (c)==']' \
					|| (c)=='*' || (c)=='/' || (c) == '=' || (c)=='&'
enum {
	L_CONSTANT = 260,
	L_STRING_LITERAL,
	L_IDENTIFIER ,
	L_INT,
	L_FLOAT,
	L_IF,
	L_FOR,
	L_ELSE,
	L_RETURN,
	L_INCREMENT,
	L_DECREMENT,
	L_LS_EQ,
	L_BG_EQ
};
typedef enum {NM_F = 1, NM_I} num_type;
struct lexval {
    string s;
    num_type num_t;
    int d;
    double f;
} * val;



void val_cpy(struct lexval * dest, const struct lexval * src);
void lex_init();
int start(const string file_name);
void printbuffer();
int lex() ;

#endif
