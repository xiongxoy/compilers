#include "haneda_cc.h"

string tokens_text[TOKEN_NR] = {
		"S","T","x","L","(", ")","&", ",", "null", "$"
};
int tokens_type[TOKEN_NR] = {
		 1,  1,  0,  1,  0,  0,   0,   0 ,   0,    0
};
C_token	tokens[TOKEN_NR] = {NULL,};
string rules_text[RULE_NR] = {
		"S: T $",
		"T: ( L )",
		"T: x",
		"L: T",
		"L: L , S"
};
C_rule rules[RULE_NR] = {NULL,};
C_state states[STATE_NR] = {NULL,}; // TODO 不知道够不够用
C_table_item action_table[STATE_NR] = {NULL,};

void getNullFirstFollow();

int getTokenNr(const string token) {
	printf("target token text test: %s\n", token);
	for(int i=0; i<128; i++) {
		assert(tokens_text[i]);
		if ( 0 == strcmp(token, tokens_text[i]) ) {
			return i;
		}
	}
	assert(0);
	return -1;
}

int getHeadNr(const string rule) {
	char temp[128];
	for(int i=0; i<strlen(rule); i++) {
		if ( rule[i] != ':' )
			temp[i] = rule[i];
		else
			temp[i] = '\0';
	}
	return getTokenNr(temp);
}

/* 将rule转换成一个token链表，每个token用token的id表示 */
C_token_list getTokenList(const string rule) {
	int i=0;
	for(i=0; i<strlen(rule);) {
		if (rule[i++] == ':')
			break;
	}

	int stack[128] = {0,};
	int size = 0;
	char buffer[128] = {0,};
	int index = 0;
	enum {
		IN_TOKEN,
		OUT_TOKEN
	} state;
	state = OUT_TOKEN;
	for (; i<strlen(rule)+1; i++) {
		if (state == OUT_TOKEN && rule[i] != ' ') {
			state = IN_TOKEN;
			buffer[index++] = rule[i];
		}
		else if (state == IN_TOKEN && (rule[i] == ' ' || rule[i] == '\0')) {
			state = OUT_TOKEN;
			buffer[index] = '\0';
			stack[size++] = getTokenNr(buffer);
			index = 0;
		}
	}

	C_token_list list = NULL;
	while(size != 0) {
		list_insert(&list, &stack[size-1], INTEGER);
		size--;
	}

	return list;
}
void list_print(U_list list) {
	printf("print list:\n");
	for( U_list p=list; p ; p=p->next) {
		switch(list->nodetype) {
		case STRING:
			printf("%s\n", p->val.s);
			break;
		case INTEGER:
			printf("%d\n", p->val.d);
			break;
		}
	}
}
void rule_textTorules() {
	for (int i=0; rules_text[i] ; i++) {
			rules[i] = malloc( sizeof(*rules[i]) );
			rules[i]->head_token_nr = getHeadNr(rules_text[i]);
			rules[i]->list = getTokenList(rules_text[i]);
			list_print(rules[i]->list);
	}
}
/*
C_rule rule_textTorules(const string rule_text) {
	C_rule rule = malloc( sizeof *rule );
	rule->head_token_nr = getHeadNr(rule_text);
	rule->list = getTokenList(rule_text);
	return rule;
}
*/

void token_textTotokens() {
	C_token p;
	int index = 0;
	for(int i=0; tokens_text[i]; i++) {
		assert( i<128 );
		p = malloc( sizeof *p );
		if( tokens_type[i] == TERMINAL) {
			p->type = TERMINAL;
		}
		else {
			p->type = NON_TERMINAL;
		}
		tokens[i] = p;
		p->first = NULL;
		p->follow = NULL;
		p->nullable = FALSE;
		p->text = tokens_text[i];
	}
}

void set_print( U_set set ) {
	for( U_set p = set; p; p=p->next ) {
		switch (set->nodetype) {
		case STRING:
			printf("%s\n", p->val.s);
		break;
		}
	}
}
void getNullFirstFollow_debug() {
	for(int i=0; tokens[i]; i++) {
		assert( i<128 );
		printf("token text: %s\n",tokens[i]->text);
		printf("token first set: \n");
		set_print(tokens[i]->first);
		printf("token follow set: \n");
		set_print(tokens[i]->follow);
	}
}
C_token_list item_getTokenAfterPoint(C_item item) {
	int point = item->point;
	C_rule rule = rules[item->rule_id];
	C_token_list p = NULL;
	for(p = rule->list; point; p=p->next,point--) {
		assert(p);
	}
	assert(!p || tokens[p->val.d]);
	return p;
}
C_item findByRule() {
	assert(0);
	return NULL;
}
static int rule_find_id(string token_text) {
	for(int i=0; tokens[i]; i++)
		if (0 == strcmp(token_text, tokens[i]->text))
				return i;
	assert(0);
	return NULL;
}
/**
 * 把记录text的first和follow集转化为几率id的集合
 */
static U_set string_to_id (U_set set) {
	U_set ret = NULL;
	int id = 0;
	for(U_set p = set; p; p = p->next) {
		id = rule_find_id(p->val.s);
		set_insert(&ret, &id, INTEGER);
		assert(strcmp(p->val.s, "null") != 0);
	}
	return ret;
}

//TEST
U_list list_dup(U_list list) {
	U_list ret = checked_malloc(sizeof *ret);
	U_list pre = NULL;
	ret->next = NULL;
	ret->nodetype = list->nodetype;
	switch(list->nodetype) {
	case INTEGER:
		ret->val.d = list->val.d; break;
	default:
		assert(0); break;
	}
	pre = ret;
	for(U_list p = list->next; p; p=p->next) {
		U_list temp = checked_malloc( sizeof *temp );
		temp->nodetype = list->nodetype;
		temp->next = pre->next;
		pre->next = temp->next;
		switch(list->nodetype) {
		case INTEGER:
			temp->val.d = p->val.d; break;
		default:
			assert(0); break;
		}
	}
	return ret;
}

typedef struct C_edge_ * C_edge;
typedef struct C_node_ * C_node;
struct C_edge_ {
	int token_id;
	C_edge next;
};
struct C_node_ {
	int state_id;
	C_edge edge_list;
};
bool set_cmp(U_set A, U_set B) {
	bool same = TRUE;
	U_set p = NULL;
	U_set q = NULL;
	int size_A, size_B;

	if( A == NULL || B == NULL) return A == B;
	for(p = A, size_A = 0; p; p=p->next, size_A++) ;
	for(p = B, size_B = 0; p; p=p->next, size_B++) ;
	if( size_A != size_B) return FALSE;
	if( A == B ) { printf("Surprise!\n"); return TRUE; }

	for(p = A; p; p = p->next) {
		for(q = B; q; q = q->next) {
			switch(A->nodetype) {
			case INTEGER:
				if(p->val.d == q->val.d)
					goto next;
				break;
			default:
				assert(0); break;
			}
		}
	next:
		if(NULL == q) { //没有找到
			same = FALSE; break;
		}
	}
	return same;
}
static void statefree(C_state I) {
	; //TODO implement state free
	return ;
	statefree(I);
}

bool statecmp(C_state I, C_state J) {
	bool same = TRUE;
	C_item_list p = NULL;
	C_item_list q = NULL;
	int size_I, size_J;

	assert(I && J);
	if(I->size != J->size) return FALSE;
	for(p = I->items; p; p = p->next) {
		for(q = J->items; q; q = q->next) {
			if(p->item->rule_id == q->item->rule_id && p->item->point == q->item->point) {
				if( TRUE == set_cmp(p->item->syms, q->item->syms) )
					break;
			}
		}
		if(NULL == q) { // NOT FOUND
			same = FALSE; break;
		}
	}
	return same;
}
/* t */
typedef enum {
	SHIFT,
	REDUCE,
	ERROR
} act_type;
/**
 *TEST
 */
int gotoClosure(int state_id, int token_id) {
	C_state I = states[state_id];
	C_token x = tokens[token_id];
	C_state J = NULL;
	act_type type = -1;

	J = checked_malloc(sizeof *J);
	J->items = NULL;
	J->size = 0;

	for(C_item_list p = I->items; p; p=p->next) {
		C_token_list rule_tail = item_getTokenAfterPoint(p->item);
		if(rule_tail && token_id == rule_tail->val.d) {
			C_item item = checked_malloc(sizeof *item);
			item->rule_id = p->item->rule_id;
			item->point = p->item->point+1;
			item->syms = list_dup(p->item->syms);   // RISKY 生成新的列表
			C_item_list ilist = checked_malloc(sizeof *ilist);
			ilist->item = item;
			ilist->next = J->items ? NULL:J->items->next; // RISKY WTF??
			J->items = ilist;
			J->size++;
		}
	}
	if (NULL == J) return -1;
	return closure(J);
}

/**
 * 求I的闭包
 */
int closure(C_state I) {
	C_item item = NULL;
	C_token token = NULL;
	C_token_list rule_tail = NULL;
	C_token_set rule_syms = NULL;

	assert(I);
	for(C_item_list p = I->items; p; p = p->next) {
		rule_tail = item_getTokenAfterPoint(p->item);
		rule_syms = p->item->syms;
		if( !rule_tail || tokens_type[rule_tail->val.d] == TERMINAL) // TODO think about when NULL == rule_tail
			continue;

		for(int i=0; rules[i]; i++) { /* 对点后的非终结符生成新的项目 */
			if (rules[i]->head_token_nr == rule_tail->val.d) {
				C_token_set syms = NULL;
				C_token_list ptr = NULL;
				for(ptr = rule_tail; ptr; ptr = ptr->next ) {
					C_token_set first =  string_to_id(tokens[ptr->val.d].first);
					set_union(&syms, &first); /* 现在因为规则中没有null的存在，暂不考虑空的情况 TODO 考虑空 */
					if( !tokens[ptr->val.d]->nullable )
						break;
				}
				if (NULL == ptr) { /* 点之后的非终结符都可为空 */
					set_union(&syms, &p->item->syms);
				}

				C_item_list q = NULL;
				for(q = I->items; q; q = q->next) {
					if(q->item.rule_id == i && q->item.point == 0) {
						set_union(&q->item->syms, &syms);
						//set_free(syms); TODO free
						goto next;
					}
				}
				/* 插入新的项目 */
				C_item temp = checked_malloc(sizeof *temp);
				temp->point = 0;
				temp->rule_id = i;
				temp->syms = syms;
				C_item_list list_temp = checked_malloc(sizeof *list_temp);
				list_temp->item = temp;
				list_temp->next = p->next;
				p->next = list_temp; //新的项目加到了当前的下面一个，从而可以在一个for循环里完全处理
next:			;
			}
		}
	}

	int i=0;
	for(i=0; states[i]; i++) {
		assert( i<1050 );
		if(statecmp(states[i],I) == TRUE) {state_free(I);return i;}
	}
	states[i] = I;
	return i;
}


void generate_LR1_Table() {
	C_state I = checked_malloc( sizeof *I );
	I->size = 1;
	I->items = checked_malloc( sizeof *I->items );
	I->items->next = NULL;
	I->items->item = checked_malloc( sizeof *I->items->item );
	I->items->item->point = 0;
	I->items->item->rule_id = 0;
	I->items->item->syms = NULL; // RISKY 因为初始的项目不会被移进
	int state_id = closure(I);
	states[0] = I;
	for(int i=0; tokens[i]; i++) {
		gotoClosure(state_id, i);
	}


}

void haneda_initialize() {
	static bool done = FALSE;
	if(done) return;
	/**** body *****/
	rule_textTorules();
	token_textTotokens();
	getNullFirstFollow();
	   getNullFirstFollow_debug();
	generate_LR1_Table();
	/**** body end *****/
	done = TRUE;
}

static C_token token_find( const string token_text ) {
	for(int i=0; tokens[i]; i++)
		if (0 == strcmp(token_text, tokens[i]->text))
				return tokens[i];
	assert(0);
	return NULL;
}


// TODO ��ϸ����null������
// TODO ����������ô������һ���ֵģ��ؼ���$��null������
// TODO 把first和follow里的东西全部改成id。。。之前用string太扯了
void getNullFirstFollow() {
	for(int i=0; tokens[i]; i++) {
		if( tokens[i]->type == NON_TERMINAL)
			continue;
		set_insert(&tokens[i]->first, &tokens[i]->text, STRING);
	}
	token_find("null")->nullable = TRUE;
	string s = "$";
	set_insert(&token_find("S")->follow, &s, STRING);

	bool touched = TRUE;
	while( TRUE ) {
		if(!touched)
			break;
		touched = FALSE;
		for(int i=0; rules[i]; i++) {
			/*****  nullable ******/
			bool flag = TRUE;
			C_token head_token = tokens[rules[i]->head_token_nr];
			assert(head_token);
			if(head_token->nullable == TRUE)
				goto next;
			assert(rules[i]->list && rules[i]->list->val.addr);
			/**** debug *****/
			printf("before seg\n");
			assert(rules[i]->list->val.addr);
			printf("debug seg fault: %d\n", rules[i]->list->val.d );
			if (0 == strcmp(tokens_text[rules[i]->list->val.d], "null")) {
				head_token->nullable = TRUE;
				touched = TRUE;
			}

			for(C_token_list p = rules[i]->list; p; p = p->next ) {
				if( !tokens[p->val.d]->nullable ) {
					flag = FALSE;
					break;
				}
			}
			if(flag == TRUE) { head_token->nullable = TRUE; touched = TRUE; }
next:
			/******* First **********/
			for(C_token_list p = rules[i]->list; p; p = p->next ) {
				touched = set_union(&head_token->first, &tokens[p->val.d]->first)? TRUE:touched;
				if( !tokens[p->val.d]->nullable ) {
					break;
				}
			}

			/******* Follow *********/
			for(C_token_list p = rules[i]->list; p; p = p->next ) {
				flag = TRUE;
				for(C_token_list q = p->next; q; q = q->next) {
					touched = set_union(&tokens[p->val.d]->follow, &tokens[q->val.d]->first)? TRUE:touched;
					if (!tokens[q->val.d]->nullable) {
						flag = FALSE;
						break;
					}
				}
				if (flag) touched = set_union(&tokens[p->val.d]->follow, &head_token->follow)? TRUE:touched;
			}
		}
	}
}

void gototable() {

}
