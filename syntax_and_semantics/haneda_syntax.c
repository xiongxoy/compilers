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
				head_token->ast = (E_ast)E_newstr(E_SYM_REF, val->s );
			}

			//		"primary_expression: CONSTANT ", numval
			if( 2 == act_num ) {
				head_token->ast = (E_ast)E_newnum(E_NUM, val->s, val->num_t);
			}

			//		"primary_expression: STRING_LITERAL ", strval
			if( 3 == act_num ) {
				head_token->ast = (E_ast)E_newstr(E_STRING, val->s);
			}

			//		"postfix_expression: primary_expression ",
			if( 5 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"postfix_expression: postfix_expression [ expression ] ", E_ARRAY_REF
			if( 6 == act_num ) {
				assert(0);
			}

			//		"postfix_expression: postfix_expression ( argument_expression_list ) ", E_FUNC_REF
			if( 8 == act_num ) {
				assert(0);
			}

			//		"postfix_expression: postfix_expression INC_OP ",
			if( 11 == act_num ) {
				assert(0);
			}

			//		"argument_expression_list: assignment_expression ",
			if( 15 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"argument_expression_list: argument_expression_list , assignment_expression ",
			if( 16 == act_num ) {
				assert(0);
				//head_token->ast = E_newast();
			}

			//		"unary_expression: postfix_expression ",
			if( 17 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"unary_expression: unary_operator cast_expression ",
			if( 20 == act_num ) {
			}

			//		"unary_operator: & ",
			if( 21 == act_num ) {
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
			}

			//		"multiplicative_expression: multiplicative_expression / cast_expression ",
			if( 31 == act_num ) {
			}

			//		"additive_expression: multiplicative_expression ",
			if( 33 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"additive_expression: additive_expression + multiplicative_expression ",
			if( 34 == act_num ) {
				head_token->ast = E_newast(E_ADD, $(3)->ast, $(1)->ast);
			}

			//		"additive_expression: additive_expression - multiplicative_expression ",
			if( 35 == act_num ) {
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
			}

			//		"relational_expression: relational_expression GE_OP shift_expression ",
			if( 43 == act_num ) {
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
				head_token->ast = $(1)->ast;
			}

			//		"init_declarator: declarator = initializer ",
			if( 81 == act_num ) {
			}

			//		"type_specifier: INT ",
			if( 82 == act_num ) {
				head_token->ast = E_newtype(E_INT);
//				head_token->id = ;
//				head_token->val
			}

			//		"type_specifier: FLOAT ",
			if( 83 == act_num ) {
			}

			//		"declarator: direct_declarator ",
			if( 84 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"direct_declarator: IDENTIFIER ",
			if( 85 == act_num ) {
				head_token->ast = (E_ast)E_newstr(E_SYM, val->s);
			}

			//		"direct_declarator: direct_declarator [ assignment_expression ] ",
			if( 87 == act_num ) {
				head_token->ast = E_newast(E_DECLAR_LIST, $(4)->ast, $(2)->ast);
			}

			//		"direct_declarator: direct_declarator ( parameter_type_list ) ",
			if( 90 == act_num ) {
				head_token->ast = E_newast(E_DECLAR_LIST, $(4)->ast, $(2)->ast);
			}

			//		"direct_declarator: direct_declarator ( ) ",
			if( 92 == act_num ) {
				head_token->ast = E_newast(E_DECLAR_LIST, $(3)->ast, NULL);
			}

			//		"parameter_type_list: parameter_list ",
			if( 93 == act_num ) {
			}

			//		"parameter_list: parameter_declaration ",
			if( 95 == act_num ) {
			}

			//		"parameter_list: parameter_list , parameter_declaration ",
			if( 96 == act_num ) {
			}

			//		"parameter_declaration: declaration_specifiers declarator ",
			if( 97 == act_num ) {
			}

			//		"initializer: assignment_expression ",
			if( 114 == act_num ) {
			}

			//		"initializer: { initializer_list } ",
			if( 115 == act_num ) {
			}

			//		"initializer_list: initializer ",
			if( 117 == act_num ) {
			}

			//		"initializer_list: initializer_list , initializer ",
			if( 119 == act_num ) {
			}

			//		"statement: compound_statement ",
			if( 126 == act_num ) {
			}

			//		"statement: expression_statement ",
			if( 127 == act_num ) {
				head_token->ast = $(1)->ast;
			}

			//		"statement: selection_statement ",
			if( 128 == act_num ) {
			}

			//		"statement: iteration_statement ",
			if( 129 == act_num ) {
			}

			//		"statement: jump_statement ",
			if( 130 == act_num ) {
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
			}

			//		"iteration_statement: FOR ( expression_statement expression_statement expression ) statement ",
			if( 144 == act_num ) {
			}

			//		"jump_statement: RETURN expression ; ",
			if( 149 == act_num ) {
			}

			//		"translation_unit: external_declaration ",
			if( 150 == act_num ) {
				assert($(1));
				head_token->ast = $(1)->ast;
			}

			//		"translation_unit: translation_unit external_declaration ",
			if( 151 == act_num ) {
			}

			//		"external_declaration: function_definition ",
			if( 152 == act_num ) {
				assert( $(1) );
				head_token->ast = $(1)->ast;
			}

			//		"function_definition: declaration_specifiers declarator compound_statement ",
			if( 155 == act_num ) {
				head_token->ast = (E_ast)E_newfunc(E_FUNC, $(3)->ast, $(2)->ast, $(1)->ast);

			}
			/* end */

			/*** clear up ***/
			C_token_list head = NULL;
//			int len = 0;
//			for( head = rules[act_num]->list; head; head = head->next ) {
//				len++; // stupid count...
//			}
//			assert( len > 0 );
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
			translate(s->ast);
			/*
		      $$ = ( expr * ) malloc( sizeof( expr ) );
		      $$->code = ( char * ) malloc( strlen( $1->code ) + 100 );
		      sprintf( $$->code, "%sEND main\n", $1->code );
		      fprintf( yyout, ".386\n.MODEL flat, stdcall\n.Stack 4096\n" );
		      fprintf( yyout, "\n%s", funBuffer );
		      fprintf( yyout, "\n.data\n%s", varBuffer );
		      fprintf( yyout, "\n.code\n%s", $$->code );
			*/
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
		"E_INT",  // 0
		"E_FLOAT", // 0
		"E_ARRAY",
};

int local_val  = 0; // the space used by local val
int temp_val = 0;

int str_count = 0;
char local_buffer[1000] = {0.};
void translate( E_ast ast ) {
	if ( NULL == ast ) {
		printf("Find NULL.\n");
		return;
	}
	E_symbol sym;
	E_str str_node;
	E_num num_node;
	switch(ast->type) {
	case E_INT:
		printf("type: %s\n", node_texts[ast->type]);
		ast->code = checked_malloc( 10 );
		sprintf(ast->code, "int");
		return;
	case E_SYM:  // id, need an adress,
		str_node = (E_str)ast;
		printf("type: %s\n", node_texts[ast->type]);
		printf("%s\n", str_node->s);
		assert( NULL == E_lookup(str_node->s) );
		sym = E_install(str_node->s);
		sym->symtype = S_ID;
		//sym->addr = checked_malloc( 100 );
		//sprintf(sym->addr, "");
		return;
	case E_NUM:  // const num
		num_node = (E_num)ast;
		printf("type: %s\n", node_texts[ast->type]);
		printf("%s\n", str_node->s);
		if( NULL == E_lookup( num_node->s )) {
			sym = E_install( num_node->s );
			if(num_node->num_t == NM_I)
				sym->symtype = E_CONST_INT;
			else if(num_node->num_t == NM_F)
				sym->symtype = E_CONST_FLOAT;
		}
		return;
	case E_STRING:
		str_node = (E_str)ast;
		printf("type: %s\n", node_texts[ast->type]);
		printf("%s\n", str_node->s);
		if( NULL == E_lookup( str_node->s )) {
			sym = E_install( str_node->s );
		}
		sym->symtype = S_STR;
		sym->addr = checked_malloc( 10 );
		sprintf(sym->addr, ".LC%d", str_count++);
		sprintf(local_buffer, "%s.align 4\n%s:\n\t%s", local_buffer, sym->addr, str_node->s);
		return;
	case E_SYM_REF:
		printf("type: %s\n", node_texts[ast->type]);
		printf("%s\n", ((E_str)ast)->s);
		assert(NULL != E_lookup( ((E_str)ast)->s ));
		return;
	case E_PASS:
	case E_ADD:
		translate(ast->l);
		translate(ast->r);
		ast->code = checked_malloc( strlen( ast->l->code ) + strlen( ast->l->addr ) +
									strlen( ast->l->code ) + strlen( ast->l->addr ) + 1024);
		sprintf(ast->code, "%s%s\tmovl %s, %%eax\n\taddl %s %%eax\n",
				ast->l->code, ast->r->code, ast->l->addr, ast->r->addr);
		ast->addr = (char *) checked_malloc( 20 );
		sprintf( ast->addr, "-%d(%%ebp)", local_val);
		if (TRUE) // FIXME deal with type latter
			local_val+=4;
		sprintf( ast->code, "%s\tmovl %%eax, %s\n", ast->code, ast->addr);
		return;
	case E_ASSIGN_EQL: //		"assignment_expression: unary_expression assignment_operator assignment_expression ",
		translate(ast->r);
		translate(ast->l);
		ast->code = checked_malloc( strlen( ast->l->code) + strlen( ast->r->code ) +
									strlen( ast->l->addr) + strlen( ast->r->addr ) + 1024);
		sprintf( ast->code, "%s\tmovl %s, %%eax\n%s\tmovl %%eax, %s\n", ast->r->code, ast->r->addr, ast->l->code, ast->l->addr);
		ast->addr = checked_malloc( 20 );
		sprintf( ast->addr, "%s", ast->l->addr);
		return;
	case E_DECLARATION: // "declaration: declaration_specifiers init_declarator_list ; "
		translate(ast->l);
		int data_size = -1;
		if( 0 == strcmp("int", ast->l) ){
			ast->r->var_t = E_INT_TYPE;
		} else {
			assert(0);
		}
		translate(ast->r);
		return;
	case E_INIT_DECL_LIST: 	// "init_declarator_list: init_declarator_list , init_declarator ",
		ast->l->var_t = ast->r->var_t = ast->var_t;
		translate(ast->l);
		translate(ast->r);
		return;
	case E_BLK_ITM_LIST:
		translate(ast->l);
		translate(ast->r);
		printf("type: %s\n", node_texts[ast->type]);
		ast->code = checked_malloc( strlen(ast->l->code) + strlen(ast->r->code) + 10);
		sprintf( ast->code, "%s%s", ast->l->code, ast->r->code);
		return;
	case E_FUNC:
		E_func func_node = (E_func)ast;
		translate(((E_func)ast)->spcifiers);
		translate(((E_func)ast)->declarator);
		translate(((E_func)ast)->ast);
		printf("type: %s\n", node_texts[ast->type]);
		ast->code = checked_malloc( strlen(func_node->spcifiers->code) + strlen(func_node->declarator->code) +
									strlen(func_node->ast->code) + 800);
		sprintf(ast->code, ".cfi_startproc\n"
	             	 	   "pushl	%%ebp\n"
						   ".cfi_def_cfa_offset 8\n"
                  	  	   ".cfi_offset 5, -8\n"
                           "movl	%%esp, %%ebp\n"
                		   ".cfi_def_cfa_register 5\n"
	                       "%s%s%s"
                           ".cfi_restore 5\n"
				           ".cfi_def_cfa 4, 4\n"
				 	 	   "ret\n"
				 	 	   ".cfi_endproc\n", func_node->spcifiers, func_node->declarator, func_node->ast);
		return;
	default:
		printf("strange type: %s\n", node_texts[ast->type]);
		return;
	}
}
