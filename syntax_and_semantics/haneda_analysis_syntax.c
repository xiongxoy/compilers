#include "haneda_analysis_syntax.h"

typedef struct S_token_ S_token;
typedef struct lexval {
    string name;
    enum {NM_F, NM_D} type;
    union {
    	int d;
    	double f;
    } num;
} * S_lexval;
struct S_token_ {
	int id;
	enum {
		TERMINAL,
		NON_TERMINAL,
		NUM, /* const */
		NAME
	} type;
	S_lexval val;
};
#define STACK_SIZE 200
typedef struct S_token_statck_{
	S_token contents[STACK_SIZE];
	int size;
} * S_token_stack;
typedef struct S_state_stack_ {
	int contents[STACK_SIZE];
	int size;
} * S_state_stack;

S_token getToken(int token_id) {
	S_token ret = checked_malloc( sizeof *ret );
	ret->id = token_id;
	if(0 == strcmp( "$", tokens_text[token_id])) {
		ret->type = TERMINAL;
		return ret;
	}
	assert(0);
	return NULL;
}

int stack_push( int * size ) {
	(*size) += 1;
	assert(*size <= STACK_SIZE);
	return *size;
}

#define STATE_STACK_TOP  s_stack->contents[s_stack->size-1]
#define TOKEN_STACK_TOP  t_stack->contents[t_stack->size-1]
void syntax() {
	S_token_stack t_stack = checked_malloc( *t_stack );
	t_stack->size = 0;
	for(int i=0; i<STACK_SIZE; i++) t_stack->contents[i] = NULL;
	S_state_stack s_stack = checked_malloc( *s_stack );
	s_stack->size = 0;
	for(int i=0; i<STACK_SIZE; i++) s_stack->contents[i] = -1;

	t_stack->contents[stack_push(&t_stack->size)-1] = getToken( token_find_id("$") ); /* 这样压栈是否可取? */
	s_stack->contents[stack_push(&s_stack->size)-1] = 0;

	start("testprog.c");
	lex();

	int state = -1;
	S_token token = NULL;
	while( TRUE ) {
		state = STATE_STACK_TOP;
		act_type act = action_table[state]->type[TOKEN_STACK_TOP->id];
		if( SHIFT == act ) {
			s_stack->contents[stack_push(&s_stack->size)-1] =  action_table[state]->actions[TOKEN_STACK_TOP->id];
		}
		else if( REDUCE == act ) {

		}
		else if( ACC == act ) {

		}
		else {
			if( GOTO == act ) assert(0); // strange
			if( ERROR == act ) assert(0); // Ooops
		}
	}

	/**
	 *lex的值出来了之后，建立一个相应的S_token，这个作为堆栈里的符号
	 *状态栈里面压的是数字， 然后根据状态栈里的东西，选择action表的项目
	 */

}

int main() {
	haneda_initialize();
	return 0;
}
