#include "haneda_util.h"

void haneda_dbg()
{
	printf("File: %s\n", __FILE__);
	printf("Line: %d\n", __LINE__);
}

void *checked_malloc(int n) {
    assert(n > 0);
    void *p = malloc(n);
    assert(p);
    return p;
}

U_list list_find(U_list head, void * val) {
	int d = *(int *)val;
	string s = *(string *)val;
	void *addr = *(void **)val;
	if (head == NULL) {
		return NULL;
	}
	switch(head->nodetype){
	case STRING:
		for (U_list p=head; p; p=p->next) {
			if (strcmp(s, p->val.s) == 0) {
				return p;
			}
		}
		break;
	case INTEGER:
		for (U_list p=head; p; p=p->next) {
			if (p->val.d == d) {
				return p;
			}
		}
		break;
	case ADDRESS:
		for (U_list p=head; p; p=p->next) {
			if (p->val.addr == addr) {
				return p;
			}
		}
		break;
	default:
		fprintf(stderr, "ERROR: WORONG TYPE\n");
		break;
	}
	return NULL;
}

void list_insert(U_list * head, void * val, val_type type) {
	U_list p = checked_malloc(sizeof **head);
	p->nodetype = type;
	p->next = NULL;
	const string s = *(string *)val;
	string temp;
	switch(type) {
	case INTEGER:
		p->val.d = *(int *)val;
		break;
	case ADDRESS:
		p->val.addr = *(void **)val;
		break;
	case STRING:
		temp = (string)checked_malloc( strlen(s) + 1);
		strcpy(temp, s);
		/*
		for(int i=0; ; i++) { // TODO 为什么在这里赋值会造成混乱
			temp[i] = (p->val.s)[i];
			if (temp[i] == '\0')
				break;
		}
		*/
		// strncpy(temp, p->val.s, strlen(p->val.s) + 1);
		// fflush(stdout);
//		p->val.s = s;
//		assert( 0 == strcmp(temp, p->val.s) );
		p->val.s = temp;
		break;
	default:
		fprintf(stderr, "ERROR: WRONG TYPE\n");
		break;
	}

	if( (*head) != NULL) {
		p->next = (*head);
		*head = p;
	}
	else
		*head = p;
}

void list_free(U_list p) {
	if( p!=NULL ) {
		list_free(p->next);
		free(p->next);
		switch(p->nodetype) {
		case STRING:
			free(p->val.s); break;
		default: break;
		}
	}
	else
		return;
}

int set_insert(U_set *set, void * val, val_type type) {
	if ( NULL == list_find(*set, val) ) {
		list_insert(set, val, type); // TODO Do I need to sort it?
		return 1;
	}
	return 0;
}

/***
 * 因为可能需要对set1进行初始化，所以要传指向set1的指针
 * @ret count set1中元素增加的个数
 */
int set_union(U_set * set1, const U_set * set2) {
	int count = 0;
	for (U_list p = (*set2); p; p=p->next) {
		count += set_insert(set1, &p->val, (*set2)->nodetype);
	}
	return count;
}

void set_free(U_set set) {
	list_free(set);
}
