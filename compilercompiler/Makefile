all	: test
haneda_cc: haneda_cc.c haneda_cc.h ../include/haneda_util.c 
	make -w -C ../include/ 
	cc -g -std=c99 -c haneda_cc.c
test: cc_test.c haneda_cc.h haneda_cc
	cc -g -std=c99 -o test cc_test.c haneda_cc.o ../include/haneda_util.o
clean: 
	rm -r haneda_cc.o test test.dSYM
