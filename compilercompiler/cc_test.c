#include "../include/haneda_util.h"
#include "haneda_cc.h"

int main() {
	string rule_text = "T: ( L )";

	C_rule rule = malloc( sizeof *rule );
	rule->head_token_nr = getHeadNr(rule_text);
	rule->list = getTokenList(rule_text);

	assert(rule->head_token_nr == 1);
	assert(getTokenNr("T") == 1);
	assert(getTokenNr("(") == 4);
	assert(getTokenNr(")") == 5);
	assert(getTokenNr("L") == 3);

	for(C_token_list p = rule->list; p; p=p->next) {
		printf("%d\n", p->val.d);
	}

	haneda_initialize();
}
