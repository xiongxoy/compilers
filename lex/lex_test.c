#include "haneda_lex.h"
#include "../include/haneda_util.h"

// TODO �о�����ô����Eclipse��make����
int	main() {
	int ret = 0;
	lex_init();
	ret = start("testprog.c");
	printf("open file: %d\n", ret);
	assert(start("testprog.c"));
	while ( (ret = lex()) != '$' && ret != 0) {
		printbuffer();
		printf("%d\n\n", ret);

	}
	printf("ret: %d\n", ret=='$');
	printf("end\n");
	return 0;
}

