#include "haneda_syntax.h"

int main() {
	syntax_init();
    printf("syntax init done.\n");
    syntax( "simple1.c" );
//	syntax( "testprog.c" );
}
