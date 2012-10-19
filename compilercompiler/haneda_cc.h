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
typedef struct C_table_item_ * C_table_item;
typedef struct C_table_	* C_table;
typedef struct C_item_ * C_item;
typedef struct C_item_list_ * C_item_list;
typedef struct C_token_ * C_token;
typedef struct C_state_list_ * C_state_list;
typedef struct C_state_ * C_state;
typedef U_list C_token_list;
typedef U_set  C_token_set;


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
		TERMINAL,
		NON_TERMINAL
	} type;
	bool nullable;
	U_set first;
	U_set follow;
};

#define TOKEN_NR 128
#define RULE_NR 100
#define STATE_NR 1050
extern string rules_text[RULE_NR];
extern C_rule rules[RULE_NR];
extern string tokens_text[TOKEN_NR];
extern C_table_item action_table[STATE_NR];


int getTokenNr(const string token);
int getHeadNr(const string rule);
C_token_list getTokenList(const string rule);
void rule_textTotokens();
void rule_textTorules();
void closure();
void gototable();
/********* debug ***************/
void haneda_initialize();

#endif

