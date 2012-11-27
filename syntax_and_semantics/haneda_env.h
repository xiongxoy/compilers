#include "../include/haneda_util.h"
#include "../lex/haneda_lex.h"

#ifndef HANEDA_ENV_H
#define HANEDA_ENV_H

typedef struct E_ast_ *E_ast;
//typedef struct E_str_ *E_str;
typedef struct E_symbol_ *E_symbol;
typedef struct E_symlist_ *E_symlist;
//typedef struct E_singlet_ *E_singlet;
//typedef struct E_func_ *E_func;
//typedef struct E_spec_ *E_spec;
//typedef struct E_num_ * E_num;
typedef union spec_map spec_map;
#define HASH_SIZE 500

struct E_singlet_ {
	int type;
};
union spec_map
{
	struct Bit
	{
		unsigned char b_int:1;
		unsigned char b_float:1;
		unsigned char b_const:1;
		unsigned char b_void:1;
		unsigned char b_char:1;
		unsigned char b6 :1;
		unsigned char b7 :1;
		unsigned char b8 :1; // higher bit
	} bit;
	unsigned char ch;

} ;

struct E_spec_ {
	int type;
	spec_map m;
};
struct E_str_ { // used for string value, it can be sym name, int, float or string literal
	int type;
	string addr;
	string code;
	string s;
};
//struct E_func_ {
//	int type;
//	string addr;
//	string code;
//	E_spec spcifiers;  // type
//	E_ast declarator; // name
//	E_ast ast; // body
//};
struct E_num_ {
	int type;
	string addr;
	string code;
	string s;
	num_type num_t;
};
typedef enum {
	E_PASS = 0,
	E_SYM ,  // 0
	E_SYM_REF,  // 0
	E_FUNC,  // 3
	E_FUNC_REF,
	E_NUM,   // 0
	E_STRING,  // 0
	E_ADD,   // 2
	E_MINUS,
	E_MUL,
	E_DIV,
	E_INIT_DECL,
	E_ASSIGN_EQL,  // 2
	E_DECLARATION,  // 2
	E_BLK_ITM_LIST, // 2
	E_INIT_DECL_LIST,
	E_DECLAR_LIST,
	E_SPEC,
	E_INT,  // 0
	E_FLOAT, // 0
	E_ARRAY,
	E_ARRAY_REF_END,
	E_ARRAY_REF_LIST,
	E_ARRAY_BODY,
	E_ARRAY_REF,
	E_FUNC_BODY,
	E_ARG_END,
	E_ARG_LIST,
	E_CONT_IF,
	E_CONT_FOR,
	E_REL_L,
	E_REL_GE,
	E_INC_OP,
	E_UNARY_EXP,
	E_TAKE_ADDR,
	E_DECLAR_STORAGE,
	E_INIT_VAL_LIST,
} node_type;
typedef enum {
	E_NONE_TYPE = 1,
	E_INT_TYPE,
	E_FLOAT_TYPE,
	E_CHAR_TYPE,
	E_STRING_LIETRAL_TYPE,
	E_POINTER,
} var_type;
typedef enum {
	E_CONST_SYM = 1,
	E_VAR_SYM,
	E_FUNC_SYM,
	E_ARRAY_SYM,
//	E_CONST_STR = 1,
//	E_CONST_INT,
//	E_CONST_FLOAT,
//	E_VAR_INT,
//	E_VAR_FLOAT,
//	E_NAME_FUNC
} sym_type;
struct E_symbol_ {
	sym_type symtype;
	bool isglobal;
	bool isfunc;
	string name;
	string addr;
	string label;
	var_type var_t;
//	string str; /* string */
	E_ast func; /* func */
	E_symlist syms; /* func params */
/******** array ***********/
	int width; // 1d array
//	U_list d_list;
//	int d_size;
};
struct E_ast_ {
	int type;
	string addr; // FIXME this one is very important
	string code;
	E_ast l;
	E_ast r;

/**** FOR *****/
	E_ast init_stat;
//	bool
	E_ast step_state;
//	E_ast ast
/*** IF ELSE ****/
	E_ast bool_exp;
	E_ast true_stat;
	E_ast false_stat;
/*** func type ****/
	E_ast spcifiers;  // type
	E_ast declarator; // name
	E_ast ast; // body
/*** spec type ****/
	spec_map m;
/*** str type ****/
	string s;
/*** num_type ****/
	num_type num_t;
/*** addtional information ***/
	var_type var_t;
	bool pending;
	bool isfunc;
	bool isarray;
	U_list list;
	int list_c;  // number of var
	// TODO add bit set
};

struct E_symref_ {
	int type;
	string addr;
	string code;
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

E_symbol E_install( string name );
E_symbol E_lookup( string name );
//E_singlet E_newsinglet(int type);
//E_spec E_newspec(int type);
E_ast E_newast(int type, E_ast l, E_ast r);
//E_str E_newstr(int type, string s) ;
//E_func E_newfunc(int type, E_ast spec, E_ast decl, E_ast comp);

#endif
