#include "haneda_env.h"

E_symlist htab[HASH_SIZE];

static unsigned int hash(const string name) {
	unsigned int val=0;
	for (char * p = name; *p ; p++)
		val = (val + *p * 17) % HASH_SIZE;
	return val;
}

E_type E_newtype(int type) {
	E_type ret = checked_malloc( sizeof *ret );
	ret->type = type;
	return ret;
}
E_func E_newfunc(int type, E_ast spec, E_ast decl, E_ast comp) {
	E_func ret = checked_malloc( sizeof *ret );
	ret->spcifiers = spec;
	ret->declarator = decl;
	ret->ast = comp;
	ret->type = E_FUNC;
	return ret;
}
E_ast E_newast(int type, E_ast l, E_ast r) {
	E_ast ret = checked_malloc( sizeof *ret );
	ret->type = type;
	ret->l = l;
	ret->r = r;
	ret->var_t = -1;
	return ret;
}
E_num E_newnum(int type, string s, int num_type) {
	E_num num_node = NULL;
	num_node = checked_malloc( sizeof *num_node );
	num_node->type = type;
	num_node->s = checked_strdup(s);
	num_node->num_t = num_type;
	return num_node;
}
E_str E_newstr(int type, string s) {
	E_str str_node = NULL;
	str_node = checked_malloc(sizeof *str_node);
	str_node->type = type;
	str_node->s = checked_strdup(s);
	return str_node;
}
//TODO  separate lookup and install
E_symbol E_install( string name ) {
	printf("install.\n");
	assert( NULL == E_lookup(name));
	E_symlist symlist = htab[hash(name)];
	symlist = checked_malloc(sizeof *symlist);
	symlist->next = NULL;
	htab[hash(name)] = symlist;

	symlist->sym = checked_malloc(sizeof *symlist->sym);
	symlist->sym->name = strdup(name);
	assert(symlist->sym->name);
	symlist->sym->syms = NULL;
//	symlist->sym->value.str = (string)0;
	symlist->sym->symtype = -1;
	return symlist->sym;
}
E_symbol E_lookup( string name ) {
	E_symlist symlist = htab[hash(name)];
	if( !symlist ) {
		return NULL;
	}

	for(E_symlist p = symlist; p; p=p->next) {
		if( 0 == strcmp(p->sym->name, name) )
			return p->sym;
	}

	return NULL;
}
