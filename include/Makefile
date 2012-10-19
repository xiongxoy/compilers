all: test

test:  test_util.c haneda_util
	cc -Wall -g -std=c99 -o test test_util.c haneda_util.o
	
haneda_util : haneda_util.h haneda_util.c
	cc -Wall -g -std=c99 -c haneda_util.c
	
clean:
	rm haneda_util.o test 
