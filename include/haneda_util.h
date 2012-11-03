#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef HANEDA_UTIL
#define HANEDA_UTIL

typedef struct U_list_ * U_list;

/*
#define iterate_with_val(head,x,type) 	for(U_list p=head;p;p=p->next) { \
											if (p->val.x == x) \
										 	 	 return p; \
										}
*/

typedef char * string;
typedef char bool;
#define TRUE 1
#define FALSE 0

typedef enum val_type_ {
	INTEGER, ADDRESS, STRING
} val_type ;

typedef union {
	int d ; /* 0 */
	string s; /* 1 */
	void * addr; /* 2 */
} list_val ;

struct U_list_ {
	val_type nodetype;
	struct U_list_ * next;
	list_val val;
};

typedef U_list U_set;

/******* debug *****************/
void haneda_dbg();
/******* Memory ****************/
string checked_strdup( string s);
void *checked_malloc(int);
/******* Linked List ***********/
U_list list_find(U_list head, void * val);
void list_insert(U_list * head, void * val, val_type type) ;
void list_free(U_list p);
/******* Set ****************/
int set_insert(U_set * set, void * val, val_type type);
int set_union(U_set * set1, const U_set * set2);
void set_free(U_set set);
/******* Hash Table ************/
// TODO TO Be Implemented

#endif
