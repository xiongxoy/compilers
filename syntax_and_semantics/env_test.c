#include "haneda_env.h"

int main() {
	//string name = "a";
	assert( NULL == E_lookup("b") );
	E_symbol s = E_install("b");
	assert( s == E_lookup("b"));
	printf("%s", "OK!\n");
	return 0;
}
