#include "../include/haneda_util.h"

#ifndef HANEDA_ENV_H
#define HANEDA_ENV_H

typedef struct E_ast_ *E_ast;
typedef struct E_numval_ *E_numval;
typedef struct E_symbol_ *E_symbol;
typedef struct E_symlist_ *E_symlist;

#define HASH_SIZE 500

struct E_ast_ {
	int nodetype;
	E_ast l;
	E_ast r;
};
struct E_numval_ {
	int nodetype;
	int numtype;
	union {
		int ival;
		double dval;
	} value;
};

/* ����б� */
struct E_symlist_ {
	E_symlist next;
	E_symbol sym;
};

struct E_symbol_ {
	string name;
	int symtype;
	union {
		string str; /* string */
		E_numval numval; /* num */
		E_ast func; /* func */
	} value;
	E_symlist syms; /* func params */
};

E_symbol E_lookup( string name );
E_ast E_newast(int nodetype, E_ast l, E_ast r);
E_ast E_newnum(void *val, int numtype);

#endif
