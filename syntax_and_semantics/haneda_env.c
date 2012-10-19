#include "haneda_env.h"

E_symlist htab[HASH_SIZE];

static unsigned int hash(const string name) {
	unsigned int val=0;
	for (char * p = name; *p ; p++)
		val = (val + *p * 17) % HASH_SIZE;
	return val;
}

E_symbol E_lookup( string name ) {
	E_symlist symlist = htab[hash(name)];
	if( !symlist ) {
		symlist = malloc(sizeof *symlist);
		symlist->next = NULL;
		htab[hash(name)] = symlist;

		symlist->sym = malloc(sizeof *symlist->sym);
		symlist->sym->name = strdup(name);
		assert(symlist->sym->name);
		symlist->sym->syms = NULL;
		symlist->sym->value.str = (string)0;
		symlist->sym->symtype = -1;
		return symlist->sym;
	}

	for(E_symlist p = symlist; p; p=p->next) {
		if( 0 == strcmp(p->sym->name, name) )
			return p->sym;
	}

	symlist = malloc(sizeof *symlist);
	symlist->next = htab[hash(name)];
	htab[hash(name)] = symlist;

	symlist->sym = malloc(sizeof *symlist->sym);
	symlist->sym->name = strdup(name);
	assert(symlist->sym->name);
	symlist->sym->syms = NULL;
	symlist->sym->value.str = (string)0;
	symlist->sym->symtype = -1;

	return symlist->sym;
}
