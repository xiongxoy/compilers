#include "haneda_syntax.h"

typedef struct S_token_ * S_token;
typedef struct lexval * S_lexval;

struct S_token_ {
	enum {
		S_TERMINAL,
		S_NON_TERMINAL,
		S_CONSTANT, /* const */
		S_IDENTIFIER
	} type;
	int id;
	E_ast ast;
	string code; /* code */
};
#define STACK_SIZE 200
typedef struct S_token_statck_{
	int size;
	S_token contents[STACK_SIZE];
} * S_token_stack;
typedef struct S_state_stack_ {
	int size;
	int contents[STACK_SIZE];
} * S_state_stack;

S_token getToken(int token_id) {
	S_token ret = checked_malloc( sizeof *ret );
	ret->id = token_id;
	if(0 == strcmp("IDENTIFIER", tokens_text[token_id])) {
		ret->type = S_IDENTIFIER;
	}
	else if(0 == strcmp("CONSTANT", tokens_text[token_id])) {
		ret->type = S_CONSTANT;
	}
	else if(TERMINAL == tokens_type[token_id]) {
		ret->type = S_TERMINAL;
	}
	else if(NON_TERMINAL == tokens_type[token_id]) {
		ret->type = S_NON_TERMINAL;
	}
	else {
		assert(0);
	}
	return ret;
}

int _stack_push( int * size ) {
	(*size) += 1;
	assert(*size >0 && *size <= STACK_SIZE);
	return *size;
}


string convert_table[] = {
	"CONSTANT", "STRING_LITERAL", "IDENTIFIER", "INT", "FLOAT", "IF",
	"FOR", "ELSE", "RETURN", "INC_OP", "DEC_OP", "LE_OP", "GE_OP", 0
};

/* return token_id */
int token_lex() {
	int lex_id = lex();
	if( lex_id < 260 ) {
		char a[2];
		a[0] = lex_id;
		a[1] = '\0';
		return token_find_id(a);
	}
	else {
		string s = convert_table[lex_id-L_CONSTANT];
		return token_find_id(s);
	}
}

//#define STATE_STACK_TOP  s_stack->contents[s_stack->size-1]
//#define TOKEN_STACK_TOP  t_stack->contents[t_stack->size-1]
#define stack_top(stack) stack->contents[stack->size-1]
// n represents the number of token from right of the expr, when n == 1, equals stack_top
#define stack_access(stack, n) stack->contents[stack->size-n]
#define $(n) stack_access(t_stack, n)
#define stack_pop(stack) do { stack->size--; assert(stack->size >= 0); } while(0)
#define stack_push(stack, item) stack->contents[_stack_push(&stack->size)-1] = item

void translate( E_ast ast );

void syntax_init() {
	static bool done = FALSE;
	if(done) return;
	lex_init();
	haneda_initialize();
	done = TRUE;
}
void syntax(string filename) {
	S_token_stack t_stack = checked_malloc( sizeof *t_stack );
	t_stack->size = 0;
	for(int i=0; i<STACK_SIZE; i++) t_stack->contents[i] = NULL;
	S_state_stack s_stack = checked_malloc( sizeof *s_stack );
	s_stack->size = 0;
	for(int i=0; i<STACK_SIZE; i++) s_stack->contents[i] = -1;
	//stack_push(t_stack, getToken(token_find_id("$")));
	stack_push(s_stack, 0);

	assert(start(filename));

	int state = -1;
	S_token token = NULL;
	act_type act = -1;
	int act_num = -1;
    token = getToken(token_lex());
	while( TRUE ) {
		state = stack_top(s_stack);
		act_num = action_table[state]->actions[token->id];
		act = action_table[state]->type[token->id];
		if( SHIFT == act ) {
			stack_push(s_stack, act_num);
			stack_push(t_stack, token);
            token = getToken(token_lex());
//        	printf("New TOKEN: %s\n", tokens_text[token->id]);
//        	if(token->type == S_IDENTIFIER) {
//        		printf("NAME: %s\n\n", token->val->s);
//        	}
//        	else {
//        		printf("\n");
//        	}
		}
		else if( REDUCE == act ) {
			S_token head_token = getToken(rules[act_num]->head_token_nr);  // new token, the target

			/* do semantic action here */
//			printf("Token is :%s\n", tokens_text[token->id]);
			printf("Reduce by rule %d : %s\n", act_num, rules_text[act_num]);

			//		"primary_expression: IDENTIFIER ", symref
			if( 1 == act_num ) {
				//head_token->ast = (E_ast)E_newstr(E_SYM_REF, val->s );
				head_token->ast = E_newast(E_SYM_REF, NULL, NULL);
				head_token->ast->s = checked_strdup(val->s);
			}

			//		"primary_expression: CONSTANT ", numval
			if( 2 == act_num ) {
//				head_token->ast = (E_ast)E_newnum(E_NUM, val->s, val->num_t);
				head_token->ast = E_newast(E_NUM, NULL, NULL);
				head_token->ast->s = checked_strdup( val->s );
				head_token->ast->num_t = val->num_t;
			}

			//		"primary_expression: STRING_LITERAL ", strval
			if( 3 == act_num ) {
//				head_token->ast = (E_ast)E_newstr(E_STRING, val->s);
				head_token->ast = E_newast(E_STRING, NULL, NULL);
				head_token->ast->s = checked_strdup( val->s );
			}

			//		"postfix_expression: primary_expression ",
			if( 5 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"postfix_expression: postfix_expression [ expression ] ", E_ARRAY_REF
			if( 6 == act_num ) {
				head_token->ast = E_newast(E_ARRAY_REF, $(4)->ast, $(2)->ast);
			}

			//		"postfix_expression: postfix_expression ( argument_expression_list ) ", E_FUNC_REF
			if( 8 == act_num ) {
				head_token->ast = E_newast(E_FUNC_REF, $(4)->ast, $(2)->ast);
			}

			//		"postfix_expression: postfix_expression INC_OP ",
			if( 11 == act_num ) {
				assert($(2)->ast != NULL);
				head_token->ast = E_newast(E_INC_OP, $(2)->ast, NULL);
			}

			//		"argument_expression_list: assignment_expression ",
			if( 15 == act_num ) {
				head_token->ast = E_newast(E_ARG_END, $(1)->ast, NULL);

			}

			//		"argument_expression_list: argument_expression_list , assignment_expression ",
			if( 16 == act_num ) {
				printf("list.\n");
				head_token->ast = E_newast(E_ARG_LIST, $(3)->ast, $(1)->ast); // TODO is this ok? direction?
				//head_token->ast = E_newast();
			}

			//		"unary_expression: postfix_expression ",
			if( 17 == act_num ) {
				//TODO make new node for processing
				head_token->ast = $(1)->ast;
			}

			//		"unary_expression: unary_operator cast_expression ",
			if( 20 == act_num ) {
				head_token->ast = E_newast(E_UNARY_EXP, $(2)->ast, $(1)->ast);
			}

			//		"unary_operator: & ",
			if( 21 == act_num ) {
				head_token->ast = E_newast(E_TAKE_ADDR, NULL, NULL);
			}

			//		"cast_expression: unary_expression ",
			if( 27 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"multiplicative_expression: cast_expression ",
			if( 29 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"multiplicative_expression: multiplicative_expression * cast_expression ",
			if( 30 == act_num ) {
				head_token->ast = E_newast(E_MUL, $(3)->ast, $(1)->ast);
			}

			//		"multiplicative_expression: multiplicative_expression / cast_expression ",
			if( 31 == act_num ) {
				head_token->ast = E_newast(E_DIV, $(3)->ast, $(1)->ast);
			}

			//		"additive_expression: multiplicative_expression ",
			if( 33 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"additive_expression: additive_expression + multiplicative_expression ",
			if( 34 == act_num ) {
				head_token->ast = E_newast(E_ADD, $(3)->ast, $(1)->ast);
//				head_token->ast->pending = TRUE;
			}

			//		"additive_expression: additive_expression - multiplicative_expression ",
			if( 35 == act_num ) {
				head_token->ast = E_newast(E_MINUS, $(3)->ast, $(1)->ast);
			}

			//		"shift_expression: additive_expression ",
			if( 36 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"relational_expression: shift_expression ",
			if( 39 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"relational_expression: relational_expression < shift_expression ",
			if( 40 == act_num ) {
				head_token->ast = E_newast(E_REL_L, $(3)->ast, $(1)->ast);
			}

			//		"relational_expression: relational_expression GE_OP shift_expression ",
			if( 43 == act_num ) {
				head_token->ast = E_newast(E_REL_GE, $(3)->ast, $(1)->ast);
			}

			//		"equality_expression: relational_expression ",
			if( 44 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"and_expression: equality_expression ",
			if( 47 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"exclusive_or_expression: and_expression ",
			if( 49 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"inclusive_or_expression: exclusive_or_expression ",
			if( 51 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"logical_and_expression: inclusive_or_expression ",
			if( 53 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"logical_or_expression: logical_and_expression ",
			if( 55 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"conditional_expression: logical_or_expression ",
			if( 57 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"assignment_expression: conditional_expression ",
			if( 59 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"assignment_expression: unary_expression assignment_operator assignment_expression ",
			if( 60 == act_num ) {
				head_token->ast = E_newast($(2)->ast->type, $(3)->ast, $(1)->ast);
			}

			//		"assignment_operator: = ",
			if( 61 == act_num ) {
//				head_token->ast = E_newsinglet(E_ASSIGN_EQL);
				head_token->ast = E_newast(E_ASSIGN_EQL, NULL, NULL);
			}

			//		"expression: assignment_expression ",
			if( 71 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"declaration_specifiers: type_specifier ",
			if( 74 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"declaration: declaration_specifiers init_declarator_list ; ",
			if( 77 == act_num ) {
				head_token->ast = E_newast(E_DECLARATION, $(3)->ast, $(2)->ast);

			}

			//		"init_declarator_list: init_declarator ",
			if( 78 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"init_declarator_list: init_declarator_list , init_declarator ",
			if( 79 == act_num ) {
			}

			//		"init_declarator: declarator ",
			if( 80 == act_num ) {
				//TODO make new node for processing
				head_token->ast = $(1)->ast;
			}

			//		"init_declarator: declarator = initializer ",
			if( 81 == act_num ) {
				head_token->ast = E_newast(E_INIT_DECL, $(3)->ast, $(1)->ast);
			}

			//		"type_specifier: INT ",
			if( 82 == act_num ) {
				head_token->ast = E_newast(E_SPEC, NULL, NULL);
				head_token->ast->m.ch = 0;
				head_token->ast->m.bit.b_int = 1;
//				head_token->ast = E_newspec(E_SPEC);
//				((E_spec)(head_token->ast))->m.bit.b_int = 1;
//				head_token->id = ;
//				head_token->val
			}

			//		"type_specifier: FLOAT ",
			if( 83 == act_num ) { //TODO do real float
				head_token->ast = E_newast(E_SPEC, NULL, NULL);
				head_token->ast->m.ch = 0;
				head_token->ast->m.bit.b_int = 1;
			}

			//		"declarator: direct_declarator ",
			if( 84 == act_num ) {
				head_token->ast = E_newast(E_DECLAR_STORAGE, $(1)->ast, NULL);
			}

			//		"direct_declarator: IDENTIFIER ",
			if( 85 == act_num ) {
//				head_token->ast = (E_ast)E_newstr(E_SYM, val->s);
				head_token->ast = E_newast(E_SYM, NULL, NULL);
				head_token->ast->s = checked_strdup( val->s );
				printf("new id: %s\n", val->s);
				//assert(0);
			}

			//		"direct_declarator: direct_declarator [ assignment_expression ] ", // E_ARRAY
			if( 87 == act_num ) {
				head_token->ast = E_newast(E_ARRAY_BODY, $(4)->ast, $(2)->ast);
//				head_token->ast->pending = TRUE;
			}

			//		"direct_declarator: direct_declarator ( parameter_type_list ) ",
			if( 90 == act_num ) {
				head_token->ast = E_newast(E_FUNC_BODY, $(4)->ast, $(2)->ast);
//				$(4)->ast->isfunc = TRUE;
//				head_token->ast->pending = TRUE;
			}

			//		"direct_declarator: direct_declarator ( ) ",
			if( 92 == act_num ) {
				head_token->ast = $(3)->ast;
//				head_token->ast = E_newast(E_DECLAR_LIST, $(3)->ast, NULL);
			}

			//		"parameter_type_list: parameter_list ",
			if( 93 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"parameter_list: parameter_declaration ",
			if( 95 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"parameter_list: parameter_list , parameter_declaration ",
			if( 96 == act_num ) {
				assert(0);
			}

			//		"parameter_declaration: declaration_specifiers declarator ",
			if( 97 == act_num ) {
				assert(0);
			}

			//		"initializer: assignment_expression ",
			if( 114 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"initializer: { initializer_list } ",
			if( 115 == act_num ) {
				head_token->ast = $(2)->ast;
			}

			//		"initializer_list: initializer ",
			if( 117 == act_num ) {
				head_token->ast = E_newast(E_INIT_VAL_LIST, NULL, $(1)->ast);
			}

			//		"initializer_list: initializer_list , initializer ",
			if( 119 == act_num ) {
				head_token->ast = E_newast(E_INIT_VAL_LIST, $(3)->ast, $(1)->ast);
			}

			//		"statement: compound_statement ",
			if( 126 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"statement: expression_statement ",
			if( 127 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"statement: selection_statement ",
			if( 128 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"statement: iteration_statement ",
			if( 129 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"statement: jump_statement ",
			if( 130 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"compound_statement: { block_item_list } ",
			if( 132 == act_num ) {
				head_token->ast = $(2)->ast;
			}

			//		"block_item_list: block_item ",
			if( 133 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"block_item_list: block_item_list block_item ",
			if( 134 == act_num ) {
				head_token->ast = (E_ast)E_newast(E_BLK_ITM_LIST, $(2)->ast, $(1)->ast);
			}

			//		"block_item: declaration ",
			if( 135 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"block_item: statement ",
			if( 136 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"expression_statement: expression ; ",
			if( 138 == act_num ) {
				head_token->ast = $(2)->ast;
			}

			//		"selection_statement: IF ( expression ) statement ELSE statement ",
			if( 140 == act_num ) {
				head_token->ast = E_newast(E_CONT_IF, NULL, NULL);
				head_token->ast->bool_exp = $(5)->ast;
				head_token->ast->true_stat = $(3)->ast;
				head_token->ast->false_stat = $(1)->ast;
			}

			//		"iteration_statement: FOR ( expression_statement expression_statement expression ) statement ",
			if( 144 == act_num ) {
				head_token->ast = E_newast(E_CONT_FOR, NULL, NULL);
				head_token->ast->init_stat = $(5)->ast;
				head_token->ast->bool_exp = $(4)->ast;
				head_token->ast->step_state = $(3)->ast;
				head_token->ast->ast = $(1)->ast;
			}

			//		"jump_statement: RETURN expression ; ",
			if( 149 == act_num ) {
				head_token->ast = E_newast(E_RET, $(3)->ast, NULL);
			}

			//		"translation_unit: external_declaration ",
			if( 150 == act_num ) {
//				assert($(1));
				head_token->ast = $(1)->ast;
			}

			//		"translation_unit: translation_unit external_declaration ",
			if( 151 == act_num ) {
				assert(0);
			}

			//		"external_declaration: function_definition ",
			if( 152 == act_num ) {
				assert( $(1) );
				head_token->ast = $(1)->ast;
			}

			//		"function_definition: declaration_specifiers declarator compound_statement ",
			if( 155 == act_num ) {
//				head_token->ast = E_newast(E_FUNC, $(3)->ast, $(2)->ast, $(1)->ast);
				head_token->ast = E_newast(E_FUNC, NULL, NULL);
				head_token->ast->spcifiers = $(3)->ast;
				head_token->ast->declarator = $(2)->ast;
				head_token->ast->ast = $(1)->ast;
			}
			/* end */

			/*** clear up ***/
			C_token_list head = NULL;
			for( head = rules[act_num]->list; head; head = head->next ) {
				stack_pop(t_stack);
				stack_pop(s_stack);
			}
			state = stack_top(s_stack);
			act_num = action_table[state]->actions[head_token->id];
			act = action_table[state]->type[head_token->id];
			assert( GOTO == act );

			assert(head_token->ast);
			stack_push(t_stack, head_token);
			stack_push(s_stack, act_num);
		}
		else if( ACC == act ) {
			assert( $(1) );
			S_token s = checked_malloc( sizeof *s );
			s->ast = $(1)->ast;

			/*** translate begins ***/
		    assert( NULL == E_lookup("printf") && NULL == E_lookup("scanf") );
			E_symbol sym = E_install("printf");
			sym->addr = sym->name;
			sym = E_install("scanf");
			sym->addr = sym->name;
			translate(s->ast);
//			printf("body is here: \n%s", s->ast->code);
			string result = checked_malloc( strlen($(1)->ast->code) + 500);
			sprintf( result, 	"\t.file	\"simple1.c\"\n"
								"\t.text\n"
								"%s"
					            "\t.ident	\"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3\"\n"
	                            "\t.section\t.note.GNU-stack,\"\",@progbits\n" , s->ast->code);
			printf("LAST !!!\n");
			printf("%s", result);
			FILE * file_out = fopen("result.s", "w");
			fprintf(file_out, "%s", result);
			fclose(file_out);
			printf("Yahoo!!!\n");
			return;
		}
		else {
			if( GOTO == act ) assert(0); // strange
			if( ERROR == act ) {
				printf("STATE: %d\n", stack_top(s_stack));
				printf("Token is: %s\n", tokens_text[stack_top(t_stack)->id]);
				assert(0); // Ooops
			}
			assert(0); // WTF?
		}
	}
	/**
	 *lex的值出来了之后，建立一个相应的S_token，这个作为堆栈里的符号
	 *状态栈里面压的是数字， 然后根据状态栈里的东西，选择action表的项目
	 */
}

string node_texts[] = {
		"E_PASS",
		"E_SYM" ,  // 0
		"E_SYM_REF",  // 0
		"E_FUNC",  // 3
		"E_FUNC_REF",
		"E_NUM",   // 0
		"E_STRING",  // 0
		"E_ADD",   // 2
		"E_ASSIGN_EQL",  // 2
		"E_DECLARATION",  // 2
		"E_BLK_ITM_LIST", // 2
		"E_INIT_DECL_LIST",
		"E_DECLAR_LIST",
		"E_SPEC",
		//"E_SPEC_LIST",
		"E_INT",  // 0
		"E_FLOAT", // 0
		"E_ARRAY",
};

void traverse( E_ast ast ) {

}
int local_val = 0; // the space used by local val
int temp_val = 0; // the space used by temp val
int temp_max = 0;

int str_count = 0; // internal count
int func_count = 0;
int param_count = 0;
int param_max = 0;
int label_count = 0;
char local_buffer[1000] = {0,};

string E_newLabel() {
	string ret = checked_malloc(10);
	sprintf(ret, ".L%d", label_count++);
	return ret;
}
string E_newTemp() {
	string ret = checked_malloc(10);
	sprintf(ret, "-%d(%%ebp)", local_val + temp_val );
	temp_val += 4;
	return ret;
}
void translate( E_ast ast ) {
	int base = 0;
	if ( NULL == ast ) {
		assert(0);
		return;
	}
	E_symbol sym;
	string label_true;
	string false_label;
	string end_label;
	string begin_label;
	string left_addr;

	switch(ast->type) {
	case E_RET:
		return;
	case E_UNARY_EXP:
//		translate(ast->l);
		translate(ast->r);
		switch(ast->l->type) {
		case E_TAKE_ADDR:
			ast->addr = E_newTemp();
			ast->code = checked_malloc( strlen(ast->r->code) + 100 );
			sprintf(ast->code, "%s"
							   "\tleal %s, %%eax\n"
							   "\tmovl %%eax, %s\n",
							   ast->r->code,
							   ast->r->addr,
							   ast->addr);
			ast->var_t = E_POINTER;
			return;
		default:
			assert(0);
			return;
		}
		return;
	case E_INIT_DECL: 	//	"init_declarator: declarator = initializer ",
		ast->l->var_t = ast->var_t;
		translate(ast->l);
		printf("after left %s\n", ast->l->addr);
		if( ast->r != NULL )
			translate(ast->r);
		else
			assert(0);
		sym = E_lookup(ast->l->s);
		assert(sym);
		ast->code = checked_malloc( strlen( ast->l->code ) + strlen( ast->r->code ) + 500 );
		if ( E_VAR_SYM == sym->symtype) {
			sprintf( ast->code, "\tmovl %s, %%eax\n"
								"\tmovl %%eax, %s\n",
								ast->r->addr,
								ast->l->addr);
			ast->addr = ast->l->addr;
		}
		else if ( E_ARRAY_SYM == sym->symtype ) {
			assert( E_INIT_VAL_LIST == ast->r->type );
			U_list head = ast->r->list;
			int val = 0;
			int count = ast->r->list_c;
			base = 0;
			sscanf(ast->l->addr, "-%d(%%ebp)", &base);
			for(U_list p=head; p; p=p->next) {
				printf("addr is %s\n", (string)p->val.addr);
				sscanf((string)p->val.addr, "$%d", &val);
				printf("get number %d\n", val);
				sprintf(ast->code+ strlen(ast->code),  "\tmovl %s, -%d(%%ebp)\n",
												       (string)p->val.addr,
												       base + 4 * (ast->r->list_c-count));
				count--;
			}
			assert(count == 0);
		}
		else {
			assert(0);
		}
		return;
	case E_ARRAY_REF:
		//		"postfix_expression: postfix_expression [ expression ] ", E_ARRAY_REF
		translate(ast->l);
		sym = E_lookup(ast->l->s);
		assert(sym);
		sscanf(ast->l->addr, "-%d(%%ebp)", &base);
		base = base + (sym->width-1) * 4;
		ast->addr = checked_malloc(40);
		sprintf(ast->addr, "-%d(%%ebp,%%ebx,4)", base);
		translate(ast->r);
		ast->code = checked_malloc( strlen(ast->l->code) + strlen(ast->r->code) + 200);
		sprintf(ast->code, "%s%s"
						   "\tmovl %s, %%ebx\n",
						   ast->r->code,
						   ast->l->code,
						   ast->r->addr);
		ast->var_t = E_INT_TYPE; //FIXME latter
		return;
//TODO support high dimensinal array
//		list_insert(&ast->list, &ast->r->addr, ADDRESS);
//		if(ast->l->type == E_SYM_REF) {
//			translate(ast->l);
//			if(E_INT_TYPE == ast->var_t) {
//			}
//			else {
//				assert(0);
//			}
//		}
//		else if(ast->l->type == E_ARRAY_REF) {
//			ast->l->var_t = ast->var_t;
//			ast->l->list = ast->list;
//			translate(ast->l);
//		}
//		else
//			assert(0);
		return;
	case E_INIT_VAL_LIST: // "initializer_list: initializer_list , initializer ",
		if(NULL == ast->l) { // in fact, this is unnecessary
			translate(ast->r);
			ast->list = NULL;
			ast->list_c = 1;
		    list_insert(&ast->list, &ast->r->addr, ADDRESS);
			return;
		}
		translate(ast->l);
		translate(ast->r);
		ast->list = ast->l->list;
		ast->list_c = ast->l->list_c+1;
		list_insert(&ast->list, &ast->r->addr, ADDRESS);
		return;
	case E_DECLAR_STORAGE:

		ast->l->var_t = ast->var_t;
		ast->l->isarray = ast->isarray;
		ast->l->isfunc = ast->isfunc;
		translate(ast->l);
		ast->s = ast->l->s;
		ast->addr = ast->l->addr;
		if( E_ARRAY_BODY == ast->l->type) {
			U_list head = ast->l->list;
			int width = 0;
			int count = ast->l->list_c;
			assert(count == 1);
			for(U_list p=head; p; p=p->next) {
				printf("addr is %s\n", (string)p->val.addr);
				sscanf((string)p->val.addr, "$%d", &width);
				printf("get number %d\n", width);
				count--;
			}
			assert(count == 0);
			if( E_INT_TYPE == ast->var_t)
				local_val += width * 4;
			else
				assert(0);
		}
		else {
//			ast = ast->l;
		}
		return;
	case E_ARRAY_BODY:
		//  "direct_declarator: direct_declarator [ assignment_expression ] ",  E_ARRAY
		translate(ast->r);
		list_insert(&ast->list, &ast->r->addr, ADDRESS);
//		if(ast->l->type == E_SYM) {
		ast->l->var_t = ast->var_t;
		ast->l->list = ast->list;
		ast->l->isarray = TRUE;
		translate(ast->l);
		ast->list_c = ast->l->list_c + 1;
		ast->code = ast->l->code;  // TODO merge code to empower assignment_exp
		ast->addr = ast->l->addr;
		ast->s = ast->l->s;
		sym = E_lookup(ast->s);
		assert(sym);
		sscanf(ast->r->addr, "$%d", &sym->width);
//		}
//		else
//			assert(0);
		return;
	case E_INC_OP:
		translate(ast->l);
		ast->code = checked_malloc( strlen(ast->l->code) + 100 );
		ast->addr = ast->l->addr;
		ast->var_t = ast->l->var_t;
		sprintf(ast->code, "%s"
						   "\taddl $1, %s\n",
						   ast->l->code,
						   ast->l->addr);
		return;
	case E_CONT_FOR:
		// TODO enter scope
		translate(ast->init_stat);
		translate(ast->bool_exp);
		translate(ast->step_state);
		translate(ast->ast);
		// exit scope

		ast->code = checked_malloc( strlen(ast->init_stat->code) +
									strlen(ast->bool_exp->code) + strlen(ast->bool_exp->addr) +
									strlen(ast->ast->code) + strlen(ast->step_state->code) + 200);
		begin_label = E_newLabel();
		end_label = E_newLabel();
		sprintf(ast->code, "%s"
						   "%s:\n" // begin label
				           "%s" // bool_exp
						   "\tcmpl $1, %s\n"
		    	           "\tjne  %s\n"
						   "%s" // body
						   "%s" // step
						   "\tjmp %s\n" // jmp begin
						   "%s:\n",  //end label
						  ast->init_stat->code,
						  begin_label,
						  ast->bool_exp->code,
						  ast->bool_exp->addr,
						  end_label,
						  ast->ast->code,
						  ast->step_state->code,
						  begin_label,
						  end_label);
		return;
	case E_REL_L:
		translate(ast->l);
		translate(ast->r);
		ast->code = checked_malloc( strlen(ast->l->code) + strlen(ast->r->code) +
									strlen(ast->l->addr) + strlen(ast->r->addr) +
									200);
		label_true = checked_malloc(10);
		sprintf(label_true, ".L%d", label_count++);
		ast->addr = checked_malloc(15);
		sprintf(ast->addr, "-%d(%%ebp)", local_val + temp_val);
		temp_val += 4;
		sprintf(ast->code, "%s%s"
						   "\tmovl $1, %s\n"
				  	  	   "\tmovl %s, %%eax\n"
				           "\tcmpl %%eax, %s\n"
						   "\tjl %s\n"
						   "\tmovl $0, %s\n"
						   "%s:\n",
						   ast->l->code,
						   ast->r->code,
						   ast->addr,
						   ast->r->addr,
						   ast->l->addr,
						   label_true,
						   ast->addr,
						   label_true);
		ast->var_t = E_INT_TYPE;
		return;
	case E_REL_GE:  // 	    "relational_expression: relational_expression GE_OP shift_expression "
		translate(ast->l);
		translate(ast->r);
		ast->code = checked_malloc( strlen(ast->l->code) + strlen(ast->r->code) +
									strlen(ast->l->addr) + strlen(ast->r->addr) +
									200);
		label_true = checked_malloc(10);
		sprintf(label_true, ".L%d", label_count++);
		ast->addr = checked_malloc(15);
		sprintf(ast->addr, "-%d(%%ebp)", local_val + temp_val);
		temp_val += 4;
		sprintf(ast->code, "%s%s"
						   "\tmovl $1, %s\n"
				  	  	   "\tmovl %s, %%eax\n"
				           "\tcmpl %s, %%eax\n"
						   "\tjl %s\n"
						   "\tmovl $0, %s\n"
						   "%s:\n",
						   ast->l->code,
						   ast->r->code,
						   ast->addr,
						   ast->r->addr,
						   ast->l->addr,
						   label_true,
						   ast->addr,
						   label_true);
		ast->var_t = E_INT_TYPE;
		return;
	case E_CONT_IF: //		"selection_statement: IF ( expression ) statement ELSE statement ",
		translate(ast->bool_exp);  // FIXME for now, only consider if statement like if ( a < b) ,
		translate(ast->true_stat);
		translate(ast->false_stat);
		ast->code = checked_malloc( strlen(ast->bool_exp->code)  +
									strlen(ast->bool_exp->addr)  +
									strlen(ast->true_stat->code) +
									strlen(ast->false_stat->code) + 200 );
		false_label = checked_malloc(10);
		sprintf(false_label, ".L%d", label_count++);
		end_label = checked_malloc(10);
		sprintf(end_label, ".L%d", label_count++);
		sprintf(ast->code, "%s"
						   "\tcmpl $1, %s\n"
				    	   "\tjne  %s\n"
				           "%s"   // true_state
						   "\tjmp %s\n"  // end
				           "%s:\n"  // false_label
						   "%s"
						   "%s:\n", // end_label,
						   ast->bool_exp->code,
						   ast->bool_exp->addr,
						   false_label,
						   ast->true_stat->code,
						   end_label,
						   false_label,
						   ast->false_stat->code,
						   end_label
						   );
		return;
	case E_ARG_END:
		translate(ast->l);
		ast->code = checked_malloc( strlen(ast->l->code) + 50);
		sprintf(ast->code, "%s"
						   "\tmovl %s, %%eax\n"
						   "\tmovl %%eax, %d(%%esp)\n",
						   ast->l->code,
						   ast->l->addr,
						   param_count);
		if( E_INT_TYPE == ast->l->var_t || E_STRING_LIETRAL_TYPE == ast->l->var_t ) {
			param_count+=4;
		}
		else {
			printf( "type not correct %d\n", ast->l->var_t);
			assert(0);
		}
		return;
	case E_ARG_LIST:
		translate(ast->l);
		translate(ast->r);
		ast->code = checked_malloc( strlen(ast->r->code) + strlen(ast->l->code) + strlen(ast->r->addr) + 100);
		sprintf(ast->code, "%s"
						   "\tmovl %s, %%eax\n"
						   "\tmovl %%eax, %d(%%esp)\n"
						   "%s",
						   ast->r->code,
						   ast->r->addr,
						   param_count,
						   ast->l->code);
		if( E_INT_TYPE == ast->r->var_t || E_STRING_LIETRAL_TYPE == ast->r->var_t || E_POINTER == ast->r->var_t ) {
			param_count+=4;
		}
		else {
			assert(0);
		}
		return;
	case E_FUNC_REF:
		//		"postfix_expression: postfix_expression ( argument_expression_list ) ", E_FUNC_REF
		translate(ast->l);
		param_count = 0;
		translate(ast->r);
		param_max = param_count > param_max? param_count : param_max;
		if( (sym = E_lookup(ast->l->s)) == NULL ) {
			assert(0);
		}
		ast->code = checked_malloc( strlen(ast->r->code) + 100 );
		sprintf(ast->code, "%s"
						   "\tcall	%s\n",
						   ast->r->code, ast->l->addr);
		return;
	case E_SPEC:
//		spec_node = (E_spec)ast;
		assert( ast->m.bit.b_int == 1);
		printf("type: %s\n", node_texts[ast->type]);
		return;
	case E_SYM:  // id, need an adress,
//		ast = (E_str)ast;
		printf("type: %s\n", node_texts[ast->type]);
		printf("%s\n", ast->s);
		assert( NULL == E_lookup(ast->s) );
		sym = E_install(ast->s);

		if( E_INT_TYPE == ast->var_t) {
			sym->symtype = E_VAR_SYM;
			sym->var_t = E_INT_TYPE;
			sym->addr = checked_malloc( 100 );
			sprintf(sym->addr, "-%d(%%ebp)", local_val);
			local_val += 4; // FIXME RISKY global is bad.
			ast->addr = sym->addr; // RISKY is this ok?
		}
		else if (E_FLOAT_TYPE == ast->var_t) {
			assert(0);
		}
		else if (E_NONE_TYPE == ast->var_t) {
			assert(0);
		}
		/*** deal with functions ***/
		if( ast->isfunc ) {
			sym->symtype = E_FUNC_SYM;
			sym->addr = checked_malloc( 100 );
//			sym->isfunc = TRUE;
			sprintf(sym->addr, "%s", ast->s);
			ast->addr = sym->addr;
		}
		/*** deal with arrays ***/
		if( ast->isarray) {
			sym->symtype = E_ARRAY_SYM;
			ast->list_c = 0;
			ast->list = NULL;
			ast->addr = sym->addr;
		}
		assert( sym->var_t == E_INT_TYPE );
		printf("sym addr is %s\n", ast->addr);
		return;
	case E_NUM:  // const num
//		ast = (E_num)ast;
		printf("type: %s\n", node_texts[ast->type]);
		printf("%s\n", ast->s);
		if( NULL == E_lookup( ast->s )) {
			sym = E_install( ast->s );
			sym->symtype = E_CONST_SYM;
			if(ast->num_t == NM_I)
				sym->var_t = E_INT_TYPE;
			else if(ast->num_t == NM_F)
				sym->var_t = E_FLOAT_TYPE;
			sym->addr = checked_malloc(strlen(sym->name) + 2);
			sprintf(sym->addr, "$%s", sym->name);
//			sym->addr = sym->name;
		} else {
			sym = E_lookup( ast->s );
		}
		ast->addr = sym->addr;
		ast->var_t = sym->var_t;
		//TODO set the raw number as the addr
		return;
	case E_STRING:
//		ast = r)ast;
		printf("type: %s\n", node_texts[ast->type]);
		printf("%s\n", ast->s);
		if( NULL == E_lookup( ast->s )) {
			sym = E_install( ast->s );
			sym->symtype = E_CONST_SYM;
		    sym->var_t = E_STRING_LIETRAL_TYPE;
		    sym->addr = checked_malloc( 10 );
	     	sprintf(sym->addr, "$.LC%d", str_count); // FIXME global..
	     	sym->label = checked_malloc( 10 );
	     	sprintf(sym->label, ".LC%d", str_count++); // FIXME global..
		    sprintf(local_buffer+strlen(local_buffer), "\t.align 4\n%s:\n\t.string \"%s\"\n", sym->label, ast->s); // FIXME not so simple
		} else {
			sym = E_lookup( ast->s );
		}
		ast->addr = sym->addr;
		ast->var_t = sym->var_t;
		return;
	case E_SYM_REF:
//		ast = (E_str)ast;
		printf("type: %s\n", node_texts[ast->type]);
		printf("%s\n", ast->s);
		sym = E_lookup( ast->s );
		assert(NULL != sym);
		ast->addr = sym->addr;
		ast->var_t = sym->var_t;
		return;
//		"additive_expression: additive_expression + multiplicative_expression ",
	case E_DIV:
		translate(ast->l);
		translate(ast->r);
		if ( E_INT_TYPE == ast->l->var_t && E_INT_TYPE == ast->r->var_t ) {
			ast->var_t = E_INT_TYPE;
		}
		else {
			assert(0);
		}
		ast->code = checked_malloc( strlen( ast->l->code ) + strlen( ast->l->code ) +
									strlen( ast->r->addr) + strlen( ast->r->addr) + 100);
		sprintf( ast->code, "%s%s"
				            "\tmovl %s, %%eax\n"
							"\tmovl $0, %%edx\n"
							"\tmovl %s, %%ecx\n"
							"\tidiv %%ecx\n",
							ast->l->code, ast->r->code,
							ast->l->addr, ast->r->addr);
		ast->addr = (char *) checked_malloc( 20 );
		sprintf( ast->addr, "-%d(%%ebp)", local_val+temp_val);
		if (ast->var_t == E_INT_TYPE) // FIXME deal with type latter
			temp_val+=4;
		else
			assert(0);
		sprintf( ast->code, "%s\tmovl %%eax, %s\n", ast->code, ast->addr); // FIXME sprintf(ast->code + strlen(ast->len), ast->addr)
		return;
	case E_MUL:
		translate(ast->l);
		translate(ast->r);
		if ( E_INT_TYPE == ast->l->var_t && E_INT_TYPE == ast->r->var_t ) {
			ast->var_t = E_INT_TYPE;
		}
		else {
			assert(0);
		}
		left_addr = E_newTemp();
		ast->code = checked_malloc( strlen( ast->l->code ) + strlen( ast->l->code ) +
									strlen( ast->r->addr) + strlen( ast->r->addr) + 100);
		sprintf( ast->code, "%s"
							"\tmovl %s, %%eax\n"
							"\tmovl %%eax, %s\n"
							"%s"
							"\tmovl %s, %%eax\n"
							"\timull %s, %%eax\n",
							ast->l->code,
							ast->l->addr,
							left_addr,
							ast->r->code,
							ast->r->addr,
							left_addr );
		ast->addr = (char *) checked_malloc( 20 );
		sprintf( ast->addr, "-%d(%%ebp)", local_val+temp_val);
		if (ast->var_t == E_INT_TYPE) // FIXME deal with type latter
			temp_val+=4;
		else
			assert(0);
		sprintf( ast->code, "%s\tmovl %%eax, %s\n", ast->code, ast->addr); // FIXME sprintf(ast->code + strlen(ast->len), ast->addr)
		return;
	case E_MINUS:
		translate(ast->l);
		translate(ast->r);
		if ( E_INT_TYPE == ast->l->var_t && E_INT_TYPE == ast->r->var_t ) {
			ast->var_t = E_INT_TYPE;
		}
		else {
			assert(0);
		}
		ast->code = checked_malloc( strlen( ast->l->code ) + strlen( ast->l->code ) +
									strlen( ast->r->addr) + strlen( ast->r->addr) + 100);
		sprintf( ast->code, "%s%s\tmovl %s, %%eax\n\tsubl %s, %%eax\n",
				 ast->l->code, ast->r->code, ast->l->addr, ast->r->addr);
		ast->addr = (char *) checked_malloc( 20 );
		sprintf( ast->addr, "-%d(%%ebp)", local_val+temp_val);
		if (ast->var_t == E_INT_TYPE) // FIXME deal with type latter
			temp_val+=4;
		else
			assert(0);
		sprintf( ast->code, "%s\tmovl %%eax, %s\n", ast->code, ast->addr); // FIXME sprintf(ast->code + strlen(ast->len), ast->addr)
		return;
	case E_ADD:
		translate(ast->l);
		translate(ast->r);
		if ( E_INT_TYPE == ast->l->var_t && E_INT_TYPE == ast->r->var_t ) {
			ast->var_t = E_INT_TYPE;
		}
		else {
			assert(0);
		}
		ast->code = checked_malloc( strlen( ast->l->code ) + strlen( ast->l->addr ) +
									strlen( ast->r->code) + strlen( ast->r->addr) + 100);
		left_addr = E_newTemp();
		sprintf( ast->code, "%s"
//							"aaa\n"
							"\tmovl %s, %%eax\n"
							"\tmovl %%eax, %s\n"
//							"ddd\n"
							"%s"
							"\tmovl %s, %%eax\n"
							"\taddl %s, %%eax\n",
							ast->l->code,
							ast->l->addr,
							left_addr,
							ast->r->code,
							ast->r->addr,
							left_addr
							);
		ast->addr = (char *) checked_malloc( 20 );
		sprintf( ast->addr, "-%d(%%ebp)", local_val+temp_val);
		if (ast->var_t == E_INT_TYPE) // FIXME deal with type latter
			temp_val+=4;
		else
			assert(0);
		sprintf( ast->code, "%s\tmovl %%eax, %s\n", ast->code, ast->addr);
		return;
	case E_ASSIGN_EQL: //		"assignment_expression: unary_expression assignment_operator assignment_expression ",
		translate(ast->r);
		translate(ast->l);
		assert(ast->l->code && ast->l->code && ast->r->code && ast->r->code );
		printf("request %d\n", strlen( ast->l->code ) + strlen( ast->r->code ) +
							   strlen( ast->r->addr ) + strlen( ast->l->addr ) + 100);
		ast->code = checked_malloc( strlen( ast->l->code ) + strlen( ast->r->code ) +
									strlen( ast->r->addr ) + strlen( ast->l->addr ) + 100);
		sprintf( ast->code, "%s%s\tmovl %s, %%eax\n\tmovl %%eax, %s\n",
				 ast->l->code,
				 ast->r->code,
				 ast->r->addr,
				 ast->l->addr);
		ast->addr = checked_malloc( 20 );
		sprintf( ast->addr, "%s", ast->l->code);
		return;
	case E_DECLARATION: // "declaration: declaration_specifiers init_declarator_list ; "
		translate(ast->l);
		assert(ast->l->type == E_SPEC);
		if( 1 == ast->l->m.bit.b_int ) {
			ast->r->var_t = E_INT_TYPE;
		} else {
			printf("type is %x.\n", ast->l->m.ch );
			assert(0);
		}
		translate(ast->r);
		assert(ast->r->code);
		ast->code = checked_malloc( strlen(ast->r->code)+ 10 );
		ast->code = ast->r->code; // RISKY OK?
//		sprintf(ast->code, "%s%s", ast->code?ast->code:"", ast->r->code?ast->r->code:"");
		return;
	case E_INIT_DECL_LIST: 	// "init_declarator_list: init_declarator_list , init_declarator ",
		ast->l->var_t = ast->r->var_t = ast->var_t;
		translate(ast->l);
		translate(ast->r);
		printf("type: %s\n", node_texts[ast->type]);
		assert(ast->l->code && ast->r->code);
		ast->code = checked_malloc( strlen(ast->l->code) + strlen(ast->r->code) + 10);
		sprintf( ast->code, "%s%s", ast->l->code, ast->r->code);
		return;
	case E_BLK_ITM_LIST:
		translate(ast->l);
		//TODO enter scope
		translate(ast->r);
		//TODO exit scope
		printf("type: %s\n", node_texts[ast->type]);
		ast->code = checked_malloc( strlen(ast->l->code) + strlen(ast->r->code) + 10);
		sprintf( ast->code, "%s%s", ast->l->code, ast->r->code);
		return;
	case E_FUNC:
//		ast = (E_func)ast;
		translate(ast->spcifiers);
		ast->declarator->isfunc = TRUE;
		if( 1 == ast->spcifiers->m.bit.b_int )
			ast->declarator->var_t = E_INT_TYPE;
		translate(ast->declarator);
		translate(ast->ast);
		printf("type: %s\n", node_texts[ast->type]);
		ast->code = checked_malloc( strlen(ast->declarator->code) +
									strlen(ast->ast->code) + 800 );

		sprintf(ast->code, "\t.section\t.rodata\n"
						   "%s"
						   "\t.global %s\n"
						   "\t.type %s, @function\n"
						   "\t%s:\n"
						   ".LFB%d:\n"
				    	   "\t.cfi_startproc\n"
	             	 	   "\tpushl	%%ebp\n"
						   "\t.cfi_def_cfa_offset 8\n"
                  	  	   "\t.cfi_offset 5, -8\n"
                           "\tmovl %%esp, %%ebp\n"
                		   "\t.cfi_def_cfa_register 5\n"
//						   "\tandl $-16, %%esp\n"
						   "\tsubl $%d, %%esp\n"
	                       "%s"
						   "\tleave\n"
                           "\t.cfi_restore 5\n"
				           "\t.cfi_def_cfa 4, 4\n"
				 	 	   "\tret\n"
				 	 	   "\t.cfi_endproc\n"
						   ".LBE%d:"
						   "\t.size %s, .-%s\n",
						   local_buffer,
						   ast->declarator->addr,
						   ast->declarator->addr,
						   ast->declarator->addr,
						   func_count,
						   local_val+temp_val+param_max,
						   ast->ast->code,
						   func_count,
						   ast->declarator->addr,
						   ast->declarator->addr);
		sprintf(local_buffer, "%s", "");
//		printf("%s",ast->code);
		return;
	default:
		assert(0);
		printf("strange type: %s\n", node_texts[ast->type]);
		return;
	}
}

