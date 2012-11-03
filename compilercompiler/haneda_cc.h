#include "../include/haneda_util.h"


/*
 * TODO ��α�ʾ����

 * 		1. token �У�����������token��
 * 			һ��token parser�� ��stringת����token
 * 			Ҫһ��token�?�������token
 * 			ѹ��ջ�ﶼ�����֣�
 * 		2. string
 * TODO ���
 */

#ifndef HANEDA_CC_H
#define HANEDA_CC_H

typedef struct C_rule_ * C_rule;
typedef struct C_table_row_ * C_table_row;
typedef struct C_table_	* C_table;
typedef struct C_item_ * C_item;
typedef struct C_item_list_ * C_item_list;
typedef struct C_token_ * C_token;
typedef struct C_state_list_ * C_state_list;
typedef struct C_state_ * C_state;
typedef U_list C_token_list;
typedef U_set  C_token_set;

#define TOKEN_NR 230
#define RULE_NR 270
#define STATE_NR 2000
typedef enum {
	SHIFT,
	REDUCE,
	GOTO,
	ERROR,
	ACC
} act_type;

extern string rules_text[RULE_NR];
extern int tokens_type[TOKEN_NR];
extern C_rule rules[RULE_NR];
extern string tokens_text[TOKEN_NR];
extern C_table_row action_table[STATE_NR];

struct C_table_row_ {
	int actions[TOKEN_NR];
	act_type type[TOKEN_NR];
};
struct C_item_ {
	int rule_id;
	int point; /* 0为起始值 */
	C_token_set syms; /* 跟随的符号 */
};
struct C_item_list_ {
	C_item item;
	C_item_list next;
};
struct C_state_ {
	C_item_list items;
	int size;
};
struct C_state_list_ {
	C_state state;
	C_state_list next;
};
struct C_rule_ {
	C_token_list list;
	int head_token_nr;
};
struct C_token_ {
	string text;
	enum {
		TERMINAL = 1,
		NON_TERMINAL
	} type;
	bool nullable;
	U_set first;
	U_set follow;
};

/************* util ****************/
void list_print(U_list list);
U_list list_dup(U_list list);
void rule_textTorules();
void token_textTotokens();
void set_print( U_set set );
bool set_cmp(U_set A, U_set B);
int getTokenNr(const string token);
int getHeadNr(const string rule);
int token_find_id( const string token_text);
C_token_list getTokenList(const string rule);
void rule_textTotokens();

/*********** LR1 table ******************/
void haneda_initialize();
void generate_LR1_Table();
C_token_list item_getTokenAfterPoint(C_item item);
bool statecmp(C_state I, C_state J);
int gotoClosure(int state_id, int token_id);
int closure(C_state I);

/********* debug ***************/
void getNullFirstFollow();

#endif

