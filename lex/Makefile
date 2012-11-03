all	: test
lex	: haneda_lex.c haneda_lex.h
	make -w -C ../include
	cc -g -c -std=c99 haneda_lex.c
test: lex_test.c lex haneda_lex.h
	cc -g -std=c99 -o test lex_test.c haneda_lex.o ../include/haneda_util.o
clean: 
	rm -r haneda_lex.o test test.dSYM
 
