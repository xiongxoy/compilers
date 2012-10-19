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
	NUM = 260,
	STR_LITERAL,
	NAME,
	INT,
	FLOAT,
	IF,
	FOR,
	ELSE,
	RETURN,
	INCREMENT,
	DECREMENT,
	LS_EQ,
	BG_EQ
};

struct lexval {
    string s;
    enum {NM_F, NM_D} type;
    int d;
    double f;
} val;

static FILE * file ;
static char buffer[1025];
static int count  = 0;

int start(const string file_name);
void printbuffer();
int lex() ;

#endif
