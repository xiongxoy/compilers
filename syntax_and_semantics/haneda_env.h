#include "../include/haneda_util.h"

#ifndef HANEDA_ENV_H
#define HANEDA_ENV_H

typedef struct E_ast_ *E_ast;
typedef struct E_str_ *E_str;
typedef struct E_symbol_ *E_symbol;
typedef struct E_symlist_ *E_symlist;
typedef struct E_type_ *E_type;
typedef struct E_func_ *E_func;
#define HASH_SIZE 500

typedef enum {
	E_PASS = 0,
	E_SYM ,  // 0
	E_SYM_REF,  // 0
	E_FUNC,  // 3
	E_FUNC_REF,
	E_NUM,   // 0
	E_STRING,  // 0
	E_ADD,   // 2
	E_ASSIGN_EQL,  // 2
	E_DECLARATION,  // 2
	E_BLK_ITM_LIST, // 2
	E_INIT_DECL_LIST,
	E_DECLAR_LIST,
	E_INT,  // 0
	E_FLOAT, // 0
	E_ARRAY,
} node_type;
struct E_ast_ {
	int type;
	string code;
	E_ast l;
	E_ast r;
	string addr;
/*** addtional information ***/
	var_type var_t;
	int list_c;  // number of var
	// TODO add bit set
};
struct E_func_ {
	int type;
	string code;
	E_ast spcifiers;  // type
	E_ast declarator; // name
	E_ast ast; // body
};
struct E_type_ {
	int type;
};
//struct E_numval_ {
//	int type;
//	enum {
//		E_INT = 1,
//		E_FLOAT
//	} num_type;//	string num_text;
//};
struct E_str_ { // used for string value, it can be sym name, int, float or string literal
	int type;
	string s;
};
struct E_strval_ {
	int type;
	string str_text; // FIXME I should use char[] instead, string may contain '/0'
};
struct E_symref_ {
	int type;
	string sym_name;
};
/* params list */
struct E_symlist_ {
	E_symlist next;
	E_symbol sym;
};

/*
 * symbol:
 * 		constant:
 * 				string literal
 * 				float
 * 				int
 * 		id:
 * 			name
 * 			variable type
 *
 * 		function name:
 * 			return type
 *			parameter list
 */
typedef enum {
	E_CONST_STR = 1,
	E_CONST_INT,
	E_CONST_FLOAT,
	E_VAR_INT,
	E_VAR_FLOAT,
	E_NAME_FUNC
} sym_type;
typedef enum {
	E_INT_TYPE = 1,
	E_FLOAT_TYPE,
	E_CHAR_TYPE,
} var_type;
struct E_symbol_ {
	int symtype;
	bool isglobal;
	string name;
	string addr;
	var_type vartype;
//	string str; /* string */
	E_ast func; /* func */
	E_symlist syms; /* func params */
};

enum {
	CONST_INT = 1,
	CONST_FLOAT ,
} const_type;
struct E_num_ {
	int type;
	string s;
	num_type num_t;
};
typedef struct E_num_ * E_num;

E_symbol E_install( string name );
E_symbol E_lookup( string name );
E_type E_newtype(int type);
E_ast E_newast(int type, E_ast l, E_ast r);
E_str E_newstr(int type, string s) ;
E_func E_newfunc(int type, E_ast spec, E_ast decl, E_ast comp);

#endif
