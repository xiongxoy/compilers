#include <stdio.h>
#include "haneda_util.h"

void test_list();
void test_set();
int main() {
	string a = checked_malloc(10);
	for(int i=0; i<9; i++) {
		a[i] = '*';
	}
	a[9] = '\0';
	bool b = FALSE;
	if(b) {
		;
	}
	else {
		printf("%s++\n", a);
	}
	test_list();
	test_set();
	//free(s);

}

void test_set() {
	U_set set = NULL; // 一定要初始化为NULL，要不可能会出问题
	string s = "A";
	set_insert(&set,&s,STRING);
	s = "B";
	set_insert(&set, &s, STRING);
	s = "A";
	set_insert(&set, &s, STRING);
	s = "A";
	set_insert(&set, &s, STRING);
	U_set set_ = NULL;
	s = "C";
	set_insert(&set_, &s, STRING);
	for(U_set p=set; p; p=p->next) {
		printf("set: %s\n", p->val.s);
	}
	printf("\n");
	set_union(&set, &set_);
	for(U_set p=set; p; p=p->next) {
		printf("set: %s\n", p->val.s);
	}
}
void test_list() {
	U_list head = NULL;
	int d = 1;
	list_insert(&head, &d, INTEGER);
	d = 2;
	list_insert(&head, &d, INTEGER);
	assert( list_find(head, &d) != NULL );
	assert( list_find(head, &d)->val.d == 2);
	list_free(head);
	head = NULL;
	string s = "test string";
	list_insert(&head, &s, STRING);
	assert( list_find(head, &s) != NULL );
	assert( 0 == strcmp(list_find(head, &s)->val.s,s));
	list_free(head);
	head = NULL;
}

