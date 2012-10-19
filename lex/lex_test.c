#include "haneda_lex.h"
#include "../include/haneda_util.h"

// TODO 研究下怎么配置Eclipse的make命令
int	main() {
	int ret = 0;
	ret = start("testprog.c");
	printf("open file: %d\n", ret);
	assert(start("testprog.c"));
	while ( (ret = lex()) != EOF && ret != 0) {
		printf("%d\n", ret);
		printbuffer();
	}
	printf("ret: %d\n", ret==EOF);
	printf("end\n");
	return 0;
}

